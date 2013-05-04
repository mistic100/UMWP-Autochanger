#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QStatusBar>
#include <QMenuBar>

#include "main.h"
#include "controller.h"


/*
 * Name: MainWindows
 * Description: The main window containing the main widget
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Controller*         m_poCtrl;

    QSystemTrayIcon*    m_poTrayIcon;
    QStatusBar*         m_poStatusBar;
    QMenuBar*           m_poMenuBar;
    QMenu*              m_poOptionsMenu;

    // reference to some QAction modified after initilization
    QAction*            m_poActionHide1; // menu
    QAction*            m_poActionHide2; // tray
    QAction*            m_poActionPause1; // menu
    QAction*            m_poActionPause2; // tray

public:
    MainWindow(Controller* _oCtrl);

    void vInit();

protected:
    void showEvent(QShowEvent*) { resize(m_poCtrl->settings()->oWindowSize()); }
    void resizeEvent(QResizeEvent* _event) { m_poCtrl->settings()->vSetWindowSize(_event->size()); }
    void closeEvent(QCloseEvent* _event);

public slots:
    void vSlotQuit();
    void vSlotApply();
    void vSlotStartPause();
    void vSlotShowHelp();

    void vSlotTrayAction(QSystemTrayIcon::ActivationReason _reason);
    void vSlotToggleWindow(bool _forcehide=false);

    void vSlotOptionMinimizeChanged(bool _a);
    void vSlotOptionCheckChanged(bool _a);
    void vSlotOptionAutostartChanged(bool _a);
};

#endif // MAINWINDOW_H
