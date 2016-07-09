#include <QDesktopServices>
#include <QMenu>
#include <QToolButton>

#include "menubar.h"
#include "mainwindow.h"

/**
 * @brief MenuBar::MenuBar
 * @param MainWindow* _parent
 * @param Controller* _ctrl
 */
MenuBar::MenuBar(MainWindow* _parent, Controller *_ctrl) :
    QToolBar((QWidget*) _parent)
{
    setMovable(false);
    setIconSize(QSize(20, 20));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect(_ctrl, SIGNAL(startedPaused(bool)), this, SLOT(setStartPause(bool)));

    QMenu* menuConfig = new QMenu();
    QAction* actionOptions = menuConfig->addAction(QIcon(":/images/icons/settings.png"),  tr("Options"));
    QAction* actionScreens = menuConfig->addAction(QIcon(":/images/icons/w_monitor.png"), tr("Manage monitors"));
                             menuConfig->addSeparator();
    QAction* actionImport =  menuConfig->addAction(QIcon(":/images/icons/import.png"), tr("Import configuration file"));
    QAction* actionExport =  menuConfig->addAction(QIcon(":/images/icons/export.png"), tr("Export configuration file"));
                             menuConfig->addSeparator();
    QAction* actionClear =   menuConfig->addAction(QIcon(":/images/icons/clear.png"), tr("Clear cache"));

    QMenu* menuHelp = new QMenu();
    QAction* actionHelp =   menuHelp->addAction(QIcon(":/images/icons/help_color.png"), tr("User guide"));
    QAction* actionFiles =  menuHelp->addAction(QIcon(":/images/icons/images.png"),     tr("Active files"));
                            menuHelp->addSeparator();
    QAction* actionIssues = menuHelp->addAction(QIcon(":/images/icons/bug.png"),   tr("Report a bug"));
    QAction* actionHome =   menuHelp->addAction(QIcon(":/images/icons/house.png"), tr("Homepage"));
    QAction* actionAbout =  menuHelp->addAction(QIcon(":/images/icons/about.png"), tr("About"));

    QAction* actionQuit =     addAction(QIcon(":/images/icons/quit.png"),       tr("Quit"));
    QAction* actionAdd =      addAction(QIcon(":/images/icons/add.png"),        tr("Add set"));
    m_actionPause =           addAction(QIcon(":/images/icons/play_pause.png"), tr("Pause"));
    QAction* actionRefresh =  addAction(QIcon(":/images/icons/refresh.png"),    tr("Refresh"));
    QAction* actionHide =     addAction(QIcon(":/images/icons/hide.png"),       tr("Hide"));

                              addMenu(QIcon(":/images/icons/config.png"), tr("Configuration"), menuConfig);
                              addMenu(QIcon(":/images/icons/help.png"),   tr("Help"), menuHelp);

    actionAdd->setShortcut(QKeySequence("Ctrl+N"));

    m_pauseBlinker = new QWidgetBlinker(widgetForAction(m_actionPause));

    connect(actionQuit,    SIGNAL(triggered()), _parent, SLOT(quit()));
    connect(actionAdd,     SIGNAL(triggered()), _parent, SLOT(addSet()));
    connect(actionHide,    SIGNAL(triggered()), _parent, SLOT(toggleWindow()));
    connect(actionRefresh, SIGNAL(triggered()), _ctrl,   SLOT(update()));
    connect(m_actionPause, SIGNAL(triggered()), _ctrl,   SLOT(startPause()));

    connect(actionOptions, SIGNAL(triggered()), _parent, SLOT(openConfigDialog()));
    connect(actionScreens, SIGNAL(triggered()), _parent, SLOT(openScreensDialog()));
    connect(actionImport,  SIGNAL(triggered()), _parent, SLOT(openImportDialog()));
    connect(actionExport,  SIGNAL(triggered()), _parent, SLOT(openExportDialog()));
    connect(actionClear,   SIGNAL(triggered()), _parent, SLOT(clearCache()));

    connect(actionAbout,   SIGNAL(triggered()), _parent, SLOT(openAboutDialog()));
    connect(actionFiles,   SIGNAL(triggered()), _parent, SLOT(openPreviewDialog()));

    // use functors to map URLS to open
    connect(actionHelp,   &QAction::triggered, this, [this]{ QDesktopServices::openUrl(QUrl(APP_DOCUMENTATION_URL)); });
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

/**
 * @brief Add a menu with text and icon
 */
QAction* MenuBar::addMenu(const QIcon &_icon, const QString &_text, QMenu* _menu)
{
    QToolButton* button = new QToolButton(this);
    button->setIcon(_icon);
    button->setText(_text);
    button->setMenu(_menu);
    button->setToolButtonStyle(toolButtonStyle());
    button->setPopupMode(QToolButton::InstantPopup);
    return addWidget(button);
}
