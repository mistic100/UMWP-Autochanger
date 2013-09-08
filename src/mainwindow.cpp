#include <QMenu>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QProgressBar>

#include "mainwindow.h"
#include "errorwidget.h"
#include "mainwidget.h"


/*
 * constructor
 */
MainWindow::MainWindow(Controller* _oCtrl) : QMainWindow(0)
{
    m_poCtrl = _oCtrl;
    connect(m_poCtrl, SIGNAL(newVersionAvailable(QString)), this, SLOT(vSlotDisplayNewVersion(QString)));
    connect(m_poCtrl, SIGNAL(listChanged()), this, SLOT(vUpdateTrayQuickMenu()));


    // WINDOW PROPERTIES
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint); // disable maximise button
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/img/icon"));
    setMinimumSize(400, 200);


    // STATUS BAR
    m_poStatusBar = new QStatusBar(this);

    QString msg = "<a href='"+QString::fromAscii(APP_HOMEPAGE)+"'>"+QString::fromAscii(APP_NAME)+"</a>";
    msg+= " "+QString::fromAscii(APP_VERSION);
    msg+= " | <a href='http://www.realtimesoft.com/ultramon'>UltraMon</a>";
    msg+= " "+m_poCtrl->settings()->sEnv("umversion");

    QLabel* poStatusLabel = new QLabel(msg);
    poStatusLabel->setTextFormat(Qt::RichText);
    poStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    poStatusLabel->setOpenExternalLinks(true);

    m_poStatusBar->addPermanentWidget(poStatusLabel);
    setStatusBar(m_poStatusBar);


    // MENUBAR
    m_poMenuBar = new QMenuBar(this);

    QAction* poActionQuit1 =    m_poMenuBar->addAction(tr("Quit"));
    m_poActionPause1 =          m_poMenuBar->addAction(tr("Pause"));
    m_poActionHide1 =           m_poMenuBar->addAction(tr("Hide"));
                               m_poMenuBar->addSeparator();
    m_poOptionsMenu =           m_poMenuBar->addMenu(tr("Options"));
                               m_poMenuBar->addSeparator();
    QAction* poActionHelp =     m_poMenuBar->addAction(tr("?"));

    connect(poActionQuit1,      SIGNAL(triggered()), this, SLOT(vSlotQuit()));
    connect(m_poActionHide1,    SIGNAL(triggered()), this, SLOT(vSlotToggleWindow()));
    connect(m_poActionPause1,   SIGNAL(triggered()), this, SLOT(vSlotStartPause()));
    connect(poActionHelp,       SIGNAL(triggered()), this, SLOT(vSlotShowHelp()));


    QAction* poOptionMinimize =     m_poOptionsMenu->addAction(tr("Minimize on startup"));
    QAction* poOptionCheckFiles =   m_poOptionsMenu->addAction(tr("Check files periodically"));
    QAction* poOptionCheckUpdates = m_poOptionsMenu->addAction(tr("Check updates"));
    QAction* poOptionAutostart =    m_poOptionsMenu->addAction(tr("Start with Windows"));

    poOptionMinimize->setCheckable(true);
    poOptionCheckFiles->setCheckable(true);
    poOptionCheckUpdates->setCheckable(true);
    poOptionAutostart->setCheckable(true);

    poOptionMinimize->setChecked(m_poCtrl->settings()->bParam("minimize"));
    poOptionCheckFiles->setChecked(m_poCtrl->settings()->bParam("check"));
    poOptionCheckUpdates->setChecked(m_poCtrl->settings()->bParam("check_updates"));
    poOptionAutostart->setChecked(m_poCtrl->settings()->bIsAutostart());

    poOptionMinimize->setProperty("name", "minimize");
    poOptionCheckFiles->setProperty("name", "check");
    poOptionCheckUpdates->setProperty("name", "check_updates");
    poOptionAutostart->setProperty("name", "autostart");

    connect(poOptionMinimize,       SIGNAL(toggled(bool)), this, SLOT(vSlotOptionToggled(bool)));
    connect(poOptionCheckFiles,     SIGNAL(toggled(bool)), this, SLOT(vSlotOptionToggled(bool)));
    connect(poOptionCheckUpdates,   SIGNAL(toggled(bool)), this, SLOT(vSlotOptionToggled(bool)));
    connect(poOptionAutostart,      SIGNAL(toggled(bool)), this, SLOT(vSlotOptionToggled(bool)));

    if (!(m_poCtrl->settings()->bCanAddShortcut()))
    {
        poOptionAutostart->setVisible(false);
    }

    setMenuBar(m_poMenuBar);


    // TRAY ICON
    m_poTrayIcon = new QSystemTrayIcon(QIcon(":/img/icon"), this);

    connect(m_poTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(vSlotTrayAction(QSystemTrayIcon::ActivationReason)));

    QMenu* poTrayMenu = new QMenu();
    m_poActionPause2 =           poTrayMenu->addAction(QIcon(":/img/playpause"), tr("Pause"));
    QAction* poActionRefresh =   poTrayMenu->addAction(QIcon(":/img/refresh"), tr("Refresh"));
    m_poActionHide2 =            poTrayMenu->addAction(QIcon(":/img/hide"), tr("Hide"));
    m_poTrayQuickMenu =          poTrayMenu->addMenu(QIcon(":/img/quick"), tr("Quick switch"));
                                poTrayMenu->addSeparator();
    QAction* poActionQuit2 =     poTrayMenu->addAction(tr("Quit"));

    vUpdateTrayQuickMenu();

    connect(poActionQuit2,      SIGNAL(triggered()), this, SLOT(vSlotQuit()));
    connect(m_poActionHide2,    SIGNAL(triggered()), this, SLOT(vSlotToggleWindow()));
    connect(m_poActionPause2,   SIGNAL(triggered()), this, SLOT(vSlotStartPause()));
    connect(poActionRefresh,    SIGNAL(triggered()), this, SLOT(vSlotApply()));

    m_poTrayIcon->setToolTip(APP_NAME);
    m_poTrayIcon->setContextMenu(poTrayMenu);


    // register global hotkey if configured
    if (m_poCtrl->settings()->iParam("safe_mod", 16) > 0
        && m_poCtrl->settings()->iParam("safe_vk", 16) > 0
        && !m_poCtrl->settings()->sParam("safe_set").isEmpty())
    {
        RegisterHotKey(winId(), 100, m_poCtrl->settings()->iParam("safe_mod", 16) | MOD_NOREPEAT, m_poCtrl->settings()->iParam("safe_vk", 16));
    }
}

/*
 * display error widget
 */
void MainWindow::vShowError()
{
    m_poTrayIcon->hide();
    m_poActionHide1->setVisible(false);
    m_poActionPause1->setVisible(false);
    m_poOptionsMenu->menuAction()->setVisible(false);

    ErrorWidget* widget = new ErrorWidget(this, m_poCtrl);
    connect(widget, SIGNAL(pathSaved()), this, SLOT(vShowMain()));

    setCentralWidget(widget);
    setMaximumSize(400, 200);

    show();
}

/*
 * display main widget
 */
void MainWindow::vShowMain()
{
    m_poTrayIcon->show();
    m_poActionHide1->setVisible(true);
    m_poActionPause1->setVisible(true);
    m_poOptionsMenu->menuAction()->setVisible(true);

    MainWidget* widget = new MainWidget(this, m_poCtrl);

    setCentralWidget(widget);
    setMaximumSize(9999, 9999); // cancel maximum size
    resize(m_poCtrl->settings()->oWindowSize());

    m_poCtrl->vStart();
    m_poCtrl->vStartTimer();

    // window is hidden by default if the config is not empty
    if (
            m_poCtrl->settings()->iNbSets()>0
         && m_poCtrl->settings()->bParam("minimize")
    ) {
        vSlotToggleWindow(true);
    }
    else
    {
        show();
    }
}

/*
 * update the quick switch menu
 */
void MainWindow::vUpdateTrayQuickMenu()
{
    m_poTrayQuickMenu->clear();

    for (int i=0; i<m_poCtrl->settings()->iNbSets(); i++)
    {
        Set* poSet = m_poCtrl->settings()->poGetSet(i);

        QAction* poAction;
        if (poSet->isActive())
            poAction = m_poTrayQuickMenu->addAction(QIcon(":/img/bullet_green"), poSet->name());
        else
            poAction = m_poTrayQuickMenu->addAction(QIcon(":/img/bullet_red"), poSet->name());
        poAction->setData(i);

        connect(poAction, SIGNAL(triggered()), this, SLOT(vSlotTrayQuickClicked()));
    }
}

/*
 * item of the quick switch menu clicked
 */
void MainWindow::vSlotTrayQuickClicked()
{
    QAction* poAction = (QAction*)(QObject::sender());
    int idx = poAction->data().toInt();

    m_poCtrl->vSetOneActiveSet(idx);

    vSlotApply();
}

/*
 * direct action on the tray icon
 */
void MainWindow::vSlotTrayAction(QSystemTrayIcon::ActivationReason _reason)
{
    if (_reason && _reason==QSystemTrayIcon::DoubleClick)
    {
        vSlotToggleWindow();
    }
}

/*
 * minimize to tray or open from tray
 */
void MainWindow::vSlotToggleWindow(bool _forcehide)
{
    if (_forcehide || isVisible())
    {
        hide();

        if (m_poCtrl->settings()->iParam("msgcount") < 3)
        {
            m_poTrayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
            m_poCtrl->settings()->vAddMsgCount();
        }

        if (!_forcehide)
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

/*
 * apply the changes and start timer
 */
void MainWindow::vSlotApply()
{
    m_poCtrl->settings()->vSetWindowSize(size());
    m_poCtrl->settings()->vWriteXML();

    m_poCtrl->vStartTimer(true);
}

/*
 * start or pause the timer
 */
void MainWindow::vSlotStartPause()
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

/*
 * show help dialog
 */
void MainWindow::vSlotShowHelp()
{
    QFile file;
    QString lang = QLocale::system().name().section('_', 0, 0);
    if (lang.compare("fr")==0)
    {
        file.setFileName(":/lang/help_fr");
    }
    else
    {
        file.setFileName(":/lang/help_en");
    }

    file.open(QIODevice::ReadOnly);
    QTextStream content(&file);
    QString main_text = "<h3>"+QString::fromAscii(APP_NAME)+" "+QString::fromAscii(APP_VERSION)+"</h3>";
    main_text.append(content.readAll());
    file.close();

    QMessageBox about(this);
    about.setIcon(QMessageBox::Information);
    about.setWindowModality(Qt::NonModal);
    about.setText(main_text);
    about.setWindowTitle(tr("About %1").arg(APP_NAME));
    about.exec();
}

/*
 * options changed
 */
void MainWindow::vSlotOptionToggled(bool _c)
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
            int ret = QMessageBox::warning(this, tr("Warning"), tr("If you disable files check you may obtain<br>a black wallpaper if you manually edit your sets."),
                                           QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);

            if (ret == QMessageBox::Cancel)
            {
                poAction->setChecked(true);
                return;
            }
        }
    }

    m_poCtrl->settings()->vSetParam(sOptionName, _c);
}


/*
 * the delay value changed
 */
void MainWindow::vSlotDelayChanged(int _val)
{
    m_poCtrl->settings()->vSetParam("delay", _val);
}


/*
 * new version message
 */
void MainWindow::vSlotDisplayNewVersion(QString _ver)
{
    if (!isVisible())
    {
        m_poTrayIcon->showMessage(APP_NAME, tr("A new version is available : %1.").arg(_ver));
    }
    else
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("New version"));
        msgBox.setText("<b>"+tr("A new version is available : %1.").arg(_ver)+"</b>");
        msgBox.setInformativeText(tr("Visit the <a href='%1'>project homepage</a> to download the latest version.").arg(APP_HOMEPAGE));
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.exec();
    }
}

/*
 * quit the application
 */
void MainWindow::vSlotQuit()
{
    if (m_poCtrl->settings()->iState()!=UM_OK)
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

    m_poCtrl->settings()->vSetWindowSize(size());
    m_poCtrl->settings()->vWriteXML();

    qApp->quit();
}

/*
 * intercept quit (X button)
 */
void MainWindow::closeEvent(QCloseEvent* _event)
{
    if (m_poCtrl->settings()->iState()==UM_OK && _event)
    {
        _event->ignore();
        vSlotToggleWindow();
    }
}

/*
 * intercept global shortcut
 */
bool MainWindow::winEvent(MSG* message, long*)
{
    int idx = -1;

    switch (message->message)
    {
    case WM_HOTKEY:
        for (int i=0; i<m_poCtrl->settings()->iNbSets(); i++)
        {
            Set* poSet = m_poCtrl->settings()->poGetSet(i);
            if (poSet->name().compare(m_poCtrl->settings()->sParam("safe_set")) == 0)
            {
                idx = i;
                break;
            }
        }
        if (idx > -1)
        {
            m_poCtrl->vSetOneActiveSet(idx);
            vSlotApply();
        }
        break;

    default:
        break;
    }

    return false;
}
