#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QFile>
#include <QtWidgets/QFileDialog>
#include <QDesktopServices>

#include "mainwindow.h"
#include "mainwidget.h"
#include "configdialog.h"
#include "screensdialog.h"
#include "previewdialog.h"
#include "newversiondialog.h"
#include "setcontextmenu.h"
#include "changelogdialog.h"


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
    m_altPressed = false;
    installEventFilter(this);

    setMinimumSize(APP_MIN_WIDTH, APP_MIN_HEIGHT);

    connect(m_ctrl, SIGNAL(newVersionAvailable()), this, SLOT(onNewVersion()));


    // WINDOW PROPERTIES
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/images/icon.png"));


    // STATUS BAR
    m_statusBar = new QStatusBar(this);

    QString copyright = "<a href='" + QString(APP_HOMEPAGE) + "'>";
    copyright+= QString(APP_NAME) + "</a>";
    copyright+= " " + QString(APP_VERSION);

    QLabel* statusLabel = new QLabel(copyright);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    statusLabel->setOpenExternalLinks(true);

    m_statusBar->addPermanentWidget(statusLabel);
    setStatusBar(m_statusBar);


    // MENUBAR
    m_menuBar = new MenuBar(this, m_ctrl);
    addToolBar(m_menuBar);


    // TRAY ICON
    m_trayIcon = new TrayIcon(this, m_ctrl);
    m_trayIcon->show();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    clearHotkeys();
}

/**
 * @brief Display error on main widget depending on app state
 */
void MainWindow::init()
{
    MainWidget* widget = new MainWidget(this, m_ctrl);

    setCentralWidget(widget);
    resize(m_settings->windowSize());

    // window is hidden by default if the config is not empty
    if (m_settings->nbSets()>0 &&
        m_settings->get("minimize").toBool()
    ) {
        toggleWindow(true);
    }
    else
    {
        show();

        if (QString(APP_VERSION).compare(m_settings->get("changelog_shown").toString()) > 0)
        {
            m_settings->setOpt("changelog_shown", APP_VERSION);

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

    if (m_settings->get("use_hotkeys").toBool())
    {
        QLOG_TRACE() << "Create global hotkeys";

        // sets hotkeys
        QHash<int, QList<int>> mergedHotkeys;
        GlobalShortcut* shortcut;

        for (int i=0, l=m_settings->nbSets(); i<l; i++)
        {
            Set* pSet = m_settings->set(i);

            if (pSet->hotkey())
            {
                mergedHotkeys[pSet->hotkey()].append(i);
            }
        }

        for (QHash<int, QList<int>>::Iterator it=mergedHotkeys.begin(); it!=mergedHotkeys.end(); ++it)
        {
            shortcut = new GlobalShortcut();
            shortcut->setShortcut(QKeySequence(it.key()));
            shortcut->setSets(it.value());
            connect(shortcut, SIGNAL(activated()), this, SLOT(onHotkey()));
            m_shortcuts.append(shortcut);
        }

        // main hotkeys
        QHash<GlobalShortcut::Type, int> otherHotkeys;
        otherHotkeys.insert(GlobalShortcut::REFRESH, m_settings->hotkey("refresh"));
        otherHotkeys.insert(GlobalShortcut::STARTPAUSE, m_settings->hotkey("startpause"));
        otherHotkeys.insert(GlobalShortcut::SHOWHIDE, m_settings->hotkey("showhide"));

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
    if (_forceHide || isVisible())
    {
        hide();

        if (m_settings->get("show_notifications").toBool() &&
            m_settings->get("msgcount").toInt() < 3)
        {
            m_trayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
            m_settings->incrementMsgCount();
        }

        if (!_forceHide)
        {
            m_settings->setWindowSize(size());
        }

        m_trayIcon->setHidden(true);
    }
    else
    {
        show();
        raise();
        setFocus();
        activateWindow();

        m_trayIcon->setHidden(false);

        if (QString(APP_VERSION).compare(m_settings->get("changelog_shown").toString()) > 0)
        {
            m_settings->setOpt("changelog_shown", APP_VERSION);
            openChangelogDialog();
        }
    }
}

/**
 * @brief Open dialog for directory selection
 */
void MainWindow::addSet()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Add set"),
                                                        m_settings->get("last_dir").toString());

    if (!dirname.isEmpty())
    {
        QDir dir(dirname);
        dir.cdUp();
        m_settings->setOpt("last_dir", dir.absolutePath());

        dirname.replace('/', '\\');
        m_settings->addSet(dirname);
        m_ctrl->emitListChanged(true);
    }
}

/**
 * @brief Open configuration dialog
 */
void MainWindow::openConfigDialog()
{
    ConfigDialog dialog(this, m_ctrl);

    // need to unbind hotkeys to allow hotkeys input
    clearHotkeys();
    if (dialog.exec())
    {
        dialog.save();
    }
    defineHotkeys();
}

/**
 * @brief Open screens dialog
 */
void MainWindow::openScreensDialog()
{
    ScreensDialog dialog(this, m_ctrl);

    if (dialog.exec())
    {
        dialog.save();
    }
}

/**
 * @brief Open dialog to export config file
 */
void MainWindow::openExportDialog()
{
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Import configuration file"),
                                                    QDir::homePath(),
                                                    tr("XML files (*.xml)"));
    if (filename.isEmpty())
    {
        return;
    }

    QLOG_TRACE() << "Import config from \""+ filename +"\"";

    if (m_settings->load(filename))
    {
        m_enviro->checkSettings();
        m_settings->save();

        m_ctrl->emitListChanged(true);
        m_ctrl->update();
    }
    else
    {
        QLOG_ERROR() << "Invalid settings file";
        QMessageBox::critical(this, tr("Error"), tr("Invalid settings file"), QMessageBox::Ok);
    }
}

/**
 * @brief Open help dialog
 */
void MainWindow::openHelpDialog()
{
    QFile file;
    QString lang = QLocale::system().name().section('_', 0, 0);
    if (lang.compare("fr")==0)
    {
        file.setFileName(":/lang/fr_FR/help.htm");
    }
    else
    {
        file.setFileName(":/lang/en_GB/help.htm");
    }

    QString text;
    text.append("<style>");
    text.append("dt { font-weight:bold; }");
    text.append("dd { margin:0 0 1em 1em; }");
    text.append("</style>");

    file.open(QIODevice::ReadOnly);
    QTextStream content(&file);
    content.setCodec("UTF-8");
    text.append(content.readAll());
    file.close();

    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Information);
    dialog.setWindowTitle(tr("User guide"));
    dialog.setText(text);
    dialog.setWindowTitle(tr("User guide"));
    dialog.exec();
}

/**
 * @brief Open changelog dialog
 */
void MainWindow::openChangelogDialog()
{
    ChangelogDialog dialog(this);
    dialog.exec();
}

/**
 * @brief Open about dialog
 */
void MainWindow::openAboutDialog()
{
    QString text = "<h3>" + QString(APP_FILEDESCRIPTION) + " " + QString(APP_VERSION) + "</h3>";
    text+= "Created by Damien \"Mistic\" Sorel.<br>";
    text+= "&copy; 2013-2015 <a href=\"http://strangeplanet.fr\">StrangePlanet.fr</a><br>";
    text+= "Licenced under <a href=\"http://www.gnu.org/licenses/gpl-3.0.txt\">GNU General Public License Version 3</a>";

    QMessageBox dialog;
    dialog.setIcon(QMessageBox::Information);
    dialog.setWindowTitle(tr("About"));
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
    if (m_ctrl->files().isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("No active files"), QMessageBox::Ok);
    }
    else
    {
        PreviewDialog dialog(this, m_ctrl);
        dialog.exec();
    }
}

/**
 * @brief Show the sets context menu
 * @param int[]  _sets
 * @param QPoint _pos
 */
void MainWindow::showContextMenu(const QList<Set*> &_sets, const QPoint &_pos)
{
    SetContextMenu menu(this, m_ctrl, _sets);
    menu.exec(_pos);
}

/**
 * @brief Open a web page
 * @param string _link
 */
void MainWindow::openLink(const QString &_link)
{
    QDesktopServices::openUrl(QUrl(_link));
}

/**
 * @brief Perform action on hotkey hit
 */
void MainWindow::onHotkey()
{
    GlobalShortcut* shortcut = (GlobalShortcut*)QObject::sender();

    QLOG_DEBUG() << "Hotkey: " << QString::number(shortcut->type());

    switch (shortcut->type())
    {
    case GlobalShortcut::REFRESH:
        m_ctrl->update();
        break;

    case GlobalShortcut::STARTPAUSE:
        m_ctrl->startPause();

        if (!isVisible() && m_settings->get("show_notifications").toBool())
        {
            if (!m_ctrl->isPaused())
            {
                m_trayIcon->showMessage(APP_NAME, tr("Paused"));
            }
            else
            {
                m_trayIcon->showMessage(APP_NAME, tr("Running"));
            }
        }
        break;

    case GlobalShortcut::SHOWHIDE:
        toggleWindow();
        break;

    case GlobalShortcut::SETS:
        m_settings->setActiveSets(shortcut->sets());
        m_ctrl->emitListChanged();
        m_ctrl->update();

        if (!isVisible() && m_settings->get("show_notifications").toBool())
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
        break;
    }
}

/**
 * @brief Called when a new version is available
 */
void MainWindow::onNewVersion()
{
    QString _version = m_enviro->newVersion().code;

    // message in status bar
    QPushButton* statusLabel = new QPushButton(tr("A new version is available : %1").arg(_version));
    statusLabel->setFlat(true);
    statusLabel->setStyleSheet("QPushButton { color : red; } QPushButton:flat:pressed { border: none; }");
    statusLabel->setCursor(Qt::PointingHandCursor);
    m_statusBar->insertPermanentWidget(0, statusLabel);

    connect(statusLabel, SIGNAL(clicked()), this, SLOT(openNewVersionDialog()));

    if (!isVisible())
    {
        // tray tootlip
        m_trayIcon->showMessage(APP_NAME, tr("A new version is available : %1").arg(_version));
    }
    else
    {
        openNewVersionDialog();
    }
}

/**
 * @brief Open update dialog
 */
void MainWindow::openNewVersionDialog()
{
    NewVersionDialog dialog(this, m_ctrl);
    dialog.exec();
}

/**
 * @brief Close the application with a confirmation message
 * Configuration is saved before close
 */
void MainWindow::quit()
{
    if (!m_settings->get("close_warning").toBool())
    {
        qApp->quit();
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
        m_settings->setOpt("close_warning", false);
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

    qApp->quit();
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
    if (_event && !m_altPressed)
    {
        _event->ignore();
        toggleWindow();

        QMainWindow::closeEvent(_event);
    }
    else
    {
        qApp->quit();
    }
}

/**
 * @brief catch Alt press to allow Alt-F4
 * @param QEvent* _event
 * @return bool
 */
bool MainWindow::eventFilter(QObject*, QEvent* _event)
{
    if (_event->type() == QEvent::ShortcutOverride)
    {
        if (((QKeyEvent*) _event)->modifiers() == Qt::AltModifier)
        {
            m_altPressed = true;
            QTimer::singleShot(300, this, SLOT(slotAltPressed()));
            return false;
        }
    }

    return QMainWindow::event(_event);
}

void MainWindow::slotAltPressed()
{
    m_altPressed = false;
}
