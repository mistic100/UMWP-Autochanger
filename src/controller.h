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
    Settings*        m_settings;
    Environment*     m_enviro;
    WallpaperGenerator* m_generator;

    QTimer*          m_mainTimer;
    QFutureWatcher<WallpaperGenerator::Result> m_generatorWatcher;

    QVector<QString> m_files;
    Set*             m_set;

public:
    Controller(Settings* _settings, Environment* _enviro);
    ~Controller();

    void checkVersion();

    Settings*       settings() const    { return m_settings; }
    Environment*    enviro() const      { return m_enviro; }

    const QVector<QString> &files() const { return m_files; }
    Set*    currentSet() const          { return m_set; }

    bool isPaused() const { return !m_mainTimer->isActive(); }

    void launchInstaller();

    void emitListChanged(bool _resetSel=false) { emit listChanged(_resetSel); }

public slots:
    void quit();
    void update();
    void startPause();

private slots:
    void onNewVersion(const UM::NewVersion _version);
    void onGenerationDone();

signals:
    void generationStarted();
    void generationFinished();
    void listChanged(bool); // true to reset QListWidget selection
    void startedPaused(bool);
    void newVersionAvailable();
};

#endif // CONTROLLER_H
