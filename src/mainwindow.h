#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QStatusBar>
#include <QMenuBar>

#include "main.h"
#include "controller.h"
#include "globalshortcut.h"


/**
 * @brief The main window containing the main widget
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Controller*         m_poCtrl;

    QSystemTrayIcon*    m_poTrayIcon;
    QStatusBar*         m_poStatusBar;
    QMenuBar*           m_poMenuBar;
    QMenu*              m_poTrayQuickMenu;

    // reference to some QAction modified after initilization
    QAction*            m_poActionOptions;
    QAction*            m_poActionHide1; // menu
    QAction*            m_poActionHide2; // tray
    QAction*            m_poActionPause1; // menu
    QAction*            m_poActionPause2; // tray

    QList<GlobalShortcut*> m_apoShortcuts;

public:
    MainWindow(Controller* _poCtrl);
    ~MainWindow();

    void vUpdateHotkeys();
    void vClearHotkeys();

    void vShowMain();
    void vShowError();

protected:
    void showEvent(QShowEvent*);
    void resizeEvent(QResizeEvent* _event);
    void closeEvent(QCloseEvent* _event);

public slots:
    void vInit();

    void vUpdateTrayQuickMenu();

    void slotQuit();
    void slotApply();
    void slotStartPause();
    void slotConfigDialog();
    void slotShowHelp();
    void slotHotkey();

    void slotTrayAction(QSystemTrayIcon::ActivationReason _reason);
    void slotTrayQuickClicked();

    void slotToggleWindow(bool _bForceHide=false);
    void slotDisplayNewVersion(const QString &_sVersion);
};

#endif // MAINWINDOW_H
