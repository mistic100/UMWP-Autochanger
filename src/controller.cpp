#include <QtConcurrentMap>
#include <functional>

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
    m_locked = false;

    m_generator = new WallpaperGenerator(this);
    m_scanner = new DirectoryScanner(this);

    m_mainTimer = new QTimer(this);
    connect(m_mainTimer, SIGNAL(timeout()), this, SLOT(update()));

    connect(&m_generatorWatcher, SIGNAL(finished()), this, SLOT(onGenerationDone()));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(onQuit()));

    if (lockEnabled() == UM::LOCK_ALL && m_settings->param(UM::CONF::lock_startup).toBool())
    {
        lock();
    }
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
 * @brief Activate default set on quit
 */
void Controller::onQuit()
{
    if (!m_settings->param(UM::CONF::default_set).toString().isEmpty())
    {
        Set* set = m_settings->setByUuid(m_settings->param(UM::CONF::default_set).toString());

        if (set != NULL && (m_settings->nbActiveSets() > 1 || set != m_settings->activeSet(0)))
        {
            QLOG_DEBUG() << "Set default close set";
            m_settings->setActiveSets(QList<Set*>() << set);
            update(false);
        }
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
    QString path = QDir::toNativeSeparators(QDir::tempPath() + "/" + APP_INSTALLER_FILENAME);

    if (QFile::exists(path))
    {
        QProcess::startDetached("\""+ path +"\" -delete-installer");
        quit();
    }
    else
    {
        QLOG_ERROR()<<"Installer file not found";
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

    emit startedPaused(!paused());
    return m_mainTimer->isActive();
}

/**
 * @brief Update the wallpaper
 */
void Controller::update(bool _async)
{
    QLOG_INFO() << "Update !";

    // restart timer
    m_mainTimer->start(m_settings->param(UM::CONF::delay).toInt()*1000);
    emit startedPaused(true);

    // update config
    m_enviro->refreshMonitors();
    m_settings->check();

    emit generationStarted();

    QList<Set*> sets;
    for (int i = 0; i < m_settings->nbSets(); i++)
    {
        Set* set = m_settings->set(i);
        if (set->isActive() && set->isValid())
        {
            sets.append(set);
        }
    }

    std::function<bool(Set* _set)> scan = [this] (Set* _set) {
        return m_scanner->scan(_set);
    };

    auto generate = [this, sets] (ScanAndGenerateResult &_result, const bool &) {
        _result.done++;
        if (_result.done == sets.size())
        {
            QLOG_INFO() << "Scan done, begin generation";
            _result.result = m_generator->generate();
        }
    };

    if (_async)
    {
        QFuture<ScanAndGenerateResult> future = QtConcurrent::mappedReduced<ScanAndGenerateResult>(sets, scan, generate);
        m_generatorWatcher.setFuture(future);
    }
    else
    {
        ScanAndGenerateResult result;
        foreach (auto set, sets)
        {
            generate(result, scan(set));
        }

        m_current = result.result;
        emit generationFinished();
        emit listChanged(false);
    }
}

/**
 * @brief Action once async wallpaper generation is done
 */
void Controller::onGenerationDone()
{
    m_current = m_generatorWatcher.future().result().result;

    emit generationFinished();
    emit listChanged(false);
}

/**
 * @brief Clear sets cache of selected sets
 */
void Controller::clearCache()
{
    for (int i=0, l=m_settings->nbSets(); i<l; i++)
    {
        m_settings->set(i)->deleteCache();
    }
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
Set* Controller::addSet(const QString &_dirname)
{
    QDir dir(_dirname);
    dir.cdUp();
    m_settings->setParam(UM::CONF::last_dir, dir.absolutePath());

    Set* set = m_settings->addSet(QString(_dirname).replace('/', '\\'));

    emit listChanged(true);

    return set;
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
 * @param Set*[] _sets
 */
void Controller::setActiveSets(const QList<Set*> &_sets)
{
    m_settings->setActiveSets(_sets);

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

/**
 * @brief Unlock the app
 */
void  Controller::unlock()
{
    if (lockEnabled() == UM::LOCK_ALL)
    {
        m_locked = false;

        QLOG_DEBUG()<<"Unlock";

        emit lockToggled(false);
    }
}

/**
 * @brief Lock the app
 */
void  Controller::lock()
{
    if (lockEnabled() == UM::LOCK_ALL)
    {
        m_locked = true;

        QLOG_DEBUG()<<"Lock";

        emit lockToggled(true);
    }
}
