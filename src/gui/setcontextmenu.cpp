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
SetContextMenu::SetContextMenu(MainWindow* _parent, const QList<Set*> &_sets) :
    QMenu(_parent),
    m_parent(_parent),
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

        if (nbActive >= nbInactive)
        {
            QAction* actionUnactivate = addAction(QIcon(":/images/icons/disable.png"), tr("Disable"));
            connect(actionUnactivate, &QAction::triggered, this, [this]{ m_parent->unactivateSets(m_sets); });
        }
        else
        {
            QAction* actionActivate = addAction(QIcon(":/images/icons/enable.png"), tr("Enable"));
            connect(actionActivate, &QAction::triggered, this, [this]{ m_parent->activateSets(m_sets); });
        }

        addSeparator();
        QAction* actionEdit =   addAction(QIcon(":/images/icons/edit.png"),   tr("Edit"));
        QAction* actionOpen =   addAction(QIcon(":/images/icons/folder.png"), tr("Open directory"));
        addSeparator();
        QAction* actionDelete = addAction(QIcon(":/images/icons/delete.png"), tr("Delete"));

        connect(actionEdit,   &QAction::triggered, this, [this]{ m_parent->editSets(m_sets); });
        connect(actionOpen,   &QAction::triggered, this, [this]{ m_parent->openSets(m_sets); });
        connect(actionDelete, &QAction::triggered, this, [this]{ m_parent->deleteSets(m_sets); });
    }

    QAction* actionAdd = addAction(QIcon(":/images/icons/add_color.png"), tr("Add set"));
    connect(actionAdd, &QAction::triggered, this, [this]{ m_parent->addSet(); });
}
