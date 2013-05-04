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

    if (m_poSettings->bParam("check_updates"))
    {
        VersionChecker* poVersionChecker = new VersionChecker(0);
        connect(poVersionChecker, SIGNAL(newVersionAvailable(QString)), this, SLOT(vSlotNewVersion(QString)));

        m_poVCThread = new QThread(this);
        connect(m_poVCThread, SIGNAL(started()), poVersionChecker, SLOT(doCheckVersion()));
        poVersionChecker->moveToThread(m_poVCThread);

        QTimer::singleShot(1000, m_poVCThread, SLOT(start()));
    }
}

/*
 * update the wallpaper and start the timer
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
void Controller::vStartPause()
{
    if (m_poMainTimer->isActive())
    {
        m_poMainTimer->stop();
    }
    else
    {
        m_poMainTimer->start();
    }
}


/*
 * update the wallpaper
 */
void Controller::vSlotUpdate()
{
    // update config
    if (m_poSettings->bParam("check"))
    {
        m_poSettings->vUpdateSets();
        m_poSettings->vReadNbWalls();
        emit listChanged();
    }

    int iTotalFiles = m_poSettings->iNbFiles();

    if (iTotalFiles == 0)
    {
        return;
    }

    // get random files
    QVector<QString> files;
    for (int i=0; i<m_poSettings->iEnv("nb_walls"); i++)
    {
        files.push_back(sGetRandomFile(iTotalFiles));
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
 * new version available
 */
void Controller::vSlotNewVersion(QString _ver)
{
    m_poVCThread->exit();
    emit newVersionAvailable(_ver);
}

/*
 * get a random file among all active sets
 */
QString const Controller::sGetRandomFile(int _total)
{
    if (_total == -1)
    {
        _total = m_poSettings->iNbFiles();
    }

    QString sPath = "";

    uniform_int<int> unif(0, _total-1);
    int iCounter = unif(m_oRandom);

    for (int i=0; i<m_poSettings->iNbSets(); i++)
    {
        Set* poSet = m_poSettings->poGetSet(i);
        if (poSet->isActive())
        {
            if (iCounter < poSet->count())
            {
                sPath = poSet->sGetFile(iCounter);
                break;
            }
            iCounter-= poSet->count();
        }
    }

    return sPath;
}

/*
 * get footer text, with hyperlinks
 */
QString const Controller::sGetStatusText() const
{
    QString msg = "<a href='"+QString::fromAscii(APP_HOMEPAGE)+"'>"+QString::fromAscii(APP_NAME)+"</a>";
    msg+= " "+QString::fromAscii(APP_VERSION);
    msg+= " | <a href='http://www.realtimesoft.com/ultramon'>UltraMon</a> "+m_poSettings->sEnv("umversion");
    return msg;
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
