#include <QDesktopServices>

#include "menubar.h"
#include "mainwindow.h"

/**
 * @brief MenuBar::MenuBar
 * @param MainWindow* _parent
 * @param Controller* _ctrl
 */
MenuBar::MenuBar(MainWindow* _parent, Controller *_ctrl) :
    QToolBarExt((QWidget*) _parent)
{
    setMovable(false);
    setIconSize(QSize(20, 20));

    connect(_ctrl, SIGNAL(startedPaused(bool)), this, SLOT(setStartPause(bool)));

    QMenu* menuConfig = new QMenu();
    QAction* actionOptions = menuConfig->addAction(QIcon(":/images/icons/settings.png"),  tr("Options"));
    QAction* actionScreens = menuConfig->addAction(QIcon(":/images/icons/w_monitor.png"), tr("Manage monitors"));
                             menuConfig->addSeparator();
    QAction* actionImport =  menuConfig->addAction(QIcon(":/images/icons/import.png"), tr("Import configuration file"));
    QAction* actionExport =  menuConfig->addAction(QIcon(":/images/icons/export.png"), tr("Export configuration file"));

    QMenu* menuHelp = new QMenu();
    QAction* actionHelp =   menuHelp->addAction(QIcon(":/images/icons/help_color.png"), tr("User guide"));
    QAction* actionFiles =  menuHelp->addAction(QIcon(":/images/icons/images.png"),     tr("Active files"));
                            menuHelp->addSeparator();
    QAction* actionIssues = menuHelp->addAction(QIcon(":/images/icons/bug.png"),   tr("Report a bug"));
    QAction* actionHome =   menuHelp->addAction(QIcon(":/images/icons/house.png"), tr("Homepage"));
    QAction* actionAbout =  menuHelp->addAction(QIcon(":/images/icons/about.png"), tr("About"));

    QToolButton* actionQuit =     addButton(QIcon(":/images/icons/quit.png"),       tr("Quit"),    Qt::ToolButtonTextUnderIcon);
    QToolButton* actionAdd =      addButton(QIcon(":/images/icons/add.png"),        tr("Add set"), Qt::ToolButtonTextUnderIcon);
    m_actionPause =               addButton(QIcon(":/images/icons/play_pause.png"), tr("Pause"),   Qt::ToolButtonTextUnderIcon);
    QToolButton* actionRefresh =  addButton(QIcon(":/images/icons/refresh.png"),    tr("Refresh"), Qt::ToolButtonTextUnderIcon);
    QToolButton* actionHide =     addButton(QIcon(":/images/icons/hide.png"),       tr("Hide"),    Qt::ToolButtonTextUnderIcon);
                                  addMenu(QIcon(":/images/icons/config.png"),       tr("Configuration"), menuConfig, Qt::ToolButtonTextUnderIcon);
                                  addMenu(QIcon(":/images/icons/help.png"),         tr("Help"), menuHelp, Qt::ToolButtonTextUnderIcon);

    m_pauseBlinker = new QWidgetBlinker(m_actionPause);

    connect(actionQuit,      SIGNAL(clicked()), _parent, SLOT(quit()));
    connect(actionAdd,       SIGNAL(clicked()), _parent, SLOT(addSet()));
    connect(actionHide,      SIGNAL(clicked()), _parent, SLOT(toggleWindow()));
    connect(actionRefresh,   SIGNAL(clicked()), _ctrl,   SLOT(update()));
    connect(m_actionPause,   SIGNAL(clicked()), _ctrl,   SLOT(startPause()));

    connect(actionOptions,   SIGNAL(triggered()), _parent, SLOT(openConfigDialog()));
    connect(actionScreens,   SIGNAL(triggered()), _parent, SLOT(openScreensDialog()));
    connect(actionImport,    SIGNAL(triggered()), _parent, SLOT(openImportDialog()));
    connect(actionExport,    SIGNAL(triggered()), _parent, SLOT(openExportDialog()));

    connect(actionHelp,      SIGNAL(triggered()), _parent, SLOT(openHelpDialog()));
    connect(actionAbout,     SIGNAL(triggered()), _parent, SLOT(openAboutDialog()));
    connect(actionFiles,     SIGNAL(triggered()), _parent, SLOT(openPreviewDialog()));

    // use functors to map URLS to open
    connect(actionIssues, &QAction::triggered, this, [this]{ QDesktopServices::openUrl(QUrl(APP_ISSUES_URL)); });
    connect(actionHome,   &QAction::triggered, this, [this]{ QDesktopServices::openUrl(QUrl(APP_HOMEPAGE)); });
}

/**
 * @brief Start or stop "Pause" blinking
 * @param _pause
 */
void MenuBar::setStartPause(bool _start)
{
    if (_start)
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
