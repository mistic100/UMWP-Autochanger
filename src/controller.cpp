#include <ctime>
#include <iostream>
#include <fstream>

#include "dirent.h"
#include "main.h"
#include "controller.h"
#include "mainwidget.h"


/*
 * constructor
 */
Controller::Controller(Settings* _data) : QObject(0)
{
    m_oRandom.seed((unsigned int)time(NULL));

    m_poSettings = _data;

    m_poMainTimer = new QTimer(this);
    connect(m_poMainTimer, SIGNAL(timeout()), this, SLOT(vSlotUpdate()));
}

/*
 * init threads
 */
void Controller::vStart()
{
    if (m_poSettings->bParam("check_updates"))
    {
        VersionChecker* poVersionChecker = new VersionChecker(0);
        connect(poVersionChecker, SIGNAL(newVersionAvailable(QString)), this, SIGNAL(newVersionAvailable(QString)));

        QThread* poVCThread = new QThread(this);
        poVersionChecker->moveToThread(poVCThread);

        connect(poVCThread, SIGNAL(started()), poVersionChecker, SLOT(run()));
        connect(poVersionChecker, SIGNAL(finished()), poVCThread, SLOT(quit()));
        connect(poVersionChecker, SIGNAL(finished()), poVersionChecker, SLOT(deleteLater()));

        QTimer::singleShot(1000, poVCThread, SLOT(start()));
    }
}

/*
 * start the timer and update the wallpaper
 */
void Controller::vStartTimer(bool _keepPause)
{
    bool was_pause = !m_poMainTimer->isActive();

    m_poMainTimer->stop();
    m_poMainTimer->setInterval(m_poSettings->iParam("delay")*1000);

    vSlotUpdate();

    if (!was_pause || !_keepPause) m_poMainTimer->start();
}

/*
 * pause or start the timer
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

/*
 * add a new set
 */
void Controller::vAddSet(QString const _dirname)
{
    m_poSettings->oAddSet(_dirname);
    emit listChanged(false);
}

/*
 * delete sets
 */
void Controller::vDeleteSets(QList<int> const _list)
{
    int off=0;
    for (QList<int>::const_iterator i=_list.begin(); i!=_list.end(); i++)
    {
        m_poSettings->vDeleteSet(*i-off);
        off++;
    }

    emit listChanged(true);
}

/*
 * activate sets
 */
void Controller::vActivateSets(QList<int> const _list)
{
    for (QList<int>::const_iterator i=_list.begin(); i!=_list.end(); i++)
    {
        m_poSettings->vSetState(*i, true);
    }

    emit listChanged(false);
}

/*
 * unactivate sets
 */
void Controller::vUnactivateSets(QList<int> const _list)
{
    for (QList<int>::const_iterator i=_list.begin(); i!=_list.end(); i++)
    {
        m_poSettings->vSetState(*i, false);
    }

    emit listChanged(false);
}

/*
 * set only one set as active and unactive any other sets
 */
void Controller::vSetOneActiveSet(int _idx)
{
    for (int i=0; i<m_poSettings->iNbSets(); i++)
    {
        m_poSettings->vSetState(i, i==_idx);
    }

    emit listChanged(false);
}

/*
 * rename a set
 */
void Controller::vRenameSet(int _idx, QString const _name)
{
    m_poSettings->vRenameSet(_idx, _name);
    emit listChanged(false);
}

/*
 * move a set
 */
void Controller::vMoveSet(int _from, int _to)
{
    m_poSettings->vMoveSet(_from, _to);
    emit listChanged(false);
}


/*
 * update the wallpaper
 */
void Controller::vSlotUpdate(bool _check)
{
    // update config
    if (_check && m_poSettings->bParam("check"))
    {
        m_poSettings->vUpdateSets();
        m_poSettings->vReadNbWalls();
        emit listChanged(false);
    }

    int iTotalSets = m_poSettings->iNbActiveSets(true);

    if (iTotalSets == 0)
    {
        return;
    }

    // get random files
    Set* poSet = poGetRandomSet(iTotalSets);

    QVector<QString> files;
    for (int i=0; i<m_poSettings->iEnv("nb_walls"); i++)
    {
        files.push_back(sGetRandomFile(poSet, files));
    }

    // generate .wallpaper file
    vGenerateFile(files);

    // remove old BMP file
    if (bFileExists(m_poSettings->sEnv("bmppath")))
    {
        QFile::remove(m_poSettings->sEnv("bmppath"));
    }

    // execute UltraMonDesktop
    QString cmd = "\""+m_poSettings->sParam("umpath")+"\" /load";
    cmd+= " \""+m_poSettings->sEnv("wallpath")+QString::fromAscii(APP_WALLPAPER_FILE)+"\"";
    QProcess::execute(cmd);
}

/*
 * get a random Set among all active sets
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

/*
 * get a random file within a Set
 */
QString Controller::sGetRandomFile(Set* _poSet, QVector<QString> const &_sFiles)
{
    int iTotal = _poSet->count();

    // only one file in the set ?!
    if (iTotal == 1)
    {
        return _poSet->sGetFile(0);
    }

    // rare case for small sets
    if (iTotal <= _sFiles.size())
    {
        uniform_int<int> unif(0, _sFiles.size()-1);
        int iCounter = unif(m_oRandom);
        return _sFiles.at(iCounter);
    }

    // search a random unused file
    short iLoop = 10;
    QString sFile;
    uniform_int<int> unif(0, iTotal-1);

    while (iLoop > 0)
    {
        int iCounter = unif(m_oRandom);
        sFile = _poSet->sGetFile(iCounter);

        if (!_sFiles.contains(sFile))
        {
            iLoop = 0;
        }

        iLoop--;
    }

    return sFile;
}

/*
 * generate .wallpaper file
 * _files must contains exactly m_iNbWallpapers elements
 */
void Controller::vGenerateFile(QVector<QString> _files)
{
    if (_files.size() != m_poSettings->iEnv("nb_walls"))
    {
        return;
    }

    QString file = m_poSettings->sEnv("wallpath");
    file.append(APP_WALLPAPER_FILE);
    int const header_size = 19 + m_poSettings->iEnv("nb_monitors")*16;

    std::fstream ofs(file.toStdString(), std::ios::out | std::ios::in | std::ios::binary);

    for (int i=0; i<m_poSettings->iEnv("nb_walls"); i++)
    {
        // seek to the begining of the imgFile field
        int const wp_file_pos = header_size + i*260*sizeof(wchar_t) + (i+1)*16;
        ofs.seekp(wp_file_pos, std::ios_base::beg);

        // change imgFile
        wchar_t temp[260];
        memset(temp, 0, 260*sizeof(wchar_t));
        _files.at(i).toWCharArray((wchar_t*)temp);
        ofs.write((char*)&temp, 260*sizeof(wchar_t));
    }

    ofs.close();
}
