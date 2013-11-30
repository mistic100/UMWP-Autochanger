#include "setedit.h"
#include "ui_setedit.h"


SetEdit::SetEdit(const QString &_sName, int const _iType, int const _iStyle, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SetEdit)
{
    ui->setupUi(this);

    ui->m_poTypeSelect->addItem(tr("One image for each monitor"), 1);
    ui->m_poTypeSelect->addItem(tr("One image for the whole desktop"), 0);

    ui->m_poStyleSelect->addItem(tr("Center"), 0);
    ui->m_poStyleSelect->addItem(tr("Tile"), 1);
    ui->m_poStyleSelect->addItem(tr("Stretch"), 2);
    ui->m_poStyleSelect->addItem(tr("Strecth proportional"), 3);

    ui->m_poNameInput->setText(_sName);

    int index = ui->m_poTypeSelect->findData(_iType);
    ui->m_poTypeSelect->setCurrentIndex(index);

    index = ui->m_poStyleSelect->findData(_iStyle);
    ui->m_poStyleSelect->setCurrentIndex(index);
}

SetEdit::~SetEdit()
{
    delete ui;
}

QString const SetEdit::name() const
{
    return ui->m_poNameInput->text();
}

int const SetEdit::type() const
{
    int index = ui->m_poTypeSelect->currentIndex();
    return ui->m_poTypeSelect->itemData(index).toInt();
}

int const SetEdit::style() const
{
    int index = ui->m_poStyleSelect->currentIndex();
    return ui->m_poStyleSelect->itemData(index).toInt();
}
