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

    ui->optionAutostart->setDisabled(!m_poSettings->bCanAddShortcut());

    int iHotkey = m_poSettings->iParam("hotkey");
    ui->hotkeyCtrl->setChecked(iHotkey & MOD_CONTROL);
    ui->hotkeyAlt->setChecked(iHotkey & MOD_ALT);
    ui->hotkeyShift->setChecked(iHotkey & MOD_SHIFT);
    ui->hotkeyWin->setChecked(iHotkey & MOD_WIN);

    ui->optionDelay->setValue(m_poSettings->iParam("delay"));

    m_bHotkeysChanged = false;
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
 * @brief Save changes in Settings object
 */
void ConfigDialog::vSave()
{
    m_poSettings->vSetParam("delay", ui->optionDelay->value());
    m_poSettings->vSetParam("minimize", ui->optionMinimize->isChecked());
    m_poSettings->vSetParam("check", ui->optionCheckFiles->isChecked());
    m_poSettings->vSetParam("check_updates", ui->optionCheckUpdates->isChecked());

    if (ui->optionAutostart->isChecked())
    {
        m_poSettings->vCreateShortcut();
    }
    else
    {
        m_poSettings->vDeleteShortcut();
    }

    int iHotKey = 0;
    if (ui->hotkeyCtrl->isChecked())
    {
        iHotKey|= MOD_CONTROL;
    }
    if (ui->hotkeyAlt->isChecked())
    {
        iHotKey|= MOD_ALT;
    }
    if (ui->hotkeyShift->isChecked())
    {
        iHotKey|= MOD_SHIFT;
    }
    if (ui->hotkeyWin->isChecked())
    {
        iHotKey|= MOD_WIN;
    }
    m_poSettings->vSetParam("hotkey", iHotKey);

    m_poSettings->vWriteXML();
}

/**
 * @brief Track hotkeys changes
 */
void ConfigDialog::hotkeysChanged()
{
    m_bHotkeysChanged = true;
}

/**
 * @brief Track delay changes
 */
void ConfigDialog::on_optionDelay_valueChanged()
{
    m_bDelayChanged = true;
}
