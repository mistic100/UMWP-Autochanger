#include <QVBoxLayout>
#include <QGridLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QSpinBox>

#include "mainwidget.h"
#include "set.h"
#include "listdelegate.h"
#include "listproxystyle.h"


/*
 * constructor
 */
MainWidget::MainWidget(QWidget* _parent, Controller* _poCtrl) : QWidget(_parent)
{
    m_poCtrl = _poCtrl;
    connect(m_poCtrl, SIGNAL(listChanged()), this, SLOT(vUpdateList()));

    m_sLastDir = QDir::homePath();


    // main list
    m_poList = new QListWidget();
    m_poList->setItemDelegate(new ListDelegate(m_poList));
    m_poList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_poList->setDragDropMode(QAbstractItemView::InternalMove);
    m_poList->setStyle(new ListProxyStyle);

    // buttons
    QPushButton* poAddButton =      new QPushButton(tr("Add"));
    m_poActivateButton =            new QPushButton(tr("Activate"));
    m_poUnactivateButton =          new QPushButton(tr("Unactivate"));
    m_poDeleteButton =              new QPushButton(tr("Delete"));
    QPushButton* poApplyButton =    new QPushButton(tr("Save"));

    // spin box
    QSpinBox* poDelayInput = new QSpinBox();
    poDelayInput->setRange(10, 3600);
    poDelayInput->setSingleStep(10);
    poDelayInput->setFixedWidth(60);
    poDelayInput->setValue(m_poCtrl->settings()->iParam("delay"));


    // layouts
    QVBoxLayout* poRightCollumn = new QVBoxLayout();
    poRightCollumn->addWidget(poAddButton);
    poRightCollumn->addWidget(m_poActivateButton);
    poRightCollumn->addWidget(m_poUnactivateButton);
    poRightCollumn->addWidget(m_poDeleteButton);
    poRightCollumn->addStretch();
    poRightCollumn->addWidget(poApplyButton);

    QHBoxLayout* poTopLine = new QHBoxLayout();
    poTopLine->addWidget(new QLabel(tr("Delay (seconds) : ")));
    poTopLine->addWidget(poDelayInput);
    poTopLine->addStretch();

    QGridLayout* poMainLayout = new QGridLayout();
    poMainLayout->addLayout(poTopLine, 0, 0);
    poMainLayout->addWidget(m_poList, 1, 0);
    poMainLayout->addLayout(poRightCollumn, 1, 1);


    // buttons events
    connect(poAddButton,            SIGNAL(clicked()), this, SLOT(vSlotAddSet()));
    connect(m_poActivateButton,     SIGNAL(clicked()), this, SLOT(vSlotActivateSets()));
    connect(m_poUnactivateButton,   SIGNAL(clicked()), this, SLOT(vSlotUnactivateSets()));
    connect(m_poDeleteButton,       SIGNAL(clicked()), this, SLOT(vSlotDeleteSets()));
    connect(poApplyButton,          SIGNAL(clicked()), _parent, SLOT(vSlotApply()));

    // list events
    connect(m_poList, SIGNAL(itemSelectionChanged()), this, SLOT(vSlotSelectionChanged()));
    connect(m_poList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(vSlotItemDoubleClicked()));
    connect(m_poList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(vSlotItemMoved(QModelIndex,int,int,QModelIndex,int)));

    // spin box event
    connect(poDelayInput, SIGNAL(valueChanged(int)), _parent, SLOT(vSlotDelayChanged(int)));


    setLayout(poMainLayout);

    vUpdateList();
}

/*
 * update list widget contents
 */
void MainWidget::vUpdateList()
{
    m_poList->clear();

    m_poActivateButton->setVisible(false);
    m_poUnactivateButton->setVisible(false);
    m_poDeleteButton->setVisible(false);

    for (int i=0; i<m_poCtrl->settings()->iNbSets(); i++)
    {
        Set* poSet = m_poCtrl->settings()->poGetSet(i);

        QListWidgetItem* poItem = new QListWidgetItem();
        poItem->setData(Qt::DisplayRole, poSet->fullName());
        poItem->setData(Qt::UserRole, i);
        poItem->setData(Qt::UserRole+1, poSet->path());
        poItem->setData(Qt::UserRole+2, poSet->isActive());

        if (poSet->isActive())
            poItem->setData(Qt::DecorationRole, QPixmap(":/img/bullet_green"));
        else
            poItem->setData(Qt::DecorationRole, QPixmap(":/img/bullet_red"));

        m_poList->addItem(poItem);
    }
}

/*
 * get numerical indexes of selected items
 */
QList<int> MainWidget::oGetSelectedIndexes()
{
    QList<QListWidgetItem*> items = m_poList->selectedItems();
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
void MainWidget::vSlotAddSet()
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
void MainWidget::vSlotDeleteSets()
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
void MainWidget::vSlotActivateSets()
{
    m_poCtrl->vActivateSets(oGetSelectedIndexes());
}

/*
 * unactivate sets
 */
void MainWidget::vSlotUnactivateSets()
{
    m_poCtrl->vUnactivateSets(oGetSelectedIndexes());
}

/*
 * prompt to rename a set
 */
void MainWidget::vSlotItemDoubleClicked()
{
    int index = oGetSelectedIndexes().at(0);
    Set* poSet = m_poCtrl->settings()->poGetSet(index);

    bool ok;
    QString text = QInputDialog::getText(this, tr("Name"), tr("New name:"), QLineEdit::Normal, poSet->name(), &ok);

    if (ok && !text.isEmpty())
    {
        m_poCtrl->vRenameSet(index, text);
    }
}

/*
 * update model when an item is moved
 */
void MainWidget::vSlotItemMoved(const QModelIndex &, int from, int, const QModelIndex &, int to)
{
    m_poCtrl->vMoveSet(from, to);
}

/*
 * update buttons depending on list selection
 */
void MainWidget::vSlotSelectionChanged()
{
    QList<int> list = oGetSelectedIndexes();

    if (list.size() == 0)
    {
        m_poActivateButton->setVisible(false);
        m_poUnactivateButton->setVisible(false);
        m_poDeleteButton->setVisible(false);
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
            m_poActivateButton->setVisible(false);
            m_poUnactivateButton->setVisible(true);
        }
        else
        {
            m_poActivateButton->setVisible(true);
            m_poUnactivateButton->setVisible(false);
        }
        m_poDeleteButton->setVisible(true);
    }
}
