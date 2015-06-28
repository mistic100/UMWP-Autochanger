#include "mainwidget.h"
#include "mainwindow.h"
#include "set.h"
#include "listdelegate.h"
#include "listproxystyle.h"
#include "seteditdialog.h"


/**
 * @brief MainWidget::MainWidget
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
MainWidget::MainWidget(QWidget* _parent, Controller* _ctrl) :
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

    onListChanged(true);
}

/**
 * @brief Update list widget contents
 * @param bool _resetSel - force reset of user selection
 */
void MainWidget::onListChanged(bool _resetSel)
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
            item->setToolTip(tr("This set doesn't exist on the disk anymore"));
        }
    }
}

/**
 * @brief Get numerical indexes of selected items
 * @return int[]
 */
QList<int> MainWidget::getSelectedIndexes()
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
 * @brief Move an item
 * @param int from
 * @param int to
 */
void MainWidget::onItemMoved(const QModelIndex &, int from, int, const QModelIndex &, int to)
{
    m_settings->moveSet(from, to);
    m_ctrl->emitListChanged(true);
}

/**
 * @brief Right click on list
 * @param QPoint _pos
 */
void MainWidget::onContextMenu(const QPoint &_pos)
{
    QList<int> sets = getSelectedIndexes();
    QPoint pos = mapToGlobal(_pos);
    ((MainWindow*)parent())->showContextMenu(sets, pos);
}
