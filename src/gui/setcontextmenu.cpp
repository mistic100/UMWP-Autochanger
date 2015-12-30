#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

#include "setcontextmenu.h"
#include "seteditdialog.h"
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

        connect(actionEdit,   SIGNAL(triggered()), this, SLOT(editSets()));
        connect(actionOpen,   SIGNAL(triggered()), this, SLOT(openSets()));
        connect(actionClear,  SIGNAL(triggered()), this, SLOT(clearCache()));
        connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteSets()));
    }

    QAction* actionAdd = addAction(QIcon(":/images/icons/add_color.png"), tr("Add set"));
    connect(actionAdd, SIGNAL(triggered()), (MainWindow*) _parent, SLOT(addSet()));
}

/**
 * @brief Open set edit dialog
 */
void SetContextMenu::editSets()
{
    SetEditDialog dialog(this, m_ctrl, m_sets);

    // need to unbind hotkeys to allow hotkeys input
    ((MainWindow*)parent())->clearHotkeys();
    if (dialog.exec())
    {
        m_ctrl->editSets(m_sets, dialog.result());
    }
    ((MainWindow*)parent())->defineHotkeys();
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
 * @brief Open confirmation to delete selected sets
 */
void SetContextMenu::deleteSets()
{
    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        m_ctrl->deleteSets(m_sets);
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
