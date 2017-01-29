#include <QMessageBox>
#include <QFileDialog>
#include <QToolTip>

#include "configdialog.h"
#include "ui_configdialog.h"
#include "../umutils.h"


/**
 * @brief ConfigDialog::ConfigDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
ConfigDialog::ConfigDialog(QWidget* _parent, Controller* _ctrl) :
    QDialog(_parent),
    ui(new Ui::ConfigDialog),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings()),
    m_enviro(_ctrl->enviro())
{
    ui->setupUi(this);

    setFixedSize(size());
    setWindowFlags(UM::SimpleDialogFlag);

    ui->tabs->setCurrentIndex(0);

    // checkboxes
    ui->optionMinimize->setChecked(         m_settings->param(UM::CONF::minimize).toBool());
    ui->optionCheckUpdates->setChecked(     m_settings->param(UM::CONF::check_updates).toBool());
    ui->optionAutostart->setChecked(        m_enviro->isAutostart());
    ui->optionUseHotkeys->setChecked(       m_settings->param(UM::CONF::use_hotkeys).toBool());
    ui->optionShowNotifications->setChecked(m_settings->param(UM::CONF::show_notifications).toBool());

    ui->optionAutostart->setDisabled(!m_enviro->canAddShortcut());

    // hotkeys
    bool useHotkeys = m_settings->param(UM::CONF::use_hotkeys).toBool();
    on_optionUseHotkeys_toggled(useHotkeys);

    ui->hotkeyRefresh->setHotkey(   m_settings->hotkey(UM::CONF::HOTKEY::refresh));
    ui->hotkeyShowHide->setHotkey(  m_settings->hotkey(UM::CONF::HOTKEY::showhide));
    ui->hotkeyStartPause->setHotkey(m_settings->hotkey(UM::CONF::HOTKEY::startpause));
    ui->hotkeyDelay->setHotkey(     m_settings->hotkey(UM::CONF::HOTKEY::delay));
    ui->hotkeyLockUnlock->setHotkey(m_settings->hotkey(UM::CONF::HOTKEY::lockunlock));

    // delay
    QTime time = QTime(0, 0, 0).addSecs(m_settings->param(UM::CONF::delay).toInt());
    ui->optionDelay->setTime(time);

    // languages
    foreach (const QString lang, m_enviro->languages())
    {
        ui->optionLang->addItem(
                    QIcon(":/lang/" + lang + "/flag"),
                    QLocale::languageToString(QLocale(lang).language()),
                    lang
        );
    }
    ui->optionLang->setCurrentData(m_settings->param(UM::CONF::language));

    // file opener
    QString opener = m_settings->param(UM::CONF::open_program).toString();
    ui->optionOpenProgram->addItem(tr("Default viewer"), "default");
    ui->optionOpenProgram->addItem(tr("Select program..."), "select");
    if (!opener.isEmpty())
    {
        ui->optionOpenProgram->addItem(QFileInfo(opener).fileName(), opener);
        ui->optionOpenProgram->setCurrentData(opener);
    }
    else {
        ui->optionOpenProgram->setCurrentData("default");
    }

    // type
    ui->optionType->addItem(QIcon(":/images/icons/w_monitor.png"), tr("One image for each monitor"),      UM::W_MONITOR);
    ui->optionType->addItem(QIcon(":/images/icons/w_desktop.png"), tr("One image for the whole desktop"), UM::W_DESKTOP);
    ui->optionType->setCurrentData(m_settings->param(UM::CONF::default_type));

    // style
    ui->optionStyle->addItem(QIcon(":/images/icons/im_center.png"),       tr("Center"),               UM::IM_CENTER);
    ui->optionStyle->addItem(QIcon(":/images/icons/im_tile.png"),         tr("Tile"),                 UM::IM_TILE);
    ui->optionStyle->addItem(QIcon(":/images/icons/im_stretch.png"),      tr("Stretch"),              UM::IM_STRETCH);
    ui->optionStyle->addItem(QIcon(":/images/icons/im_stretch_prop.png"), tr("Strecth proportional"), UM::IM_STRETCH_PROP);
    ui->optionStyle->addItem(QIcon(":/images/icons/im_fill.png"),         tr("Fill"),                 UM::IM_FILL);
    ui->optionStyle->setCurrentData(m_settings->param(UM::CONF::default_style));

    // mode
    ui->optionMode->addItem(QIcon(":/images/icons/mode_random.png"),     tr("Random"),     UM::MODE_RANDOM);
    ui->optionMode->addItem(QIcon(":/images/icons/mode_sequential.png"), tr("Sequential"), UM::MODE_SEQUENTIAL);
    ui->optionMode->setCurrentData(m_settings->param(UM::CONF::default_mode));

    // lock
    boolean lockEnabled = m_settings->param(UM::CONF::lock_enabled).toBool();
    on_optionLockEnabled_toggled(lockEnabled);
    ui->currentPassword->setDisabled(!lockEnabled);

    ui->optionLockEnabled->setChecked(  lockEnabled);
    ui->optionLockStartup->setChecked(  m_settings->param(UM::CONF::lock_startup).toBool());
    ui->optionLockMinimize->setChecked( m_settings->param(UM::CONF::lock_minimize).toBool());

    QLOG_TRACE() << "ConfigDialog openned";
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

        bool lockWasEnabled = m_settings->param(UM::CONF::lock_enabled).toBool();
        bool lockEnabled = ui->optionLockEnabled->isChecked();
        QString lockCurrentHash = m_settings->param(UM::CONF::lock_password).toString();
        QString lockCurrentPassword = ui->currentPassword->text();
        QString lockPassword = ui->optionLockPassword->text();

        QHash<QString, QHotKeyWidget*> requestHotkeys;
        requestHotkeys.insert(tr("Refresh"),        ui->hotkeyRefresh);
        requestHotkeys.insert(tr("Show/Hide"),      ui->hotkeyShowHide);
        requestHotkeys.insert(tr("Start/Pause"),    ui->hotkeyStartPause);
        requestHotkeys.insert(tr("Change delay"),   ui->hotkeyDelay);
        requestHotkeys.insert(tr("Lock/Unlock"),    ui->hotkeyLockUnlock);

        // validate delay
        if (ui->optionDelay->time() < QTime(0, 0, 10))
        {
            error = tr("Delay can not be lower than 10 seconds");
            ui->optionDelay->setTime(QTime(0, 0, 10));
            ui->tabs->setCurrentTab("general");
            goto afterCheck;
        }

        // validate lock
        // just enabled, password required
        if (lockEnabled && !lockWasEnabled && lockPassword.isEmpty()) {
            error = tr("You must enter a password to activate the lock.");
            ui->tabs->setCurrentTab("lock");
            goto afterCheck;
        }

        // just disabled / new password, check current password
        if (lockWasEnabled && !lockEnabled || lockWasEnabled && lockEnabled && !lockPassword.isEmpty()) {
            if (lockCurrentPassword.isEmpty() || lockCurrentHash != UM::hash(lockCurrentPassword, UM::PasswordHash)) {
                error = tr("Current lock password is invalid.");
                ui->tabs->setCurrentTab("lock");
                goto afterCheck;
            }
        }

        // validate hotkeys
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
                    ui->tabs->setCurrentTab("hotkeys");
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
                    ui->tabs->setCurrentTab("hotkeys");
                    goto afterCheck;
                }
            }
        }

        afterCheck:
        if (!error.isEmpty())
        {
            QLOG_ERROR() << error;
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
    QTime time = ui->optionDelay->time();
    int delay = time.hour()*3600 + time.minute()*60 + time.second();

    QVariant lang = ui->optionLang->currentData();
    if (lang != m_settings->param(UM::CONF::language))
    {
        QMessageBox::warning(this, tr("Language changed"),
                             tr("You must restart %1 to apply the new language.").arg(APP_NAME),
                             QMessageBox::Ok, QMessageBox::Ok);
    }

    QString opener = ui->optionOpenProgram->currentData().toString();
    if (opener == "default" || opener == "select")
    {
        opener = "";
    }

    bool lockEnabled = ui->optionLockEnabled->isChecked();
    QString lockPassword = ui->optionLockPassword->text();

    m_settings->setParam(UM::CONF::delay,               delay);
    m_settings->setParam(UM::CONF::language,            lang);
    m_settings->setParam(UM::CONF::open_program,        opener);
    m_settings->setParam(UM::CONF::default_mode,        ui->optionMode->currentData());
    m_settings->setParam(UM::CONF::default_type,        ui->optionType->currentData());
    m_settings->setParam(UM::CONF::default_style,       ui->optionStyle->currentData());
    m_settings->setParam(UM::CONF::minimize,            ui->optionMinimize->isChecked());
    m_settings->setParam(UM::CONF::check_updates,       ui->optionCheckUpdates->isChecked());
    m_settings->setParam(UM::CONF::use_hotkeys,         ui->optionUseHotkeys->isChecked());
    m_settings->setParam(UM::CONF::show_notifications,  ui->optionShowNotifications->isChecked());

    m_settings->setParam(UM::CONF::lock_enabled,        lockEnabled);
    if (!lockEnabled)
    {
        m_settings->setParam(UM::CONF::lock_password,   "");
    }
    else if (!lockPassword.isEmpty())
    {
        m_settings->setParam(UM::CONF::lock_password,   UM::hash(lockPassword, UM::PasswordHash));
    }
    m_settings->setParam(UM::CONF::lock_startup,        ui->optionLockStartup->isChecked());
    m_settings->setParam(UM::CONF::lock_minimize,       ui->optionLockMinimize->isChecked());

    m_settings->setHotkey(UM::CONF::HOTKEY::refresh,    ui->hotkeyRefresh->hotkey());
    m_settings->setHotkey(UM::CONF::HOTKEY::showhide,   ui->hotkeyShowHide->hotkey());
    m_settings->setHotkey(UM::CONF::HOTKEY::startpause, ui->hotkeyStartPause->hotkey());
    m_settings->setHotkey(UM::CONF::HOTKEY::delay,      ui->hotkeyDelay->hotkey());
    m_settings->setHotkey(UM::CONF::HOTKEY::lockunlock, ui->hotkeyLockUnlock->hotkey());

    if (ui->optionAutostart->isChecked())
    {
        m_enviro->createShortcut();
    }
    else
    {
        m_enviro->deleteShortcut();
    }

    m_settings->save();

    QLOG_TRACE() << "Configuration updated";
}

/**
 * @brief Disable hotkeys field
 * @param bool _checked
 */
void ConfigDialog::on_optionUseHotkeys_toggled(bool _checked)
{
    ui->hotkeyRefresh->setDisabled(!_checked);
    ui->hotkeyShowHide->setDisabled(!_checked);
    ui->hotkeyStartPause->setDisabled(!_checked);
    ui->hotkeyDelay->setDisabled(!_checked);
    ui->hotkeyLockUnlock->setDisabled(!_checked || !ui->optionLockEnabled->isChecked());
}

/**
 * @brief Disable lock fields
 * @param bool _checked
 */
void ConfigDialog::on_optionLockEnabled_toggled(bool _checked)
{
    ui->optionLockPassword->setDisabled(!_checked);
    ui->optionLockStartup->setDisabled(!_checked);
    ui->optionLockMinimize->setDisabled(!_checked);
    ui->hotkeyLockUnlock->setDisabled(!_checked || !ui->optionUseHotkeys->isChecked());
}

/**
 * @brief Choose open program
 */
void ConfigDialog::on_optionOpenProgram_currentIndexChanged(int)
{
    if (ui->optionOpenProgram->currentData().toString() == "select")
    {
        QString opener = QFileDialog::getOpenFileName(this, tr("Select program..."), getenv("PROGRAMFILES"), tr("Executables (*.exe)"));

        if (!opener.isEmpty())
        {
            if (ui->optionOpenProgram->count() == 3)
            {
                ui->optionOpenProgram->removeItem(2);
            }

            ui->optionOpenProgram->addItem(QFileInfo(opener).fileName(), opener);
            ui->optionOpenProgram->setCurrentIndex(2);
        }
        else
        {
            if (ui->optionOpenProgram->count() == 3)
            {
                ui->optionOpenProgram->setCurrentIndex(2);
            }
            else
            {
                ui->optionOpenProgram->setCurrentData("default");
            }
        }
    }
}

/**
 * @brief Click on help button
 */
void ConfigDialog::on_lockHelp_clicked()
{
    QToolTip::showText(QCursor::pos(), tr("This option allows to lock any changes in the app configuration until a password is entered.<br><b>Warning:</b> even though the password is hashed, the config file is not secure and can be manually modified."), this);
}
