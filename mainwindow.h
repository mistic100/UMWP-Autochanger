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
 * the main window containing the main widget
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Controller*         m_poCtrl;
    QWidget*            m_poCentralWidget;
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
    ~MainWindow();

    void vInit();

    void showEvent(QShowEvent*) { resize(m_poCtrl->settings()->oWindowSize()); }
    void resizeEvent(QResizeEvent* _event) { m_poCtrl->settings()->vSetWindowSize(_event->size()); }
    void closeEvent(QCloseEvent* _event);

public slots:
    void vSlotTrayAction(QSystemTrayIcon::ActivationReason _reason);
    void vSlotQuit();
    void vSlotApply();
    void vSlotToggleWindow(bool _forcehide=false);
    void vSlotStartPause();
    void vSlotShowHelp();
    void vSlotOptionMinimizeChanged(bool _a);
    void vSlotOptionCheckChanged(bool _a);
};

#endif // MAINWINDOW_H
