#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include "dirent.h"
#include "main.h"
#include "controller.h"
#include "mainwidget.h"
#include <QDebug>


/*
 * constructor
 */
Controller::Controller(Settings* _data) : QObject(0)
{
    srand(time(NULL));

    m_poSettings = _data;
    m_poMainTimer = new QTimer();
    m_poMainWidget = NULL;

    connect(m_poMainTimer, SIGNAL(timeout()), this, SLOT(vSlotUpdate()));
}

/*
 * destructor
 */
Controller::~Controller()
{
    delete m_poMainTimer;
}

/*
 * update the wallpaper and start the timer
 */
void Controller::vStartTimer(bool _forcecheck)
{
    m_poMainTimer->stop();
    m_poMainTimer->setInterval(m_poSettings->iDelay()*1000);
    vSlotUpdate(_forcecheck);
    m_poMainTimer->start();
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
void Controller::vSlotUpdate(bool _forcecheck)
{
    // update config
    if (_forcecheck || m_poSettings->bCheckFiles())
    {
        m_poSettings->vUpdateSets();
        m_poSettings->vReadNbWalls();
        if (m_poMainWidget != NULL)
        {
            m_poMainWidget->vUpdateList();
        }
    }

    int iTotalFiles = m_poSettings->iNbFiles();

    if (iTotalFiles == 0)
    {
        return;
    }

    // get random files
    QVector<QString> files;
    for (int i=0; i<m_poSettings->iNbWallpapers(); i++)
    {
        files.push_back(sGetRandomFile(iTotalFiles));
    }

    // generate .wallpaper file
    vGenerateFile(files);

    // remove old BMP file
    std::string bmp_file = m_poSettings->sBMPPath().toStdString();
    if (bFileExists(bmp_file))
    {
        remove(bmp_file.c_str());
    }

    // execute UltraMonDesktop
    QString cmd = "\""+m_poSettings->sExePath()+"\" /load";
    cmd+= " \""+m_poSettings->sWallPath()+QString::fromAscii(APP_WALLPAPER_FILE)+"\"";
    QProcess::execute(cmd);
}

/*
 * get a random file among all active sets
 */
QString Controller::sGetRandomFile(int _total)
{
    if (_total == -1)
    {
        _total = m_poSettings->iNbFiles();
    }
    _total--;

    QString sPath = "";
    int iCounter = (int)( ( rand()/(double)RAND_MAX )*_total );
    iCounter = min(iCounter, _total);

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
QString const Controller::sGetStatusText()
{
    QString msg = "<a href='http://www.strangeplanet.fr'>"+QString::fromAscii(APP_NAME)+"</a>";
    msg+= " "+QString::fromAscii(APP_VERSION);
    msg+= " | <a href='http://www.realtimesoft.com/ultramon'>UltraMon</a> "+m_poSettings->sUMVersion();
    return msg;
}


/*
 * generate .wallpaper file
 * _files must contains exactly m_iNbWallpapers elements
 */
void Controller::vGenerateFile(QVector<QString> _files)
{
    if (_files.size() != m_poSettings->iNbWallpapers())
    {
        return;
    }

    std::string file = m_poSettings->sWallPath().toStdString().append(APP_WALLPAPER_FILE);
    int const header_size = 19 + m_poSettings->iNbMonitors()*16;

    std::fstream ofs(file, std::ios::out | std::ios::in | std::ios::binary);

    for (int i=0; i<m_poSettings->iNbWallpapers(); i++)
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

