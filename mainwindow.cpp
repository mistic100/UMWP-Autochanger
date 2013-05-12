#include <QMenu>
#include <QMessageBox>
#include <QLabel>
#include <QFile>

#include "mainwindow.h"
#include "errorwidget.h"
#include "mainwidget.h"


/*
 * constructor
 */
MainWindow::MainWindow(Controller* _oCtrl) : QMainWindow(0)
{
    m_poCtrl = _oCtrl;
    m_poStatusBar = NULL;
    m_poMenuBar = NULL;
    m_poTrayIcon = NULL;

    connect(m_poCtrl, SIGNAL(newVersionAvailable(QString)), this, SLOT(vSlotDisplayNewVersion(QString)));

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint); // disable maximise button
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/img/icon"));

    vInit();

    // window is hidden by default if the config is not empty
    if (
            m_poCtrl->settings()->iNbSets()>0
         && m_poCtrl->settings()->bParam("minimize")
         && m_poCtrl->settings()->iState()==UM_OK
    ) {
        vSlotToggleWindow(true);
    }
    else
    {
        show();
    }
}

/*
 * init window, called in the constructor and by ErrorWidget
 */
void MainWindow::vInit()
{
    int iState = m_poCtrl->settings()->iState();

    // SET CENTRAL WIDGET AND WINDOW SIZE
    if (centralWidget() != 0)
    {
        setMaximumSize(9999, 9999); // cancel maximum size
    }

    QSize base_size = QSize(400, 200);
    setMinimumSize(base_size);

    if (iState == UM_OK)
    {
        setCentralWidget(new MainWidget(this, m_poCtrl));
        resize(m_poCtrl->settings()->oWindowSize());
    }
    else
    {
        setCentralWidget(new ErrorWidget(this, m_poCtrl));
        setMaximumSize(base_size);
    }


    // START TIMER if config is ok
    if (iState == UM_OK)
    {
        m_poCtrl->vStartTimer();
    }


    // SET STATUS BAR
    if (m_poStatusBar == NULL)
    {
        QLabel* poStatusLabel = new QLabel(m_poCtrl->sGetStatusText());
        poStatusLabel->setTextFormat(Qt::RichText);
        poStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        poStatusLabel->setOpenExternalLinks(true);

        m_poStatusBar = new QStatusBar();
        m_poStatusBar->addPermanentWidget(poStatusLabel);
        setStatusBar(m_poStatusBar);
    }


    // SET MENUBAR
    if (m_poMenuBar == NULL)
    {
        m_poMenuBar = new QMenuBar();

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
    }
    if (iState == UM_OK)
    {
        m_poActionHide1->setVisible(true);
        m_poActionPause1->setVisible(true);
        m_poOptionsMenu->menuAction()->setVisible(true);
    }
    else
    {
        m_poActionHide1->setVisible(false);
        m_poActionPause1->setVisible(false);
        m_poOptionsMenu->menuAction()->setVisible(false);
    }


    // SET TRAY ICON
    if (m_poTrayIcon == NULL && iState == UM_OK)
    {
        m_poTrayIcon = new QSystemTrayIcon(QIcon(":/img/icon"), this);

        connect(m_poTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(vSlotTrayAction(QSystemTrayIcon::ActivationReason)));

        QMenu* poTrayMenu = new QMenu;
        m_poActionPause2 =           poTrayMenu->addAction(QIcon(":/img/playpause"), tr("Pause"));
        QAction* poActionRefresh =   poTrayMenu->addAction(QIcon(":/img/refresh"), tr("Refresh"));
        m_poActionHide2 =            poTrayMenu->addAction(QIcon(":/img/hide"), tr("Hide"));
                                    poTrayMenu->addSeparator();
        QAction* poActionQuit2 =     poTrayMenu->addAction(tr("Quit"));

        connect(poActionQuit2,      SIGNAL(triggered()), this, SLOT(vSlotQuit()));
        connect(m_poActionHide2,    SIGNAL(triggered()), this, SLOT(vSlotToggleWindow()));
        connect(m_poActionPause2,   SIGNAL(triggered()), this, SLOT(vSlotStartPause()));
        connect(poActionRefresh,    SIGNAL(triggered()), this, SLOT(vSlotApply()));

        m_poTrayIcon->setToolTip(APP_NAME);
        m_poTrayIcon->setContextMenu(poTrayMenu);
        m_poTrayIcon->show();
    }
}

/*
 * action on the tray icon
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

        m_poCtrl->settings()->vSetWindowSize(size());
        m_poCtrl->settings()->vWriteXML();

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
    m_poCtrl->vStartPause();

    if (m_poCtrl->bIsRunning())
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
