#include <QFileDialog>
#include <QMessageBox>

#include "mainwidget.h"
#include "ui_mainwidget.h"
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
    QWidget(_parent),
    ui(new Ui::MainWidget),
    m_ctrl(_ctrl)
{
    ui->setupUi(this);

    ui->mainList->setItemDelegate(new ListDelegate(ui->mainList, m_ctrl->settings()));
    ui->mainList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->mainList->setDragDropMode(QAbstractItemView::InternalMove);
    ui->mainList->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->mainList->setStyle(new ListProxyStyle());

    connect(ui->mainList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(onItemMoved(QModelIndex,int,int,QModelIndex,int)));

    connect(m_ctrl, SIGNAL(listChanged(bool)), this, SLOT(onListChanged(bool)));

    onListChanged(true);
}

/**
 * @brief MainWidget::~MainWidget
 */
MainWidget::~MainWidget()
{
    delete ui;
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

    ui->mainList->clear();

    for (int i=0, l=m_ctrl->settings()->nbSets(); i<l; i++)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::UserRole, i);

        ui->mainList->addItem(item);
        item->setSelected(indexes.contains(i));

        if (!m_ctrl->settings()->set(i)->isValid())
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
    QList<QListWidgetItem*> items = ui->mainList->selectedItems();
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
    m_ctrl->settings()->moveSet(from, to);
    m_ctrl->emitListChanged(true);
}

/**
 * @brief Right click on list
 * @param QPoint _pos
 */
void MainWidget::on_mainList_customContextMenuRequested(const QPoint &_pos)
{
    QList<int> sets = getSelectedIndexes();
    QPoint pos = ui->mainList->mapToGlobal(_pos);
    ((MainWindow*)parent())->showContextMenu(sets, pos);
}
