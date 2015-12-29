#include <QtWidgets/QMessageBox>

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
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings()),
    m_enviro(_ctrl->enviro())
{
    ui->setupUi(this);

    setFixedSize(size());

    setWindowFlags(UM::SimpleDialogFlag);

    ui->tabWidget->setCurrentIndex(0);

    // checkboxes
    ui->optionMinimize->setChecked(         m_settings->param(UM::CONF::minimize).toBool());
    ui->optionCheckUpdates->setChecked(     m_settings->param(UM::CONF::check_updates).toBool());
    ui->optionAutostart->setChecked(        m_enviro->isAutostart());
    ui->optionUseHotkeys->setChecked(       m_settings->param(UM::CONF::use_hotkeys).toBool());
    ui->optionShowNotifications->setChecked(m_settings->param(UM::CONF::show_notifications).toBool());

    ui->optionAutostart->setDisabled(!m_enviro->canAddShortcut());

    // hotkeys
    bool useHotkeys = m_settings->param(UM::CONF::use_hotkeys).toBool();
    ui->hotkeyRefresh->setDisabled(!useHotkeys);
    ui->hotkeyShowHide->setDisabled(!useHotkeys);
    ui->hotkeyStartPause->setDisabled(!useHotkeys);

    ui->hotkeyRefresh->setHotkey(   m_settings->hotkey(UM::CONF::HOTKEY::refresh));
    ui->hotkeyShowHide->setHotkey(  m_settings->hotkey(UM::CONF::HOTKEY::showhide));
    ui->hotkeyStartPause->setHotkey(m_settings->hotkey(UM::CONF::HOTKEY::startpause));

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

    int langIndex = ui->optionLang->currentIndex();
    QVariant lang = ui->optionLang->itemData(langIndex);

    if (lang != m_settings->param(UM::CONF::language))
    {
        QMessageBox::warning(this, tr("Language changed"),
                             tr("You must restart %1 to apply the new language.").arg(APP_NAME),
                             QMessageBox::Ok, QMessageBox::Ok);
    }

    m_settings->setParam(UM::CONF::delay,                 delay);
    m_settings->setParam(UM::CONF::language,              lang);
    m_settings->setParam(UM::CONF::default_mode,          ui->optionMode->currentData());
    m_settings->setParam(UM::CONF::default_type,          ui->optionType->currentData());
    m_settings->setParam(UM::CONF::default_style,         ui->optionStyle->currentData());
    m_settings->setParam(UM::CONF::minimize,              ui->optionMinimize->isChecked());
    m_settings->setParam(UM::CONF::check_updates,         ui->optionCheckUpdates->isChecked());
    m_settings->setParam(UM::CONF::use_hotkeys,           ui->optionUseHotkeys->isChecked());
    m_settings->setParam(UM::CONF::show_notifications,    ui->optionShowNotifications->isChecked());

    m_settings->setHotkey(UM::CONF::HOTKEY::refresh,    ui->hotkeyRefresh->hotkey());
    m_settings->setHotkey(UM::CONF::HOTKEY::showhide,   ui->hotkeyShowHide->hotkey());
    m_settings->setHotkey(UM::CONF::HOTKEY::startpause, ui->hotkeyStartPause->hotkey());

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
 * @param bool checked
 */
void ConfigDialog::on_optionUseHotkeys_toggled(bool checked)
{
    ui->hotkeyRefresh->setDisabled(!checked);
    ui->hotkeyShowHide->setDisabled(!checked);
    ui->hotkeyStartPause->setDisabled(!checked);
}
