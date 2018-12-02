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
    ui_lockType = new QButtonGroupExt(this);

    connect(ui_lockType, SIGNAL(buttonToggled(int,bool)), this, SLOT(on_lockType_toggled(int,bool)));

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

    // sets
    ui->optionDefaultSet->addItem(tr("[do not change]"), "");

    for (int i=0, l=m_settings->nbSets(); i<l; i++)
    {
        Set* set = m_settings->set(i);
        ui->optionDefaultSet->addItem(set->name(), set->uuid());
    }

    ui->optionDefaultSet->setCurrentData(m_settings->param(UM::CONF::default_set));

    // lock
    boolean lockEnabled = m_settings->param(UM::CONF::lock_enabled).toBool();
    on_optionLockEnabled_toggled(lockEnabled);
    ui->currentPassword->setDisabled(!lockEnabled);

    ui->optionLockEnabled->setChecked(  lockEnabled);
    ui->optionLockStartup->setChecked(  m_settings->param(UM::CONF::lock_startup).toBool());
    ui->optionLockMinimize->setChecked( m_settings->param(UM::CONF::lock_minimize).toBool());

    ui_lockType->addButton(ui->optionLockAll,  UM::LOCK_ALL);
    ui_lockType->addButton(ui->optionLockSets, UM::LOCK_SETS);
    ui_lockType->setCheckedId(m_settings->param(UM::CONF::lock_type).toInt());
    on_lockType_toggled(ui_lockType->checkedId(), true);

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
    m_settings->setParam(UM::CONF::minimize,            ui->optionMinimize->isChecked());
    m_settings->setParam(UM::CONF::check_updates,       ui->optionCheckUpdates->isChecked());
    m_settings->setParam(UM::CONF::use_hotkeys,         ui->optionUseHotkeys->isChecked());
    m_settings->setParam(UM::CONF::show_notifications,  ui->optionShowNotifications->isChecked());
    m_settings->setParam(UM::CONF::default_set,         ui->optionDefaultSet->currentData());

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
    m_settings->setParam(UM::CONF::lock_type,           ui_lockType->checkedId());

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
    ui->hotkeyRefresh->setEnabled(_checked);
    ui->hotkeyShowHide->setEnabled(_checked);
    ui->hotkeyStartPause->setEnabled(_checked);
    ui->hotkeyDelay->setEnabled(_checked);
    ui->hotkeyLockUnlock->setEnabled(_checked && ui->optionLockEnabled->isChecked());
}

/**
 * @brief Disable lock fields
 * @param bool _checked
 */
void ConfigDialog::on_optionLockEnabled_toggled(bool _checked)
{
    ui->optionLockPassword->setEnabled(_checked);
    ui->optionLockAll->setEnabled(_checked);
    ui->optionLockSets->setEnabled(_checked);
    ui->hotkeyLockUnlock->setEnabled(_checked && ui->optionUseHotkeys->isChecked());
    on_lockType_toggled(ui_lockType->checkedId(), true);
}

/**
 * @brief Disable lock sub-fields
 * @param int _id - mode
 * @param bool _checked
 */
void ConfigDialog::on_lockType_toggled(int _id, bool _checked)
{
    ui->optionLockMinimize->setEnabled(_id == UM::LOCK_ALL && _checked && ui->optionLockEnabled->isChecked());
    ui->optionLockStartup->setEnabled(_id == UM::LOCK_ALL && _checked && ui->optionLockEnabled->isChecked());
    ui->lockSetsHelp->setEnabled(_id == UM::LOCK_SETS && _checked && ui->optionLockEnabled->isChecked());
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
