#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QSystemTrayIcon>
#include <QAction>

#include "../controller.h"


// cyclic reference
class MainWindow;

/**
 * @brief Main system tray icon
 */
class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

private:
    Controller* m_ctrl;

    QAction* m_actionHide;
    QAction* m_actionPause;
    QMenu* m_quickMenu;

public:
    TrayIcon(MainWindow* _parent, Controller* _ctrl);

public slots:
    void onListChanged();

private slots:
    void onStartPause(bool _start);
    void onQuickClicked();
    void onActivated(QSystemTrayIcon::ActivationReason _reason);
    void onWindowShown(bool _visible);
};

#endif // TRAYMENU_H
