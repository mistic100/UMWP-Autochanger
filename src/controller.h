#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QTimer>

#include "main.h"
#include "set.h"
#include "settings.h"
#include "environment.h"
#include "versionchecker.h"
#include "wallpapergenerator.h"
#include "directoryscanner.h"

/**
 * @brief Main controller of the application
 */
class Controller : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief The result of a scan & generate mapReduce
     */
    struct ScanAndGenerateResult {
        int done = 0;
        WallpaperGenerator::Result result;
    };

private:
    Settings* m_settings;
    Environment* m_enviro;

    QTimer* m_mainTimer;

    WallpaperGenerator* m_generator;
    DirectoryScanner* m_scanner;
    QFutureWatcher<ScanAndGenerateResult> m_generatorWatcher;

    WallpaperGenerator::Result m_current;

    boolean m_locked;

public:
    Controller(Settings* _settings, Environment* _enviro);
    ~Controller();

    void checkVersion();
    void launchInstaller();

    Settings* settings() const { return m_settings; }
    Environment* enviro() const { return m_enviro; }

    const WallpaperGenerator::Result &current() const { return m_current; }

    const UM::LOCK_TYPE lockEnabled() const {
        return m_settings->param(UM::CONF::lock_enabled).toBool() ?
                    static_cast<UM::LOCK_TYPE>(m_settings->param(UM::CONF::lock_type).toInt()) :
                    UM::LOCK_DISABLED;
    }
    const boolean locked() const { return m_locked; }
    const boolean paused() const { return !m_mainTimer->isActive(); }

public slots:
    void quit();
    void update(bool _async=true);
    bool startPause();
    void clearCache();

    void editSets(const QList<Set*> &_sets, const Set &_data);
    void moveSet(int _from, int _to);
    Set* addSet(const QString &_dirname);
    bool loadConfig(const QString &_file);
    void setActiveSets(const QList<Set*> &_sets);
    void deleteSets(const QList<Set*> &_sets);
    void activateSets(const QList<Set*> &_sets);
    void unactivateSets(const QList<Set*> &_sets);
    void unlock();
    void lock();

private slots:
    void onNewVersion(const UM::NewVersion _version);
    void onGenerationDone();
    void onQuit();

signals:
    void generationStarted();
    void generationFinished();
    void listChanged(bool); // true to reset QListWidget selection
    void startedPaused(bool);
    void newVersionAvailable();
    void lockToggled(bool);
};

#endif // CONTROLLER_H
