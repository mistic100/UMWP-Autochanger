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
    bool eventFilter(QObject* _target, QEvent* _event);

public slots:
    void quit();
    void toggleWindow(bool _forceHide=false);
    void onHotkey();

    void addSet();
    void activateSets(const QList<Set*> _sets);
    void unactivateSets(const QList<Set*> _sets);
    boolean setActiveSets(const QList<int> _idx);
    void editSets(const QList<Set*> _sets);
    void deleteSets(const QList<Set*> _sets);
    void openSets(const QList<Set*> _sets);

    void openConfigDialog();
    void openScreensDialog();
    void openAboutDialog();
    void openChangelogDialog();
    void openPreviewDialog();
    void openExportDialog();
    void openImportDialog();
    void openDelayDialog();
    boolean openUnlockDialog(boolean _force) { return openUnlockDialog(QList<Set*>(), _force); }
    boolean openUnlockDialog(const QList<Set*> _sets = QList<Set*>(), boolean _force = false);

    void onNewVersion();
    void openNewVersionDialog();

signals:
    void settingsChanged();
    void showHidden(bool);
};

#endif // MAINWINDOW_H
