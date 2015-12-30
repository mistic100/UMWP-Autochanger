#include <QMessageBox>

#include "customlayoutdialog.h"

#include "seteditdialog.h"
#include "ui_seteditdialog.h"


/**
 * @brief SetEditDialog::SetEditDialog
 * @param QWidget* _parent
 * @param Set* _set
 * @param Settings* _settings
 */
SetEditDialog::SetEditDialog(QWidget* _parent, Controller* _ctrl, const QList<Set*> &_sets) :
    QDialog(_parent),
    ui(new Ui::SetEditDialog),
    m_ctrl(_ctrl),
    m_settings(_ctrl->settings()),
    m_sets(_sets)
{
    ui->setupUi(this);

    setFixedSize(size());
    setWindowFlags(UM::SimpleDialogFlag);

    m_custLayout = m_sets.at(0)->custLayout();

    if (m_sets.size() > 1)
    {
        ui->selectType->addItem(tr("[keep]"),  UM::W_NONE);
        ui->selectStyle->addItem(tr("[keep]"), UM::IM_NONE);
        ui->selectMode->addItem(tr("[keep]"),  UM::MODE_NONE);
    }

    ui->selectType->addItem(QIcon(":/images/icons/w_monitor.png"), tr("One image for each monitor"),      UM::W_MONITOR);
    ui->selectType->addItem(QIcon(":/images/icons/w_desktop.png"), tr("One image for the whole desktop"), UM::W_DESKTOP);

    ui->selectStyle->addItem(QIcon(":/images/icons/im_center.png"),       tr("Center"),               UM::IM_CENTER);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_tile.png"),         tr("Tile"),                 UM::IM_TILE);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_stretch.png"),      tr("Stretch"),              UM::IM_STRETCH);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_stretch_prop.png"), tr("Strecth proportional"), UM::IM_STRETCH_PROP);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_fill.png"),         tr("Fill"),                 UM::IM_FILL);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_custom.png"),       tr("Custom..."),            UM::IM_CUSTOM);

    ui->selectMode->addItem(QIcon(":/images/icons/mode_random.png"),     tr("Random"),     UM::MODE_RANDOM);
    ui->selectMode->addItem(QIcon(":/images/icons/mode_sequential.png"), tr("Sequential"), UM::MODE_SEQUENTIAL);

    if (m_sets.size() == 1)
    {
        Set* set = m_sets.at(0);

        ui->inputName->setText(set->name());
        ui->selectType->setCurrentData(set->type());
        ui->selectStyle->setCurrentData(set->style());
        ui->selectMode->setCurrentData(set->mode());
        ui->inputHotkey->setHotkey(set->hotkey());
        ui->styleConfig->setVisible(set->style() == UM::IM_CUSTOM);
    }
    else
    {
        ui->inputName->setText(tr("[multiple sets]"));
        ui->inputName->setDisabled(true);
        ui->selectType->setCurrentIndex(0);
        ui->selectStyle->setCurrentIndex(0);
        ui->selectMode->setCurrentIndex(0);
        ui->inputHotkey->setHotkey(QHotKeyWidget::KEEP_KEY);
        ui->styleConfig->setVisible(false);
    }

    ui->inputHotkey->setDisabled(!m_settings->param(UM::CONF::use_hotkeys).toBool());

    ui->inputName->setFocus();

    QLOG_TRACE() << "SetEditDialog openned";
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

        if (hotkey > QHotKeyWidget::NO_KEY)
        {
            if (hotkey == m_settings->hotkey(UM::CONF::HOTKEY::refresh))
            {
                error = tr("Hotkey already used for \"%1\"").arg(tr("Refresh"));
            }
            else if (hotkey == m_settings->hotkey(UM::CONF::HOTKEY::startpause))
            {
                error = tr("Hotkey already used for \"%1\"").arg(tr("Start/Pause"));
            }
            else if (hotkey == m_settings->hotkey(UM::CONF::HOTKEY::showhide))
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
 * @param int index
 */
const Set SetEditDialog::result()
{
    UM::WALLPAPER type = static_cast<UM::WALLPAPER>(ui->selectType->currentData().toInt());
    UM::IMAGE style = static_cast<UM::IMAGE>(ui->selectStyle->currentData().toInt());
    UM::MODE mode = static_cast<UM::MODE>(ui->selectMode->currentData().toInt());

    Set result;
    result.setName(ui->inputName->text());
    result.setType(type);
    result.setStyle(style);
    result.setMode(mode);
    result.setHotkey(ui->inputHotkey->hotkey());
    result.setCustLayout(m_custLayout);

    return result;
}

/**
 * @brief Change "Configure" button visiblity depending on style
 * @param data
 */
void SetEditDialog::on_selectStyle_currentDataChanged(QVariant data)
{
    UM::IMAGE style = static_cast<UM::IMAGE>(data.toInt());
    ui->styleConfig->setVisible(style == UM::IM_CUSTOM);
}

/**
 * @brief Open custom layout dialog
 */
void SetEditDialog::on_styleConfig_clicked()
{
    CustomLayoutDialog dialog(this, m_ctrl);

    dialog.setCustLayout(m_custLayout);

    if (dialog.exec())
    {
        m_custLayout = dialog.getCustLayout();
    }
}
