#include <QtConcurrentRun>

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

    connect(&m_generatorWatcher, SIGNAL(finished()), this, SLOT(onGenerationDone()));
}

/**
 * @brief Controller::~Controller
 */
Controller::~Controller()
{
    delete m_generator;
}

/**
 * @brief Try to quit the app. If the WallpaperGenerator is running, try again in 500 ms
 */
void Controller::quit()
{
    if (m_generatorWatcher.isRunning())
    {
        QTimer::singleShot(500, this, SLOT(quit()));
    }
    else
    {
        qApp->quit();
    }
}

/**
 * @brief Init version checker thread
 */
void Controller::checkVersion()
{
    if (m_settings->get("check_updates").toBool())
    {
        VersionChecker* checker = new VersionChecker();
        connect(checker, SIGNAL(newVersionAvailable(const UM::NewVersion)), this, SLOT(onNewVersion(const UM::NewVersion)));

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
void Controller::onNewVersion(const UM::NewVersion _version)
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
        quit();
    }
}

/**
 * @brief Pause or start the timer
 * @return bool - true if the timer is running
 */
void Controller::startPause()
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
}

/**
 * @brief Update the wallpaper
 */
void Controller::update()
{
    QLOG_INFO() << "Update !";

    // restart timer
    m_mainTimer->start(m_settings->get("delay").toInt()*1000);
    emit startedPaused(true);

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

    emit generationStarted();

    QFuture<WallpaperGenerator::Result> future = QtConcurrent::run(m_generator, &WallpaperGenerator::generate, m_set);
    m_generatorWatcher.setFuture(future);
}

/**
 * @brief Action once async wallpaper generation is done
 */
void Controller::onGenerationDone()
{
    WallpaperGenerator::Result result = m_generatorWatcher.future().result();

    m_enviro->setWallpaper(result.wallpaper);
    m_files = result.files;

    emit generationFinished();
}
