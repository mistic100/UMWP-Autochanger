#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

#include "setcontextmenu.h"
#include "mainwindow.h"

/**
 * @brief SetContextMenu::SetContextMenu
 * @param MainWindow* _parent
 * @param Controller* _ctrl
 * @param int[] _sets
 */
SetContextMenu::SetContextMenu(QWidget *_parent, Controller *_ctrl, const QList<Set *> &_sets) :
    QMenu(_parent),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings()),
    m_sets(_sets)
{
    if (m_sets.size() > 0)
    {
        int nbActive = 0, nbInactive=0;
        foreach (const Set* set, m_sets)
        {
            if (set->isActive())
                nbActive++;
            else
                nbInactive++;
        }

        // using fuctors to pass arguments without creating own methods
        if (nbActive >= nbInactive)
        {
            QAction* actionUnactivate = addAction(QIcon(":/images/icons/disable.png"), tr("Disable"));
            connect(actionUnactivate, &QAction::triggered, this, [this]{ m_ctrl->unactivateSets(m_sets); });
        }
        else
        {
            QAction* actionActivate = addAction(QIcon(":/images/icons/enable.png"), tr("Enable"));
            connect(actionActivate, &QAction::triggered, this, [this]{ m_ctrl->activateSets(m_sets); });
        }

        addSeparator();
        QAction* actionEdit =   addAction(QIcon(":/images/icons/edit.png"),   tr("Edit"));
        QAction* actionOpen =   addAction(QIcon(":/images/icons/folder.png"), tr("Open directory"));
        QAction* actionClear =  addAction(QIcon(":/images/icons/clear.png"),  tr("Clear cache"));
        addSeparator();
        QAction* actionDelete = addAction(QIcon(":/images/icons/delete.png"), tr("Delete"));

        connect(actionEdit,   &QAction::triggered, this, [this]{ ((MainWindow*) parent())->editSets(m_sets); });
        connect(actionOpen,   SIGNAL(triggered()), this, SLOT(openSets()));
        connect(actionClear,  SIGNAL(triggered()), this, SLOT(clearCache()));
        connect(actionDelete, &QAction::triggered, this, [this]{ ((MainWindow*) parent())->deleteSets(m_sets); });
    }

    QAction* actionAdd = addAction(QIcon(":/images/icons/add_color.png"), tr("Add set"));
    connect(actionAdd, SIGNAL(triggered()), (MainWindow*) parent(), SLOT(addSet()));
}

/**
 * @brief Open directories of selected sets
 */
void SetContextMenu::openSets()
{
    foreach (const Set* set, m_sets)
    {
        QDesktopServices::openUrl(QUrl("file:///" + set->path()));
    }
}

/**
 * @brief Clear sets cache of selected sets
 */
void SetContextMenu::clearCache()
{
    foreach (Set* set, m_sets)
    {
        set->deleteCache();
    }
}
