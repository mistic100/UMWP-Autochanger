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
    m_poCtrl->vSetWidget(this);
    //m_iSelectedSet = -1; // done in vUpdateList()

    m_poList = new QListWidget();
    m_poList->setItemDelegate(new ListDelegate(m_poList));


    // buttons
    QPushButton* poAddButton = new QPushButton(tr("Add"));
    m_poActivateButton = new QPushButton(tr("Activate"));
    m_poDeleteButton = new QPushButton(tr("Delete"));
    QPushButton* poApplyButton = new QPushButton(tr("Apply"));

    //m_poActivateButton->setDisabled(true); // done in vUpdateList()
    //m_poDeleteButton->setDisabled(true); // done in vUpdateList()

    m_poDelayInput = new QSpinBox();
    m_poDelayInput->setValue(m_poCtrl->settings()->iDelay());
    m_poDelayInput->setRange(10, 3600);
    m_poDelayInput->setSingleStep(10);
    m_poDelayInput->setFixedWidth(60);


    // layouts
    QVBoxLayout* poRightCollumn = new QVBoxLayout();
    poRightCollumn->addWidget(poAddButton);
    poRightCollumn->addWidget(m_poActivateButton);
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


    // events
    connect(poAddButton, SIGNAL(clicked()), this, SLOT(vSlotAddSet()));
    connect(m_poActivateButton, SIGNAL(clicked()), this, SLOT(vSlotSwitchSet()));
    connect(m_poDeleteButton, SIGNAL(clicked()), this, SLOT(vSlotDeleteSet()));
    connect(poApplyButton, SIGNAL(clicked()), _parent, SLOT(vSlotApply()));

    connect(m_poList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(vSlotItemClicked(QListWidgetItem*)));
    connect(m_poList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(vSlotItemDoubleClicked()));

    connect(m_poDelayInput, SIGNAL(valueChanged(int)), this, SLOT(vSlotDelayChanged(int)));


    setLayout(poMainLayout);

    vUpdateList();
}

/*
 * destructor
 */
MainWidget::~MainWidget()
{
    delete m_poDeleteButton;
    delete m_poActivateButton;
    delete m_poDelayInput;
    delete m_poList;
}

/*
 * update list widget contents
 */
void MainWidget::vUpdateList()
{
    m_iSelectedSet = -1;
    m_poList->clear();

    m_poActivateButton->setDisabled(true);
    m_poDeleteButton->setDisabled(true);

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
 * prompt to delete a set
 */
void MainWidget::vSlotDeleteSet()
{
    int ret = QMessageBox::warning(this, tr("Delete"), tr("Are you sure?"), QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        m_poCtrl->settings()->vDeleteSet(m_iSelectedSet);
        vUpdateList();
    }
}

/*
 * switch the state of a set
 */
void MainWidget::vSlotSwitchSet()
{
    bool active = m_poCtrl->settings()->bSwitchSet(m_iSelectedSet);
    QListWidgetItem* poItem = m_poList->item(m_iSelectedSet);

    poItem->setData(Qt::UserRole+2, active);
    vSetListItemIcon(poItem, active);
    vSlotItemClicked(poItem);
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
 * an item is selected
 */
void MainWidget::vSlotItemClicked(QListWidgetItem* _poItem)
{
    m_iSelectedSet = _poItem->data(Qt::UserRole).toInt();
    Set* poSet = m_poCtrl->settings()->poGetSet(m_iSelectedSet);

    if (poSet->isActive())
    {
        m_poActivateButton->setText(tr("Unactivate"));
    }
    else
    {
        m_poActivateButton->setText(tr("Activate"));
    }

    m_poActivateButton->setDisabled(false);
    m_poDeleteButton->setDisabled(false);
}

/*
 * prompt to rename a set (always called AFTER vItemClicked)
 */
void MainWidget::vSlotItemDoubleClicked()
{
    Set* poSet = m_poCtrl->settings()->poGetSet(m_iSelectedSet);
    QListWidgetItem* poItem = m_poList->item(m_iSelectedSet);

    bool ok;
    QString text = QInputDialog::getText(this, tr("Name"), tr("New name:"), QLineEdit::Normal, poSet->name(), &ok);

    if (ok && !text.isEmpty())
    {
        QString name = m_poCtrl->settings()->sRenameSet(m_iSelectedSet, text, true);
        poItem->setText(name);
    }
}

/*
 * the delay value changed
 */
void MainWidget::vSlotDelayChanged(int _val)
{
    m_poCtrl->settings()->vSetDelay(_val);
}
