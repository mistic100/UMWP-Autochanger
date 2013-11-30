#include <QFileDialog>
#include <QMessageBox>

#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "set.h"
#include "listdelegate.h"
#include "listproxystyle.h"
#include "setedit.h"


/*
 * constructor
 */
MainWidget::MainWidget(QWidget* _parent, Controller* _poCtrl) :
    QWidget(_parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_sLastDir = QDir::homePath();

    // controller
    m_poCtrl = _poCtrl;

    connect(m_poCtrl, SIGNAL(listChanged(bool)), this, SLOT(vUpdateList(bool)));

    // main list
    ui->m_poList->setItemDelegate(new ListDelegate(ui->m_poList, m_poCtrl));
    ui->m_poList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->m_poList->setDragDropMode(QAbstractItemView::InternalMove);
    ui->m_poList->setStyle(new ListProxyStyle);

    connect(ui->m_poList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(vSlotItemMoved(QModelIndex,int,int,QModelIndex,int)));

    // spin box
    ui->m_poInputDelay->setValue(m_poCtrl->settings()->iParam("delay"));

    connect(ui->m_poInputDelay, SIGNAL(valueChanged(int)), _parent, SLOT(vSlotDelayChanged(int)));

    // save button
    connect(ui->m_poButtonSave, SIGNAL(clicked()), _parent, SLOT(vSlotApply()));

    vUpdateList(true);
}

/*
 * destructor
 */
MainWidget::~MainWidget()
{
    delete ui;
}

/*
 * update list widget contents
 */
void MainWidget::vUpdateList(bool _bResetSel)
{
    QList<int> aiSelection;

    if (!_bResetSel)
    {
        aiSelection = oGetSelectedIndexes();
    }
    else
    {
        ui->m_poButtonActivate->setVisible(false);
        ui->m_poButtonDeactivate->setVisible(false);
        ui->m_poButtonDelete->setVisible(false);
    }

    ui->m_poList->clear();

    for (int i=0; i<m_poCtrl->settings()->iNbSets(); i++)
    {
        QListWidgetItem* poItem = new QListWidgetItem();
        poItem->setData(Qt::UserRole, i);

        ui->m_poList->addItem(poItem);
        poItem->setSelected(aiSelection.contains(i));
    }
}

/*
 * get numerical indexes of selected items
 */
QList<int> MainWidget::oGetSelectedIndexes()
{
    QList<QListWidgetItem*> items = ui->m_poList->selectedItems();
    QList<int> list;

    for (QList<QListWidgetItem*>::iterator it=items.begin(); it!=items.end(); it++)
    {
        list.push_back((*it)->data(Qt::UserRole).toInt());
    }

    qSort(list);

    return list;
}

/*
 * prompt to add a new set
 */
void MainWidget::on_m_poButtonAdd_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Add"), m_sLastDir);

    if (!dirname.isEmpty())
    {
        QDir dir(dirname);
        dir.cdUp();
        m_sLastDir = dir.absolutePath();

        dirname.replace('/', '\\');
        m_poCtrl->vAddSet(dirname);
    }
}

/*
 * prompt to delete sets
 */
void MainWidget::on_m_poButtonDelete_clicked()
{
    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        m_poCtrl->vDeleteSets(oGetSelectedIndexes());
    }
}

/*
 * activate sets
 */
void MainWidget::on_m_poButtonActivate_clicked()
{
    m_poCtrl->vActivateSets(oGetSelectedIndexes());
}

/*
 * deactivate sets
 */
void MainWidget::on_m_poButtonDeactivate_clicked()
{
    m_poCtrl->vUnactivateSets(oGetSelectedIndexes());
}

/*
 * prompt to rename a set
 */
void MainWidget::on_m_poList_itemDoubleClicked(QListWidgetItem*)
{
    int index = oGetSelectedIndexes().at(0);
    Set* poSet = m_poCtrl->settings()->poGetSet(index);

    SetEdit* dialog = new SetEdit(poSet->name(), poSet->type(), poSet->style());

    if (dialog->exec())
    {
        m_poCtrl->vEditSet(index, dialog->name(), dialog->type(), dialog->style());
    }
}

/*
 * update buttons depending on list selection
 */
void MainWidget::on_m_poList_itemSelectionChanged()
{
    QList<int> list = oGetSelectedIndexes();

    if (list.size() == 0)
    {
        ui->m_poButtonActivate->setVisible(false);
        ui->m_poButtonDeactivate->setVisible(false);
        ui->m_poButtonDelete->setVisible(false);
    }
    else
    {
        int nb_active=0, nb_inactive=0;

        for (QList<int>::iterator i=list.begin(); i!=list.end(); i++)
        {
            if (m_poCtrl->settings()->poGetSet(*i)->isActive())
            {
                nb_active++;
            }
            else
            {
                nb_inactive++;
            }
        }

        if (nb_active>=nb_inactive)
        {
            ui->m_poButtonActivate->setVisible(false);
            ui->m_poButtonDeactivate->setVisible(true);
        }
        else
        {
            ui->m_poButtonActivate->setVisible(true);
            ui->m_poButtonDeactivate->setVisible(false);
        }
        ui->m_poButtonDelete->setVisible(true);
    }
}

/*
 * update model when an item is moved
 */
void MainWidget::vSlotItemMoved(const QModelIndex &, int from, int, const QModelIndex &, int to)
{
    m_poCtrl->vMoveSet(from, to);
}
