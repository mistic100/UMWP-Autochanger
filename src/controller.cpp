#include <iostream>
#include <fstream>

#include "main.h"
#include "controller.h"
#include "gui/mainwidget.h"


/**
 * @brief Controller::Controller
 * @param Settings* _settings
 * @param Environment* _enviro
 */
Controller::Controller(Settings* _settings, Environment* _enviro) :
    QObject(),
    m_settings(_settings),
    m_enviro(_enviro),
    m_set(NULL)
{
    m_generator = new WallpaperGenerator(this);

    m_mainTimer = new QTimer(this);
    connect(m_mainTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}

/**
 * @brief Init version checker thread
 */
void Controller::checkVersion()
{
    if (m_settings->get("check_updates").toBool())
    {
        VersionChecker* checker = new VersionChecker();
        connect(checker, SIGNAL(newVersionAvailable(const NewVersion)), this, SLOT(onNewVersion(const NewVersion)));

        QThread* thread = new QThread(this);
        checker->moveToThread(thread);

        connect(thread, SIGNAL(started()), checker, SLOT(run()));
        connect(checker, SIGNAL(finished()), thread, SLOT(quit()));
        connect(checker, SIGNAL(finished()), checker, SLOT(deleteLater()));

        qxtLog->trace("Start version checker thread");
        QTimer::singleShot(500, thread, SLOT(start()));
    }
}

/**
 * @brief Called when a new version is available
 * @param string _version
 * @param string _link
 */
void Controller::onNewVersion(const NewVersion _version)
{
    m_enviro->setNewVersion(_version);
    emit newVersionAvailable();
}

/**
 * @brief Launch installer.exe if existing and close the app
 */
void Controller::launchInstaller()
{
    QString path = QDir::currentPath() +"/"+ QString::fromAscii(APP_INSTALLER_FILENAME);

    if (QFile::exists(path))
    {
        QProcess::startDetached("\""+ path +"\" -delete-installer");
        qApp->quit();
    }
}

/**
 * @brief Stop the timer, update the delay and restart the timer
 */
void Controller::startTimer()
{
    qxtLog->info("Start timer");

    m_mainTimer->stop();
    m_mainTimer->setInterval(m_settings->get("delay").toInt()*1000);
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
void Controller::onUpdate(bool _forceRefresh)
{
    qxtLog->info("Update !");

    // update delay if needed
    int delay = m_settings->get("delay").toInt()*1000;
    if (delay != m_mainTimer->interval())
    {
        qxtLog->debug("Timer delay changed to: "+QString::number(delay));
        m_mainTimer->setInterval(delay);
    }

    // update config
    if (_forceRefresh || m_settings->get("check").toBool())
    {
        m_settings->updateSets();
        m_enviro->refreshMonitors();
        emit listChanged(false);
    }

    // get random files
    m_set = m_generator->getRandomSet();

    if (m_set == NULL)
    {
        qxtLog->warning("No active set");
        return;
    }

    qxtLog->debug("Current set: "+m_set->name());

    m_files = m_generator->getFiles(m_set);

    if (qxtLog->isLogLevelEnabled("debug", QxtLogger::DebugLevel))
    {
        foreach (QString file, m_files)
        {
            if (!file.isEmpty())
            {
                qxtLog->debug("Current file: "+file);
            }
        }
    }

    QVector<QString> tempFiles = m_generator->adaptFiles(m_set, m_files);

    QString filename = m_enviro->get("wallpath").toString() + QString::fromAscii(APP_WALLPAPER_FILE);

    // generate .wallpaper file
    generateFile(filename, tempFiles, m_set);

    // remove old BMP file
    QFile::remove(m_enviro->get("bmppath").toString());

    // execute UltraMonDesktop
    QString cmd = "\"" + m_settings->get("umpath").toString() + "\" /load \"" + filename + "\"";
    QProcess::startDetached(cmd);

    qxtLog->trace("Launch UltraMonDesktop");

    emit wallpaperChanged();
}

/**
 * @brief Generate AutoChanger.wallpaper file
 * @param string _filename
 * @param string[] _files
 * @param Set* _set
 */
void Controller::generateFile(const QString &_filename, const QVector<QString> &_files, const Set* _set)
{
    // get header from default.wallpaper
    QByteArray buffer(m_enviro->header());

    // write wallpaper type
    UM::WALLPAPER wp_type = _set->type();
    buffer.append((char*)&wp_type, sizeof(UM::WALLPAPER));

    // write number of wallpapers
    DWORD nb_walls = _files.size();
    buffer.append((char*)&nb_walls, sizeof(DWORD));

    UM::IMAGE wp_style = _set->style();
    if (wp_style == UM::IM_FILL)
    {
        wp_style = UM::IM_STRETCH_PROP;
    }

    // write wallpapers
    for (unsigned int i=0; i<nb_walls; i++)
    {
        UM::WP_MONITOR_FILE wall;
        wall.bgType = UM::BG_SOLID;
        wall.color1 = m_settings->monitor(i).color;
        wall.color2 = 0x00000000;
        wall.imgStyle = wp_style;

        memset(wall.imgFile, 0, 260*sizeof(wchar_t));

        if (!_files.at(i).isEmpty())
        {
            _files.at(i).toWCharArray((wchar_t*)wall.imgFile);
        }

        buffer.append((char*)&wall, sizeof(UM::WP_MONITOR_FILE));
    }

    // save file
    QFile file(_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(buffer);
    file.close();
}

/**
 * @brief Move a file to trash bin
 * @param string _filename
 * @return bool
 */
bool Controller::moveFileToTrash(const QString &_filename)
{
    wchar_t path[MAX_PATH];
    memset(path, 0, sizeof(path));
    int l = _filename.toWCharArray(path);
    path[l] = '\0';

    SHFILEOPSTRUCT shfos = {0};
    shfos.wFunc  = FO_DELETE;
    shfos.pFrom  = path;
    shfos.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

    int ret = SHFileOperation(&shfos);

    return ret == 0;
}
