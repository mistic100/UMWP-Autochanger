#include <QMessageBox>

#include "configdialog.h"
#include "ui_configdialog.h"


/**
 * @brief ConfigDialog::ConfigDialog
 * @param QWidget* _parent
 * @param Settings* _poSettings
 */
ConfigDialog::ConfigDialog(QWidget* _parent, Settings* _poSettings) : QDialog(_parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    setFixedSize(size());

    m_poSettings = _poSettings;

    ui->optionMinimize->setChecked(m_poSettings->bParam("minimize"));
    ui->optionCheckFiles->setChecked(m_poSettings->bParam("check"));
    ui->optionCheckUpdates->setChecked(m_poSettings->bParam("check_updates"));
    ui->optionAutostart->setChecked(m_poSettings->bIsAutostart());
    ui->optionUseHotkeys->setChecked(m_poSettings->bParam("use_hotkeys"));
    ui->optionShowNotifications->setChecked(m_poSettings->bParam("show_notifications"));

    ui->optionAutostart->setDisabled(!m_poSettings->bCanAddShortcut());

    ui->hotkeyRefresh->setDisabled(!m_poSettings->bParam("use_hotkeys"));
    ui->hotkeyShowHide->setDisabled(!m_poSettings->bParam("use_hotkeys"));
    ui->hotkeyStartPause->setDisabled(!m_poSettings->bParam("use_hotkeys"));

    ui->hotkeyRefresh->vSetHotkey(m_poSettings->oHotkey("refresh"));
    ui->hotkeyShowHide->vSetHotkey(m_poSettings->oHotkey("showhide"));
    ui->hotkeyStartPause->vSetHotkey(m_poSettings->oHotkey("startpause"));

    ui->optionDelay->setValue(m_poSettings->iParam("delay"));

    m_bDelayChanged = false;
}

/**
 * @brief ConfigDialog::~ConfigDialog
 */
ConfigDialog::~ConfigDialog()
{
    delete ui;
}

/**
 * @brief Validate hotkeys before save
 * @param int result
 */
void ConfigDialog::done(int result)
{
    if (result == QDialog::Accepted)
    {
        QHash<char*, QLineEditHotkey*> requestHotkeys;
        requestHotkeys.insert("Refresh", ui->hotkeyRefresh);
        requestHotkeys.insert("Show/Hide", ui->hotkeyShowHide);
        requestHotkeys.insert("Start/Pause", ui->hotkeyStartPause);

        QString error;

        for (QHash<char*, QLineEditHotkey*>::iterator it=requestHotkeys.begin(); it!=requestHotkeys.end(); ++it)
        {
            // check against other main hotkeys
            for (QHash<char*, QLineEditHotkey*>::iterator it2=requestHotkeys.begin(); it2!=it; ++it2)
            {
                if (it.value()->hotkey() == it2.value()->hotkey())
                {
                    error = tr(it2.key());
                    break;
                }
            }

            if (!error.isEmpty())
            {
                QMessageBox::critical(this, tr(it.key()),
                                      tr("Hotkey already used for \"%1\"").arg(error),
                                      QMessageBox::Ok, QMessageBox::Ok);
                break;
            }

            // check against sets hotkeys
            for (int i=0, l=m_poSettings->iNbSets(); i<l; i++)
            {
                Set* poSet = m_poSettings->poGetSet(i);

                if (poSet->hotkey() == it.value()->hotkey())
                {
                    error = poSet->name();
                    break;
                }
            }

            if (!error.isEmpty())
            {
                QMessageBox::critical(this, tr(it.key()),
                                      tr("Hotkey already used for set \"%1\"").arg(error),
                                      QMessageBox::Ok, QMessageBox::Ok);
                break;
            }
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
 * @brief Save changes in Settings object
 */
void ConfigDialog::vSave()
{
    m_poSettings->vSetParam("delay", ui->optionDelay->value());
    m_poSettings->vSetParam("minimize", ui->optionMinimize->isChecked());
    m_poSettings->vSetParam("check", ui->optionCheckFiles->isChecked());
    m_poSettings->vSetParam("check_updates", ui->optionCheckUpdates->isChecked());
    m_poSettings->vSetParam("use_hotkeys", ui->optionUseHotkeys->isChecked());
    m_poSettings->vSetParam("show_notifications", ui->optionShowNotifications->isChecked());

    m_poSettings->vSetHotkey("refresh", ui->hotkeyRefresh->hotkey());
    m_poSettings->vSetHotkey("showhide", ui->hotkeyShowHide->hotkey());
    m_poSettings->vSetHotkey("startpause", ui->hotkeyStartPause->hotkey());

    if (ui->optionAutostart->isChecked())
    {
        m_poSettings->vCreateShortcut();
    }
    else
    {
        m_poSettings->vDeleteShortcut();
    }

    m_poSettings->vWriteXML();
}

/**
 * @brief Track delay changes
 */
void ConfigDialog::on_optionDelay_valueChanged()
{
    m_bDelayChanged = true;
}

/**
 * @brief Disable hotkeys field
 * @param bool checked
 */
void ConfigDialog::on_optionUseHotkeys_toggled(bool checked)
{
    ui->hotkeyRefresh->setDisabled(!checked);
    ui->hotkeyShowHide->setDisabled(!checked);
    ui->hotkeyStartPause->setDisabled(!checked);
}
