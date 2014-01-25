#include <QMessageBox>

#include "seteditdialog.h"
#include "ui_seteditdialog.h"


/**
 * @brief SetEditDialog::SetEditDialog
 * @param QWidget* _parent
 * @param Set* _poSet
 */
SetEditDialog::SetEditDialog(QWidget* _parent, Set* _pSet, Settings* _pSettings) : QDialog(_parent),
    ui(new Ui::SetEditDialog)
{
    ui->setupUi(this);

    m_pSettings = _pSettings;

    setFixedSize(size());

    ui->selectType->addItem(QIcon(":/icon/w_monitor"), tr("One image for each monitor"),      1);
    ui->selectType->addItem(QIcon(":/icon/w_desktop"), tr("One image for the whole desktop"), 0);

    ui->selectStyle->addItem(QIcon(":/icon/im_center"),         tr("Center"),               0);
    ui->selectStyle->addItem(QIcon(":/icon/im_tile"),           tr("Tile"),                 1);
    ui->selectStyle->addItem(QIcon(":/icon/im_stretch"),        tr("Stretch"),              2);
    ui->selectStyle->addItem(QIcon(":/icon/im_stretch_prop"),   tr("Strecth proportional"), 3);

    ui->inputName->setText(_pSet->name());

    int index = ui->selectType->findData(_pSet->type());
    ui->selectType->setCurrentIndex(index);

    index = ui->selectStyle->findData(_pSet->style());
    ui->selectStyle->setCurrentIndex(index);

    ui->inputHotkey->setHotkey(_pSet->hotkey());
    ui->inputHotkey->setDisabled(!m_pSettings->bParam("use_hotkeys"));
}

/**
 * @brief SetEditDialog::~SetEditDialog
 */
SetEditDialog::~SetEditDialog()
{
    delete ui;
}

/**
 * @brief Validate hotkey before save
 * @param int result
 */
void SetEditDialog::done(int result)
{
    if (result == QDialog::Accepted)
    {
        int hotkey = ui->inputHotkey->hotkey();
        QString error;

        if (hotkey)
        {
            if (hotkey == m_pSettings->hotkey("refresh"))
            {
                error = tr("Refresh");
            }
            else if (hotkey == m_pSettings->hotkey("startpause"))
            {
                error = tr("Start/Pause");
            }
            else if (hotkey == m_pSettings->hotkey("showhide"))
            {
                error = tr("Show/Hide");
            }
        }

        if (!error.isEmpty())
        {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Hotkey already used for \"%1\"").arg(error),
                                  QMessageBox::Ok, QMessageBox::Ok);
        }
        else if (ui->inputName->text().isEmpty())
        {
            error = "name";
            QMessageBox::critical(this, tr("Error"),
                                  tr("Name can not be empty"),
                                  QMessageBox::Ok, QMessageBox::Ok);
        }

        if (error.isEmpty())
        {
            QDialog::done(result);
        }
    }
    else
    {
        QDialog::done(result);
    }
}

/**
 * @brief SetEditDialog::name
 * @return string
 */
const QString SetEditDialog::name() const
{
    return ui->inputName->text();
}

/**
 * @brief SetEditDialog::type
 * @return int
 */
const UM::WALLPAPER SetEditDialog::type() const
{
    int index = ui->selectType->currentIndex();
    return static_cast<UM::WALLPAPER>(ui->selectType->itemData(index).toInt());
}

/**
 * @brief SetEditDialog::style
 * @return int
 */
const UM::IMAGE SetEditDialog::style() const
{
    int index = ui->selectStyle->currentIndex();
    return static_cast<UM::IMAGE>(ui->selectStyle->itemData(index).toInt());
}

/**
 * @brief SetEditDialog::hotkey
 * @return Hotkey
 */
const int SetEditDialog::hotkey() const
{
    return ui->inputHotkey->hotkey();
}
