#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QStatusBar>
#include <QMenuBar>

#include "main.h"
#include "controller.h"


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
    QMenu*              m_poOptionsMenu;
    QMenu*              m_poTrayQuickMenu;

    // reference to some QAction modified after initilization
    QAction*            m_poActionHide1; // menu
    QAction*            m_poActionHide2; // tray
    QAction*            m_poActionPause1; // menu
    QAction*            m_poActionPause2; // tray

public:
    MainWindow(Controller* _poCtrl);

    void vUpdateHotkeys();

    void vShowMain();
    void vShowError();

protected:
    void showEvent(QShowEvent*);
    void resizeEvent(QResizeEvent* _event);
    void closeEvent(QCloseEvent* _event);
    bool winEvent(MSG* message, long*);

public slots:
    void vInit();

    void vUpdateTrayQuickMenu();

    void slotQuit();
    void slotApply();
    void slotStartPause();
    void slotShowHelp();

    void slotOptionToggled(bool _c);
    void slotHotkeyToggled(bool _c);
    void slotDelayChanged(int _iVal);

    void slotTrayAction(QSystemTrayIcon::ActivationReason _reason);
    void slotTrayQuickClicked();

    void slotToggleWindow(bool _bForceHide=false);
    void slotDisplayNewVersion(const QString &_sVersion);
};

#endif // MAINWINDOW_H
