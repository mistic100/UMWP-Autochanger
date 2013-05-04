#include <QVBoxLayout>
#include <QGridLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QLabel>

#include "mainwidget.h"
#include "set.h"
#include "listdelegate.h"


/*
 * constructor
 */
MainWidget::MainWidget(QWidget* _parent, Controller* _poCtrl) : QWidget(_parent)
{
    m_poCtrl = _poCtrl;

    // main list
    m_poList = new QListWidget();
    m_poList->setItemDelegate(new ListDelegate(m_poList));
    m_poList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // buttons
    QPushButton* poAddButton =      new QPushButton(tr("Add"));
    m_poActivateButton =            new QPushButton(tr("Activate"));
    m_poUnactivateButton =          new QPushButton(tr("Unactivate"));
    m_poDeleteButton =              new QPushButton(tr("Delete"));
    QPushButton* poApplyButton =    new QPushButton(tr("Apply"));

    // spin box
    m_poDelayInput = new QSpinBox();
    m_poDelayInput->setRange(10, 3600);
    m_poDelayInput->setSingleStep(10);
    m_poDelayInput->setFixedWidth(60);
    m_poDelayInput->setValue(m_poCtrl->settings()->iDelay());


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
    poTopLine->addWidget(m_poDelayInput);
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

    // spin box changed
    connect(m_poDelayInput, SIGNAL(valueChanged(int)), this, SLOT(vSlotDelayChanged(int)));

    // list updated by the controller
    connect(m_poCtrl, SIGNAL(listChanged()), this, SLOT(vUpdateList()));


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
        poItem->setData(Qt::DisplayRole, poSet->sGetFullName());
        poItem->setData(Qt::UserRole, i);
        poItem->setData(Qt::UserRole+1, poSet->path());
        poItem->setData(Qt::UserRole+2, poSet->isActive());
        vSetListItemIcon(poItem, poSet->isActive());

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

    return list;
}

/*
 * prompt to add a new set
 */
void MainWidget::vSlotAddSet()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Add"), QDir::homePath());

    if (!dirname.isEmpty())
    {
        dirname.replace('/', '\\');
        m_poCtrl->settings()->oAddSet(dirname);
        vUpdateList();
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
        m_poCtrl->settings()->vDeleteSets(oGetSelectedIndexes());
        vUpdateList();
    }
}

/*
 * activate sets
 */
void MainWidget::vSlotActivateSets()
{
    QList<int> list = oGetSelectedIndexes();

    for (QList<int>::iterator i=list.begin(); i!=list.end(); i++)
    {
        m_poCtrl->settings()->vSetState(*i, true);
        QListWidgetItem* poItem = m_poList->item(*i);
        poItem->setData(Qt::UserRole+2, true);
        vSetListItemIcon(poItem, true);
    }

    m_poActivateButton->setVisible(false);
    m_poUnactivateButton->setVisible(true);
}

/*
 * unactivate sets
 */
void MainWidget::vSlotUnactivateSets()
{
    QList<int> list = oGetSelectedIndexes();

    for (QList<int>::iterator i=list.begin(); i!=list.end(); i++)
    {
        m_poCtrl->settings()->vSetState(*i, false);
        QListWidgetItem* poItem = m_poList->item(*i);
        poItem->setData(Qt::UserRole+2, false);
        vSetListItemIcon(poItem, false);
    }

    m_poActivateButton->setVisible(true);
    m_poUnactivateButton->setVisible(false);
}

/*
 * set the icon of a list widget item
 */
void MainWidget::vSetListItemIcon(QListWidgetItem* _poItem, bool _active)
{
    if (_active)
    {
        _poItem->setData(Qt::DecorationRole, QPixmap(":/img/bullet_green"));
    }
    else
    {
        _poItem->setData(Qt::DecorationRole, QPixmap(":/img/bullet_red"));
    }
}

/*
 * prompt to rename a set
 */
void MainWidget::vSlotItemDoubleClicked()
{
    int index = oGetSelectedIndexes().at(0);
    Set* poSet = m_poCtrl->settings()->poGetSet(index);
    QListWidgetItem* poItem = m_poList->item(index);

    bool ok;
    QString text = QInputDialog::getText(this, tr("Name"), tr("New name:"), QLineEdit::Normal, poSet->name(), &ok);

    if (ok && !text.isEmpty())
    {
        QString name = m_poCtrl->settings()->sRenameSet(index, text, true);
        poItem->setText(name);
    }
}

/*
 * the selection of the list changed
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


/*
 * the delay value changed
 */
void MainWidget::vSlotDelayChanged(int _val)
{
    m_poCtrl->settings()->vSetDelay(_val);
}
