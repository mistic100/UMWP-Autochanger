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
    MainWindow* m_parent;

    QAction* m_actionHide;
    QAction* m_actionPause;
    QAction* m_actionLock;
    QAction* m_actionUnlock;
    QMenu* m_quickMenu;

public:
    TrayIcon(MainWindow* _parent, Controller* _ctrl);

private:
    void updateIcon();

public slots:
    void onListChanged();

private slots:
    void onStartPause(bool _start);
    void setLocked(bool _locked);
    void setLockEnabled(bool _lockEnabled);
    void onQuickClicked();
    void onActivated(QSystemTrayIcon::ActivationReason _reason);
    void onWindowShown(bool _visible);
};

#endif // TRAYMENU_H
