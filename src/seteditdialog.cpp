#include <QMessageBox>

#include "seteditdialog.h"
#include "ui_seteditdialog.h"


/**
 * @brief SetEditDialog::SetEditDialog
 * @param QWidget* _parent
 * @param Set* _set
 * @param Settings* _settings
 */
SetEditDialog::SetEditDialog(QWidget* _parent, Set* _set, Settings* _settings) : QDialog(_parent),
    ui(new Ui::SetEditDialog)
{
    ui->setupUi(this);

    setFixedSize(size());

    m_settings = _settings;

    ui->selectType->addItem(QIcon(":/icon/w_monitor"), tr("One image for each monitor"),      UM::W_MONITOR);
    ui->selectType->addItem(QIcon(":/icon/w_desktop"), tr("One image for the whole desktop"), UM::W_DESKTOP);

    ui->selectStyle->addItem(QIcon(":/icon/im_center"),       tr("Center"),               UM::IM_CENTER);
    ui->selectStyle->addItem(QIcon(":/icon/im_tile"),         tr("Tile"),                 UM::IM_TILE);
    ui->selectStyle->addItem(QIcon(":/icon/im_stretch"),      tr("Stretch"),              UM::IM_STRETCH);
    ui->selectStyle->addItem(QIcon(":/icon/im_stretch_prop"), tr("Strecth proportional"), UM::IM_STRETCH_PROP);
    ui->selectStyle->addItem(QIcon(":/icon/im_fill"),         tr("Fill"),                 UM::IM_FILL);

    ui->inputName->setText(_set->name());

    int index = ui->selectType->findData(_set->type());
    ui->selectType->setCurrentIndex(index);

    index = ui->selectStyle->findData(_set->style());
    ui->selectStyle->setCurrentIndex(index);

    ui->inputHotkey->setHotkey(_set->hotkey());
    ui->inputHotkey->setDisabled(!m_settings->bParam("use_hotkeys"));

    qxtLog->trace("SetEditDialog openned");
}

/**
 * @brief SetEditDialog::~SetEditDialog
 */
SetEditDialog::~SetEditDialog()
{
    delete ui;
}

/**
 * @brief Validate config before save
 * @param int result
 */
void SetEditDialog::done(int result)
{
    if (result == QDialog::Accepted)
    {
        QString error;

        int hotkey = ui->inputHotkey->hotkey();

        if (hotkey)
        {
            if (hotkey == m_settings->hotkey("refresh"))
            {
                error = tr("Hotkey already used for \"%1\"").arg(tr("Refresh"));
            }
            else if (hotkey == m_settings->hotkey("startpause"))
            {
                error = tr("Hotkey already used for \"%1\"").arg(tr("Start/Pause"));
            }
            else if (hotkey == m_settings->hotkey("showhide"))
            {
                error = tr("Hotkey already used for \"%1\"").arg(tr("Show/Hide"));
            }
        }

        if (ui->inputName->text().isEmpty())
        {
            error = tr("Name can not be empty");
        }

        if (!error.isEmpty())
        {
            qxtLog->error(error);
            QMessageBox::critical(this, tr("Error"), error, QMessageBox::Ok, QMessageBox::Ok);
        }
        else
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
 * @brief Save changes in Settings object
 * @param int index
 */
void SetEditDialog::save(int index)
{
    QVariant selectedType = ui->selectType->itemData(ui->selectType->currentIndex());
    QVariant selectedStyle = ui->selectStyle->itemData(ui->selectStyle->currentIndex());

    m_settings->editSet(index,
                        ui->inputName->text(),
                        static_cast<UM::WALLPAPER>(selectedType.toInt()),
                        static_cast<UM::IMAGE>(selectedStyle.toInt()),
                        ui->inputHotkey->hotkey()
                        );

    qxtLog->trace("Set \""+ ui->inputName->text() +"\" updated");
}
