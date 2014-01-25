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
Controller::Controller(Settings* _pSettings) : QObject(0)
{
    m_randomEngine.seed((unsigned int)time(NULL));

    m_pSettings = _pSettings;

    m_pMainTimer = new QTimer(this);
    connect(m_pMainTimer, SIGNAL(timeout()), this, SLOT(slotUpdate()));

    m_headerSize = 4 + 2 + 1; // "UMWP",  version, activedesktop
    m_headerSize+= sizeof(DWORD); // nb monitors
    m_headerSize+= m_pSettings->iEnv("nb_monitors") * sizeof(RECT);
}

/**
 * @brief Init version checker thread
 */
void Controller::checkVersion()
{
    if (m_pSettings->bParam("check_updates"))
    {
        VersionChecker* pVersionChecker = new VersionChecker(0);
        connect(pVersionChecker, SIGNAL(newVersionAvailable(const QString)),
                this, SIGNAL(newVersionAvailable(const QString)));

        QThread* pVCThread = new QThread(this);
        pVersionChecker->moveToThread(pVCThread);

        connect(pVCThread, SIGNAL(started()), pVersionChecker, SLOT(run()));
        connect(pVersionChecker, SIGNAL(finished()), pVCThread, SLOT(quit()));
        connect(pVersionChecker, SIGNAL(finished()), pVersionChecker, SLOT(deleteLater()));

        QTimer::singleShot(1000, pVCThread, SLOT(start()));
    }
}

/**
 * @brief Stop the timer, update the wallpaper and restart the timer
 * @param bool _bKeepPause - prevent timer to restart
 */
void Controller::startTimer(bool _keepPause)
{
    bool wasPaused = !m_pMainTimer->isActive();

    m_pMainTimer->stop();
    m_pMainTimer->setInterval(m_pSettings->iParam("delay")*1000);

    slotUpdate();

    if (!wasPaused || !_keepPause)
    {
        m_pMainTimer->start();
    }
}

/**
 * @brief Pause or start the timer
 * @return bool - true if the timer is running
 */
bool Controller::startPause()
{
    if (m_pMainTimer->isActive())
    {
        m_pMainTimer->stop();
    }
    else
    {
        m_pMainTimer->start();
    }

    return m_pMainTimer->isActive();
}

/**
 * @brief Add a new set
 * @param string _sDirname
 */
void Controller::addSet(const QString _dirname)
{
    m_pSettings->pAddSet(_dirname);

    m_pSettings->writeXML();
    emit listChanged(false);
}

/**
 * @brief Delete sets
 * @param int[] _ai
 */
void Controller::deleteSets(const QList<int> _aSets)
{
    int offset = 0;
    for (QList<int>::const_iterator i=_aSets.begin(); i!=_aSets.end(); i++)
    {
        m_pSettings->deleteSet(*i-offset);
        offset++;
    }

    m_pSettings->writeXML();
    emit listChanged(true);
}

/**
 * @brief Activate sets
 * @param int[] _ai
 */
void Controller::activateSets(const QList<int> _aSets)
{
    for (QList<int>::const_iterator i=_aSets.begin(); i!=_aSets.end(); i++)
    {
        m_pSettings->setState(*i, true);
    }

    m_pSettings->writeXML();
    emit listChanged(false);
}

/**
 * @brief Unactivate sets
 * @param int[] _ai
 */
void Controller::unactivateSets(const QList<int> _aSets)
{
    for (QList<int>::const_iterator i=_aSets.begin(); i!=_aSets.end(); i++)
    {
        m_pSettings->setState(*i, false);
    }

    m_pSettings->writeXML();
    emit listChanged(false);
}

/**
 * @brief Activate only some sets
 * @param int[] _ai
 */
void Controller::setActiveSets(const QList<int> _aSets)
{
    for (int i=0, l=m_pSettings->nbSets(); i<l; i++)
    {
        m_pSettings->setState(i, _aSets.contains(i));
    }

    m_pSettings->writeXML();
    emit listChanged(false);
}

/**
 * @brief Activate only one set and unactive any other sets
 * @param int _i
 */
void Controller::setOneActiveSet(int _set)
{
    for (int i=0, l=m_pSettings->nbSets(); i<l; i++)
    {
        m_pSettings->setState(i, i==_set);
    }

    m_pSettings->writeXML();
    emit listChanged(false);
}

/**
 * @brief Edit a set
 * @param int _i - position in Settings vector
 * @param string _sName
 * @param int _iType
 * @param int _iStyle
 */
void Controller::editSet(int _set, QString const &_name, const UM::WALLPAPER _type, const UM::IMAGE _style, const int _hotkey)
{
    m_pSettings->editSet(_set, _name, _type, _style, _hotkey);

    m_pSettings->writeXML();
    emit listChanged(false);
}

/**
 * @brief Move a set
 * @param int _from - position in Settings vector
 * @param int _to - new position in Settings vector
 */
void Controller::moveSet(int _from, int _to)
{
    m_pSettings->moveSet(_from, _to);

    m_pSettings->writeXML();
    emit listChanged(true);
}


/**
 * @brief Update the wallpaper
 * @param bool _bCheckFiles - if true, depends on configuration
 */
void Controller::slotUpdate(bool _checkFiles)
{
    // update config
    if (_checkFiles && m_pSettings->bParam("check"))
    {
        m_pSettings->updateSets();
        m_pSettings->readNbMonitors();
        emit listChanged(false);
    }

    int totalSets = m_pSettings->nbActiveSets(true);

    if (totalSets == 0)
    {
        return;
    }

    // get random files
    Set* pSet = pGetRandomSet(totalSets);

    m_aFiles.clear();

    if (pSet->type() == 1)
    {
        for (int i=0, l=m_pSettings->iEnv("nb_monitors"); i<l; i++)
        {
            m_aFiles.push_back(getRandomFile(pSet));
        }
    }
    else
    {
        m_aFiles.push_back(getRandomFile(pSet));
    }

    QString filename = m_pSettings->sEnv("wallpath") + QString::fromAscii(APP_WALLPAPER_FILE);

    // generate .wallpaper file
    generateFile(filename, pSet);

    // remove old BMP file
    if (fileExists(m_pSettings->sEnv("bmppath")))
    {
        QFile::remove(m_pSettings->sEnv("bmppath"));
    }

    // execute UltraMonDesktop
    QString cmd = "\"" + m_pSettings->sParam("umpath") + "\" /load \"" + filename + "\"";
    QProcess::execute(cmd);
}

/**
 * @brief Get a random Set among all active sets
 * @param int _iTotal - total number of Sets
 * @return Set*
 */
Set* Controller::pGetRandomSet(int _total)
{
    if (_total == 1)
    {
        return m_pSettings->pGetActiveSet(0);
    }

    uniform_int<int> unif(0, _total-1);
    int counter = unif(m_randomEngine);

    return m_pSettings->pGetActiveSet(counter);
}

/**
 * @brief Get a random file within a Set
 * @param Set* _poSet
 * @return string
 */
QString Controller::getRandomFile(Set* _poSet)
{
    int total = _poSet->count();

    // only one file in the set ?!
    if (total == 1)
    {
        return _poSet->getFile(0);
    }

    // rare case for small sets
    if (total <= m_aFiles.size())
    {
        uniform_int<int> unif(0, m_aFiles.size()-1);
        int counter = unif(m_randomEngine);
        return m_aFiles.at(counter);
    }

    // search a random unused file
    short loop = 10;
    QString file;
    uniform_int<int> unif(0, total-1);

    while (loop > 0)
    {
        int counter = unif(m_randomEngine);
        file = _poSet->getFile(counter);

        if (!m_aFiles.contains(file))
        {
            loop = 0;
        }

        loop--;
    }

    return file;
}

/**
 * @brief Generate AutoChanger.wallpaper file
 * @param string _sFilename
 * @param Set* _poSet
 */
void Controller::generateFile(const QString &_filename, const Set* _pSet)
{
    // open default file
    QString defaultFilename = m_pSettings->sEnv("wallpath") + "default.wallpaper";
    QFile defaultFile(defaultFilename);
    defaultFile.open(QIODevice::ReadOnly);

    // get header from default.wallpaper
    QByteArray buffer = defaultFile.readAll();
    defaultFile.close();
    buffer.truncate(m_headerSize);

    // write wallpaper type
    UM::WALLPAPER wp_type = _pSet->type();
    buffer.append((char*)&wp_type, sizeof(UM::WALLPAPER));

    // write number of wallpapers
    DWORD nb_walls = m_aFiles.size();
    buffer.append((char*)&nb_walls, sizeof(DWORD));

    // write wallpapers
    for (unsigned int i=0; i<nb_walls; i++)
    {
        UM::WP_MONITOR_FILE wall;
        wall.bgType = UM::BG_SOLID;
        wall.color1 = 0x00000000;
        wall.color2 = 0x00000000;
        wall.imgStyle = _pSet->style();
        memset(wall.imgFile, 0, 260*sizeof(wchar_t));
        m_aFiles.at(i).toWCharArray((wchar_t*)wall.imgFile);

        buffer.append((char*)&wall, sizeof(UM::WP_MONITOR_FILE));
    }

    // save file
    QFile file(_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(buffer);
    file.close();
}
