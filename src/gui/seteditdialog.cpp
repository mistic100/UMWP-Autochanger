#include <QMessageBox>
#include <QToolTip>
#include <QStandardItemModel>

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
        ui->selectLock->addItem(tr("[keep]"),  UNKNOW_BOOL);
    }

    ui->selectType->addItem(QIcon(":/images/icons/w_monitor.png"), tr("One image for each monitor"),      UM::W_MONITOR);
    ui->selectType->addItem(QIcon(":/images/icons/w_desktop.png"), tr("One image for the whole desktop"), UM::W_DESKTOP);

    ui->selectStyle->addItem(QIcon(":/images/icons/im_center.png"),       tr("Center"),               UM::IM_CENTER);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_tile.png"),         tr("Tile"),                 UM::IM_TILE);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_stretch.png"),      tr("Stretch"),              UM::IM_STRETCH);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_stretch_prop.png"), tr("Fit"), UM::IM_STRETCH_PROP);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_fill.png"),         tr("Fill"),                 UM::IM_FILL);
    ui->selectStyle->addItem(QIcon(":/images/icons/im_custom.png"),       tr("Custom..."),            UM::IM_CUSTOM);

    ui->selectMode->addItem(QIcon(":/images/icons/mode_random.png"),     tr("Random"),     UM::MODE_RANDOM);
    ui->selectMode->addItem(QIcon(":/images/icons/mode_sequential.png"), tr("Sequential"), UM::MODE_SEQUENTIAL);

    ui->selectMonitors->setAllCheckedText(tr("All monitors"));
    ui->selectMonitors->setNoneCheckedText(tr("All monitors"));
    ui->selectMonitors->setUnknownlyCheckedText(tr("[keep]"));

    for (int i = 0; i < m_ctrl->enviro()->nbMonitors(); i++)
    {
        if (m_settings->monitor(i).enabled)
        {
            Qt::CheckState checkState = m_sets.size() > 1 ? Qt::PartiallyChecked : m_sets.at(0)->monitors().contains(i) ? Qt::Checked : Qt::Unchecked;
            ui->selectMonitors->addCheckItem(tr("Monitor %1").arg(i+1), i, checkState);
        }
    }

    ui->selectLock->addItem(QIcon(":/images/icons/unlock.png"), tr("Disabled"), 0);
    ui->selectLock->addItem(QIcon(":/images/icons/lock.png"),   tr("Enabled"),  1);

    if (m_sets.size() == 1)
    {
        Set* set = m_sets.at(0);

        ui->inputName->setText(set->name());
        ui->selectType->setCurrentData(set->type());
        ui->selectStyle->setCurrentData(set->style());
        ui->selectMode->setCurrentData(set->mode());
        ui->inputHotkey->setHotkey(set->hotkey());
        ui->inputFreq->setValue(set->frequency());
        ui->selectLock->setCurrentData((int) set->lock());
        ui->styleConfig->setVisible(set->style() == UM::IM_CUSTOM);
    }
    else
    {
        ui->inputName->setText(tr("[multiple sets]"));
        ui->inputName->setDisabled(true);
        ui->selectType->setCurrentData(UM::W_NONE);
        ui->selectStyle->setCurrentData(UM::IM_NONE);
        ui->selectMode->setCurrentData(UM::MODE_NONE);
        ui->inputHotkey->setHotkey(QHotKeyWidget::KEEP_KEY);
        ui->inputFreq->setMinimum(0);
        ui->inputFreq->setValue(0);
        ui->inputFreq->setSpecialValueText(tr("[keep]"));
        ui->selectLock->setCurrentData(UNKNOW_BOOL);
        ui->styleConfig->setVisible(false);
    }

    ui->inputHotkey->setEnabled(m_settings->param(UM::CONF::use_hotkeys).toBool());
    ui->selectLock->setEnabled(m_ctrl->lockEnabled() == UM::LOCK_SETS);

    ui->inputName->setFocus();

    updateUi();

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
    TRI_BOOL lock = static_cast<TRI_BOOL>(ui->selectLock->currentData().toInt());

    QVector<int> monitors;
    QStandardItemModel* model = (QStandardItemModel*) ui->selectMonitors->model();
    switch (ui->selectMonitors->globalCheckState())
    {
    case Qt::PartiallyChecked:
        for (int i = 0; i < model->rowCount(); i++)
        {
            if (model->item(i)->checkState() == Qt::Checked)
            {
                monitors.append(model->item(i)->data().toInt());
            }
        }
        break;

    case QCheckList::StateUnknown:
        monitors.append(-1);
        break;

    // default: all monitors, empty array
    }

    Set result;
    result.setName(ui->inputName->text());
    result.setType(type);
    result.setStyle(style);
    result.setMode(mode);
    result.setHotkey(ui->inputHotkey->hotkey());
    result.setCustLayout(m_custLayout);
    result.setFrequency(ui->inputFreq->value());
    result.setLock(lock);
    result.setMonitors(monitors);

    return result;
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
/**
 * @brief Update ui state depending on style/mode/type
 */
void SetEditDialog::updateUi()
{
    UM::WALLPAPER type = static_cast<UM::WALLPAPER>(ui->selectType->currentData().toInt());
    UM::IMAGE style = static_cast<UM::IMAGE>(ui->selectStyle->currentData().toInt());
    UM::MODE mode = static_cast<UM::MODE>(ui->selectMode->currentData().toInt());

    ui->selectMonitors->setEnabled(type == UM::W_MONITOR);
    ui->styleConfig->setVisible(style == UM::IM_CUSTOM);

    if (type == UM::W_DESKTOP)
    {
        QStandardItemModel* model = (QStandardItemModel*) ui->selectMonitors->model();
        for (int i = 0; i < model->rowCount(); i++)
        {
            model->item(i)->setCheckState(Qt::Checked);
        }
    }
}

void SetEditDialog::on_freqHelp_clicked()
{
    QToolTip::showText(QCursor::pos(), tr("The frequency is the chance of this set to be randomly selected among others.<br><b>Note:</b> at equal frequency, a set with more images has higher chance to be selected."), this);
}

void SetEditDialog::on_selectType_currentIndexChanged(int)
{
    updateUi();
}

void SetEditDialog::on_selectStyle_currentIndexChanged(int)
{
    updateUi();
}

void SetEditDialog::on_selectMode_currentIndexChanged(int)
{
    updateUi();
}
