#include "trayicon.h"
#include "mainwindow.h"

/**
 * @brief TrayIcon::TrayIcon
 * @param MainWindow* _parent
 * @param Controller* _ctrl
 */
TrayIcon::TrayIcon(MainWindow* _parent, Controller* _ctrl) :
    QSystemTrayIcon((QWidget*) _parent),
    m_ctrl(_ctrl)
{
    connect(m_ctrl, SIGNAL(listChanged(bool)), this, SLOT(onListChanged()));
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));

    QMenu* menu = new QMenu();
    m_actionPause =          menu->addAction(QIcon(":/images/icons/play_pause.png"), tr("Pause"));
    QAction* actionRefresh = menu->addAction(QIcon(":/images/icons/refresh.png"),    tr("Refresh"));
    m_actionHide =           menu->addAction(QIcon(":/images/icons/hide.png"),       tr("Hide"));
    m_quickMenu =            menu->addMenu(QIcon(":/images/icons/quick.png"),        tr("Quick switch"));
                             menu->addSeparator();
    QAction* actionQuit =    menu->addAction(QIcon(":/images/icons/quit.png"),       tr("Quit"));

    connect(actionQuit,    SIGNAL(triggered()), _parent, SLOT(quit()));
    connect(m_actionHide,  SIGNAL(triggered()), _parent, SLOT(toggleWindow()));
    connect(m_actionPause, SIGNAL(triggered()), _parent, SLOT(startPause()));
    connect(actionRefresh, SIGNAL(triggered()), m_ctrl,  SLOT(update()));

    setIcon(QIcon(":/images/icon.png"));
    setToolTip(APP_NAME);
    setContextMenu(menu);

    onListChanged();
}

/**
 * @brief Change "Hide" label
 * @param _hide
 */
void TrayIcon::setHidden(bool _hide)
{
    if (_hide)
    {
        m_actionHide->setText(tr("Show"));
    }
    else
    {
        m_actionHide->setText(tr("Hide"));
    }
}

/**
 * @brief Change "Pause" label
 * @param _pause
 */
void TrayIcon::setPause(bool _pause)
{
    if (!_pause)
    {
        m_actionPause->setText(tr("Pause"));
        setIcon(QIcon(":/images/icon.png"));
    }
    else
    {
        m_actionPause->setText(tr("Start"));
        setIcon(QIcon(":/images/icon_pause.png"));
    }
}

/**
 * @brief Populate quick change list
 */
void TrayIcon::onListChanged()
{
    m_quickMenu->clear();

    for (int i=0, l=m_ctrl->settings()->nbSets(); i<l; i++)
    {
        Set* set = m_ctrl->settings()->set(i);

        QAction* action = m_quickMenu->addAction(set->name());
        action->setData(i);

        if (set->isActive())
        {
            action->setIcon(QIcon(":/images/icons/bullet_green.png"));
        }
        else
        {
            action->setIcon(QIcon(":/images/icons/bullet_red.png"));
        }

        connect(action, SIGNAL(triggered()), this, SLOT(onQuickClicked()));
    }
}

/**
 * @brief Quick change item click event
 */
void TrayIcon::onQuickClicked()
{
    QAction* action = (QAction*)(QObject::sender());
    int idx = action->data().toInt();

    m_ctrl->settings()->setActiveSets(QList<int>()<<idx);
    m_ctrl->emitListChanged();
    m_ctrl->update();
}

/**
 * @brief Double click on icon
 */
void TrayIcon::onActivated(QSystemTrayIcon::ActivationReason _reason)
{
    if (_reason && _reason == QSystemTrayIcon::DoubleClick)
    {
        ((MainWindow*)parent())->toggleWindow();
    }
}
