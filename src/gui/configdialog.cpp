#include <QMessageBox>

#include "configdialog.h"
#include "ui_configdialog.h"


/**
 * @brief ConfigDialog::ConfigDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
ConfigDialog::ConfigDialog(QWidget* _parent, Controller* _ctrl) :
    QDialog(_parent),
    ui(new Ui::ConfigDialog),
    m_ctrl(_ctrl)
{
    ui->setupUi(this);

    setFixedSize(size());

    setWindowFlags(SimpleDialogFlag);

    Settings* settings = m_ctrl->settings();
    Environment* enviro = m_ctrl->enviro();

    ui->optionMinimize->setChecked(         settings->get("minimize").toBool());
    ui->optionCheckUpdates->setChecked(     settings->get("check_updates").toBool());
    ui->optionAutostart->setChecked(        enviro->isAutostart());
    ui->optionUseHotkeys->setChecked(       settings->get("use_hotkeys").toBool());
    ui->optionShowNotifications->setChecked(settings->get("show_notifications").toBool());

    ui->optionAutostart->setDisabled(!enviro->canAddShortcut());

    ui->hotkeyRefresh->setDisabled(     !settings->get("use_hotkeys").toBool());
    ui->hotkeyShowHide->setDisabled(    !settings->get("use_hotkeys").toBool());
    ui->hotkeyStartPause->setDisabled(  !settings->get("use_hotkeys").toBool());

    ui->hotkeyRefresh->setHotkey(   settings->hotkey("refresh"));
    ui->hotkeyShowHide->setHotkey(  settings->hotkey("showhide"));
    ui->hotkeyStartPause->setHotkey(settings->hotkey("startpause"));

    QTime time = QTime(0, 0, 0).addSecs(settings->get("delay").toInt());
    ui->optionDelay->setTime(time);

    foreach (QString lang, m_ctrl->enviro()->languages())
    {
        ui->optionLang->addItem(
                    QIcon(":/lang/" + lang + "/flag"),
                    QLocale::languageToString(QLocale(lang).language()),
                    lang
        );
    }

    int langIndex = ui->optionLang->findData(settings->get("language"));
    ui->optionLang->setCurrentIndex(langIndex);

    ui->optionMode->addItem(tr("Random"), "random");
    ui->optionMode->addItem(tr("Sequential"), "sequential");

    int modeIndex = ui->optionMode->findData(settings->get("mode"));
    ui->optionMode->setCurrentIndex(modeIndex);

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
        Settings* settings = m_ctrl->settings();

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
            for (int i=0, l=settings->nbSets(); i<l; i++)
            {
                Set* poSet = settings->set(i);

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
 */
void ConfigDialog::save()
{
    Settings* settings = m_ctrl->settings();
    Environment* enviro = m_ctrl->enviro();

    QTime time = ui->optionDelay->time();
    int delay = time.hour()*3600 + time.minute()*60 + time.second();

    int langIndex = ui->optionLang->currentIndex();
    QVariant lang = ui->optionLang->itemData(langIndex);

    if (lang != settings->get("language"))
    {
        QMessageBox::warning(this, tr("Language changed"),
                             tr("You must restart %1 to apply the new language.").arg(APP_NAME),
                             QMessageBox::Ok, QMessageBox::Ok);
    }

    int modeIndex = ui->optionMode->currentIndex();

    settings->setOpt("delay",                 delay);
    settings->setOpt("language",              lang);
    settings->setOpt("mode",                  ui->optionMode->itemData(modeIndex));
    settings->setOpt("minimize",              ui->optionMinimize->isChecked());
    settings->setOpt("check_updates",         ui->optionCheckUpdates->isChecked());
    settings->setOpt("use_hotkeys",           ui->optionUseHotkeys->isChecked());
    settings->setOpt("show_notifications",    ui->optionShowNotifications->isChecked());

    settings->setHotkey("refresh",    ui->hotkeyRefresh->hotkey());
    settings->setHotkey("showhide",   ui->hotkeyShowHide->hotkey());
    settings->setHotkey("startpause", ui->hotkeyStartPause->hotkey());

    if (ui->optionAutostart->isChecked())
    {
        enviro->createShortcut();
    }
    else
    {
        enviro->deleteShortcut();
    }

    settings->save();

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
