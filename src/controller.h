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
    Settings*           m_pSettings;
    QTimer*             m_pMainTimer;
    mt19937             m_randomEngine; // mersenne_twister
    int                 m_headerSize;
    QVector<QString>    m_aFiles;

public:
    Controller(Settings* _pSettings);

    void checkVersion();

    Settings* pSettings() const { return m_pSettings; }
    const QVector<QString> &aFiles() const { return m_aFiles; }

    Set*    pGetRandomSet(int _total);
    QString getRandomFile(Set* _pSet);
    void    generateFile(const QString &_filename, const Set* _pSet);

    void startTimer(bool _keepPause=false);
    bool startPause();
    bool isPaused() const { return !m_pMainTimer->isActive(); }

    void addSet(const QString _dirname);
    void deleteSets(const QList<int> _aSets);
    void activateSets(const QList<int> _aSets);
    void unactivateSets(const QList<int> _aSets);
    void setActiveSets(const QList<int> _aSets);
    void setOneActiveSet(int _set);
    void editSet(int _set, const QString &_name, const UM::WALLPAPER _type, const UM::IMAGE _style, const int _hotkey);
    void moveSet(int _from, int _to);

    void emitListChanged(bool _resetSel) { emit listChanged(_resetSel); }

public slots:
    void slotUpdate(bool _checkFiles=true);

signals:
    void listChanged(bool); // true to reset QListWidget selection
    void newVersionAvailable(const QString);
};

#endif // CONTROLLER_H
