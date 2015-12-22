#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include <QtWidgets/QStatusBar>

#include "../main.h"
#include "../controller.h"
#include "../globalshortcut.h"
#include "menubar.h"
#include "trayicon.h"


/**
 * @brief The main window containing the main widget
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Controller*     m_ctrl;
    Settings*       m_settings;
    Environment*    m_enviro;

    QStatusBar* m_statusBar;
    MenuBar*    m_menuBar;
    TrayIcon*   m_trayIcon;

    QList<GlobalShortcut*> m_shortcuts;

    bool m_altPressed;

public:
    MainWindow(Controller* _ctrl);
    ~MainWindow();

    void defineHotkeys();
    void clearHotkeys();

    void showContextMenu(const QList<int> &_sets, const QPoint &_pos);

protected:
    void showEvent(QShowEvent* _event);
    void resizeEvent(QResizeEvent* _event);
    void closeEvent(QCloseEvent* _event);
    bool eventFilter(QObject*, QEvent* _event);

public slots:
    void init();
    void quit();
    void addSet();
    void toggleWindow(bool _forceHide=false);
    void onHotkey();

    void openConfigDialog();
    void openScreensDialog();
    void openHelpDialog();
    void openAboutDialog();
    void openChangelogDialog();
    void openPreviewDialog();
    void openExportDialog();
    void openImportDialog();
    void openLink(const QString &_link);

    void onNewVersion();
    void openNewVersionDialog();

    void slotAltPressed();
};

#endif // MAINWINDOW_H
