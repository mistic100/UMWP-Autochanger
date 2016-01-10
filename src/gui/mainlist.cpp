#include "mainlist.h"
#include "../set.h"
#include "listdelegate.h"
#include "listproxystyle.h"
#include "seteditdialog.h"
#include "setcontextmenu.h"
#include "mainwindow.h"


/**
 * @brief MainList::MainList
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
MainList::MainList(QWidget* _parent, Controller* _ctrl) :
    QListWidget(_parent),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings())
{
    setItemDelegate(new ListDelegate(this, m_ctrl->settings()));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setStyle(new ListProxyStyle());

    connect(model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(onItemMoved(QModelIndex,int,int,QModelIndex,int)));

    connect(m_ctrl, SIGNAL(listChanged(bool)), this, SLOT(onListChanged(bool)));

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenu(QPoint)));

    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onItemDoubleClicked(QListWidgetItem*)));

    installEventFilter(this);

    onListChanged(true);
}

/**
 * @brief Update list widget contents
 * @param bool _resetSel - force reset of user selection
 */
void MainList::onListChanged(bool _resetSel)
{
    QList<int> indexes;

    if (!_resetSel)
    {
        indexes = getSelectedIndexes();
    }

    clear();

    for (int i=0, l=m_settings->nbSets(); i<l; i++)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::UserRole, i);

        addItem(item);
        item->setSelected(indexes.contains(i));

        if (!m_settings->set(i)->isValid())
        {
            if (m_settings->set(i)->count() == 0)
            {
                item->setToolTip(tr("This set does not have any image files."));
            }
            else
            {
                item->setToolTip(tr("This set does not exist on the disk anymore."));
            }
        }
    }
}

/**
 * @brief Get numerical indexes of selected items
 * @return int[]
 */
QList<int> MainList::getSelectedIndexes() const
{
    QList<QListWidgetItem*> items = selectedItems();
    QList<int> indexes;

    foreach (const QListWidgetItem* item, items)
    {
        indexes.append(item->data(Qt::UserRole).toInt());
    }

    qSort(indexes);
    return indexes;
}

/**
 * @brief Get selected sets
 * @return Set*[]
 */
QList<Set*> MainList::getSelectedSets() const
{
    QList<Set*> sets;

    foreach (const int index, getSelectedIndexes())
    {
        sets.append(m_settings->set(index));
    }

    return sets;
}

/**
 * @brief Move an item
 * @param int from
 * @param int to
 */
void MainList::onItemMoved(const QModelIndex &, int from, int, const QModelIndex &, int to)
{
    m_ctrl->moveSet(from, to);
}

/**
 * @brief Edit double clicked set
 */
void MainList::onItemDoubleClicked(QListWidgetItem*)
{
    QList<Set*> sets = getSelectedSets();
    ((MainWindow*) parent())->editSets(sets);
}

/**
 * @brief Right click on list
 * @param QPoint _pos
 */
void MainList::onContextMenu(const QPoint &_pos)
{
    QList<Set*> sets = getSelectedSets();
    QPoint pos = mapToGlobal(_pos);

    SetContextMenu menu((QWidget*) parent(), m_ctrl, sets);
    menu.exec(pos);
}

/**
 * @brief Delete sets whene DEL key is pressed
 * @param QObject* _target
 * @param Qevent* _event
 * @return bool
 */
bool MainList::eventFilter(QObject* _target, QEvent* _event)
{
    if (_event->type() == QEvent::KeyPress)
    {
        QKeyEvent* key = (QKeyEvent*) _event;
        if (key->key() == Qt::Key_Delete && key->modifiers() ==Qt::NoModifier)
        {
            QList<Set*> sets = getSelectedSets();
            ((MainWindow*) parent())->deleteSets(sets);

            return true;
        }
    }

    return QListWidget::eventFilter(_target, _event);
}
