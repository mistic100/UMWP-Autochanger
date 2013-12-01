#include "setedit.h"
#include "ui_setedit.h"


/**
 * @brief SetEdit::SetEdit
 * @param QWidget* _parent
 * @param Set* _poSet
 */
SetEdit::SetEdit(QWidget* _parent, Set* _poSet) : QDialog(_parent),
    ui(new Ui::SetEdit)
{
    ui->setupUi(this);

    ui->m_poTypeSelect->addItem(QIcon(":/icon/w_monitor"), tr("One image for each monitor"), 1);
    ui->m_poTypeSelect->addItem(QIcon(":/icon/w_desktop"), tr("One image for the whole desktop"), 0);

    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_center"), tr("Center"), 0);
    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_tile"), tr("Tile"), 1);
    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_stretch"), tr("Stretch"), 2);
    ui->m_poStyleSelect->addItem(QIcon(":/icon/im_stretch_prop"), tr("Strecth proportional"), 3);

    ui->m_poNameInput->setText(_poSet->name());

    int iIndex = ui->m_poTypeSelect->findData(_poSet->type());
    ui->m_poTypeSelect->setCurrentIndex(iIndex);

    iIndex = ui->m_poStyleSelect->findData(_poSet->style());
    ui->m_poStyleSelect->setCurrentIndex(iIndex);
}

/**
 * @brief SetEdit::~SetEdit
 */
SetEdit::~SetEdit()
{
    delete ui;
}

/**
 * @brief SetEdit::name
 * @return string
 */
const QString SetEdit::name() const
{
    return ui->m_poNameInput->text();
}

/**
 * @brief SetEdit::type
 * @return int
 */
const UM::WALLPAPER SetEdit::type() const
{
    int iIndex = ui->m_poTypeSelect->currentIndex();
    return static_cast<UM::WALLPAPER>(ui->m_poTypeSelect->itemData(iIndex).toInt());
}

/**
 * @brief SetEdit::style
 * @return int
 */
const UM::IMAGE SetEdit::style() const
{
    int iIndex = ui->m_poStyleSelect->currentIndex();
    return static_cast<UM::IMAGE>(ui->m_poStyleSelect->itemData(iIndex).toInt());
}
