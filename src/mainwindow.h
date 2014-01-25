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
#include "qtoolbarext.h"


/**
 * @brief The main window containing the main widget
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Controller*         m_pCtrl;

    QStatusBar*         m_pStatusBar;
    QToolBarExt*        m_pMenuBar;
    QSystemTrayIcon*    m_pTrayIcon;
    QMenu*              m_pTrayQuickMenu;

    // reference to some QAction modified after initilization
    QToolButton*        m_pActionConfig;
    QToolButton*        m_pActionHide1; // menu
    QAction*            m_pActionHide2; // tray
    QToolButton*        m_pActionPause1; // menu
    QAction*            m_pActionPause2; // tray

    QList<GlobalShortcut*> m_apShortcuts;

public:
    MainWindow(Controller* _pCtrl);
    ~MainWindow();

    void defineHotkeys();
    void clearHotkeys();

    void showMain();
    void showError();

protected:
    void showEvent(QShowEvent*);
    void resizeEvent(QResizeEvent* _event);
    void closeEvent(QCloseEvent* _event);

public slots:
    void init();

    void updateTrayQuickMenu();

    void slotQuit();
    void slotApply();
    void slotStartPause();
    void slotConfigDialog();
    void slotShowHelp();
    void slotHotkey();

    void slotExport();
    void slotImport();

    void slotTrayAction(QSystemTrayIcon::ActivationReason _reason);
    void slotTrayQuickClicked();

    void slotToggleWindow(bool _forceHide=false);
    void slotDisplayNewVersion(const QString &_version);
};

#endif // MAINWINDOW_H
