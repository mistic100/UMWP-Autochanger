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
    QThread*        m_poVCThread;

    mt19937         m_oRandom; // mersenne_twister

public:
    Controller(Settings* _data);

    Settings* const settings() const { return m_poSettings; }

    bool const      bIsRunning() const { return m_poMainTimer->isActive(); }
    QString const   sGetRandomFile(int _total=-1);
    QString const   sGetStatusText() const;

    void vStartTimer(bool _keepPause=false);
    void vStartPause();
    void vGenerateFile(QVector<QString> _files);

public slots:
    void vSlotUpdate();
    void vSlotNewVersion(QString _ver);

signals:
    void listChanged();
    void newVersionAvailable(QString);
};

#endif // CONTROLLER_H
