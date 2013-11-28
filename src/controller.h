#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <random>
#include <QTimer>

using namespace std::tr1;

#include "main.h"
#include "set.h"
#include "settings.h"
#include "versionchecker.h"


/*
 * Name: Controller
 * Description: main controller of the application
 */
class Controller : public QObject
{
    Q_OBJECT

private:
    Settings*       m_poSettings;

    QTimer*         m_poMainTimer;

    mt19937         m_oRandom; // mersenne_twister

public:
    Controller(Settings* _data);

    void vStart();

    Settings* const settings() const { return m_poSettings; }

    Set*    poGetRandomSet(int _iTotal);
    QString sGetRandomFile(Set* _poSet, QVector<QString> const &_sFiles);

    void vStartTimer(bool _keepPause=false);
    bool bStartPause();
    void vGenerateFile(QVector<QString> _files);

    void vAddSet(QString const _dirname);
    void vDeleteSets(QList<int> const _list);
    void vActivateSets(QList<int> const _list);
    void vUnactivateSets(QList<int> const _list);
    void vSetOneActiveSet(int _idx);
    void vRenameSet(int _idx, QString const _name);
    void vMoveSet(int _from, int _to);

public slots:
    void vSlotUpdate(bool _check=true);

signals:
    void listChanged(bool);
    void newVersionAvailable(QString);
};

#endif // CONTROLLER_H
