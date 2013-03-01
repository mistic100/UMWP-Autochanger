#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QTimer>
#include "main.h"
#include "set.h"
#include "settings.h"

class MainWidget;

/*
 * controller + data of the software
 */
class Controller : public QObject
{
    Q_OBJECT

private:
    Settings*       m_poSettings;
    QTimer*         m_poMainTimer;

public:
    Controller(Settings* _data);

    Settings* const &settings() { return m_poSettings; }

    bool    const bIsRunning() { return m_poMainTimer->isActive(); }
    void    vStartTimer(bool _forcecheck=false);
    void    vStartPause();
    QString sGetRandomFile(int _total=-1);
    QString const sGetStatusText();
    void    vGenerateFile(QVector<QString> _files);

public slots:
    void    vSlotUpdate(bool _forcecheck=false);

signals:
    void listChanged();
};

#endif // CONTROLLER_H
