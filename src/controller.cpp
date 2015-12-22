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
    m_enviro(_enviro)
{
    m_generator = new WallpaperGenerator(this);
    m_set = NULL;

    m_mainTimer = new QTimer(this);
    m_mainTimer->setInterval(m_settings->get("delay").toInt()*1000);
    connect(m_mainTimer, SIGNAL(timeout()), this, SLOT(update()));
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

        QLOG_TRACE() << "Start version checker thread";
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
    QString path = QDir::currentPath() +"/"+ APP_INSTALLER_FILENAME;

    if (QFile::exists(path))
    {
        QProcess::startDetached("\""+ path +"\" -delete-installer");
        qApp->quit();
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
        QLOG_INFO() << "Pause timer";
        m_mainTimer->stop();
    }
    else
    {
        QLOG_INFO() << "Restart timer";
        m_mainTimer->start();
    }

    emit startedPaused(m_mainTimer->isActive());

    return m_mainTimer->isActive();
}

/**
 * @brief Update the wallpaper
 */
void Controller::update()
{
    QLOG_INFO() << "Update !";

    // refresh timer interval
    if (m_mainTimer->isActive()) {
        m_mainTimer->start(m_settings->get("delay").toInt()*1000);
    }

    // update config
    m_settings->updateSets();
    m_enviro->refreshMonitors();
    emit listChanged(false);
    m_files.clear();

    // get random files
    m_set = m_generator->getRandomSet();

    if (m_set == NULL)
    {
        QLOG_WARN() << "No active set";
        return;
    }

    QLOG_DEBUG() << "Current set:" << m_set->name() << "Type:" << m_set->type() << "Style:" << m_set->style();

    m_files = m_generator->getFiles(m_set);
    QLOG_DEBUG() << "Current files:" << m_files;

    QVector<QString> tempFiles = m_generator->adaptFiles(m_set, m_files);

    QString wallpaper = m_generator->generateFile(m_set, tempFiles);

    m_enviro->setWallpaper(wallpaper);

    emit wallpaperChanged();
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
