#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <random>
#include <QTimer>

using namespace std::tr1;

#include "main.h"
#include "set.h"
#include "settings.h"


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

    Settings* const &settings() const { return m_poSettings; }

    bool const bIsRunning() const { return m_poMainTimer->isActive(); }
    QString const sGetRandomFile(int _total=-1);
    QString const sGetStatusText();

    void vStartTimer(bool _forcecheck=false);
    void vStartPause();
    void vGenerateFile(QVector<QString> _files);

public slots:
    void vSlotUpdate(bool _forcecheck=false);

signals:
    void listChanged();
};

#endif // CONTROLLER_H
