#include <QNetworkAccessManager>
#include <QMessageBox>

#include "newversiondialog.h"
#include "ui_newversiondialog.h"


/**
 * @brief NewVersionDialog::NewVersionDialog
 * @param QWidget* _parent
 * @param Controller* _ctrl
 */
NewVersionDialog::NewVersionDialog(QWidget* _parent, Controller* _ctrl) :
    QDialog(_parent),
    ui(new Ui::NewVersionDialog),
    m_ctrl(_ctrl)
{
    ui->setupUi(this);

    m_version = m_ctrl->enviro()->newVersion();

    ui->progressBar->setVisible(false);
    ui->label->setText(tr("A new version is available : %1").arg(m_version.code));
}

/**
 * @brief NewVersionDialog::~NewVersionDialog
 */
NewVersionDialog::~NewVersionDialog()
{
    delete ui;
}

/**
 * @brief Launch download on click on main button
 */
void NewVersionDialog::on_updateButton_clicked()
{
    ui->updateButton->setVisible(false);
    ui->progressBar->setVisible(true);
    ui->buttonBox->setDisabled(true);

    m_file.setFileName(QString::fromAscii(APP_INSTALLER_FILENAME));

    if (!m_file.open(QIODevice::WriteOnly))
    {
        qxtLog->error("Unable to open installer destination file.");

        errorMessage();
    }

    ui->label->setText(tr("Downloading UMWP_Autochanger_%1_Setup.exe ...").arg(m_version.code));

    QNetworkAccessManager* manager = new QNetworkAccessManager();
    m_reply = manager->get(QNetworkRequest(QUrl(m_version.link)));

    connect(m_reply, SIGNAL(readyRead()), this, SLOT(onDataReady()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(onDownloadFinished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
}

/**
 * @brief Display error dialog and close update dialog
 */
void NewVersionDialog::errorMessage()
{
    QMessageBox::critical(this, tr("Error"), tr("Unable to download installer file."), QMessageBox::Ok, QMessageBox::Ok);
    done(0);
}

/**
 * @brief Dump available data in file
 */
void NewVersionDialog::onDataReady()
{
    m_file.write(m_reply->readAll());
}

/**
 * @brief Update progress bar during download
 * @param int _received
 * @param int _total
 */
void NewVersionDialog::onDownloadProgress(qint64 _received, qint64 _total)
{
    if (_total == -1)
    {
        _total = qMax(_received, (qint64) APP_INSTALLER_SIZE);
    }

    ui->progressBar->setMaximum(_total);
    ui->progressBar->setValue(_received);
}

/**
 * @brief Finalize installer file on download end and ask controller to open it
 */
void NewVersionDialog::onDownloadFinished()
{
    onDataReady();
    m_file.close();

    m_reply->manager()->deleteLater();
    m_reply->deleteLater();

    if (m_reply->error() != QNetworkReply::NoError)
    {
        qxtLog->error("Network error");

        m_file.remove();
        errorMessage();
    }
    else
    {
        qxtLog->trace("Download finished");

        ui->progressBar->setValue(100);
        ui->progressBar->setMaximum(100);

        m_ctrl->launchInstaller();
    }
}
