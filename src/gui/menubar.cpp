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
    QAction* actionOptions = menuConfig->addAction(QIcon(":/icon/settings"), tr("Options"));
                             menuConfig->addSeparator();
    QAction* actionImport =  menuConfig->addAction(QIcon(":/icon/import"), tr("Import configuration file"));
    QAction* actionExport =  menuConfig->addAction(QIcon(":/icon/export"), tr("Export configuration file"));

    QMenu* menuHelp = new QMenu();
    QAction* actionHelp =  menuHelp->addAction(QIcon(":/icon/help-color"), tr("Help"));
    m_actionFiles =        menuHelp->addAction(QIcon(":/icon/images"), tr("Active files"));
                           menuHelp->addSeparator();
    QAction* actionAbout = menuHelp->addAction(QIcon(":/icon/about"), tr("About"));

    QToolButton* actionQuit =    this->addButton(QIcon(":/icon/quit"), tr("Quit"));
    m_actionPause =              this->addButton(QIcon(":/icon/playpause"), tr("Pause"));
    m_actionRefresh =            this->addButton(QIcon(":/icon/refresh"), tr("Refresh"));
    m_actionHide =               this->addButton(QIcon(":/icon/hide"), tr("Hide"));
    m_actionConfig =             this->addMenu(QIcon(":/icon/config"), tr("Configuration"), menuConfig);
                                 this->addMenu(QIcon(":/icon/help"), tr("?"), menuHelp, Qt::ToolButtonIconOnly);

    m_pauseBlinker = new QWidgetBlinker(m_actionPause);

    connect(actionQuit,      SIGNAL(clicked()), _parent, SLOT(quit()));
    connect(m_actionHide,    SIGNAL(clicked()), _parent, SLOT(toggleWindow()));
    connect(m_actionRefresh, SIGNAL(clicked()), m_ctrl,  SLOT(onUpdate()));
    connect(m_actionPause,   SIGNAL(clicked()), _parent, SLOT(startPause()));

    connect(actionOptions,   SIGNAL(triggered()), _parent, SLOT(openConfigDialog()));
    connect(actionImport,    SIGNAL(triggered()), _parent, SLOT(openImportDialog()));
    connect(actionExport,    SIGNAL(triggered()), _parent, SLOT(openExportDialog()));

    connect(actionHelp,      SIGNAL(triggered()), _parent, SLOT(openHelpDialog()));
    connect(actionAbout,     SIGNAL(triggered()), _parent, SLOT(openAboutDialog()));
    connect(m_actionFiles,   SIGNAL(triggered()), _parent, SLOT(openPreviewDialog()));
}

/**
 * @brief Show or hide some buttons
 * @param _hide
 */
void MenuBar::setMinimal(bool _hide)
{
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
