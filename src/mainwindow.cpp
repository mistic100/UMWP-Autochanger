#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QProgressBar>
#include <QFileDialog>

#include "mainwindow.h"
#include "errorwidget.h"
#include "mainwidget.h"
#include "configdialog.h"
#include "previewdialog.h"

extern short UMWP_STATE;


/**
 * @brief MainWindow::MainWindow
 * @param Controller* _ctrl
 */
MainWindow::MainWindow(Controller* _ctrl) : QMainWindow(0)
{
    m_altPressed = false;
    installEventFilter(this);

    setMinimumSize(APP_MIN_WIDTH, APP_MIN_HEIGHT);

    m_ctrl = _ctrl;
    connect(m_ctrl, SIGNAL(newVersionAvailable(const QString)), this, SLOT(slotDisplayNewVersion(const QString)));
    connect(m_ctrl, SIGNAL(listChanged(bool)), this, SLOT(updateTrayQuickMenu()));


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
    copyright+= " " + m_ctrl->settings()->env("umversion").toString();

    QLabel* statusLabel = new QLabel(copyright);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    statusLabel->setOpenExternalLinks(true);

    m_statusBar->addPermanentWidget(statusLabel);
    setStatusBar(m_statusBar);


    // MENUBAR
    m_menuBar = new QToolBarExt(this);
    m_menuBar->setMovable(false);
    m_menuBar->setIconSize(QSize(20, 20));

    QMenu* menuConfig = new QMenu();
    QAction* actionOptions = menuConfig->addAction(QIcon(":/icon/settings"), tr("Options"));
                             menuConfig->addSeparator();
    QAction* actionImport =  menuConfig->addAction(QIcon(":/icon/import"), tr("Import configuration file"));
    QAction* actionExport =  menuConfig->addAction(QIcon(":/icon/export"), tr("Export configuration file"));

    QMenu* menuHelp = new QMenu();
    QAction* actionHelp =  menuHelp->addAction(QIcon(":/icon/help-color"), tr("Help"));
    QAction* actionFiles = menuHelp->addAction(QIcon(":/icon/images"), tr("Active files"));
                           menuHelp->addSeparator();
    QAction* actionAbout = menuHelp->addAction(QIcon(":/icon/about"), tr("About"));

    QToolButton* actionQuit1 =    m_menuBar->addButton(QIcon(":/icon/quit"), tr("Quit"), Qt::ToolButtonTextBesideIcon);
    m_actionPause1 =              m_menuBar->addButton(QIcon(":/icon/playpause"), tr("Pause"), Qt::ToolButtonTextBesideIcon);
    QToolButton* actionRefresh1 = m_menuBar->addButton(QIcon(":/icon/refresh"), tr("Refresh"), Qt::ToolButtonTextBesideIcon);
    m_actionHide1 =               m_menuBar->addButton(QIcon(":/icon/hide"), tr("Hide"), Qt::ToolButtonTextBesideIcon);
    m_actionConfig =              m_menuBar->addButton(QIcon(":/icon/config"), tr("Configuration"), menuConfig, Qt::ToolButtonTextBesideIcon);
                                  m_menuBar->addButton(QIcon(":/icon/help"), tr("?"), menuHelp);

    m_pauseBlinker = new WidgetBlinker(m_actionPause1);

    connect(actionQuit1,    SIGNAL(clicked()), this, SLOT(slotQuit()));
    connect(m_actionHide1,  SIGNAL(clicked()), this, SLOT(slotToggleWindow()));
    connect(actionRefresh1, SIGNAL(clicked()), this, SLOT(slotRefresh()));
    connect(m_actionPause1, SIGNAL(clicked()), this, SLOT(slotStartPause()));

    connect(actionOptions,  SIGNAL(triggered()), this, SLOT(slotConfigDialog()));
    connect(actionImport,   SIGNAL(triggered()), this, SLOT(slotImport()));
    connect(actionExport,   SIGNAL(triggered()), this, SLOT(slotExport()));

    connect(actionHelp,     SIGNAL(triggered()), this, SLOT(slotShowHelp()));
    connect(actionAbout,    SIGNAL(triggered()), this, SLOT(slotShowAbout()));
    connect(actionFiles,    SIGNAL(triggered()), this, SLOT(slotShowPreview()));

    addToolBar(m_menuBar);


    // TRAY ICON
    m_trayIcon = new QSystemTrayIcon(QIcon(":/img/icon"), this);

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotTrayAction(QSystemTrayIcon::ActivationReason)));

    QMenu* trayMenu = new QMenu();
    m_actionPause2 =           trayMenu->addAction(QIcon(":/icon/playpause"), tr("Pause"));
    QAction* actionRefresh2 =  trayMenu->addAction(QIcon(":/icon/refresh"), tr("Refresh"));
    m_actionHide2 =            trayMenu->addAction(QIcon(":/icon/hide"), tr("Hide"));
    m_trayQuickMenu =          trayMenu->addMenu(QIcon(":/icon/quick"), tr("Quick switch"));
                               trayMenu->addSeparator();
    QAction* actionQuit2 =     trayMenu->addAction(QIcon(":/icon/quit"), tr("Quit"));

    updateTrayQuickMenu();

    connect(actionQuit2,    SIGNAL(triggered()), this, SLOT(slotQuit()));
    connect(m_actionHide2,  SIGNAL(triggered()), this, SLOT(slotToggleWindow()));
    connect(m_actionPause2, SIGNAL(triggered()), this, SLOT(slotStartPause()));
    connect(actionRefresh2, SIGNAL(triggered()), this, SLOT(slotRefresh()));

    m_trayIcon->setToolTip(APP_NAME);
    m_trayIcon->setContextMenu(trayMenu);
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
    m_actionHide1->defaultAction()->setVisible(false);
    m_actionPause1->defaultAction()->setVisible(false);
    m_actionConfig->defaultAction()->setVisible(false);

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
    m_actionHide1->defaultAction()->setVisible(true);
    m_actionPause1->defaultAction()->setVisible(true);
    m_actionConfig->defaultAction()->setVisible(true);

    MainWidget* widget = new MainWidget(this, m_ctrl);
    setCentralWidget(widget);

    resize(m_ctrl->settings()->windowSize());

    m_ctrl->checkVersion();
    m_ctrl->startTimer();
    m_ctrl->slotUpdate();

    // window is hidden by default if the config is not empty
    if (
            m_ctrl->settings()->nbSets()>0
         && m_ctrl->settings()->opt("minimize").toBool()
    ) {
        slotToggleWindow(true);
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

    if (m_ctrl->settings()->opt("use_hotkeys").toBool())
    {
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
            connect(shortcut, SIGNAL(activated()), this, SLOT(slotHotkey()));
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
                connect(shortcut, SIGNAL(activated()), this, SLOT(slotHotkey()));
                m_shortcuts.append(shortcut);
            }
        }
    }
}

/**
 * @brief Update quick switch in tray menu
 */
void MainWindow::updateTrayQuickMenu()
{
    m_trayQuickMenu->clear();

    for (int i=0, l=m_ctrl->settings()->nbSets(); i<l; i++)
    {
        Set* set = m_ctrl->settings()->set(i);

        QAction* action = m_trayQuickMenu->addAction(set->name());
        action->setData(i);

        if (set->isActive())
        {
            action->setIcon(QIcon(":/icon/bullet_green"));
        }
        else
        {
            action->setIcon(QIcon(":/icon/bullet_red"));
        }

        connect(action, SIGNAL(triggered()), this, SLOT(slotTrayQuickClicked()));
    }
}

/**
 * @brief Change active set on click in the quick switch menu
 */
void MainWindow::slotTrayQuickClicked()
{
    QAction* action = (QAction*)(QObject::sender());
    int idx = action->data().toInt();

    m_ctrl->settings()->setActiveSets(QList<int>()<<idx);
    m_ctrl->emitListChanged();
    m_ctrl->slotUpdate();
}

/**
 * @brief Toggle window on double click on the tray icon
 * @param QSystemTrayIcon::ActivationReason _reason
 */
void MainWindow::slotTrayAction(QSystemTrayIcon::ActivationReason _reason)
{
    if (_reason && _reason == QSystemTrayIcon::DoubleClick)
    {
        slotToggleWindow();
    }
}

/**
 * @brief Minimize to tray or open from tray
 * @param bool _forceHide
 */
void MainWindow::slotToggleWindow(bool _forceHide)
{
    if (_forceHide || isVisible())
    {
        hide();

        if (m_ctrl->settings()->opt("show_notifications").toBool() &&
                m_ctrl->settings()->opt("msgcount").toInt() < 3)
        {
            m_trayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
            m_ctrl->settings()->incrementMsgCount();
        }

        if (!_forceHide)
        {
            m_ctrl->settings()->setWindowSize(size());
        }

        m_actionHide2->setText(tr("Show"));
    }
    else
    {
        show();
        raise();
        setFocus();
        activateWindow();

        m_actionHide2->setText(tr("Hide"));
    }
}

/**
 * @brief Save configuration and start the timer
 */
void MainWindow::slotRefresh()
{
    m_ctrl->slotUpdate();
}

/**
 * @brief start or pause the timer
 */
void MainWindow::slotStartPause()
{
    if (m_ctrl->startPause())
    {
        m_pauseBlinker->stop();
        m_actionPause1->setText(tr("Pause"));
        m_actionPause2->setText(tr("Pause"));
    }
    else
    {
        m_pauseBlinker->start();
        m_actionPause1->setText(tr("Start"));
        m_actionPause2->setText(tr("Start"));
    }
}

/**
 * @brief Open configuration dialog
 */
void MainWindow::slotConfigDialog()
{
    ConfigDialog dialog(this, m_ctrl->settings());

    clearHotkeys();
    if (dialog.exec())
    {
        dialog.save();
    }
    defineHotkeys();
}

/**
 * @brief Open dialog to export config file
 */
void MainWindow::slotExport()
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
void MainWindow::slotImport()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Import configuration file"),
                                                     QDir::homePath(),
                                                     tr("XML files (*.xml)"));
    if (filename.isEmpty())
    {
        return;
    }

    // preserve UM path
    QString UMPath = m_ctrl->settings()->opt("umpath").toString();

    qxtLog->trace("Import config from \""+ filename +"\"");

    if (m_ctrl->settings()->load(filename))
    {
        m_ctrl->settings()->setOpt("umpath", UMPath);

        m_ctrl->emitListChanged(true);
        m_ctrl->slotUpdate();
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
void MainWindow::slotShowHelp()
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
    mainText.append(content.readAll());
    helpFile.close();

    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Information);
    dialog.setText(mainText);
    dialog.setWindowTitle(tr("Help"));
    dialog.exec();
}

/**
 * @brief Open about dialog
 */
void MainWindow::slotShowAbout()
{
    QString mainText = "<h3>" + QString::fromAscii(APP_NAME) + " " + QString::fromAscii(APP_VERSION) + "</h3>";
    mainText+= "Created by Damien \"Mistic\" Sorel.<br>";
    mainText+= "&copy; 2013-2014 <a href=\"http://strangeplanet.fr\">StrangePlanet.fr</a><br>";
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
void MainWindow::slotShowPreview()
{
    PreviewDialog* dialog = new PreviewDialog(this, m_ctrl);
    dialog->move(geometry().left()+geometry().width()+15, geometry().top());
    dialog->show();
}

/**
 * @brief Perform action on hotkey hit
 */
void MainWindow::slotHotkey()
{
    GlobalShortcut* shortcut = (GlobalShortcut*)QObject::sender();

    qxtLog->debug("Hotkey: "+QString::number(shortcut->type()));

    switch (shortcut->type())
    {
    case GlobalShortcut::REFRESH:
        m_ctrl->slotUpdate();
        break;

    case GlobalShortcut::STARTPAUSE:
        slotStartPause();

        if (!isVisible() && m_ctrl->settings()->opt("show_notifications").toBool())
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
        slotToggleWindow();
        break;

    case GlobalShortcut::SETS:
        m_ctrl->settings()->setActiveSets(shortcut->sets());
        m_ctrl->emitListChanged();
        m_ctrl->slotUpdate();

        if (!isVisible() && m_ctrl->settings()->opt("show_notifications").toBool())
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
 * @brief Display a message when a new version is available
 * @param string _version
 */
void MainWindow::slotDisplayNewVersion(const QString &_version)
{
    // message in status bar
    QLabel* statusLabel = new QLabel(tr("A new version is available : %1").arg(_version));
    statusLabel->setStyleSheet("QLabel { color : red; }");
    m_statusBar->insertPermanentWidget(0, statusLabel);

    if (!isVisible())
    {
        // tray tootlip
        m_trayIcon->showMessage(APP_NAME, tr("A new version is available : %1").arg(_version));
    }
    else
    {
        // popup alert
        QMessageBox dialog(this);
        dialog.setWindowTitle(tr("New version"));
        dialog.setText("<b>" + tr("A new version is available : %1").arg(_version) + "</b>");
        dialog.setInformativeText(tr("Visit the <a href='%1'>project homepage</a> to download the latest version.")
                                  .arg(APP_HOMEPAGE));
        dialog.setStandardButtons(QMessageBox::Close);
        dialog.exec();
    }
}

/**
 * @brief Close the application with a confirmation message
 * Configuration is saved before close
 */
void MainWindow::slotQuit()
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
    m_ctrl->settings()->save();

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
    m_ctrl->settings()->setWindowSize(_event->size());

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
        slotToggleWindow();

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
