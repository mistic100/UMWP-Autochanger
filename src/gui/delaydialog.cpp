#include <QMessageBox>

#include "delaydialog.h"
#include "ui_delaydialog.h"


/**
 * @brief DelayDialog::DelayDialog
 * @param QWidget* _parent
 * @param Settings* _settings
 */
DelayDialog::DelayDialog(QWidget* _parent, Settings* _settings) :
    QDialog(_parent),
    ui(new Ui::DelayDialog),
    m_settings(_settings)
{
    ui->setupUi(this);

    setFixedSize(size());
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);

    QTime time = QTime(0, 0, 0).addSecs(m_settings->param(UM::CONF::delay).toInt());
    ui->optionDelay->setTime(time);

    ui->optionDelay->setFocus();

    QLOG_TRACE() << "DelayDialog openned";
}

/**
 * @brief DelayDialog::~DelayDialog
 */
DelayDialog::~DelayDialog()
{
    delete ui;
}

/**
 * @brief Validate config before save
 * @param int result
 */
void DelayDialog::done(int result)
{
    if (result == QDialog::Accepted)
    {
        QTime time = ui->optionDelay->time();
        if (time < QTime(0, 0, 10))
        {
            QLOG_ERROR() << tr("Delay can not be lower than 10 seconds");
            QMessageBox::critical(this, tr("Error"), tr("Delay can not be lower than 10 seconds"), QMessageBox::Ok, QMessageBox::Ok);
            ui->optionDelay->setTime(QTime(0, 0, 10));
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
void DelayDialog::save()
{
    QTime time = ui->optionDelay->time();
    int delay = time.hour()*3600 + time.minute()*60 + time.second();
    m_settings->setParam(UM::CONF::delay, delay);

    m_settings->save();

    QLOG_TRACE() << "Configuration updated";
}
