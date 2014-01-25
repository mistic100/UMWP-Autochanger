#include <QMessageBox>

#include "configdialog.h"
#include "ui_configdialog.h"


/**
 * @brief ConfigDialog::ConfigDialog
 * @param QWidget* _parent
 * @param Settings* _poSettings
 */
ConfigDialog::ConfigDialog(QWidget* _parent, Settings* _pSettings) : QDialog(_parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    setFixedSize(size());

    m_pSettings = _pSettings;

    ui->optionMinimize->setChecked(m_pSettings->bParam("minimize"));
    ui->optionCheckFiles->setChecked(m_pSettings->bParam("check"));
    ui->optionCheckUpdates->setChecked(m_pSettings->bParam("check_updates"));
    ui->optionAutostart->setChecked(m_pSettings->isAutostart());
    ui->optionUseHotkeys->setChecked(m_pSettings->bParam("use_hotkeys"));
    ui->optionShowNotifications->setChecked(m_pSettings->bParam("show_notifications"));

    ui->optionAutostart->setDisabled(!m_pSettings->canAddShortcut());

    ui->hotkeyRefresh->setDisabled(!m_pSettings->bParam("use_hotkeys"));
    ui->hotkeyShowHide->setDisabled(!m_pSettings->bParam("use_hotkeys"));
    ui->hotkeyStartPause->setDisabled(!m_pSettings->bParam("use_hotkeys"));

    ui->hotkeyRefresh->setHotkey(m_pSettings->hotkey("refresh"));
    ui->hotkeyShowHide->setHotkey(m_pSettings->hotkey("showhide"));
    ui->hotkeyStartPause->setHotkey(m_pSettings->hotkey("startpause"));

    ui->optionDelay->setValue(m_pSettings->iParam("delay"));

    m_delayChanged = false;
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
            if (!it.value()->hotkey())
            {
                continue;
            }

            // check against other main hotkeys
            for (QHash<char*, QLineEditHotkey*>::iterator it2=requestHotkeys.begin(); it2!=it; ++it2)
            {
                if (!it2.value()->hotkey())
                {
                    continue;
                }
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
            for (int i=0, l=m_pSettings->nbSets(); i<l; i++)
            {
                Set* poSet = m_pSettings->pGetSet(i);

                if (!poSet->hotkey())
                {
                    continue;
                }
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
void ConfigDialog::save()
{
    m_pSettings->setParam("delay", ui->optionDelay->value());
    m_pSettings->setParam("minimize", ui->optionMinimize->isChecked());
    m_pSettings->setParam("check", ui->optionCheckFiles->isChecked());
    m_pSettings->setParam("check_updates", ui->optionCheckUpdates->isChecked());
    m_pSettings->setParam("use_hotkeys", ui->optionUseHotkeys->isChecked());
    m_pSettings->setParam("show_notifications", ui->optionShowNotifications->isChecked());

    m_pSettings->setHotkey("refresh", ui->hotkeyRefresh->hotkey());
    m_pSettings->setHotkey("showhide", ui->hotkeyShowHide->hotkey());
    m_pSettings->setHotkey("startpause", ui->hotkeyStartPause->hotkey());

    if (ui->optionAutostart->isChecked())
    {
        m_pSettings->createShortcut();
    }
    else
    {
        m_pSettings->deleteShortcut();
    }

    m_pSettings->writeXML();
}

/**
 * @brief Track delay changes
 */
void ConfigDialog::on_optionDelay_valueChanged()
{
    m_delayChanged = true;
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
