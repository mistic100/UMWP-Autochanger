#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <random>
#include <QTimer>

using namespace std::tr1;

#include "main.h"
#include "set.h"
#include "settings.h"
#include "environment.h"
#include "versionchecker.h"


/**
 * @brief Main controller of the application
 */
class Controller : public QObject
{
    Q_OBJECT

private:
    Settings*        m_settings;
    Environment*     m_enviro;

    QTimer*          m_mainTimer;
    mt19937          m_randomEngine; // mersenne_twister

    QVector<QString> m_files;
    Set*             m_set;

public:
    Controller(Settings* _settings, Environment* _enviro);

    void checkVersion();

    Settings*       settings() const    { return m_settings; }
    Environment*    enviro() const      { return m_enviro; }

    const QVector<QString> &files() const { return m_files; }
    Set*    currentSet() const          { return m_set; }

    void startTimer();
    bool startPause();
    bool isPaused() const { return !m_mainTimer->isActive(); }

    void launchInstaller();

    void emitListChanged(bool _resetSel=false) { emit listChanged(_resetSel); }

private:
    Set*    getRandomSet(int _total);
    QString getRandomFile(Set* _set);
    void    generateFile(const QString &_filename, const QVector<QString> &_files, const Set* _set);
    QVector<QString> adaptFilesToFillMode(const QVector<QString> &_files, const Set* _set);

public slots:
    void onUpdate();

private slots:
    void onNewVersion(const QString &_ver, const QString &_link);

signals:
    void listChanged(bool); // true to reset QListWidget selection
    void wallpaperChanged();
    void newVersionAvailable();
};

#endif // CONTROLLER_H
