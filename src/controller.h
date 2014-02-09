#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <random>
#include <QTimer>

using namespace std::tr1;

#include "main.h"
#include "set.h"
#include "settings.h"
#include "versionchecker.h"


/**
 * @brief Main controller of the application
 */
class Controller : public QObject
{
    Q_OBJECT

private:
    Settings*        m_settings;
    QTimer*          m_mainTimer;
    mt19937          m_randomEngine; // mersenne_twister
    QVector<QString> m_files;
    Set*             m_set;

public:
    Controller(Settings* _settings);

    void checkVersion();

    Settings* settings() const { return m_settings; }
    const QVector<QString> &files() const { return m_files; }
    Set* set() const { return m_set; }

    Set*    getRandomSet(int _total);
    QString getRandomFile(Set* _set);
    void    generateFile(const QString &_filename, const QVector<QString> &_files, const Set* _set);

    void startTimer();
    bool startPause();
    bool isPaused() const { return !m_mainTimer->isActive(); }

    void emitListChanged(bool _resetSel=false) { emit listChanged(_resetSel); }

public slots:
    void slotUpdate();

signals:
    void listChanged(bool); // true to reset QListWidget selection
    void newVersionAvailable(const QString);
};

#endif // CONTROLLER_H
