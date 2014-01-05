#include <QMenu>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QProgressBar>

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
    m_poCtrl = _poCtrl;
    connect(m_poCtrl, SIGNAL(newVersionAvailable(const QString)),
            this, SLOT(slotDisplayNewVersion(const QString)));
    connect(m_poCtrl, SIGNAL(listChanged(bool)), this, SLOT(vUpdateTrayQuickMenu()));


    // WINDOW PROPERTIES
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/img/icon"));


    // STATUS BAR
    m_poStatusBar = new QStatusBar(this);

    QString sCopyright = "<a href='" + QString::fromAscii(APP_HOMEPAGE) + "'>";
    sCopyright+= QString::fromAscii(APP_NAME) + "</a>";
    sCopyright+= " " + QString::fromAscii(APP_VERSION);
    sCopyright+= " | <a href='http://www.realtimesoft.com/ultramon'>UltraMon</a>";
    sCopyright+= " " + m_poCtrl->settings()->sEnv("umversion");

    QLabel* poStatusLabel = new QLabel(sCopyright);
    poStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    poStatusLabel->setOpenExternalLinks(true);

    m_poStatusBar->addPermanentWidget(poStatusLabel);
    setStatusBar(m_poStatusBar);


    // MENUBAR
    m_poMenuBar = new QMenuBar(this);

    QAction* poActionQuit1 = m_poMenuBar->addAction(tr("Quit"));
    m_poActionPause1 =       m_poMenuBar->addAction(tr("Pause"));
    m_poActionHide1 =        m_poMenuBar->addAction(tr("Hide"));
                            m_poMenuBar->addSeparator();
    m_poActionOptions =      m_poMenuBar->addAction(tr("Options"));
                            m_poMenuBar->addSeparator();
    QAction* poActionHelp =  m_poMenuBar->addAction(tr("?"));

    connect(poActionQuit1,      SIGNAL(triggered()), this, SLOT(slotQuit()));
    connect(m_poActionHide1,    SIGNAL(triggered()), this, SLOT(slotToggleWindow()));
    connect(m_poActionPause1,   SIGNAL(triggered()), this, SLOT(slotStartPause()));
    connect(m_poActionOptions,  SIGNAL(triggered()), this, SLOT(slotConfigDialog()));
    connect(poActionHelp,       SIGNAL(triggered()), this, SLOT(slotShowHelp()));

    setMenuBar(m_poMenuBar);


    // TRAY ICON
    m_poTrayIcon = new QSystemTrayIcon(QIcon(":/img/icon"), this);

    connect(m_poTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotTrayAction(QSystemTrayIcon::ActivationReason)));

    QMenu* poTrayMenu = new QMenu();
    m_poActionPause2 =           poTrayMenu->addAction(QIcon(":/icon/playpause"), tr("Pause"));
    QAction* poActionRefresh =   poTrayMenu->addAction(QIcon(":/icon/refresh"), tr("Refresh"));
    m_poActionHide2 =            poTrayMenu->addAction(QIcon(":/icon/hide"), tr("Hide"));
    m_poTrayQuickMenu =          poTrayMenu->addMenu(QIcon(":/icon/quick"), tr("Quick switch"));
                                poTrayMenu->addSeparator();
    QAction* poActionQuit2 =     poTrayMenu->addAction(tr("Quit"));

    vUpdateTrayQuickMenu();

    connect(poActionQuit2,      SIGNAL(triggered()), this, SLOT(slotQuit()));
    connect(m_poActionHide2,    SIGNAL(triggered()), this, SLOT(slotToggleWindow()));
    connect(m_poActionPause2,   SIGNAL(triggered()), this, SLOT(slotStartPause()));
    connect(poActionRefresh,    SIGNAL(triggered()), this, SLOT(slotApply()));

    m_poTrayIcon->setToolTip(APP_NAME);
    m_poTrayIcon->setContextMenu(poTrayMenu);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    vClearHotkeys();
}

/**
 * @brief Display error on main widget depending on app state
 */
void MainWindow::vInit()
{
    if (m_poCtrl->settings()->iState() == UM_OK)
    {
        vShowMain();
        vUpdateHotkeys();
    }
    else
    {
        vShowError();
    }
}

/**
 * @brief Display error widget
 */
void MainWindow::vShowError()
{
    m_poTrayIcon->hide();
    m_poActionHide1->setVisible(false);
    m_poActionPause1->setVisible(false);
    m_poActionOptions->setVisible(false);

    ErrorWidget* widget = new ErrorWidget(this, m_poCtrl);
    connect(widget, SIGNAL(pathSaved()), this, SLOT(vInit()));

    setCentralWidget(widget);
    setMinimumSize(400, 200);
    setMaximumSize(400, 200);

    show();
}

/**
 * @brief Display main widget
 */
void MainWindow::vShowMain()
{
    m_poTrayIcon->show();
    m_poActionHide1->setVisible(true);
    m_poActionPause1->setVisible(true);
    m_poActionOptions->setVisible(true);

    MainWidget* widget = new MainWidget(this, m_poCtrl);

    setCentralWidget(widget);
    setMinimumSize(400, 240);
    setMaximumSize(9999, 9999); // no maximum size
    resize(m_poCtrl->settings()->oWindowSize());

    m_poCtrl->vCheckVersion();
    m_poCtrl->vStartTimer();

    // window is hidden by default if the config is not empty
    if (
            m_poCtrl->settings()->iNbSets()>0
         && m_poCtrl->settings()->bParam("minimize")
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
void MainWindow::vClearHotkeys()
{
    qDeleteAll(m_apoShortcuts);
    m_apoShortcuts.clear();
}

/**
 * @brief Update hotkeys
 */
void MainWindow::vUpdateHotkeys()
{
    vClearHotkeys();

    if (m_poCtrl->settings()->bParam("use_hotkeys"))
    {
        QHash<int, QListInt> mergedHotkeys;
        GlobalShortcut* sh;

        for (int i=0, l=m_poCtrl->settings()->iNbSets(); i<l; i++)
        {
            Set* poSet = m_poCtrl->settings()->poGetSet(i);

            if (poSet->hotkey().valid())
            {
                mergedHotkeys[poSet->hotkey().res()].push_back(i);
            }
        }

        for (QHash<int, QListInt>::Iterator it=mergedHotkeys.begin(); it!=mergedHotkeys.end(); ++it)
        {
            sh = new GlobalShortcut();
            sh->setShortcut(QKeySequence(it.key()));
            sh->vSetSets(it.value());
            connect(sh, SIGNAL(activated()), this, SLOT(slotHotkey()));
            m_apoShortcuts.push_back(sh);
        }

        sh = new GlobalShortcut();
        sh->setShortcut(QKeySequence(m_poCtrl->settings()->oHotkey("refresh").res()));
        sh->vSetRefresh();
        connect(sh, SIGNAL(activated()), this, SLOT(slotHotkey()));
        m_apoShortcuts.push_back(sh);

        sh = new GlobalShortcut();
        sh->setShortcut(QKeySequence(m_poCtrl->settings()->oHotkey("startpause").res()));
        sh->vSetStartPause();
        connect(sh, SIGNAL(activated()), this, SLOT(slotHotkey()));
        m_apoShortcuts.push_back(sh);

        sh = new GlobalShortcut();
        sh->setShortcut(QKeySequence(m_poCtrl->settings()->oHotkey("showhide").res()));
        sh->vSetShowHide();
        connect(sh, SIGNAL(activated()), this, SLOT(slotHotkey()));
        m_apoShortcuts.push_back(sh);
    }
}

/**
 * @brief Update quick switch in tray menu
 */
void MainWindow::vUpdateTrayQuickMenu()
{
    m_poTrayQuickMenu->clear();

    for (int i=0, l=m_poCtrl->settings()->iNbSets(); i<l; i++)
    {
        Set* poSet = m_poCtrl->settings()->poGetSet(i);

        QAction* poAction;
        if (poSet->isActive())
        {
            poAction = m_poTrayQuickMenu->addAction(QIcon(":/icon/bullet_green"), poSet->name());
        }
        else
        {
            poAction = m_poTrayQuickMenu->addAction(QIcon(":/icon/bullet_red"), poSet->name());
        }
        poAction->setData(i);

        connect(poAction, SIGNAL(triggered()), this, SLOT(slotTrayQuickClicked()));
    }
}

/**
 * @brief Change active set on click in the quick switch menu
 */
void MainWindow::slotTrayQuickClicked()
{
    QAction* poAction = (QAction*)(QObject::sender());
    int idx = poAction->data().toInt();

    m_poCtrl->vSetOneActiveSet(idx);
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
void MainWindow::slotToggleWindow(bool _bForceHide)
{
    if (_bForceHide || isVisible())
    {
        hide();

        if (m_poCtrl->settings()->iParam("msgcount") < 3)
        {
            if (m_poCtrl->settings()->bParam("show_notifications"))
            {
                m_poTrayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
            }
            m_poCtrl->settings()->vAddMsgCount();
        }

        if (!_bForceHide)
        {
            m_poCtrl->settings()->vSetWindowSize(size());
            m_poCtrl->settings()->vWriteXML();
        }

        m_poActionHide2->setText(tr("Show"));
    }
    else
    {
        show();
        raise();
        setFocus();
        activateWindow();

        m_poActionHide2->setText(tr("Hide"));
    }
}

/**
 * @brief Save configuration and start the timer
 */
void MainWindow::slotApply()
{
    m_poCtrl->settings()->vSetWindowSize(size());
    m_poCtrl->settings()->vWriteXML();

    m_poCtrl->vStartTimer(true);
}

/**
 * @brief start or pause the timer
 */
void MainWindow::slotStartPause()
{
    if (m_poCtrl->bStartPause())
    {
        m_poActionPause1->setText(tr("Pause"));
        m_poActionPause2->setText(tr("Pause"));
    }
    else
    {
        m_poActionPause1->setText(tr("Start"));
        m_poActionPause2->setText(tr("Start"));
    }
}

/**
 * @brief Open configuration dialog
 */
void MainWindow::slotConfigDialog()
{
    ConfigDialog oDialog(this, m_poCtrl->settings());

    vClearHotkeys();
    if (oDialog.exec())
    {
        oDialog.vSave();
        if (oDialog.bDelayChanged())
        {
            m_poCtrl->slotUpdate();
        }
    }
    vUpdateHotkeys();
}

/**
 * @brief Open help dialog
 */
void MainWindow::slotShowHelp()
{
    // title
    QString sMainText = "<h3>" + QString::fromAscii(APP_NAME) + " " + QString::fromAscii(APP_VERSION) + "</h3>";

    // help
    QFile oFile;
    QString sLang = QLocale::system().name().section('_', 0, 0);
    if (sLang.compare("fr")==0)
    {
        oFile.setFileName(":/lang/help_fr");
    }
    else
    {
        oFile.setFileName(":/lang/help_en");
    }

    oFile.open(QIODevice::ReadOnly);
    QTextStream content(&oFile);
    sMainText.append(content.readAll());
    oFile.close();

    // files list
    if (m_poCtrl->files().size() > 0)
    {
        sMainText.append("<hr><h3>" + tr("Current files") + "</h3><ul>");
        for (QVector<QString>::const_iterator it=m_poCtrl->files().begin(); it!=m_poCtrl->files().end(); ++it)
        {
            sMainText.append("<li>" + (*it) + "</li>");
        }
        sMainText.append("</ul>");
    }

    QMessageBox oDialog(this);
    oDialog.setIcon(QMessageBox::Information);
    oDialog.setText(sMainText);
    oDialog.setWindowTitle(tr("About %1").arg(APP_NAME));
    oDialog.exec();
}

/**
 * @brief Perform action on hotkey hit
 */
void MainWindow::slotHotkey()
{
    GlobalShortcut* sender = (GlobalShortcut*)QObject::sender();

    if (sender->refresh())
    {
        slotApply();
    }
    else if (sender->startPause())
    {
        if (!isVisible() && m_poCtrl->settings()->bParam("show_notifications"))
        {
            if (!m_poCtrl->bIsPaused())
            {
                m_poTrayIcon->showMessage(APP_NAME, tr("Paused"));
            }
            else
            {
                m_poTrayIcon->showMessage(APP_NAME, tr("Running"));
            }
        }

        slotStartPause();
    }
    else if (sender->showHide())
    {
        slotToggleWindow();
    }
    else if (!sender->sets().empty())
    {

        if (!isVisible() && m_poCtrl->settings()->bParam("show_notifications"))
        {
            QString setsName;
            for (int i=0, l=m_poCtrl->settings()->iNbActiveSets(false); i<l; i++)
            {
                Set* poSet = m_poCtrl->settings()->poGetActiveSet(i);

                if (i>0) setsName+= ", ";
                setsName+= poSet->name();
            }

            m_poTrayIcon->showMessage(APP_NAME, tr("Current sets : %1").arg(setsName));
        }

        m_poCtrl->vSetActiveSets(sender->sets());
        slotApply();
    }
}

/**
 * @brief Display a message when a new version is available
 * @param string _sVersion
 */
void MainWindow::slotDisplayNewVersion(const QString &_sVersion)
{
    // message in status bar
    QLabel* poVersionLabel = new QLabel(tr("A new version is available : %1").arg(_sVersion));
    poVersionLabel->setStyleSheet("QLabel { color : red; }");
    m_poStatusBar->insertPermanentWidget(0, poVersionLabel);

    if (!isVisible())
    {
        // tray tootlip
        m_poTrayIcon->showMessage(APP_NAME, tr("A new version is available : %1").arg(_sVersion));
    }
    else
    {
        // popup alert
        QMessageBox oDialog(this);
        oDialog.setWindowTitle(tr("New version"));
        oDialog.setText("<b>" + tr("A new version is available : %1").arg(_sVersion) + "</b>");
        oDialog.setInformativeText(tr("Visit the <a href='%1'>project homepage</a> to download the latest version.")
                                   .arg(APP_HOMEPAGE));
        oDialog.setStandardButtons(QMessageBox::Close);
        oDialog.exec();
    }
}

/**
 * @brief Close the application with a confirmation message
 * Configuration is saved before close
 */
void MainWindow::slotQuit()
{
    if (m_poCtrl->settings()->iState()!=UM_OK)
    {
        qApp->quit();
        return;
    }

    int iRet = QMessageBox::warning(this, tr("Quit"), tr("If you quit the application now,<br>the wallpaper will not change anymore."),
                                   QMessageBox::Cancel | QMessageBox::Close, QMessageBox::Close);

    if (iRet == QMessageBox::Cancel)
    {
        return;
    }

    m_poCtrl->settings()->vSetWindowSize(size());
    m_poCtrl->settings()->vWriteXML();

    qApp->quit();
}

/**
 * @brief Resize the window when showed
 */
void MainWindow::showEvent(QShowEvent*)
{
    resize(m_poCtrl->settings()->oWindowSize());
}

/**
 * @brief Save the window size
 * @param QResizeEvent* _event
 */
void MainWindow::resizeEvent(QResizeEvent* _event)
{
    m_poCtrl->settings()->vSetWindowSize(_event->size());
}

/**
 * @brief Intercept window close button
 * @param QCloseEvent* _event
 */
void MainWindow::closeEvent(QCloseEvent* _event)
{
    if (m_poCtrl->settings()->iState()==UM_OK && _event)
    {
        _event->ignore();
        slotToggleWindow();
    }
}
