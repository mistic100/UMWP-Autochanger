#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

#include "setcontextmenu.h"
#include "seteditdialog.h"

/**
 * @brief SetContextMenu::SetContextMenu
 * @param MainWindow* _parent
 * @param Controller* _ctrl
 * @param int[] _sets
 */
SetContextMenu::SetContextMenu(MainWindow* _parent, Controller *_ctrl, const QList<int> &_sets) :
    QMenu(_parent),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings()),
    m_sets(_sets)
{
    if (m_sets.size() > 0)
    {
        int nbActive = 0, nbInactive=0;
        foreach (int i, m_sets)
        {
            if (m_settings->set(i)->isActive())
                nbActive++;
            else
                nbInactive++;
        }

        if (nbActive >= nbInactive)
        {
            QAction* actionUnactivate = addAction(QIcon(":/icon/disable"), tr("Disable"));
            connect(actionUnactivate, SIGNAL(triggered()), this, SLOT(unactivateSets()));
        }
        else
        {
            QAction* actionActivate = addAction(QIcon(":/icon/enable"), tr("Enable"));
            connect(actionActivate, SIGNAL(triggered()), this, SLOT(activateSets()));
        }

        addSeparator();

        QAction* actionEdit = addAction(QIcon(":/icon/edit"), tr("Edit"));
        connect(actionEdit, SIGNAL(triggered()), this, SLOT(editSets()));

        QAction* actionOpen = addAction(QIcon(":/icon/folder"), tr("Open directory"));
        connect(actionOpen, SIGNAL(triggered()), this, SLOT(openSets()));

        QAction* actionClearCache = addAction(QIcon(":/icon/clear"), tr("Clear cache"));
        connect(actionClearCache, SIGNAL(triggered()), this, SLOT(clearCache()));

        addSeparator();

        QAction* actionDelete = addAction(QIcon(":/icon/delete"), tr("Delete"));
        connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteSets()));
    }

    QAction* actionAdd = addAction(QIcon(":/icon/add"), tr("Add set"));
    connect(actionAdd, SIGNAL(triggered()), _parent, SLOT(addSet()));
}

/**
 * @brief Open set edit dialog
 */
void SetContextMenu::editSets()
{
    SetEditDialog dialog(this, m_settings, m_sets);

    // need to unbind hotkeys to allow hotkeys input
    ((MainWindow*)parent())->clearHotkeys();
    if (dialog.exec())
    {
        dialog.save();
        m_ctrl->emitListChanged();
    }
    ((MainWindow*)parent())->defineHotkeys();
}

/**
 * @brief Open directories of selected sets
 */
void SetContextMenu::openSets()
{
    foreach (int i, m_sets)
    {
        QDesktopServices::openUrl(QUrl("file:///" + m_settings->set(i)->path()));
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
        m_settings->deleteSets(m_sets);
        m_ctrl->emitListChanged(true);
    }
}

/**
 * @brief Clear sets cache of selected sets
 */
void SetContextMenu::clearCache()
{
    foreach (int i, m_sets)
    {
        m_settings->set(i)->deleteCache();
    }
}

/**
 * @brief Activate selected sets
 */
void SetContextMenu::activateSets()
{
    m_settings->activateSets(m_sets);
    m_ctrl->emitListChanged();
}

/**
 * @brief Unactivate selected sets
 */
void SetContextMenu::unactivateSets()
{
    m_settings->unactivateSets(m_sets);
    m_ctrl->emitListChanged();
}
