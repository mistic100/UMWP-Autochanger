#include <ctime>
#include <iostream>
#include <fstream>

#include "dirent.h"
#include "main.h"
#include "controller.h"
#include "mainwidget.h"


/**
 * @brief Controller::Controller
 * @param Settings* _data
 */
Controller::Controller(Settings* _data) : QObject(0)
{
    m_oRandom.seed((unsigned int)time(NULL));

    m_poSettings = _data;

    m_poMainTimer = new QTimer(this);
    connect(m_poMainTimer, SIGNAL(timeout()), this, SLOT(slotUpdate()));

    m_iHeaderSize = 4 + 2 + 1; // "UMWP",  version, activedesktop
    m_iHeaderSize+= sizeof(DWORD); // nb monitors
    m_iHeaderSize+= m_poSettings->iEnv("nb_monitors") * sizeof(RECT);
}

/**
 * @brief Init version checker thread
 */
void Controller::vCheckVersion()
{
    if (m_poSettings->bParam("check_updates"))
    {
        VersionChecker* poVersionChecker = new VersionChecker(0);
        connect(poVersionChecker, SIGNAL(newVersionAvailable(const QString)),
                this, SIGNAL(newVersionAvailable(const QString)));

        QThread* poVCThread = new QThread(this);
        poVersionChecker->moveToThread(poVCThread);

        connect(poVCThread, SIGNAL(started()), poVersionChecker, SLOT(run()));
        connect(poVersionChecker, SIGNAL(finished()), poVCThread, SLOT(quit()));
        connect(poVersionChecker, SIGNAL(finished()), poVersionChecker, SLOT(deleteLater()));

        QTimer::singleShot(1000, poVCThread, SLOT(start()));
    }
}

/**
 * @brief Stop the timer, update the wallpaper and restart the timer
 * @param bool _bKeepPause - prevent timer to restart
 */
void Controller::vStartTimer(bool _bKeepPause)
{
    bool bWasPaused = !m_poMainTimer->isActive();

    m_poMainTimer->stop();
    m_poMainTimer->setInterval(m_poSettings->iParam("delay")*1000);

    slotUpdate();

    if (!bWasPaused || !_bKeepPause)
    {
        m_poMainTimer->start();
    }
}

/**
 * @brief Pause or start the timer
 * @return bool - true if the timer is running
 */
bool Controller::bStartPause()
{
    if (m_poMainTimer->isActive())
    {
        m_poMainTimer->stop();
    }
    else
    {
        m_poMainTimer->start();
    }

    return m_poMainTimer->isActive();
}

/**
 * @brief Add a new set
 * @param string _sDirname
 */
void Controller::vAddSet(const QString _sDirname)
{
    m_poSettings->poAddSet(_sDirname);
    emit listChanged(false);
}

/**
 * @brief Delete sets
 * @param int[] _ai
 */
void Controller::vDeleteSets(const QList<int> _ai)
{
    int iOffset = 0;
    for (QList<int>::const_iterator i=_ai.begin(); i!=_ai.end(); i++)
    {
        m_poSettings->vDeleteSet(*i-iOffset);
        iOffset++;
    }

    emit listChanged(true);
}

/**
 * @brief Activate sets
 * @param int[] _ai
 */
void Controller::vActivateSets(const QList<int> _ai)
{
    for (QList<int>::const_iterator i=_ai.begin(); i!=_ai.end(); i++)
    {
        m_poSettings->vSetState(*i, true);
    }

    emit listChanged(false);
}

/**
 * @brief Unactivate sets
 * @param int[] _ai
 */
void Controller::vUnactivateSets(const QList<int> _ai)
{
    for (QList<int>::const_iterator i=_ai.begin(); i!=_ai.end(); i++)
    {
        m_poSettings->vSetState(*i, false);
    }

    emit listChanged(false);
}

/**
 * @brief Activate only some sets
 * @param int[] _ai
 */
void Controller::vSetActiveSets(const QList<int> _ai)
{
    for (int i=0, l=m_poSettings->iNbSets(); i<l; i++)
    {
        m_poSettings->vSetState(i, _ai.contains(i));
    }

    emit listChanged(false);
}

/**
 * @brief Activate only one set and unactive any other sets
 * @param int _i
 */
void Controller::vSetOneActiveSet(int _i)
{
    for (int i=0, l=m_poSettings->iNbSets(); i<l; i++)
    {
        m_poSettings->vSetState(i, i==_i);
    }

    emit listChanged(false);
}

/**
 * @brief Edit a set
 * @param int _i - position in Settings vector
 * @param string _sName
 * @param int _iType
 * @param int _iStyle
 */
void Controller::vEditSet(int _i, QString const &_sName, const UM::WALLPAPER _iType, const UM::IMAGE _iStyle, const int _iHotkey)
{
    m_poSettings->vEditSet(_i, _sName, _iType, _iStyle, _iHotkey);
    emit listChanged(false);
}

/**
 * @brief Move a set
 * @param int _from - position in Settings vector
 * @param int _to - new position in Settings vector
 */
void Controller::vMoveSet(int _from, int _to)
{
    m_poSettings->vMoveSet(_from, _to);
    emit listChanged(true);
}


/**
 * @brief Update the wallpaper
 * @param bool _bCheckFiles - if true, depends on configuration
 */
void Controller::slotUpdate(bool _bCheckFiles)
{
    // update config
    if (_bCheckFiles && m_poSettings->bParam("check"))
    {
        m_poSettings->vUpdateSets();
        m_poSettings->vReadNbMonitors();
        emit listChanged(false);
    }

    int iTotalSets = m_poSettings->iNbActiveSets(true);

    if (iTotalSets == 0)
    {
        return;
    }

    // get random files
    Set* poSet = poGetRandomSet(iTotalSets);

    m_asFiles.clear();

    if (poSet->type() == 1)
    {
        for (int i=0, l=m_poSettings->iEnv("nb_monitors"); i<l; i++)
        {
            m_asFiles.push_back(sGetRandomFile(poSet));
        }
    }
    else
    {
        m_asFiles.push_back(sGetRandomFile(poSet));
    }

    QString sFilename = m_poSettings->sEnv("wallpath") + QString::fromAscii(APP_WALLPAPER_FILE);

    // generate .wallpaper file
    vGenerateFile(sFilename, poSet);

    // remove old BMP file
    if (bFileExists(m_poSettings->sEnv("bmppath")))
    {
        QFile::remove(m_poSettings->sEnv("bmppath"));
    }

    // execute UltraMonDesktop
    QString cmd = "\"" + m_poSettings->sParam("umpath") + "\" /load \"" + sFilename + "\"";
    QProcess::execute(cmd);
}

/**
 * @brief Get a random Set among all active sets
 * @param int _iTotal - total number of Sets
 * @return Set*
 */
Set* Controller::poGetRandomSet(int _iTotal)
{
    if (_iTotal == 1)
    {
        return m_poSettings->poGetActiveSet(0);
    }

    uniform_int<int> unif(0, _iTotal-1);
    int iCounter = unif(m_oRandom);

    return m_poSettings->poGetActiveSet(iCounter);
}

/**
 * @brief Get a random file within a Set
 * @param Set* _poSet
 * @return string
 */
QString Controller::sGetRandomFile(Set* _poSet)
{
    int iTotal = _poSet->count();

    // only one file in the set ?!
    if (iTotal == 1)
    {
        return _poSet->sGetFile(0);
    }

    // rare case for small sets
    if (iTotal <= m_asFiles.size())
    {
        uniform_int<int> unif(0, m_asFiles.size()-1);
        int iCounter = unif(m_oRandom);
        return m_asFiles.at(iCounter);
    }

    // search a random unused file
    short iLoop = 10;
    QString sFile;
    uniform_int<int> unif(0, iTotal-1);

    while (iLoop > 0)
    {
        int iCounter = unif(m_oRandom);
        sFile = _poSet->sGetFile(iCounter);

        if (!m_asFiles.contains(sFile))
        {
            iLoop = 0;
        }

        iLoop--;
    }

    return sFile;
}

/**
 * @brief Generate AutoChanger.wallpaper file
 * @param string _sFilename
 * @param Set* _poSet
 */
void Controller::vGenerateFile(const QString &_sFilename, const Set* _poSet)
{
    // open default file
    QString sDefaultFilename = m_poSettings->sEnv("wallpath") + "default.wallpaper";
    QFile oDefaultFile(sDefaultFilename);
    oDefaultFile.open(QIODevice::ReadOnly);

    // get header from default.wallpaper
    QByteArray aBuffer = oDefaultFile.readAll();
    oDefaultFile.close();
    aBuffer.truncate(m_iHeaderSize);

    // write wallpaper type
    UM::WALLPAPER wp_type = _poSet->type();
    aBuffer.append((char*)&wp_type, sizeof(UM::WALLPAPER));

    // write number of wallpapers
    DWORD nb_walls = m_asFiles.size();
    aBuffer.append((char*)&nb_walls, sizeof(DWORD));

    // write wallpapers
    for (unsigned int i=0; i<nb_walls; i++)
    {
        UM::WP_MONITOR_FILE wall;
        wall.bgType = UM::BG_SOLID;
        wall.color1 = 0x00000000;
        wall.color2 = 0x00000000;
        wall.imgStyle = _poSet->style();
        memset(wall.imgFile, 0, 260*sizeof(wchar_t));
        m_asFiles.at(i).toWCharArray((wchar_t*)wall.imgFile);

        aBuffer.append((char*)&wall, sizeof(UM::WP_MONITOR_FILE));
    }

    // save file
    QFile file(_sFilename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(aBuffer);
    file.close();
}
