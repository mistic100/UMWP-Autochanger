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
    Controller* m_ctrl;

    QStatusBar*      m_statusBar;
    QToolBarExt*     m_menuBar;
    QSystemTrayIcon* m_trayIcon;
    QMenu*           m_trayQuickMenu;

    // reference to some QAction modified after initilization
    QToolButton* m_actionConfig;
    QToolButton* m_actionHide1; // menu
    QAction*     m_actionHide2; // tray
    QToolButton* m_actionPause1; // menu
    QAction*     m_actionPause2; // tray

    QList<GlobalShortcut*> m_shortcuts;

public:
    MainWindow(Controller* _ctrl);
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
