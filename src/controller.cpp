#include <ctime>
#include <iostream>
#include <fstream>

#include "dirent.h"
#include "main.h"
#include "controller.h"
#include "mainwidget.h"


/**
 * @brief Controller::Controller
 * @param Settings* _settings
 */
Controller::Controller(Settings* _settings) : QObject(0)
{
    m_randomEngine.seed((unsigned int)time(NULL));

    m_settings = _settings;

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

        qxtLog->trace("Start version checker thread");
        QTimer::singleShot(1000, pVCThread, SLOT(start()));
    }
}

/**
 * @brief Stop the timer, update the wallpaper and restart the timer
 */
void Controller::startTimer()
{
    qxtLog->info("Start timer");
    m_mainTimer->stop();
    m_mainTimer->setInterval(m_settings->iParam("delay")*1000);
    m_mainTimer->start();
}

/**
 * @brief Pause or start the timer
 * @return bool - true if the timer is running
 */
bool Controller::startPause()
{
    if (m_mainTimer->isActive())
    {
        qxtLog->info("Pause timer");
        m_mainTimer->stop();
    }
    else
    {
        qxtLog->info("Restart timer");
        m_mainTimer->start();
    }

    return m_mainTimer->isActive();
}

/**
 * @brief Update the wallpaper
 */
void Controller::slotUpdate()
{
    qxtLog->info("Update !");

    // update delay if needed
    int delay = m_settings->iParam("delay")*1000;
    if (delay != m_mainTimer->interval())
    {
        qxtLog->debug("Timer delay changed to: "+delay);
        m_mainTimer->setInterval(delay);
    }

    // update config
    if (m_settings->bParam("check"))
    {
        m_settings->updateSets();
        m_settings->readNbMonitors();
        emit listChanged(false);
    }

    int totalSets = m_settings->nbActiveSets(true);

    if (totalSets == 0)
    {
        qxtLog->warning("No active set");
        return;
    }

    // get random files
    Set* set = getRandomSet(totalSets);
    qxtLog->debug("Current set: "+set->name());

    m_files.clear();

    if (set->type() == 1)
    {
        for (int i=0, l=m_settings->iEnv("nb_monitors"); i<l; i++)
        {
            m_files.push_back(getRandomFile(set));
        }
    }
    else
    {
        m_files.push_back(getRandomFile(set));
    }

    QString filename = m_settings->sEnv("wallpath") + QString::fromAscii(APP_WALLPAPER_FILE);

    // generate .wallpaper file
    generateFile(filename, set);

    // remove old BMP file
    QFile::remove(m_settings->sEnv("bmppath"));

    // execute UltraMonDesktop
    QString cmd = "\"" + m_settings->sParam("umpath") + "\" /load \"" + filename + "\"";
    QProcess::startDetached(cmd);

    qxtLog->trace("Launch UltraMonDesktop");
}

/**
 * @brief Get a random Set among all active sets
 * @param int _total - total number of Sets
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
 * @param Set* _set
 * @return string
 */
QString Controller::getRandomFile(Set* _set)
{
    int total = _set->count();

    // only one file in the set ?!
    if (total == 1)
    {
        return _set->getFile(0);
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
        file = _set->getFile(counter);

        if (!m_files.contains(file))
        {
            loop = 0;
        }

        loop--;
    }

    qxtLog->debug("Current file: "+file);
    return file;
}

/**
 * @brief Generate AutoChanger.wallpaper file
 * @param string _filename
 * @param Set* _set
 */
void Controller::generateFile(const QString &_filename, const Set* _set)
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
    UM::WALLPAPER wp_type = _set->type();
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
        wall.imgStyle = _set->style();
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
