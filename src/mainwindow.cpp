#include <QMenu>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QProgressBar>

#include "mainwindow.h"
#include "errorwidget.h"
#include "mainwidget.h"


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
    m_poOptionsMenu =        m_poMenuBar->addMenu(tr("Options"));
                            m_poMenuBar->addSeparator();
    QAction* poActionHelp =  m_poMenuBar->addAction(tr("?"));

    connect(poActionQuit1,      SIGNAL(triggered()), this, SLOT(slotQuit()));
    connect(m_poActionHide1,    SIGNAL(triggered()), this, SLOT(slotToggleWindow()));
    connect(m_poActionPause1,   SIGNAL(triggered()), this, SLOT(slotStartPause()));
    connect(poActionHelp,       SIGNAL(triggered()), this, SLOT(slotShowHelp()));


    QAction* poOptionMinimize =     m_poOptionsMenu->addAction(tr("Minimize on startup"));
    QAction* poOptionCheckFiles =   m_poOptionsMenu->addAction(tr("Check files periodically"));
    QAction* poOptionCheckUpdates = m_poOptionsMenu->addAction(tr("Check updates"));
    QAction* poOptionAutostart =    m_poOptionsMenu->addAction(tr("Start with Windows"));
    QMenu* poHotkeysMenu =          m_poOptionsMenu->addMenu(tr("Hotkeys"));

    QIcon icon_alt = QIcon(":/icon/key_alt");
    icon_alt.addPixmap(QPixmap(":/icon/key_/alt_on"), QIcon::Normal, QIcon::On);
    QIcon icon_ctrl = QIcon(":/icon/key_ctrl");
    icon_ctrl.addPixmap(QPixmap(":/icon/key_ctrl_on"), QIcon::Normal, QIcon::On);
    QIcon icon_shift = QIcon(":/icon/key_shift");
    icon_shift.addPixmap(QPixmap(":/icon/key_shift_on"), QIcon::Normal, QIcon::On);
    QIcon icon_win = QIcon(":/icon/key_win");
    icon_win.addPixmap(QPixmap(":/icon/key_win_on"), QIcon::Normal, QIcon::On);

    QAction* poHotkeysAlt =   poHotkeysMenu->addAction(icon_alt, tr("Alt"));
    QAction* poHotkeysCtrl =  poHotkeysMenu->addAction(icon_ctrl, tr("Ctrl"));
    QAction* poHotkeysShift = poHotkeysMenu->addAction(icon_shift, tr("Shift"));
    QAction* poHotkeysWin =   poHotkeysMenu->addAction(icon_win, tr("Win"));

    poOptionMinimize->setCheckable(true);
    poOptionCheckFiles->setCheckable(true);
    poOptionCheckUpdates->setCheckable(true);
    poOptionAutostart->setCheckable(true);

    poHotkeysAlt->setCheckable(true);
    poHotkeysCtrl->setCheckable(true);
    poHotkeysShift->setCheckable(true);
    poHotkeysWin->setCheckable(true);

    poOptionMinimize->setChecked(m_poCtrl->settings()->bParam("minimize"));
    poOptionCheckFiles->setChecked(m_poCtrl->settings()->bParam("check"));
    poOptionCheckUpdates->setChecked(m_poCtrl->settings()->bParam("check_updates"));
    poOptionAutostart->setChecked(m_poCtrl->settings()->bIsAutostart());

    int iHotkey = m_poCtrl->settings()->iParam("hotkey");
    poHotkeysAlt->setChecked(iHotkey & MOD_ALT);
    poHotkeysCtrl->setChecked(iHotkey & MOD_CONTROL);
    poHotkeysShift->setChecked(iHotkey & MOD_SHIFT);
    poHotkeysWin->setChecked(iHotkey & MOD_WIN);

    poOptionMinimize->setProperty("name", "minimize");
    poOptionCheckFiles->setProperty("name", "check");
    poOptionCheckUpdates->setProperty("name", "check_updates");
    poOptionAutostart->setProperty("name", "autostart");

    poHotkeysAlt->setProperty("key", MOD_ALT);
    poHotkeysCtrl->setProperty("key", MOD_CONTROL);
    poHotkeysShift->setProperty("key", MOD_SHIFT);
    poHotkeysWin->setProperty("key", MOD_WIN);

    connect(poOptionMinimize,     SIGNAL(toggled(bool)), this, SLOT(slotOptionToggled(bool)));
    connect(poOptionCheckFiles,   SIGNAL(toggled(bool)), this, SLOT(slotOptionToggled(bool)));
    connect(poOptionCheckUpdates, SIGNAL(toggled(bool)), this, SLOT(slotOptionToggled(bool)));
    connect(poOptionAutostart,    SIGNAL(toggled(bool)), this, SLOT(slotOptionToggled(bool)));

    connect(poHotkeysAlt,   SIGNAL(toggled(bool)), this, SLOT(slotHotkeyToggled(bool)));
    connect(poHotkeysCtrl,  SIGNAL(toggled(bool)), this, SLOT(slotHotkeyToggled(bool)));
    connect(poHotkeysShift, SIGNAL(toggled(bool)), this, SLOT(slotHotkeyToggled(bool)));
    connect(poHotkeysWin,   SIGNAL(toggled(bool)), this, SLOT(slotHotkeyToggled(bool)));

    if (!(m_poCtrl->settings()->bCanAddShortcut()))
    {
        poOptionAutostart->setVisible(false);
    }

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


    // register global hotkeys
    vUpdateHotkeys();
}

/**
 * @brief Display error on main widget depending on app state
 */
void MainWindow::vInit()
{
    if (m_poCtrl->settings()->iState() == UM_OK)
    {
        vShowMain();
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
    m_poOptionsMenu->menuAction()->setVisible(false);

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
    m_poOptionsMenu->menuAction()->setVisible(true);

    MainWidget* widget = new MainWidget(this, m_poCtrl);

    setCentralWidget(widget);
    setMinimumSize(450, 262);
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
 * @brief Register or remove hotkeys
 */
void MainWindow::vUpdateHotkeys()
{
    int iHotkey = m_poCtrl->settings()->iParam("hotkey");

    if (iHotkey > 0)
    {
        for (int i=0; i<9; i++)
        {
            RegisterHotKey(winId(), 100 + i, iHotkey | MOD_NOREPEAT, 0x31 + i);
        }
    }
    else
    {
        for (int i=0; i<9; i++)
        {
            UnregisterHotKey(winId(), 100 + i);
        }
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
            m_poTrayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
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
 * @brief Open help dialog
 */
void MainWindow::slotShowHelp()
{
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

    QString sMainText = "<h3>" + QString::fromAscii(APP_NAME) + " " + QString::fromAscii(APP_VERSION) + "</h3>";

    oFile.open(QIODevice::ReadOnly);
    QTextStream content(&oFile);
    sMainText.append(content.readAll());
    oFile.close();

    QMessageBox oDialog(this);
    oDialog.setIcon(QMessageBox::Information);
    oDialog.setWindowModality(Qt::NonModal);
    oDialog.setText(sMainText);
    oDialog.setWindowTitle(tr("About %1").arg(APP_NAME));
    oDialog.exec();
}

/**
 * @brief Update configuration when option is changed
 * @param bool _c
 */
void MainWindow::slotOptionToggled(bool _c)
{
    QAction* poAction = (QAction*)(QObject::sender());
    QString sOptionName = poAction->property("name").toString();

    if (sOptionName == "autostart")
    {
        if (_c)
        {
            m_poCtrl->settings()->vCreateShortcut();
        }
        else
        {
            m_poCtrl->settings()->vDeleteShortcut();
        }

        return;
    }
    else if (sOptionName == "check")
    {
        if (!_c)
        {
            int iRet = QMessageBox::warning(this, tr("Warning"),tr("If you disable files check you may obtain<br>a black wallpaper if you manually edit your sets."),
                                           QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);

            if (iRet == QMessageBox::Cancel)
            {
                poAction->setChecked(true);
                return;
            }
        }
    }

    m_poCtrl->settings()->vSetParam(sOptionName, _c);
}

/**
 * @brief Update configuration when hotkeys are changed
 * @param bool _c
 */
void MainWindow::slotHotkeyToggled(bool _c)
{
    QAction* poAction = (QAction*)(QObject::sender());
    int iModifier = poAction->property("key").toInt();
    int iHotkey = m_poCtrl->settings()->iParam("hotkey");

    if (_c)
    {
        iHotkey |= iModifier;
    }
    else
    {
        iHotkey &= ~iModifier;
    }

    m_poCtrl->settings()->vSetParam("hotkey", iHotkey);
    vUpdateHotkeys();
}

/**
 * @brief Update configuration when delay is changed
 * @param int _iVal
 */
void MainWindow::slotDelayChanged(int _iVal)
{
    m_poCtrl->settings()->vSetParam("delay", _iVal);
}


/**
 * @brief Display a message when a new version is available
 * @param string _sVersion
 */
void MainWindow::slotDisplayNewVersion(const QString &_sVersion)
{
    if (!isVisible())
    {
        m_poTrayIcon->showMessage(APP_NAME, tr("A new version is available : %1.").arg(_sVersion));
    }
    else
    {
        QMessageBox oDialog(this);
        oDialog.setWindowTitle(tr("New version"));
        oDialog.setText("<b>" + tr("A new version is available : %1.").arg(_sVersion) + "</b>");
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

/**
 * @brief Intercept windows messages for hotkeys
 * @param MSG* message
 * @return bool
 */
bool MainWindow::winEvent(MSG* message, long*)
{
    int idx = -1;

    switch (message->message)
    {
    case WM_HOTKEY:
        idx = message->wParam - 100;
        if (idx >=0 && idx < m_poCtrl->settings()->iNbSets())
        {
            m_poCtrl->vSetOneActiveSet(idx);
            slotApply();
        }
        break;

    default:
        break;
    }

    return false;
}
