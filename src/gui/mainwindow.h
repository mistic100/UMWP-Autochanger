#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QStatusBar>

#include "../main.h"
#include "../controller.h"
#include "../globalshortcut.h"
#include "menubar.h"
#include "trayicon.h"
#include "statusbar.h"


/**
 * @brief The app main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Controller* m_ctrl;
    Settings* m_settings;
    Environment* m_enviro;

    StatusBar* m_statusBar;
    MenuBar* m_menuBar;
    TrayIcon* m_trayIcon;

    QList<GlobalShortcut*> m_shortcuts;

public:
    MainWindow(Controller* _ctrl);
    ~MainWindow();

    void defineHotkeys();
    void clearHotkeys();

private:
    void init();

protected:
    void showEvent(QShowEvent* _event);
    void resizeEvent(QResizeEvent* _event);
    void closeEvent(QCloseEvent* _event);

public slots:
    void quit();
    void toggleWindow(bool _forceHide=false);
    void onHotkey();

    void addSet();
    void editSets(const QList<Set *> _sets);
    void deleteSets(const QList<Set*> _sets);
    void openSets(const QList<Set*> _sets);
    void clearCache();

    void openConfigDialog();
    void openScreensDialog();
    void openHelpDialog();
    void openAboutDialog();
    void openChangelogDialog();
    void openPreviewDialog();
    void openExportDialog();
    void openImportDialog();

    void onNewVersion();
    void openNewVersionDialog();

signals:
    void showHidden(bool);
};

#endif // MAINWINDOW_H
