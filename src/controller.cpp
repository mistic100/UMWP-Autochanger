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

    m_settings = _pSettings;

    m_mainTimer = new QTimer(this);
    connect(m_mainTimer, SIGNAL(timeout()), this, SLOT(slotUpdate()));
}

/**
 * @brief Init version checker thread
 */
void Controller::checkVersion()
{
    if (m_settings->bParam("check_updates"))
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
    bool wasPaused = !m_mainTimer->isActive();

    m_mainTimer->stop();
    m_mainTimer->setInterval(m_settings->iParam("delay")*1000);

    slotUpdate();

    if (!wasPaused || !_keepPause)
    {
        m_mainTimer->start();
    }
}

/**
 * @brief Pause or start the timer
 * @return bool - true if the timer is running
 */
bool Controller::startPause()
{
    if (m_mainTimer->isActive())
    {
        m_mainTimer->stop();
    }
    else
    {
        m_mainTimer->start();
    }

    return m_mainTimer->isActive();
}

/**
 * @brief Update the wallpaper
 * @param bool _bCheckFiles - if true, depends on configuration
 */
void Controller::slotUpdate(bool _checkFiles)
{
    // update config
    if (_checkFiles && m_settings->bParam("check"))
    {
        m_settings->updateSets();
        m_settings->readNbMonitors();
        emit listChanged(false);
    }

    int totalSets = m_settings->nbActiveSets(true);

    if (totalSets == 0)
    {
        return;
    }

    // get random files
    Set* pSet = getRandomSet(totalSets);

    m_files.clear();

    if (pSet->type() == 1)
    {
        for (int i=0, l=m_settings->iEnv("nb_monitors"); i<l; i++)
        {
            m_files.push_back(getRandomFile(pSet));
        }
    }
    else
    {
        m_files.push_back(getRandomFile(pSet));
    }

    QString filename = m_settings->sEnv("wallpath") + QString::fromAscii(APP_WALLPAPER_FILE);

    // generate .wallpaper file
    generateFile(filename, pSet);

    // remove old BMP file
    if (fileExists(m_settings->sEnv("bmppath")))
    {
        QFile::remove(m_settings->sEnv("bmppath"));
    }

    // execute UltraMonDesktop
    QString cmd = "\"" + m_settings->sParam("umpath") + "\" /load \"" + filename + "\"";
    QProcess::execute(cmd);
}

/**
 * @brief Get a random Set among all active sets
 * @param int _iTotal - total number of Sets
 * @return Set*
 */
Set* Controller::getRandomSet(int _total)
{
    if (_total == 1)
    {
        return m_settings->getActiveSet(0);
    }

    uniform_int<int> unif(0, _total-1);
    int counter = unif(m_randomEngine);

    return m_settings->getActiveSet(counter);
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
    if (total <= m_files.size())
    {
        uniform_int<int> unif(0, m_files.size()-1);
        int counter = unif(m_randomEngine);
        return m_files.at(counter);
    }

    // search a random unused file
    short loop = 10;
    QString file;
    uniform_int<int> unif(0, total-1);

    while (loop > 0)
    {
        int counter = unif(m_randomEngine);
        file = _poSet->getFile(counter);

        if (!m_files.contains(file))
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
    QString defaultFilename = m_settings->sEnv("wallpath") + "default.wallpaper";
    QFile defaultFile(defaultFilename);
    defaultFile.open(QIODevice::ReadOnly);

    // get header from default.wallpaper
    QByteArray buffer = defaultFile.readAll();
    defaultFile.close();
    buffer.truncate(m_settings->iEnv("header_size"));

    // write wallpaper type
    UM::WALLPAPER wp_type = _pSet->type();
    buffer.append((char*)&wp_type, sizeof(UM::WALLPAPER));

    // write number of wallpapers
    DWORD nb_walls = m_files.size();
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
        m_files.at(i).toWCharArray((wchar_t*)wall.imgFile);

        buffer.append((char*)&wall, sizeof(UM::WP_MONITOR_FILE));
    }

    // save file
    QFile file(_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(buffer);
    file.close();
}
