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

public:
    Controller(Settings* _data);

    void vCheckVersion();

    Settings* settings() const { return m_poSettings; }

    Set* poGetRandomSet(int _iTotal);
    void vGetRandomFile(Set* _poSet, QVector<QString> &_asFiles);
    void vGenerateFile(const QString &_sFilename, const Set* _poSet, const QVector<QString> &_asFiles);

    void vStartTimer(bool _bKeepPause=false);
    bool bStartPause();

    void vAddSet(const QString _sDirname);
    void vDeleteSets(const QList<int> _ai);
    void vActivateSets(const QList<int> _ai);
    void vUnactivateSets(const QList<int> _ai);
    void vSetOneActiveSet(int _i);
    void vEditSet(int _i, const QString &_sName, const UM::WALLPAPER _iType, const UM::IMAGE _iStyle);
    void vMoveSet(int _from, int _to);

public slots:
    void slotUpdate(bool _bCheckFiles=true);

signals:
    void listChanged(bool);
    void newVersionAvailable(const QString);
};

#endif // CONTROLLER_H
