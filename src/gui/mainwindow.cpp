#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QDesktopServices>

#include "mainwindow.h"
#include "errorwidget.h"
#include "mainwidget.h"
#include "configdialog.h"
#include "screensdialog.h"
#include "previewdialog.h"
#include "newversiondialog.h"
#include "setcontextmenu.h"

extern short UMWP_STATE;


/**
 * @brief MainWindow::MainWindow
 * @param Controller* _ctrl
 */
MainWindow::MainWindow(Controller* _ctrl) :
    QMainWindow(),
    m_ctrl(_ctrl)
{
    m_altPressed = false;
    installEventFilter(this);

    setMinimumSize(APP_MIN_WIDTH, APP_MIN_HEIGHT);

    connect(m_ctrl, SIGNAL(newVersionAvailable()), this, SLOT(onNewVersion()));


    // WINDOW PROPERTIES
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/img/icon"));


    // STATUS BAR
    m_statusBar = new QStatusBar(this);

    QString copyright = "<a href='" + QString::fromAscii(APP_HOMEPAGE) + "'>";
    copyright+= QString::fromAscii(APP_NAME) + "</a>";
    copyright+= " " + QString::fromAscii(APP_VERSION);
    copyright+= " | <a href='http://www.realtimesoft.com/ultramon'>UltraMon</a>";
    copyright+= " " + m_ctrl->enviro()->get("umversion").toString();

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
    if (UMWP_STATE == UMWP::OK)
    {
        showMain();
        defineHotkeys();

        m_ctrl->checkVersion();
        m_ctrl->onUpdate();
    }
    else
    {
        showError();
    }
}

/**
 * @brief Display error widget
 */
void MainWindow::showError()
{
    qxtLog->trace("Init error widget");

    m_trayIcon->hide();
    m_menuBar->setMinimal(true);

    ErrorWidget* widget = new ErrorWidget(this, m_ctrl);
    setCentralWidget(widget);

    connect(widget, SIGNAL(pathSaved()), this, SLOT(init()));

    resize(APP_MIN_WIDTH, APP_MIN_HEIGHT);

    show();
}

/**
 * @brief Display main widget
 */
void MainWindow::showMain()
{
    qxtLog->trace("Init main widget");

    m_trayIcon->show();
    m_menuBar->setMinimal(false);

    MainWidget* widget = new MainWidget(this, m_ctrl);
    setCentralWidget(widget);

    resize(m_ctrl->settings()->windowSize());

    // window is hidden by default if the config is not empty
    if (m_ctrl->settings()->nbSets()>0 &&
        m_ctrl->settings()->get("minimize").toBool()
    ) {
        toggleWindow(true);
    }
    else
    {
        show();
    }
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

    if (m_ctrl->settings()->get("use_hotkeys").toBool())
    {
        qxtLog->trace("Create global hotkeys");

        // sets hotkeys
        QHash<int, QList<int>> mergedHotkeys;
        GlobalShortcut* shortcut;

        for (int i=0, l=m_ctrl->settings()->nbSets(); i<l; i++)
        {
            Set* pSet = m_ctrl->settings()->set(i);

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
        otherHotkeys.insert(GlobalShortcut::REFRESH, m_ctrl->settings()->hotkey("refresh"));
        otherHotkeys.insert(GlobalShortcut::STARTPAUSE, m_ctrl->settings()->hotkey("startpause"));
        otherHotkeys.insert(GlobalShortcut::SHOWHIDE, m_ctrl->settings()->hotkey("showhide"));

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

        if (m_ctrl->settings()->get("show_notifications").toBool() &&
            m_ctrl->settings()->get("msgcount").toInt() < 3)
        {
            m_trayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
            m_ctrl->settings()->incrementMsgCount();
        }

        if (!_forceHide)
        {
            m_ctrl->settings()->setWindowSize(size());
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
    }
}

/**
 * @brief Start or pause the timer
 */
void MainWindow::startPause()
{
    bool run = m_ctrl->startPause();
    m_menuBar->setPause(!run);
    m_trayIcon->setPause(!run);
}

/**
 * @brief Open dialog for directory selection
 */
void MainWindow::addSet()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Add set"),
                                                        m_ctrl->settings()->get("last_dir").toString());

    if (!dirname.isEmpty())
    {
        QDir dir(dirname);
        dir.cdUp();
        m_ctrl->settings()->setOpt("last_dir", dir.absolutePath());

        dirname.replace('/', '\\');
        m_ctrl->settings()->addSet(dirname);
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

    m_ctrl->settings()->save(filename);

    qxtLog->trace("Export config to \""+ filename +"\"");
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

    // preserve UM path
    QString UMPath = m_ctrl->settings()->get("umpath").toString();

    qxtLog->trace("Import config from \""+ filename +"\"");

    if (m_ctrl->settings()->load(filename))
    {
        m_ctrl->enviro()->checkSettings();
        m_ctrl->settings()->setOpt("umpath", UMPath);
        m_ctrl->settings()->save();

        m_ctrl->emitListChanged(true);
        m_ctrl->onUpdate();
    }
    else
    {
        qxtLog->error("Invalid settings file");
        QMessageBox::critical(this, tr("Error"), tr("Invalid settings file"), QMessageBox::Ok);
    }
}

/**
 * @brief Open help dialog
 */
void MainWindow::openHelpDialog()
{
    QFile helpFile;
    QString lang = QLocale::system().name().section('_', 0, 0);
    if (lang.compare("fr")==0)
    {
        helpFile.setFileName(":/lang/fr_FR/help");
    }
    else
    {
        helpFile.setFileName(":/lang/en_GB/help");
    }

    QString mainText;
    mainText.append("<style>");
    mainText.append("dt { font-weight:bold; }");
    mainText.append("dd { margin-bottom:1em; margin-left:1em; }");
    mainText.append("</style>");

    helpFile.open(QIODevice::ReadOnly);
    QTextStream content(&helpFile);
    content.setCodec("UTF-8");
    mainText.append(content.readAll());
    helpFile.close();

    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Information);
    dialog.setText(mainText);
    dialog.setWindowTitle(tr("User guide"));
    dialog.exec();
}

/**
 * @brief Open about dialog
 */
void MainWindow::openAboutDialog()
{
    QString mainText = "<h3>" + QString::fromAscii(APP_NAME) + " " + QString::fromAscii(APP_VERSION) + "</h3>";
    mainText+= "Created by Damien \"Mistic\" Sorel.<br>";
    mainText+= "&copy; 2013-2015 <a href=\"http://strangeplanet.fr\">StrangePlanet.fr</a><br>";
    mainText+= "Licenced under <a href=\"http://www.gnu.org/licenses/gpl-3.0.txt\">GNU General Public License Version 3</a>";

    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Information);
    dialog.setText(mainText);
    dialog.setWindowTitle(tr("About"));
    dialog.exec();
}

/**
 * @brief Display "Active files" dialog
 */
void MainWindow::openPreviewDialog()
{
    PreviewDialog dialog(this, m_ctrl);
    dialog.exec();
}

/**
 * @brief Show the sets context menu
 * @param int[]  _sets
 * @param QPoint _pos
 */
void MainWindow::showContextMenu(const QList<int> &_sets, const QPoint &_pos)
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

    qxtLog->debug("Hotkey: "+QString::number(shortcut->type()));

    switch (shortcut->type())
    {
    case GlobalShortcut::REFRESH:
        m_ctrl->onUpdate();
        break;

    case GlobalShortcut::STARTPAUSE:
        startPause();

        if (!isVisible() && m_ctrl->settings()->get("show_notifications").toBool())
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
        m_ctrl->settings()->setActiveSets(shortcut->sets());
        m_ctrl->emitListChanged();
        m_ctrl->onUpdate();

        if (!isVisible() && m_ctrl->settings()->get("show_notifications").toBool())
        {
            QString setsName;
            for (int i=0, l=m_ctrl->settings()->nbActiveSets(); i<l; i++)
            {
                Set* set = m_ctrl->settings()->activeSet(i);

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
    QString _version = m_ctrl->enviro()->newVersion().code;

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
    if (UMWP_STATE != UMWP::OK)
    {
        qApp->quit();
        return;
    }

    int ret = QMessageBox::warning(this, tr("Quit"), tr("If you quit the application now,<br>the wallpaper will not change anymore."),
                                   QMessageBox::Cancel | QMessageBox::Close, QMessageBox::Close);

    if (ret == QMessageBox::Cancel)
    {
        return;
    }

    if (isVisible())
    {
        m_ctrl->settings()->setWindowSize(size());
    }

    qApp->quit();
}

/**
 * @brief Resize the window when showed
 */
void MainWindow::showEvent(QShowEvent* _event)
{
    resize(m_ctrl->settings()->windowSize());

    QMainWindow::showEvent(_event);
}

/**
 * @brief Save the window size
 * @param QResizeEvent* _event
 */
void MainWindow::resizeEvent(QResizeEvent* _event)
{
    m_ctrl->settings()->setWindowSize(_event->size(), false);

    QMainWindow::resizeEvent(_event);
}

/**
 * @brief Intercept window close button
 * @param QCloseEvent* _event
 */
void MainWindow::closeEvent(QCloseEvent* _event)
{
    if (UMWP_STATE == UMWP::OK && _event && !m_altPressed)
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
