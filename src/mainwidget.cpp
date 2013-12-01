#include <QFileDialog>
#include <QMessageBox>

#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "set.h"
#include "listdelegate.h"
#include "listproxystyle.h"
#include "setedit.h"


/**
 * @brief MainWidget::MainWidget
 * @param QWidget* _parent
 * @param Controller* _poCtrl
 */
MainWidget::MainWidget(QWidget* _parent, Controller* _poCtrl) : QWidget(_parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_sLastDir = QDir::homePath();
    m_poCtrl = _poCtrl;

    connect(m_poCtrl, SIGNAL(listChanged(bool)), this, SLOT(slotUpdateList(bool)));

    // main list
    ui->m_poList->setItemDelegate(new ListDelegate(ui->m_poList, m_poCtrl));
    ui->m_poList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->m_poList->setDragDropMode(QAbstractItemView::InternalMove);
    ui->m_poList->setStyle(new ListProxyStyle);
    connect(ui->m_poList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(slotMoveItem(QModelIndex,int,int,QModelIndex,int)));

    // spin box
    ui->m_poInputDelay->setValue(m_poCtrl->settings()->iParam("delay"));
    connect(ui->m_poInputDelay, SIGNAL(valueChanged(int)), _parent, SLOT(slotDelayChanged(int)));

    // save button
    connect(ui->m_poButtonSave, SIGNAL(clicked()), _parent, SLOT(slotApply()));

    slotUpdateList(true);
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
 * @param bool _bResetSel - force reset of user selection
 */
void MainWidget::slotUpdateList(bool _bResetSel)
{
    QList<int> aiSelection;

    if (!_bResetSel)
    {
        aiSelection = aiGetSelectedIndexes();
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

/**
 * @brief Get numerical indexes of selected items
 * @return int[]
 */
QList<int> MainWidget::aiGetSelectedIndexes()
{
    QList<QListWidgetItem*> apoItems = ui->m_poList->selectedItems();
    QList<int> aiList;

    for (QList<QListWidgetItem*>::iterator it=apoItems.begin(); it!=apoItems.end(); it++)
    {
        aiList.push_back((*it)->data(Qt::UserRole).toInt());
    }

    qSort(aiList);
    return aiList;
}

/**
 * @brief Open file dialog to add a new set
 */
void MainWidget::on_m_poButtonAdd_clicked()
{
    QString sDirname = QFileDialog::getExistingDirectory(this, tr("Add"), m_sLastDir);

    if (!sDirname.isEmpty())
    {
        QDir oDir(sDirname);
        oDir.cdUp();
        m_sLastDir = oDir.absolutePath();

        sDirname.replace('/', '\\');
        m_poCtrl->vAddSet(sDirname);
    }
}

/**
 * @brief Open confirmation to delete selected sets
 */
void MainWidget::on_m_poButtonDelete_clicked()
{
    int iRet = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (iRet == QMessageBox::Ok)
    {
        m_poCtrl->vDeleteSets(aiGetSelectedIndexes());
    }
}

/**
 * @brief Activate selected sets
 */
void MainWidget::on_m_poButtonActivate_clicked()
{
    m_poCtrl->vActivateSets(aiGetSelectedIndexes());
}

/**
 * @brief Deactivate selected sets
 */
void MainWidget::on_m_poButtonDeactivate_clicked()
{
    m_poCtrl->vUnactivateSets(aiGetSelectedIndexes());
}

/**
 * @brief Open dialog for set edition
 */
void MainWidget::on_m_poList_itemDoubleClicked(QListWidgetItem*)
{
    int iIndex = aiGetSelectedIndexes().at(0);
    Set* poSet = m_poCtrl->settings()->poGetSet(iIndex);

    SetEdit oDialog(this, poSet);

    if (oDialog.exec())
    {
        m_poCtrl->vEditSet(iIndex, oDialog.name(), oDialog.type(), oDialog.style());
    }
}

/**
 * @brief Update buttons visibility on user selection
 */
void MainWidget::on_m_poList_itemSelectionChanged()
{
    QList<int> aiList = aiGetSelectedIndexes();

    if (aiList.size() == 0)
    {
        ui->m_poButtonActivate->setVisible(false);
        ui->m_poButtonDeactivate->setVisible(false);
        ui->m_poButtonDelete->setVisible(false);
    }
    else
    {
        int nb_active=0, nb_inactive=0;

        for (QList<int>::iterator i=aiList.begin(); i!=aiList.end(); i++)
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

/**
 * @brief Move an item
 * @param int from
 * @param int to
 */
void MainWidget::slotMoveItem(const QModelIndex &, int from, int, const QModelIndex &, int to)
{
    m_poCtrl->vMoveSet(from, to);
}
