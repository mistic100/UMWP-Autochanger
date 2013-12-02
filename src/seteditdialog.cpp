#include "seteditdialog.h"
#include "ui_seteditdialog.h"


/**
 * @brief SetEditDialog::SetEditDialog
 * @param QWidget* _parent
 * @param Set* _poSet
 */
SetEditDialog::SetEditDialog(QWidget* _parent, Set* _poSet) : QDialog(_parent),
    ui(new Ui::SetEditDialog)
{
    ui->setupUi(this);

    setFixedSize(size());

    ui->m_poTypeSelect->addItem(QIcon(":/icon/w_monitor"), tr("One image for each monitor"),        1);
    ui->m_poTypeSelect->addItem(QIcon(":/icon/w_desktop"), tr("One image for the whole desktop"),   0);

    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_center"),         tr("Center"),               0);
    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_tile"),           tr("Tile"),                 1);
    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_stretch"),        tr("Stretch"),              2);
    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_stretch_prop"),   tr("Strecth proportional"), 3);

    ui->m_poNameInput->setText(_poSet->name());

    int iIndex = ui->m_poTypeSelect->findData(_poSet->type());
    ui->m_poTypeSelect->setCurrentIndex(iIndex);

    iIndex = ui->m_poStyleSelect->findData(_poSet->style());
    ui->m_poStyleSelect->setCurrentIndex(iIndex);
}

/**
 * @brief SetEditDialog::~SetEditDialog
 */
SetEditDialog::~SetEditDialog()
{
    delete ui;
}

/**
 * @brief SetEditDialog::name
 * @return string
 */
const QString SetEditDialog::name() const
{
    return ui->m_poNameInput->text();
}

/**
 * @brief SetEditDialog::type
 * @return int
 */
const UM::WALLPAPER SetEditDialog::type() const
{
    int iIndex = ui->m_poTypeSelect->currentIndex();
    return static_cast<UM::WALLPAPER>(ui->m_poTypeSelect->itemData(iIndex).toInt());
}

/**
 * @brief SetEditDialog::style
 * @return int
 */
const UM::IMAGE SetEditDialog::style() const
{
    int iIndex = ui->m_poStyleSelect->currentIndex();
    return static_cast<UM::IMAGE>(ui->m_poStyleSelect->itemData(iIndex).toInt());
}
