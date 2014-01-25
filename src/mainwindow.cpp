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


/**
 * @brief MainWindow::MainWindow
 * @param Controller* _poCtrl
 */
MainWindow::MainWindow(Controller* _poCtrl) : QMainWindow(0)
{
    m_pCtrl = _poCtrl;
    connect(m_pCtrl, SIGNAL(newVersionAvailable(const QString)), this, SLOT(slotDisplayNewVersion(const QString)));
    connect(m_pCtrl, SIGNAL(listChanged(bool)), this, SLOT(updateTrayQuickMenu()));


    // WINDOW PROPERTIES
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/img/icon"));


    // STATUS BAR
    m_pStatusBar = new QStatusBar(this);

    QString copyright = "<a href='" + QString::fromAscii(APP_HOMEPAGE) + "'>";
    copyright+= QString::fromAscii(APP_NAME) + "</a>";
    copyright+= " " + QString::fromAscii(APP_VERSION);
    copyright+= " | <a href='http://www.realtimesoft.com/ultramon'>UltraMon</a>";
    copyright+= " " + m_pCtrl->pSettings()->sEnv("umversion");

    QLabel* pStatusLabel = new QLabel(copyright);
    pStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    pStatusLabel->setOpenExternalLinks(true);

    m_pStatusBar->addPermanentWidget(pStatusLabel);
    setStatusBar(m_pStatusBar);


    // MENUBAR
    m_pMenuBar = new QToolBarExt(this);
    m_pMenuBar->setMovable(false);
    m_pMenuBar->setIconSize(QSize(20, 20));

    QMenu* pMenuConfig = new QMenu();
    QAction* pActionOptions = pMenuConfig->addAction(QIcon(":/icon/settings"), tr("Options"));
                              pMenuConfig->addSeparator();
    QAction* pActionImport =  pMenuConfig->addAction(QIcon(":/icon/import"), tr("Import configuration file"));
    QAction* pActionExport =  pMenuConfig->addAction(QIcon(":/icon/export"), tr("Export configuration file"));

    QToolButton* pActionQuit1 =    m_pMenuBar->addButton(QIcon(":/icon/quit"), tr("Quit"), Qt::ToolButtonTextBesideIcon);
    m_pActionPause1 =              m_pMenuBar->addButton(QIcon(":/icon/playpause"), tr("Pause"), Qt::ToolButtonTextBesideIcon);
    QToolButton* pActionRefresh1 = m_pMenuBar->addButton(QIcon(":/icon/refresh"), tr("Refresh"), Qt::ToolButtonTextBesideIcon);
    m_pActionHide1 =               m_pMenuBar->addButton(QIcon(":/icon/hide"), tr("Hide"), Qt::ToolButtonTextBesideIcon);
    m_pActionConfig =              m_pMenuBar->addButton(QIcon(":/icon/config"), tr("Configuration"), pMenuConfig, Qt::ToolButtonTextBesideIcon);
    QToolButton* pActionHelp =     m_pMenuBar->addButton(QIcon(":/icon/help"), tr("?"));

    connect(pActionQuit1,    SIGNAL(clicked()), this, SLOT(slotQuit()));
    connect(m_pActionHide1,  SIGNAL(clicked()), this, SLOT(slotToggleWindow()));
    connect(pActionRefresh1, SIGNAL(clicked()), this, SLOT(slotApply()));
    connect(m_pActionPause1, SIGNAL(clicked()), this, SLOT(slotStartPause()));
    connect(pActionHelp,     SIGNAL(clicked()), this, SLOT(slotShowHelp()));

    connect(pActionOptions,  SIGNAL(triggered()), this, SLOT(slotConfigDialog()));
    connect(pActionImport,   SIGNAL(triggered()), this, SLOT(slotImport()));
    connect(pActionExport,   SIGNAL(triggered()), this, SLOT(slotExport()));

    addToolBar(m_pMenuBar);


    // TRAY ICON
    m_pTrayIcon = new QSystemTrayIcon(QIcon(":/img/icon"), this);

    connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotTrayAction(QSystemTrayIcon::ActivationReason)));

    QMenu* pTrayMenu = new QMenu();
    m_pActionPause2 =           pTrayMenu->addAction(QIcon(":/icon/playpause"), tr("Pause"));
    QAction* pActionRefresh2 =  pTrayMenu->addAction(QIcon(":/icon/refresh"), tr("Refresh"));
    m_pActionHide2 =            pTrayMenu->addAction(QIcon(":/icon/hide"), tr("Hide"));
    m_pTrayQuickMenu =          pTrayMenu->addMenu(QIcon(":/icon/quick"), tr("Quick switch"));
                                pTrayMenu->addSeparator();
    QAction* pActionQuit2 =     pTrayMenu->addAction(QIcon(":/icon/quit"), tr("Quit"));

    updateTrayQuickMenu();

    connect(pActionQuit2,      SIGNAL(triggered()), this, SLOT(slotQuit()));
    connect(m_pActionHide2,    SIGNAL(triggered()), this, SLOT(slotToggleWindow()));
    connect(m_pActionPause2,   SIGNAL(triggered()), this, SLOT(slotStartPause()));
    connect(pActionRefresh2,   SIGNAL(triggered()), this, SLOT(slotApply()));

    m_pTrayIcon->setToolTip(APP_NAME);
    m_pTrayIcon->setContextMenu(pTrayMenu);
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
    if (m_pCtrl->pSettings()->state() == UM_OK)
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
    m_pTrayIcon->hide();
    m_pActionHide1->defaultAction()->setVisible(false);
    m_pActionPause1->defaultAction()->setVisible(false);
    m_pActionConfig->defaultAction()->setVisible(false);

    ErrorWidget* widget = new ErrorWidget(this, m_pCtrl);
    setCentralWidget(widget);

    connect(widget, SIGNAL(pathSaved()), this, SLOT(init()));

    setMinimumSize(400, 200);
    setMaximumSize(400, 200);

    show();
}

/**
 * @brief Display main widget
 */
void MainWindow::showMain()
{
    m_pTrayIcon->show();
    m_pActionHide1->defaultAction()->setVisible(true);
    m_pActionPause1->defaultAction()->setVisible(true);
    m_pActionConfig->defaultAction()->setVisible(true);

    MainWidget* widget = new MainWidget(this, m_pCtrl);
    setCentralWidget(widget);

    setMinimumSize(440, 240);
    setMaximumSize(9999, 9999); // no maximum size
    resize(m_pCtrl->pSettings()->windowSize());

    m_pCtrl->checkVersion();
    m_pCtrl->startTimer();

    // window is hidden by default if the config is not empty
    if (
            m_pCtrl->pSettings()->nbSets()>0
         && m_pCtrl->pSettings()->bParam("minimize")
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
    qDeleteAll(m_apShortcuts);
    m_apShortcuts.clear();
}

/**
 * @brief Update hotkeys
 */
void MainWindow::defineHotkeys()
{
    clearHotkeys();

    if (m_pCtrl->pSettings()->bParam("use_hotkeys"))
    {
        QHash<int, QList<int>> aMergedHotkeys;
        GlobalShortcut* pShortcut;

        for (int i=0, l=m_pCtrl->pSettings()->nbSets(); i<l; i++)
        {
            Set* pSet = m_pCtrl->pSettings()->pGetSet(i);

            if (pSet->hotkey())
            {
                aMergedHotkeys[pSet->hotkey()].push_back(i);
            }
        }

        for (QHash<int, QList<int>>::Iterator it=aMergedHotkeys.begin(); it!=aMergedHotkeys.end(); ++it)
        {
            pShortcut = new GlobalShortcut();
            pShortcut->setShortcut(QKeySequence(it.key()));
            pShortcut->vSetSets(it.value());
            connect(pShortcut, SIGNAL(activated()), this, SLOT(slotHotkey()));
            m_apShortcuts.push_back(pShortcut);
        }

        QHash<GlobalShortcut::Type, int> aOtherHotkeys;
        aOtherHotkeys.insert(GlobalShortcut::REFRESH, m_pCtrl->pSettings()->hotkey("refresh"));
        aOtherHotkeys.insert(GlobalShortcut::STARTPAUSE, m_pCtrl->pSettings()->hotkey("startpause"));
        aOtherHotkeys.insert(GlobalShortcut::SHOWHIDE, m_pCtrl->pSettings()->hotkey("showhide"));

        for (QHash<GlobalShortcut::Type, int>::Iterator it=aOtherHotkeys.begin(); it!=aOtherHotkeys.end(); ++it)
        {
            if (it.value()>0)
            {
                pShortcut = new GlobalShortcut();
                pShortcut->setShortcut(QKeySequence(it.value()));
                pShortcut->vSetType(it.key());
                connect(pShortcut, SIGNAL(activated()), this, SLOT(slotHotkey()));
                m_apShortcuts.push_back(pShortcut);
            }
        }
    }
}

/**
 * @brief Update quick switch in tray menu
 */
void MainWindow::updateTrayQuickMenu()
{
    m_pTrayQuickMenu->clear();

    for (int i=0, l=m_pCtrl->pSettings()->nbSets(); i<l; i++)
    {
        Set* pSet = m_pCtrl->pSettings()->pGetSet(i);

        QAction* pAction = m_pTrayQuickMenu->addAction(pSet->name());
        pAction->setData(i);

        if (pSet->isActive())
        {
            pAction->setIcon(QIcon(":/icon/bullet_green"));
        }
        else
        {
            pAction->setIcon(QIcon(":/icon/bullet_red"));
        }

        connect(pAction, SIGNAL(triggered()), this, SLOT(slotTrayQuickClicked()));
    }
}

/**
 * @brief Change active set on click in the quick switch menu
 */
void MainWindow::slotTrayQuickClicked()
{
    QAction* pAction = (QAction*)(QObject::sender());
    int idx = pAction->data().toInt();

    m_pCtrl->setOneActiveSet(idx);
    slotApply();
}

/**
 * @brief Toggle window on double click on the tray icon
 * @param QSystemTrayIcon::ActivationReason _reason
 */
void MainWindow::slotTrayAction(QSystemTrayIcon::ActivationReason _reason)
{
    if (_reason && _reason==QSystemTrayIcon::DoubleClick)
    {
        slotToggleWindow();
    }
}

/**
 * @brief Minimize to tray or open from tray
 * Configuration is saved when minimizing if _bForceHide is false
 * @param bool _bForceHide
 */
void MainWindow::slotToggleWindow(bool _forceHide)
{
    if (_forceHide || isVisible())
    {
        hide();

        if (m_pCtrl->pSettings()->iParam("msgcount") < 3)
        {
            if (m_pCtrl->pSettings()->bParam("show_notifications"))
            {
                m_pTrayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
            }
            m_pCtrl->pSettings()->addMsgCount();
        }

        if (!_forceHide)
        {
            m_pCtrl->pSettings()->setWindowSize(size());
            m_pCtrl->pSettings()->writeXML();
        }

        m_pActionHide2->setText(tr("Show"));
    }
    else
    {
        show();
        raise();
        setFocus();
        activateWindow();

        m_pActionHide2->setText(tr("Hide"));
    }
}

/**
 * @brief Save configuration and start the timer
 */
void MainWindow::slotApply()
{
    m_pCtrl->pSettings()->setWindowSize(size());

    m_pCtrl->startTimer(true);
}

/**
 * @brief start or pause the timer
 */
void MainWindow::slotStartPause()
{
    if (m_pCtrl->startPause())
    {
        m_pActionPause1->setText(tr("Pause"));
        m_pActionPause2->setText(tr("Pause"));
    }
    else
    {
        m_pActionPause1->setText(tr("Start"));
        m_pActionPause2->setText(tr("Start"));
    }
}

/**
 * @brief Open configuration dialog
 */
void MainWindow::slotConfigDialog()
{
    ConfigDialog dialog(this, m_pCtrl->pSettings());

    clearHotkeys();
    if (dialog.exec())
    {
        dialog.save();
        if (dialog.delayChanged())
        {
            m_pCtrl->startTimer();
        }
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

    m_pCtrl->pSettings()->writeXML(filename);
}

/**
 * @brief Open dialog to import config file
 */
void MainWindow::slotImport()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Import configuration file"),
                                                     QDir::homePath(),
                                                     tr("XML files (*.xml)"));

    // preserve UM path
    QString UMPath = m_pCtrl->pSettings()->sParam("umpath");

    if (m_pCtrl->pSettings()->readXML(filename))
    {
        m_pCtrl->pSettings()->setParam("umpath", UMPath);

        m_pCtrl->emitListChanged(true);
        m_pCtrl->slotUpdate();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid settings file"), QMessageBox::Ok);
    }
}

/**
 * @brief Open help dialog
 */
void MainWindow::slotShowHelp()
{
    // title
    QString mainText = "<h3>" + QString::fromAscii(APP_NAME) + " " + QString::fromAscii(APP_VERSION) + "</h3>";

    // help
    QFile helpFile;
    QString lang = QLocale::system().name().section('_', 0, 0);
    if (lang.compare("fr")==0)
    {
        helpFile.setFileName(":/lang/help_fr");
    }
    else
    {
        helpFile.setFileName(":/lang/help_en");
    }

    helpFile.open(QIODevice::ReadOnly);
    QTextStream content(&helpFile);
    mainText.append(content.readAll());
    helpFile.close();

    // files list
    if (m_pCtrl->aFiles().size() > 0)
    {
        mainText.append("<hr><h3>" + tr("Current files") + "</h3><ul>");
        for (QVector<QString>::const_iterator it=m_pCtrl->aFiles().begin(); it!=m_pCtrl->aFiles().end(); ++it)
        {
            mainText.append("<li>" + (*it) + "</li>");
        }
        mainText.append("</ul>");
    }

    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Information);
    dialog.setText(mainText);
    dialog.setWindowTitle(tr("About %1").arg(APP_NAME));
    dialog.exec();
}

/**
 * @brief Perform action on hotkey hit
 */
void MainWindow::slotHotkey()
{
    GlobalShortcut* pShortcut = (GlobalShortcut*)QObject::sender();

    switch (pShortcut->type())
    {
    case GlobalShortcut::REFRESH:
        slotApply();
        break;

    case GlobalShortcut::STARTPAUSE:
        slotStartPause();

        if (!isVisible() && m_pCtrl->pSettings()->bParam("show_notifications"))
        {
            if (!m_pCtrl->isPaused())
            {
                m_pTrayIcon->showMessage(APP_NAME, tr("Paused"));
            }
            else
            {
                m_pTrayIcon->showMessage(APP_NAME, tr("Running"));
            }
        }
        break;

    case GlobalShortcut::SHOWHIDE:
        slotToggleWindow();
        break;

    case GlobalShortcut::SETS:
        m_pCtrl->setActiveSets(pShortcut->sets());
        slotApply();

        if (!isVisible() && m_pCtrl->pSettings()->bParam("show_notifications"))
        {
            QString setsName;
            for (int i=0, l=m_pCtrl->pSettings()->nbActiveSets(false); i<l; i++)
            {
                Set* pSet = m_pCtrl->pSettings()->pGetActiveSet(i);

                if (i>0) setsName.append(", ");
                setsName.append(pSet->name());
            }

            m_pTrayIcon->showMessage(APP_NAME, tr("Current sets : %1").arg(setsName));
        }
        break;
    }
}

/**
 * @brief Display a message when a new version is available
 * @param string _sVersion
 */
void MainWindow::slotDisplayNewVersion(const QString &_version)
{
    // message in status bar
    QLabel* pLabel = new QLabel(tr("A new version is available : %1").arg(_version));
    pLabel->setStyleSheet("QLabel { color : red; }");
    m_pStatusBar->insertPermanentWidget(0, pLabel);

    if (!isVisible())
    {
        // tray tootlip
        m_pTrayIcon->showMessage(APP_NAME, tr("A new version is available : %1").arg(_version));
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
    if (m_pCtrl->pSettings()->state()!=UM_OK)
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

    m_pCtrl->pSettings()->setWindowSize(size());
    m_pCtrl->pSettings()->writeXML();

    qApp->quit();
}

/**
 * @brief Resize the window when showed
 */
void MainWindow::showEvent(QShowEvent*)
{
    resize(m_pCtrl->pSettings()->windowSize());
}

/**
 * @brief Save the window size
 * @param QResizeEvent* _event
 */
void MainWindow::resizeEvent(QResizeEvent* _event)
{
    m_pCtrl->pSettings()->setWindowSize(_event->size());
}

/**
 * @brief Intercept window close button
 * @param QCloseEvent* _event
 */
void MainWindow::closeEvent(QCloseEvent* _event)
{
    if (m_pCtrl->pSettings()->state()==UM_OK && _event)
    {
        _event->ignore();
        slotToggleWindow();
    }
}
