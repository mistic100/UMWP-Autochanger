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
 * @param Controller* _poCtrl
 */
MainWidget::MainWidget(QWidget* _parent, Controller* _ctrl) : QWidget(_parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_ctrl = _ctrl;

    connect(m_ctrl, SIGNAL(listChanged(bool)), this, SLOT(slotUpdateList(bool)));

    // main list
    ui->mainList->setItemDelegate(new ListDelegate(ui->mainList, m_ctrl));
    ui->mainList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->mainList->setDragDropMode(QAbstractItemView::InternalMove);
    ui->mainList->setStyle(new ListProxyStyle);

    connect(ui->mainList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(slotMoveItem(QModelIndex,int,int,QModelIndex,int)));

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
void MainWidget::slotUpdateList(bool _resetSel)
{
    QList<int> aIndexes;

    if (!_resetSel)
    {
        aIndexes = aGetSelectedIndexes();
    }
    else
    {
        ui->buttonActivate->setVisible(false);
        ui->buttonDeactivate->setVisible(false);
        ui->buttonDelete->setVisible(false);
    }

    ui->mainList->clear();

    for (int i=0; i<m_ctrl->settings()->nbSets(); i++)
    {
        QListWidgetItem* pItem = new QListWidgetItem();
        pItem->setData(Qt::UserRole, i);

        ui->mainList->addItem(pItem);
        pItem->setSelected(aIndexes.contains(i));
    }
}

/**
 * @brief Get numerical indexes of selected items
 * @return int[]
 */
QList<int> MainWidget::aGetSelectedIndexes()
{
    QList<QListWidgetItem*> apItems = ui->mainList->selectedItems();
    QList<int> aIndexes;

    for (QList<QListWidgetItem*>::iterator it=apItems.begin(); it!=apItems.end(); it++)
    {
        aIndexes.push_back((*it)->data(Qt::UserRole).toInt());
    }

    qSort(aIndexes);
    return aIndexes;
}

/**
 * @brief Open file dialog to add a new set
 */
void MainWidget::on_buttonAdd_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Add"),
                                                        m_ctrl->settings()->sParam("last_dir"));

    if (!dirname.isEmpty())
    {
        QDir dir(dirname);
        dir.cdUp();
        m_ctrl->settings()->setParam("last_dir", dir.absolutePath());

        dirname.replace('/', '\\');
        m_ctrl->settings()->addSet(dirname);
        m_ctrl->emitListChanged();
    }
}

/**
 * @brief Open confirmation to delete selected sets
 */
void MainWidget::on_buttonDelete_clicked()
{
    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        m_ctrl->settings()->deleteSets(aGetSelectedIndexes());
        m_ctrl->emitListChanged();
    }
}

/**
 * @brief Activate selected sets
 */
void MainWidget::on_buttonActivate_clicked()
{
    m_ctrl->settings()->activateSets(aGetSelectedIndexes());
    m_ctrl->emitListChanged();
}

/**
 * @brief Deactivate selected sets
 */
void MainWidget::on_buttonDeactivate_clicked()
{
    m_ctrl->settings()->unactivateSets(aGetSelectedIndexes());
    m_ctrl->emitListChanged();
}

/**
 * @brief Open dialog for set edition
 */
void MainWidget::on_mainList_itemDoubleClicked(QListWidgetItem*)
{
    int index = aGetSelectedIndexes().at(0);
    Set* pSet = m_ctrl->settings()->getSet(index);

    SetEditDialog dialog(this, pSet, m_ctrl->settings());

    ((MainWindow*)parent())->clearHotkeys();
    if (dialog.exec())
    {
        m_ctrl->settings()->editSet(index, dialog.name(), dialog.type(), dialog.style(), dialog.hotkey());
        m_ctrl->emitListChanged();
    }
    ((MainWindow*)parent())->defineHotkeys();
}

/**
 * @brief Update buttons visibility on user selection
 */
void MainWidget::on_mainList_itemSelectionChanged()
{
    QList<int> aIndexes = aGetSelectedIndexes();

    if (aIndexes.size() == 0)
    {
        ui->buttonActivate->setVisible(false);
        ui->buttonDeactivate->setVisible(false);
        ui->buttonDelete->setVisible(false);
    }
    else
    {
        int nbActive = 0, nbInactive=0;

        for (QList<int>::iterator i=aIndexes.begin(); i!=aIndexes.end(); i++)
        {
            if (m_ctrl->settings()->getSet(*i)->isActive())
            {
                nbActive++;
            }
            else
            {
                nbInactive++;
            }
        }

        if (nbActive >= nbInactive)
        {
            ui->buttonActivate->setVisible(false);
            ui->buttonDeactivate->setVisible(true);
        }
        else
        {
            ui->buttonActivate->setVisible(true);
            ui->buttonDeactivate->setVisible(false);
        }
        ui->buttonDelete->setVisible(true);
    }
}

/**
 * @brief Move an item
 * @param int from
 * @param int to
 */
void MainWidget::slotMoveItem(const QModelIndex &, int from, int, const QModelIndex &, int to)
{
    m_ctrl->settings()->moveSet(from, to);
    m_ctrl->emitListChanged();
}
