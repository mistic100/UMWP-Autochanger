#include <QMessageBox>

#include "configdialog.h"
#include "ui_configdialog.h"


/**
 * @brief ConfigDialog::ConfigDialog
 * @param QWidget* _parent
 * @param Settings* _poSettings
 */
ConfigDialog::ConfigDialog(QWidget* _parent, Settings* _settings) : QDialog(_parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    setFixedSize(size());

    m_settings = _settings;

    ui->optionMinimize->setChecked(m_settings->bParam("minimize"));
    ui->optionCheckFiles->setChecked(m_settings->bParam("check"));
    ui->optionCheckUpdates->setChecked(m_settings->bParam("check_updates"));
    ui->optionAutostart->setChecked(m_settings->isAutostart());
    ui->optionUseHotkeys->setChecked(m_settings->bParam("use_hotkeys"));
    ui->optionShowNotifications->setChecked(m_settings->bParam("show_notifications"));

    ui->optionAutostart->setDisabled(!m_settings->canAddShortcut());

    ui->hotkeyRefresh->setDisabled(!m_settings->bParam("use_hotkeys"));
    ui->hotkeyShowHide->setDisabled(!m_settings->bParam("use_hotkeys"));
    ui->hotkeyStartPause->setDisabled(!m_settings->bParam("use_hotkeys"));

    ui->hotkeyRefresh->setHotkey(m_settings->hotkey("refresh"));
    ui->hotkeyShowHide->setHotkey(m_settings->hotkey("showhide"));
    ui->hotkeyStartPause->setHotkey(m_settings->hotkey("startpause"));

    ui->optionDelay->setValue(m_settings->iParam("delay"));
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
            for (int i=0, l=m_settings->nbSets(); i<l; i++)
            {
                Set* poSet = m_settings->getSet(i);

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
    m_settings->setParam("delay", ui->optionDelay->value());
    m_settings->setParam("minimize", ui->optionMinimize->isChecked());
    m_settings->setParam("check", ui->optionCheckFiles->isChecked());
    m_settings->setParam("check_updates", ui->optionCheckUpdates->isChecked());
    m_settings->setParam("use_hotkeys", ui->optionUseHotkeys->isChecked());
    m_settings->setParam("show_notifications", ui->optionShowNotifications->isChecked());

    m_settings->setHotkey("refresh", ui->hotkeyRefresh->hotkey());
    m_settings->setHotkey("showhide", ui->hotkeyShowHide->hotkey());
    m_settings->setHotkey("startpause", ui->hotkeyStartPause->hotkey());

    if (ui->optionAutostart->isChecked())
    {
        m_settings->createShortcut();
    }
    else
    {
        m_settings->deleteShortcut();
    }

    m_settings->save();
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
