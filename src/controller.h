#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QTimer>

#include "main.h"
#include "set.h"
#include "settings.h"
#include "environment.h"
#include "versionchecker.h"
#include "wallpapergenerator.h"


/**
 * @brief Main controller of the application
 */
class Controller : public QObject
{
    Q_OBJECT

private:
    Settings* m_settings;
    Environment* m_enviro;

    QTimer* m_mainTimer;

    WallpaperGenerator* m_generator;
    QFutureWatcher<WallpaperGenerator::Result> m_generatorWatcher;

    QVector<QString> m_files;
    Set* m_set;

    boolean m_locked;

public:
    Controller(Settings* _settings, Environment* _enviro);
    ~Controller();

    void checkVersion();
    void launchInstaller();

    Settings* settings() const { return m_settings; }
    Environment* enviro() const { return m_enviro; }

    const QVector<QString> &currentFiles() const { return m_files; }
    const Set* currentSet() const { return m_set; }

    const boolean lockEnabled() const { return m_settings->param(UM::CONF::lock_enabled).toBool(); }
    const boolean locked() const { return m_locked; }
    const boolean paused() const { return !m_mainTimer->isActive(); }

public slots:
    void quit();
    void update();
    bool startPause();

    void editSets(const QList<Set*> &_sets, const Set &_data);
    void moveSet(int _from, int _to);
    void addSet(const QString &_dirname);
    bool loadConfig(const QString &_file);
    void setActiveSets(const QList<int> &_idx);
    void deleteSets(const QList<Set*> &_sets);
    void activateSets(const QList<Set*> &_sets);
    void unactivateSets(const QList<Set*> &_sets);
    void unlock();
    void lock();

private slots:
    void onNewVersion(const UM::NewVersion _version);
    void onGenerationDone();

signals:
    void generationStarted();
    void generationFinished();
    void listChanged(bool); // true to reset QListWidget selection
    void startedPaused(bool);
    void newVersionAvailable();
    void lockToggled(bool);
    boolean requestUnlock();
};

#endif // CONTROLLER_H
