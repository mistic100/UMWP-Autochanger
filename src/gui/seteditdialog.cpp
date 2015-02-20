#include <QMessageBox>

#include "seteditdialog.h"
#include "ui_seteditdialog.h"


/**
 * @brief SetEditDialog::SetEditDialog
 * @param QWidget* _parent
 * @param Set* _set
 * @param Settings* _settings
 */
SetEditDialog::SetEditDialog(QWidget* _parent, Settings* _settings, int _set) :
    QDialog(_parent),
    ui(new Ui::SetEditDialog),
    m_settings(_settings)
{
    ui->setupUi(this);

    setFixedSize(size());

    setWindowFlags(SimpleDialogFlag);

    ui->selectType->addItem(QIcon(":/icon/w_monitor"), tr("One image for each monitor"),      UM::W_MONITOR);
    ui->selectType->addItem(QIcon(":/icon/w_desktop"), tr("One image for the whole desktop"), UM::W_DESKTOP);

    ui->selectStyle->addItem(QIcon(":/icon/im_center"),       tr("Center"),               UM::IM_CENTER);
    ui->selectStyle->addItem(QIcon(":/icon/im_tile"),         tr("Tile"),                 UM::IM_TILE);
    ui->selectStyle->addItem(QIcon(":/icon/im_stretch"),      tr("Stretch"),              UM::IM_STRETCH);
    ui->selectStyle->addItem(QIcon(":/icon/im_stretch_prop"), tr("Strecth proportional"), UM::IM_STRETCH_PROP);
    ui->selectStyle->addItem(QIcon(":/icon/im_fill"),         tr("Fill"),                 UM::IM_FILL);

    ui->selectMode->addItem(QIcon(":/icon/mode_random"),     tr("Random"),     UM::RANDOM);
    ui->selectMode->addItem(QIcon(":/icon/mode_sequential"), tr("Sequential"), UM::SEQUENTIAL);

    Set* set = m_settings->set(_set);

    ui->inputName->setText(set->name());
    ui->selectType->setCurrentData(set->type());
    ui->selectStyle->setCurrentData(set->style());
    ui->selectMode->setCurrentData(set->mode());
    ui->inputHotkey->setHotkey(set->hotkey());
    ui->inputHotkey->setDisabled(!m_settings->get("use_hotkeys").toBool());

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
    m_settings->editSet(index,
                        ui->inputName->text(),
                        static_cast<UM::WALLPAPER>(ui->selectType->currentData().toInt()),
                        static_cast<UM::IMAGE>(ui->selectStyle->currentData().toInt()),
                        static_cast<UM::MODE>(ui->selectMode->currentData().toInt()),
                        ui->inputHotkey->hotkey()
                        );

    qxtLog->trace("Set \""+ ui->inputName->text() +"\" updated");
}
