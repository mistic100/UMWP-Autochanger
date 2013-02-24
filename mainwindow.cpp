#include <QMenu>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "errorwidget.h"
#include "mainwidget.h"
#include <QDebug>

/*
 * constructor
 */
MainWindow::MainWindow(Controller* _oCtrl) : QMainWindow(0)
{
    m_poCtrl = _oCtrl;
    m_poCentralWidget = NULL;
    m_poStatusBar = NULL;
    m_poMenuBar = NULL;
    m_poTrayIcon = NULL;

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint); // disable maximise button
    setWindowTitle(APP_NAME);
    setWindowIcon(QPixmap(":/img/icon"));

    vInit();

    // window is hidden by default if the config is not empty
    if (
            m_poCtrl->settings()->iNbSets()>0
            && m_poCtrl->settings()->bMinimize()
            && m_poCtrl->settings()->iState()==UM_OK
    ) {
        vSlotToggleWindow();
    }
    else
    {
        show();
    }
}

/*
 * destructor
 */
MainWindow::~MainWindow()
{
    if (m_poCentralWidget != NULL)  delete m_poCentralWidget;
    if (m_poStatusBar != NULL)      delete m_poStatusBar;
    if (m_poMenuBar != NULL)        delete m_poMenuBar;
    if (m_poTrayIcon != NULL)       delete m_poTrayIcon;
}

/*
 * init window, called in the constructor and by ErrorWidget
 */
void MainWindow::vInit()
{
    int iState = m_poCtrl->settings()->iState();

    // SET CENTRAL WIDGET AND WINDOW SIZE
    if (m_poCentralWidget != NULL)
    {
        setMaximumSize(9999, 9999); // cancel maximum size
        delete m_poCentralWidget;
    }

    QSize base_size = QSize(400, 200);
    setMinimumSize(base_size);

    if (iState == UM_OK)
    {
        m_poCentralWidget = new MainWidget(this, m_poCtrl);
        resize(m_poCtrl->settings()->oWindowSize());
    }
    else
    {
        m_poCentralWidget = new ErrorWidget(this, m_poCtrl);
        setMaximumSize(base_size);
    }

    setCentralWidget(m_poCentralWidget);


    // START TIMER if config is ok
    if (iState == UM_OK)
    {
        m_poCtrl->vStartTimer(true);
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
        m_poActionHide1 =           m_poMenuBar->addAction(tr("Hide"));
                                    m_poMenuBar->addSeparator();
        m_poOptionsMenu =           m_poMenuBar->addMenu(tr("Options"));
                                    m_poMenuBar->addSeparator();
        QAction* poActionHelp =     m_poMenuBar->addAction(tr("?"));

        QAction* poOptionMinimize = m_poOptionsMenu->addAction(tr("Minimize on startup"));
        QAction* poOptionCheck =    m_poOptionsMenu->addAction(tr("Check files periodically"));

        poOptionMinimize->setCheckable(true);
        poOptionMinimize->setChecked(m_poCtrl->settings()->bMinimize());
        poOptionCheck->setCheckable(true);
        poOptionCheck->setChecked(m_poCtrl->settings()->bMinimize());

        connect(poActionQuit1, SIGNAL(triggered()), this, SLOT(vSlotQuit()));
        connect(m_poActionHide1, SIGNAL(triggered()), this, SLOT(vSlotToggleWindow()));
        connect(poActionHelp, SIGNAL(triggered()), this, SLOT(vSlotShowHelp()));
        connect(poOptionMinimize, SIGNAL(toggled(bool)), this, SLOT(vSlotOptionMinimizeChanged(bool)));
        connect(poOptionCheck, SIGNAL(toggled(bool)), this, SLOT(vSlotOptionCheckChanged(bool)));

        setMenuBar(m_poMenuBar);
    }
    if (iState == UM_OK)
    {
        m_poActionHide1->setDisabled(false);
        m_poOptionsMenu->setDisabled(false);
    }
    else
    {
        m_poActionHide1->setDisabled(true);
        m_poOptionsMenu->setDisabled(true);
    }


    // SET TRAY ICON
    if (m_poTrayIcon == NULL && iState == UM_OK)
    {
        m_poTrayIcon = new QSystemTrayIcon(QIcon(":/img/icon"), this);
        m_poTrayIcon->setToolTip(APP_NAME);

        connect(m_poTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(vSlotTrayAction(QSystemTrayIcon::ActivationReason)));

        QMenu* poTrayMenu = new QMenu;
        m_poActionPause =           poTrayMenu->addAction(tr("Pause"));
        QAction* poActionRefresh =  poTrayMenu->addAction(tr("Refresh"));
        m_poActionHide2 =           poTrayMenu->addAction(tr("Hide"));
                                    poTrayMenu->addSeparator();
        QAction* poActionQuit2 =    poTrayMenu->addAction(tr("Quit"));

        m_poActionPause->setIcon(QPixmap(":/img/playpause"));
        poActionRefresh->setIcon(QPixmap(":/img/refresh"));
        m_poActionHide2->setIcon(QPixmap(":/img/hide"));

        connect(poActionQuit2, SIGNAL(triggered()), this, SLOT(vSlotQuit()));
        connect(m_poActionHide2, SIGNAL(triggered()), this, SLOT(vSlotToggleWindow()));
        connect(m_poActionPause, SIGNAL(triggered()), this, SLOT(vSlotStartPause()));
        connect(poActionRefresh, SIGNAL(triggered()), this, SLOT(vSlotApply()));

        m_poTrayIcon->setContextMenu(poTrayMenu);
        m_poTrayIcon->show();
    }
}

/*
 * action on the tray menu
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
void MainWindow::vSlotToggleWindow()
{
    if (isVisible())
    {
        hide();

        m_poCtrl->settings()->vSetWindowSize(size());
        m_poCtrl->settings()->vWriteXML();

        m_poActionHide2->setText(tr("Show"));
        m_poTrayIcon->showMessage(APP_NAME, tr("%1 is still running").arg(APP_NAME));
    }
    else
    {
        show();
        raise();
        setFocus();

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
    m_poCtrl->vStartTimer();
}

/*
 * start or pause the timer
 */
void MainWindow::vSlotStartPause()
{
    m_poCtrl->vStartPause();

    if (m_poCtrl->bIsRunning())
    {
        m_poActionPause->setText(tr("Pause"));
    }
    else
    {
        m_poActionPause->setText(tr("Start"));
    }
}

/*
 * show help dialog
 */
void MainWindow::vSlotShowHelp()
{
    QFile* file;
    QString lang = QLocale::system().name().section('_', 0, 0);
    if (lang.compare("fr")==0)
    {
        file = new QFile(":/lang/help_fr");
    }
    else
    {
        file = new QFile(":/lang/help_en");
    }

    file->open(QIODevice::ReadOnly);
    QTextStream* content = new QTextStream(file);
    QString main_text = "<h3>"+QString::fromAscii(APP_NAME)+" "+QString::fromAscii(APP_VERSION)+"</h3>";
    main_text.append(content->readAll());
    file->close();

    QMessageBox* about = new QMessageBox(this);
    about->setIcon(QMessageBox::Information);
    about->setWindowModality(Qt::NonModal);
    about->setText(main_text);
    about->setWindowTitle(tr("About %1").arg(APP_NAME));
    about->exec();

    delete file;
    delete content;
    delete about;
}

/*
 * options changed
 */
void MainWindow::vSlotOptionMinimizeChanged(bool _a)
{
    m_poCtrl->settings()->vSetMinimize(_a);
}

void MainWindow::vSlotOptionCheckChanged(bool _a)
{
    if (!_a)
    {
        int ret = QMessageBox::warning(this, tr("Warning"), tr("If you disable files check you may obtain<br>a black wallpaper if you manually edit your sets."),
                                       QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);

        if (ret == QMessageBox::Cancel)
        {
            ((QAction*) sender())->setChecked(true);
            return;
        }
    }

    m_poCtrl->settings()->vSetCheckFiles(_a);
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

    if (m_poCtrl->settings()->bIsUnsaved())
    {
        int ret = QMessageBox::warning(this, tr("Quit"), tr("Quit without save?"),
                                       QMessageBox::Cancel | QMessageBox::Save | QMessageBox::Close, QMessageBox::Save);

        if (ret == QMessageBox::Cancel)
        {
            return;
        }
        else if (ret == QMessageBox::Save)
        {
            m_poCtrl->settings()->vSetWindowSize(size());
            m_poCtrl->settings()->vWriteXML();
        }
    }
    else
    {
        int ret = QMessageBox::warning(this, tr("Quit"), tr("If you quit the application now,<br>the wallpaper will not change anymore."),
                                       QMessageBox::Cancel | QMessageBox::Close, QMessageBox::Close);

        if (ret == QMessageBox::Cancel)
        {
            return;
        }
    }

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
