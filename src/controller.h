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
    Settings*       m_poSettings;
    QTimer*         m_poMainTimer;
    mt19937         m_oRandom; // mersenne_twister
    int             m_iHeaderSize;
    QVector<QString> m_asFiles;

public:
    Controller(Settings* _data);

    void vCheckVersion();

    Settings* settings() const { return m_poSettings; }
    const QVector<QString> &files() const { return m_asFiles; }

    Set*    poGetRandomSet(int _iTotal);
    QString sGetRandomFile(Set* _poSet);
    void    vGenerateFile(const QString &_sFilename, const Set* _poSet);

    void vStartTimer(bool _bKeepPause=false);
    bool bStartPause();
    bool bIsPaused() const { return !m_poMainTimer->isActive(); }

    void vAddSet(const QString _sDirname);
    void vDeleteSets(const QList<int> _ai);
    void vActivateSets(const QList<int> _ai);
    void vUnactivateSets(const QList<int> _ai);
    void vSetActiveSets(const QList<int> _ai);
    void vSetOneActiveSet(int _i);
    void vEditSet(int _i, const QString &_sName, const UM::WALLPAPER _iType, const UM::IMAGE _iStyle, const Hotkey _oHotkey);
    void vMoveSet(int _from, int _to);

public slots:
    void slotUpdate(bool _bCheckFiles=true);

signals:
    void listChanged(bool); // true to reset QListWidget selection
    void newVersionAvailable(const QString);
};

#endif // CONTROLLER_H
