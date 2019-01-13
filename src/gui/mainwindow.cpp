#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>

#include "mainwindow.h"
#include "mainlist.h"
#include "configdialog.h"
#include "screensdialog.h"
#include "previewdialog.h"
#include "newversiondialog.h"
#include "changelogdialog.h"
#include "seteditdialog.h"
#include "delaydialog.h"
#include "../umutils.h"


/**
 * @brief MainWindow::MainWindow
 * @param Controller* _ctrl
 */
MainWindow::MainWindow(Controller* _ctrl) :
    QMainWindow(),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings()),
    m_enviro(_ctrl->enviro())
{
    // WINDOW PROPERTIES
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/images/icon.png"));
    setMinimumSize(APP_MIN_WIDTH, APP_MIN_HEIGHT);

    // STATUS BAR
    m_statusBar = new StatusBar(this, m_ctrl);
    setStatusBar(m_statusBar);

    // MENUBAR
    m_menuBar = new MenuBar(this, m_ctrl);
    addToolBar(m_menuBar);

    // TRAY ICON
    m_trayIcon = new TrayIcon(this, m_ctrl);
    m_trayIcon->show();

    connect(m_ctrl, SIGNAL(newVersionAvailable()), this, SLOT(onNewVersion()));

    installEventFilter(this);

    init();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    clearHotkeys();
}

/**
 * @brief Display error or main list depending on app state
 */
void MainWindow::init()
{
    // TODO : critical errors

    MainList* widget = new MainList(this, m_ctrl);

    setCentralWidget(widget);
    resize(m_settings->windowSize());

    // the window is hidden by default if the config is not empty
    if (m_settings->nbSets()>0 &&
        m_settings->param(UM::CONF::minimize).toBool()
    ) {
        toggleWindow(true);
    }
    else
    {
        show();

        if (QString(APP_VERSION).compare(m_settings->param(UM::CONF::changelog_shown).toString()) > 0)
        {
            m_settings->setParam(UM::CONF::changelog_shown, APP_VERSION);

            if (m_settings->nbSets()>0) // do not show changelog at first startup
            {
                openChangelogDialog();
            }
        }
    }

    defineHotkeys();
}

/**
 * @brief Remove all hotkeys
 */
void MainWindow::clearHotkeys()
{
    qDeleteAll(m_shortcuts);
    m_shortcuts.clear();
}

/**
 * @brief Update hotkeys
 */
void MainWindow::defineHotkeys()
{
    clearHotkeys();

    if (m_settings->param(UM::CONF::use_hotkeys).toBool())
    {
        QLOG_TRACE() << "Create global hotkeys";

        // sets hotkeys
        QHash<int, QList<int>> mergedHotkeys;
        GlobalShortcut* shortcut;

        for (int i=0, l=m_settings->nbSets(); i<l; i++)
        {
            Set* set = m_settings->set(i);

            if (set->hotkey())
            {
                mergedHotkeys[set->hotkey()].append(i);
            }
        }

        for (QHash<int, QList<int>>::Iterator it=mergedHotkeys.begin(); it!=mergedHotkeys.end(); ++it)
        {
            shortcut = new GlobalShortcut();
            shortcut->setShortcut(QKeySequence(it.key()));
            shortcut->setType(GlobalShortcut::SETS);
            shortcut->setSets(it.value());
            connect(shortcut, SIGNAL(activated()), this, SLOT(onHotkey()));
            m_shortcuts.append(shortcut);
        }

        // main hotkeys
        QHash<GlobalShortcut::Type, int> otherHotkeys;
        otherHotkeys.insert(GlobalShortcut::REFRESH,    m_settings->hotkey(UM::CONF::HOTKEY::refresh));
        otherHotkeys.insert(GlobalShortcut::STARTPAUSE, m_settings->hotkey(UM::CONF::HOTKEY::startpause));
        otherHotkeys.insert(GlobalShortcut::SHOWHIDE,   m_settings->hotkey(UM::CONF::HOTKEY::showhide));
        otherHotkeys.insert(GlobalShortcut::DELAY,      m_settings->hotkey(UM::CONF::HOTKEY::delay));
        otherHotkeys.insert(GlobalShortcut::LOCKUNLOCK, m_settings->hotkey(UM::CONF::HOTKEY::lockunlock));

        for (QHash<GlobalShortcut::Type, int>::Iterator it=otherHotkeys.begin(); it!=otherHotkeys.end(); ++it)
        {
            if (it.value()>0)
            {
                shortcut = new GlobalShortcut();
                shortcut->setShortcut(QKeySequence(it.value()));
                shortcut->setType(it.key());
                connect(shortcut, SIGNAL(activated()), this, SLOT(onHotkey()));
                m_shortcuts.append(shortcut);
            }
        }
    }
}

/**
 * @brief Minimize to tray or open from tray
 * @param bool _forceHide
 */
void MainWindow::toggleWindow(bool _forceHide)
{
    if (_forceHide || isVisible() && !isMinimized())
    {
        if (!_forceHide)
        {
            m_settings->setWindowSize(size());
        }

        hide();

        if (m_ctrl->lockEnabled() == UM::LOCK_ALL && m_settings->param(UM::CONF::lock_minimize).toBool())
        {
            m_ctrl->lock();
        }

        emit showHidden(false);

        if (!_forceHide &&
            m_settings->param(UM::CONF::show_notifications).toBool() &&
            m_settings->param(UM::CONF::msgcount).toInt() < APP_MAX_APP_RUNNING_MESSAGE_COUNT)
        {
            m_trayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
            m_settings->setParam(UM::CONF::msgcount, m_settings->param(UM::CONF::msgcount).toInt() + 1);
        }
    }
    else
    {
        show();
        raise();
        setFocus();
        activateWindow();
        setWindowState(Qt::WindowNoState);

        emit showHidden(true);

        if (QString(APP_VERSION).compare(m_settings->param(UM::CONF::changelog_shown).toString()) > 0)
        {
            m_settings->setParam(UM::CONF::changelog_shown, APP_VERSION);
            openChangelogDialog();
        }
    }
}

/**
 * @brief Open dialog for directory selection
 */
void MainWindow::addSet()
{
    if (!openUnlockDialog())
    {
        return;
    }

    QString dirname = QFileDialog::getExistingDirectory(this, tr("Add set"),
                                                        m_settings->param(UM::CONF::last_dir).toString());

    if (!dirname.isEmpty())
    {
        Set* set = m_ctrl->addSet(dirname);

        if (set != NULL)
        {
            this->editSets(QList<Set*>() << set);
        }
    }
}

/**
 * @brief Activate sets
 * @param Set*[] _sets
 */
void MainWindow::activateSets(const QList<Set *> _sets)
{
    if (openUnlockDialog(_sets))
    {
        m_ctrl->activateSets(_sets);
    }
}

/**
 * @brief Deactivate sets
 * @param Set*[] _sets
 */
void MainWindow::unactivateSets(const QList<Set *> _sets)
{
    m_ctrl->unactivateSets(_sets);
}

/**
 * @brief Change which sets are active
 * @param int[] _idx
 */
boolean MainWindow::setActiveSets(const QList<int> _idx)
{
    QList<Set*> sets;
    foreach (const int i, _idx)
    {
        sets.append(m_settings->set(i));
    }

    if (openUnlockDialog(sets))
    {
        m_ctrl->setActiveSets(sets);
        return true;
    }

    return false;
}

/**
 * @brief Open set edit dialog
 */
void MainWindow::editSets(const QList<Set*> _sets)
{
    if (!openUnlockDialog(_sets))
    {
        return;
    }

    SetEditDialog dialog(this, m_ctrl, _sets);

    // need to unbind hotkeys to allow hotkeys input
    clearHotkeys();
    if (dialog.exec())
    {
        m_ctrl->editSets(_sets, dialog.result());
    }
    defineHotkeys();
}

/**
 * @brief Open confirmation to delete sets
 */
void MainWindow::deleteSets(const QList<Set*> _sets)
{
    if (!openUnlockDialog(_sets))
    {
        return;
    }

    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        m_ctrl->deleteSets(_sets);
    }
}

/**
 * @brief Open directories of selected sets
 */
void MainWindow::openSets(const QList<Set*> _sets)
{
    if (!openUnlockDialog(_sets))
    {
        return;
    }

    foreach (const Set* set, _sets)
    {
        QDesktopServices::openUrl(QUrl("file:///" + set->path()));
    }
}

/**
 * @brief Open configuration dialog
 */
void MainWindow::openConfigDialog()
{
    if (!openUnlockDialog(true))
    {
        return;
    }

    ConfigDialog dialog(this, m_ctrl);

    // need to unbind hotkeys to allow hotkeys input
    clearHotkeys();
    if (dialog.exec())
    {
        dialog.save();
        emit settingsChanged();
        m_ctrl->update();
    }
    defineHotkeys();
}

/**
 * @brief Open screens dialog
 */
void MainWindow::openScreensDialog()
{
    if (!openUnlockDialog())
    {
        return;
    }

    ScreensDialog dialog(this, m_ctrl);

    if (dialog.exec())
    {
        dialog.save();
        m_ctrl->update();
    }
}

/**
 * @brief Open dialog to export config file
 */
void MainWindow::openExportDialog()
{
    if (!openUnlockDialog(true))
    {
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Export configuration file"),
                                                    QDir::homePath() + QDir::separator() + "umwp_settings.xml",
                                                    tr("XML files (*.xml)"));
    if (filename.isEmpty())
    {
        return;
    }

    m_settings->save(filename);

    QLOG_TRACE() << "Export config to \""+ filename +"\"";
}

/**
 * @brief Open dialog to import config file
 */
void MainWindow::openImportDialog()
{
    if (!openUnlockDialog(true))
    {
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this, tr("Import configuration file"),
                                                    QDir::homePath(),
                                                    tr("XML files (*.xml)"));
    if (filename.isEmpty())
    {
        return;
    }

    QLOG_TRACE() << "Import config from \""+ filename +"\"";

    if (!m_ctrl->loadConfig(filename))
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid settings file"), QMessageBox::Ok);
    }
}

/**
 * @brief Open changelog dialog
 */
void MainWindow::openChangelogDialog()
{
    ChangelogDialog dialog(this, m_settings);
    dialog.exec();
}

/**
 * @brief Open delay dialog
 */
void MainWindow::openDelayDialog()
{
    if (m_ctrl->locked())
    {
        return;
    }

    DelayDialog* existingDialog = findChild<DelayDialog*>();

    if (existingDialog != NULL)
    {
        existingDialog->raise();
        existingDialog->activateWindow();
    }
    else
    {
        DelayDialog dialog(this, m_settings);
        if (dialog.exec())
        {
            dialog.save();
            m_ctrl->update();
        }
    }
}

/**
 * @brief Open unlock dialog
 * @param Set*[] _sets
 * @param bool _force
 * @return bool
 */
boolean MainWindow::openUnlockDialog(const QList<Set *> _sets, boolean _force)
{
    if (m_ctrl->lockEnabled() == UM::LOCK_DISABLED)
    {
        return true;
    }

    if (m_ctrl->lockEnabled() == UM::LOCK_ALL && !m_ctrl->locked())
    {
        return true;
    }

    if (m_ctrl->lockEnabled() == UM::LOCK_SETS && !_force) {
        boolean hasLock = false;

        foreach (const Set* set, _sets) {
            if (set->lock() == TRUE_BOOL) {
                hasLock = true;
                break;
            }
        }

        if (!hasLock) {
            return true;
        }
    }

    QLOG_TRACE() << "Password dialog openned";

    QInputDialog dialog(this);
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setTextEchoMode(QLineEdit::Password);
    dialog.setLabelText(tr("Password"));
    dialog.setWindowTitle(tr("Unlock"));
    dialog.setProperty("umwp-unlock", true);
    dialog.installEventFilter(this);

    bool ok = dialog.exec();

    if (!ok)
    {
        QLOG_INFO()<<"Password cancelled";
        return false;
    }

    QString password = dialog.textValue();

    if (m_settings->param(UM::CONF::lock_password) == UM::hash(password, UM::PasswordHash))
    {
        m_ctrl->unlock();
        return true;
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid password"), QMessageBox::Ok);
        QLOG_ERROR()<<"Invalid password";
        return false;
    }
}

/**
 * @brief Open about dialog
 */
void MainWindow::openAboutDialog()
{
    QString text = "<h3>" + QString(APP_FILEDESCRIPTION) + " " + QString(APP_VERSION) + "</h3>";
    text+= "Created by Damien \"Mistic\" Sorel.<br>";
    text+= "&copy; 2013-" + QString(CURRENT_YEAR) + " <a href=\"https://strangeplanet.fr\">StrangePlanet.fr</a><br>";
    text+= "Licenced under <a href=\"https://www.gnu.org/licenses/gpl-3.0.txt\">GNU General Public License Version 3</a><br>";
    text+= "Built with <a href=\"https://www.qt.io\"><img src=\":/images/qt-logo-16.png\" style=\"vertical-align: baseline\"></a>";

    QMessageBox dialog;
    dialog.setIconPixmap(QPixmap(":/images/icon-large.png").scaled(96, 96));
    dialog.setWindowTitle(tr("About"));
    dialog.setWindowIcon(windowIcon());
    dialog.setText(text);
    dialog.setStandardButtons(QMessageBox::Close | QMessageBox::Open);
    dialog.setDefaultButton(QMessageBox::Close);
    dialog.setButtonText(QMessageBox::Open, tr("Changelog"));

    int ret = dialog.exec();

    if (ret == QMessageBox::Open)
    {
        openChangelogDialog();
    }
}

/**
 * @brief Display "Active files" dialog
 */
void MainWindow::openPreviewDialog()
{
    if (m_ctrl->current().files.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("No active files"), QMessageBox::Ok);
    }
    else
    {
        bool isPaused = m_ctrl->paused();
        if (!isPaused) {
            m_ctrl->startPause();
        }

        PreviewDialog dialog(this, m_ctrl);
        dialog.exec();

        if (!isPaused) {
            m_ctrl->startPause();
        }
    }
}

/**
 * @brief Perform action on hotkey hit
 */
void MainWindow::onHotkey()
{
    GlobalShortcut* shortcut = (GlobalShortcut*)QObject::sender();

    QLOG_DEBUG() << "Hotkey: " << QString::number(shortcut->type());

    boolean showNotification = !isVisible() && m_settings->param(UM::CONF::show_notifications).toBool();

    switch (shortcut->type())
    {
    case GlobalShortcut::REFRESH:
        m_ctrl->update();
        break;

    case GlobalShortcut::SHOWHIDE:
        toggleWindow();
        break;

    case GlobalShortcut::DELAY:
        openDelayDialog();
        break;

    case GlobalShortcut::LOCKUNLOCK:
    {
        if (m_ctrl->lockEnabled() != UM::LOCK_ALL)
        {
            if (showNotification)
            {
                m_trayIcon->showMessage(APP_NAME, tr("The lock is not configured"), QSystemTrayIcon::Critical);
            }
            break;
        }

        if (m_ctrl->locked())
        {
            if (openUnlockDialog() && showNotification)
            {
                m_trayIcon->showMessage(APP_NAME, tr("Unlocked"));
            }
        }
        else
        {
            m_ctrl->lock();

            if (showNotification)
            {
                m_trayIcon->showMessage(APP_NAME, tr("Locked"));
            }
        }
        break;
    }

    case GlobalShortcut::STARTPAUSE:
    {
        bool running = m_ctrl->startPause();

        if (showNotification)
        {
            if (!running)
            {
                m_trayIcon->showMessage(APP_NAME, tr("Paused"));
            }
            else
            {
                m_trayIcon->showMessage(APP_NAME, tr("Running"));
            }
        }
        break;
    }

    case GlobalShortcut::SETS:
    {
        boolean locked = m_ctrl->lockEnabled() == UM::LOCK_ALL && m_ctrl->locked();

        if (!setActiveSets(shortcut->sets()))
        {
            return;
        }

        if (showNotification)
        {
            QString setsName;
            for (int i=0, l=m_settings->nbActiveSets(); i<l; i++)
            {
                Set* set = m_settings->activeSet(i);

                if (i>0) setsName.append(", ");
                setsName.append(set->name());
            }

            m_trayIcon->showMessage(APP_NAME, tr("Current sets : %1").arg(setsName));
        }

        if (locked)
        {
            m_ctrl->lock();
        }
        break;
    }
    }
}

/**
 * @brief Called when a new version is available
 */
void MainWindow::onNewVersion()
{
    QString version = m_enviro->newVersion().code;

    if (QString(version).compare(m_settings->param(UM::CONF::ignore_update).toString()) > 0)
    {
        m_statusBar->newVersion(version);

        if (!isVisible())
        {
            // tray tootlip
            m_trayIcon->showMessage(APP_NAME, tr("A new version is available : %1").arg(version));
            connect(m_trayIcon, &TrayIcon::messageClicked, this, [this]{ toggleWindow(); openNewVersionDialog(); });
        }
        else
        {
            openNewVersionDialog();
        }
    }
}

/**
 * @brief Open update dialog
 */
void MainWindow::openNewVersionDialog()
{
    if (!openUnlockDialog())
    {
        return;
    }

    NewVersionDialog dialog(this, m_ctrl);
    int res = dialog.exec();

    if (res == QDialog::Rejected)
    {
        m_settings->setParam(UM::CONF::ignore_update, m_enviro->newVersion().code);
    }
}

/**
 * @brief Close the application with a confirmation message
 * Configuration is saved before close
 */
void MainWindow::quit()
{
    if (!m_settings->param(UM::CONF::close_warning).toBool())
    {
        m_settings->save();
        m_ctrl->quit();
        return;
    }

    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Warning);
    dialog.setWindowTitle(tr("Quit"));
    dialog.setText(tr("If you quit the application now,<br>the wallpaper will not change anymore."));
    dialog.setStandardButtons(QMessageBox::Cancel | QMessageBox::Close | QMessageBox::Discard);
    dialog.setDefaultButton(QMessageBox::Close);
    dialog.setButtonText(QMessageBox::Discard, tr("Close and don't show this message again"));

    int ret = dialog.exec();

    if (ret == QMessageBox::Discard)
    {
        m_settings->setParam(UM::CONF::close_warning, false);
    }
    else if (ret == QMessageBox::Cancel)
    {
        return;
    }

    if (isVisible())
    {
        m_settings->setWindowSize(size());
    }

    m_settings->save();
    m_ctrl->quit();
}

/**
 * @brief Resize the window when showed
 */
void MainWindow::showEvent(QShowEvent* _event)
{
    resize(m_settings->windowSize());

    QMainWindow::showEvent(_event);
}

/**
 * @brief Save the window size
 * @param QResizeEvent* _event
 */
void MainWindow::resizeEvent(QResizeEvent* _event)
{
    m_settings->setWindowSize(_event->size());

    QMainWindow::resizeEvent(_event);
}

/**
 * @brief Intercept window close button
 * @param QCloseEvent* _event
 */
void MainWindow::closeEvent(QCloseEvent* _event)
{
    toggleWindow();

    _event->ignore();
    QMainWindow::closeEvent(_event);
}

bool MainWindow::eventFilter(QObject* _target, QEvent* _event)
{
    // Force raise the password dialog
    if (_target->property("umwp-unlock").isValid() && _event->type() == QEvent::Show)
    {
        ((QInputDialog*) _target)->activateWindow();
        ((QInputDialog*) _target)->raise();
    }

    // Notify tray-menu when the window is minimized
    if (_target == this && _event->type() == QEvent::WindowStateChange)
    {
        emit showHidden(!isMinimized());
    }

    return false;
}
