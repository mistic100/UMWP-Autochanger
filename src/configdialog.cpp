#include <QMessageBox>

#include "configdialog.h"
#include "ui_configdialog.h"


/**
 * @brief ConfigDialog::ConfigDialog
 * @param QWidget* _parent
 * @param Settings* _settings
 */
ConfigDialog::ConfigDialog(QWidget* _parent, Settings* _settings) : QDialog(_parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    setFixedSize(size());

    m_settings = _settings;

    ui->optionMinimize->setChecked(         m_settings->opt("minimize").toBool());
    ui->optionCheckFiles->setChecked(       m_settings->opt("check").toBool());
    ui->optionCheckUpdates->setChecked(     m_settings->opt("check_updates").toBool());
    ui->optionAutostart->setChecked(        m_settings->isAutostart());
    ui->optionUseHotkeys->setChecked(       m_settings->opt("use_hotkeys").toBool());
    ui->optionShowNotifications->setChecked(m_settings->opt("show_notifications").toBool());

    ui->optionAutostart->setDisabled(!m_settings->canAddShortcut());

    ui->hotkeyRefresh->setDisabled(     !m_settings->opt("use_hotkeys").toBool());
    ui->hotkeyShowHide->setDisabled(    !m_settings->opt("use_hotkeys").toBool());
    ui->hotkeyStartPause->setDisabled(  !m_settings->opt("use_hotkeys").toBool());

    ui->hotkeyRefresh->setHotkey(   m_settings->hotkey("refresh"));
    ui->hotkeyShowHide->setHotkey(  m_settings->hotkey("showhide"));
    ui->hotkeyStartPause->setHotkey(m_settings->hotkey("startpause"));

    QTime time = QTime(0, 0, 0).addSecs(m_settings->opt("delay").toInt());
    ui->optionDelay->setTime(time);

    qxtLog->trace("ConfigDialog openned");
}

/**
 * @brief ConfigDialog::~ConfigDialog
 */
ConfigDialog::~ConfigDialog()
{
    delete ui;
}

/**
 * @brief Validate config before save
 * @param int result
 */
void ConfigDialog::done(int result)
{
    if (result == QDialog::Accepted)
    {
        QString error;

        // validate hotkeys
        QHash<QString, QHotKeyWidget*> requestHotkeys;
        requestHotkeys.insert(tr("Refresh"), ui->hotkeyRefresh);
        requestHotkeys.insert(tr("Show/Hide"), ui->hotkeyShowHide);
        requestHotkeys.insert(tr("Start/Pause"), ui->hotkeyStartPause);

        for (QHash<QString, QHotKeyWidget*>::iterator it=requestHotkeys.begin(); it!=requestHotkeys.end(); ++it)
        {
            if (!it.value()->hotkey())
            {
                continue;
            }

            // check against other main hotkeys
            for (QHash<QString, QHotKeyWidget*>::iterator it2=requestHotkeys.begin(); it2!=it; ++it2)
            {
                if (!it2.value()->hotkey())
                {
                    continue;
                }
                if (it.value()->hotkey() == it2.value()->hotkey())
                {
                    error = tr("Hotkey for \"%1\" already used for \"%2\"")
                            .arg(it.key()).arg(it2.key());

                    goto afterCheck;
                }
            }

            // check against sets hotkeys
            for (int i=0, l=m_settings->nbSets(); i<l; i++)
            {
                Set* poSet = m_settings->set(i);

                if (!poSet->hotkey())
                {
                    continue;
                }
                if (poSet->hotkey() == it.value()->hotkey())
                {
                    error = tr("Hotkey for \"%1\" already used for set \"%2\"")
                            .arg(it.key()).arg(poSet->name());

                    goto afterCheck;
                }
            }
        }
        afterCheck:

        // validate delay
        QTime time = ui->optionDelay->time();
        if (time < QTime(0, 0, 10))
        {
            error = tr("Delay can not be lower than 10 seconds");
            ui->optionDelay->setTime(QTime(0, 0, 10));
        }

        if (!error.isEmpty())
        {
            qxtLog->error(error);
            QMessageBox::critical(this, tr("Error"),  error, QMessageBox::Ok, QMessageBox::Ok);
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
 */
void ConfigDialog::save()
{
    QTime time = ui->optionDelay->time();
    m_settings->setOpt("delay", time.hour()*3600 + time.minute()*60 + time.second());

    m_settings->setOpt("minimize",              ui->optionMinimize->isChecked());
    m_settings->setOpt("check",                 ui->optionCheckFiles->isChecked());
    m_settings->setOpt("check_updates",         ui->optionCheckUpdates->isChecked());
    m_settings->setOpt("use_hotkeys",           ui->optionUseHotkeys->isChecked());
    m_settings->setOpt("show_notifications",    ui->optionShowNotifications->isChecked());

    m_settings->setHotkey("refresh",    ui->hotkeyRefresh->hotkey());
    m_settings->setHotkey("showhide",   ui->hotkeyShowHide->hotkey());
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

    qxtLog->trace("Configuration updated");
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
