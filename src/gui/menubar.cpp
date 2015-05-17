#include "menubar.h"
#include "mainwindow.h"

/**
 * @brief MenuBar::MenuBar
 * @param MainWindow* _parent
 * @param Controller* _ctrl
 */
MenuBar::MenuBar(MainWindow* _parent, Controller *_ctrl) :
    QToolBarExt((QWidget*) _parent),
    m_ctrl(_ctrl)
{
    this->setMovable(false);
    this->setIconSize(QSize(20, 20));

    QMenu* menuConfig = new QMenu();
    QAction* actionOptions = menuConfig->addAction(QIcon(":/images/icons/settings.png"),  tr("Options"));
    QAction* actionScreens = menuConfig->addAction(QIcon(":/images/icons/w_monitor.png"), tr("Manage monitors"));
                             menuConfig->addSeparator();
    QAction* actionImport =  menuConfig->addAction(QIcon(":/images/icons/import.png"), tr("Import configuration file"));
    QAction* actionExport =  menuConfig->addAction(QIcon(":/images/icons/export.png"), tr("Export configuration file"));

    QMenu* menuHelp = new QMenu();
    QAction* actionHelp =   menuHelp->addAction(QIcon(":/images/icons/help_color.png"), tr("User guide"));
    m_actionFiles =         menuHelp->addAction(QIcon(":/images/icons/images.png"),     tr("Active files"));
                            menuHelp->addSeparator();
    QAction* actionIssues = menuHelp->addAction(QIcon(":/images/icons/bug.png"),   tr("Report a bug"));
    QAction* actionHome =   menuHelp->addAction(QIcon(":/images/icons/house.png"), tr("Homepage"));
    QAction* actionAbout =  menuHelp->addAction(QIcon(":/images/icons/about.png"), tr("About"));

    QToolButton* actionQuit = this->addButton(QIcon(":/images/icons/quit.png"),       tr("Quit"),    Qt::ToolButtonTextUnderIcon);
    m_actionAdd =             this->addButton(QIcon(":/images/icons/add.png"),        tr("Add set"), Qt::ToolButtonTextUnderIcon);
    m_actionPause =           this->addButton(QIcon(":/images/icons/play_pause.png"), tr("Pause"),   Qt::ToolButtonTextUnderIcon);
    m_actionRefresh =         this->addButton(QIcon(":/images/icons/refresh.png"),    tr("Refresh"), Qt::ToolButtonTextUnderIcon);
    m_actionHide =            this->addButton(QIcon(":/images/icons/hide.png"),       tr("Hide"),    Qt::ToolButtonTextUnderIcon);
    m_actionConfig =          this->addMenu(QIcon(":/images/icons/config.png"),       tr("Configuration"), menuConfig, Qt::ToolButtonTextUnderIcon);
                              this->addMenu(QIcon(":/images/icons/help.png"),         tr("Help"), menuHelp, Qt::ToolButtonTextUnderIcon);

    m_pauseBlinker = new QWidgetBlinker(m_actionPause);

    connect(actionQuit,      SIGNAL(clicked()), _parent, SLOT(quit()));
    connect(m_actionAdd,     SIGNAL(clicked()), _parent, SLOT(addSet()));
    connect(m_actionHide,    SIGNAL(clicked()), _parent, SLOT(toggleWindow()));
    connect(m_actionRefresh, SIGNAL(clicked()), m_ctrl,  SLOT(update()));
    connect(m_actionPause,   SIGNAL(clicked()), _parent, SLOT(startPause()));

    connect(actionOptions,   SIGNAL(triggered()), _parent, SLOT(openConfigDialog()));
    connect(actionScreens,   SIGNAL(triggered()), _parent, SLOT(openScreensDialog()));
    connect(actionImport,    SIGNAL(triggered()), _parent, SLOT(openImportDialog()));
    connect(actionExport,    SIGNAL(triggered()), _parent, SLOT(openExportDialog()));

    connect(actionHelp,      SIGNAL(triggered()), _parent, SLOT(openHelpDialog()));
    connect(actionAbout,     SIGNAL(triggered()), _parent, SLOT(openAboutDialog()));
    connect(m_actionFiles,   SIGNAL(triggered()), _parent, SLOT(openPreviewDialog()));

    // use signal mapper for all buttons oppening a web page
    QSignalMapper* mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString &)), _parent, SLOT(openLink(const QString &)));

    connect(actionIssues, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(actionIssues, QString::fromAscii(APP_ISSUES_URL));
    connect(actionHome, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(actionHome, QString::fromAscii(APP_HOMEPAGE));
}

/**
 * @brief Show or hide some buttons
 * @param _hide
 */
void MenuBar::setMinimal(bool _hide)
{
    m_actionAdd->defaultAction()->setVisible(!_hide);
    m_actionHide->defaultAction()->setVisible(!_hide);
    m_actionPause->defaultAction()->setVisible(!_hide);
    m_actionConfig->defaultAction()->setVisible(!_hide);
    m_actionRefresh->defaultAction()->setVisible(!_hide);
    m_actionFiles->setVisible(!_hide);
}

/**
 * @brief Start or stop "Pause" blinking
 * @param _pause
 */
void MenuBar::setPause(bool _pause)
{
    if (!_pause)
    {
        m_pauseBlinker->stop();
        m_actionPause->setText(tr("Pause"));
    }
    else
    {
        m_pauseBlinker->start();
        m_actionPause->setText(tr("Start"));
    }
}
