#include <QtConcurrentRun>

#include "main.h"
#include "controller.h"
#include "gui/mainlist.h"


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
    if (m_settings->param(UM::CONF::check_updates).toBool())
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
    QString path = QDir::toNativeSeparators(Environment::APPDATA_DIR + APP_INSTALLER_FILENAME);

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

    // restart timer
    m_mainTimer->start(m_settings->param(UM::CONF::delay).toInt()*1000);
    emit startedPaused(true);

    // update config
    m_enviro->refreshMonitors();
    m_settings->check();

    m_settings->updateSets();
    emit listChanged(false);

    m_files.clear();

    emit generationStarted();

    QFuture<WallpaperGenerator::Result> future = QtConcurrent::run(m_generator, &WallpaperGenerator::generate);
    m_generatorWatcher.setFuture(future);
}

/**
 * @brief Action once async wallpaper generation is done
 */
void Controller::onGenerationDone()
{
    WallpaperGenerator::Result result = m_generatorWatcher.future().result();

    if (result.set != NULL)
    {
        m_set = result.set;
        m_files = result.files;
    }

    emit generationFinished();
}

/**
 * @brief Move a set
 * @param int _from
 * @param int _to
 */
void Controller::moveSet(int _from, int _to)
{
    m_settings->moveSet(_from, _to);

    emit listChanged(true);
}

/**
 * @brief Add a new set
 * @param string _dirname
 */
void Controller::addSet(const QString &_dirname)
{
    QDir dir(_dirname);
    dir.cdUp();
    m_settings->setParam(UM::CONF::last_dir, dir.absolutePath());

    m_settings->addSet(QString(_dirname).replace('/', '\\'));

    emit listChanged(true);
}

/**
 * @brief Load config from file
 * @param string _file
 * @return bool
 */
bool Controller::loadConfig(const QString &_file)
{
    if (m_settings->load(_file))
    {
        m_settings->check();
        m_settings->save();

        emit listChanged(true);
        update();

        return true;
    }
    else
    {
        QLOG_ERROR() << "Invalid settings file";

        return false;
    }
}

/**
 * @brief Change which sets are active
 * @param int[] _idx
 */
void Controller::setActiveSets(const QList<int> &_idx)
{
    QList<Set*> sets;

    foreach (const int i, _idx)
    {
        sets.append(m_settings->set(i));
    }

    m_settings->setActiveSets(sets);

    update();
}

/**
 * @brief Delete sets
 * @param Set*[] _sets
 */
void Controller::deleteSets(const QList<Set*> &_sets)
{
    m_settings->deleteSets(_sets);

    emit listChanged(true);
}

/**
 * @brief Activate sets
 * @param Set*[] _sets
 */
void Controller::activateSets(const QList<Set*> &_sets)
{
    m_settings->activateSets(_sets);

    emit listChanged(false);
}

/**
 * @brief Deactivate sets
 * @param Set*[] _sets
 */
void Controller::unactivateSets(const QList<Set*> &_sets)
{
    m_settings->unactivateSets(_sets);

    emit listChanged(false);
}

/**
 * @brief Edit sets
 * @param Set*[] _sets
 * @param Set _data
 */
void Controller::editSets(const QList<Set*> &_sets, const Set &_data)
{
    m_settings->editSets(_sets, _data);

    emit listChanged(false);
}
