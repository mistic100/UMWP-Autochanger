#include <QNetworkAccessManager>
#include <QMessageBox>

#include "newversiondialog.h"
#include "ui_newversiondialog.h"


NewVersionDialog::NewVersionDialog(Controller* _ctrl, QWidget* _parent) :
    QDialog(_parent),
    ui(new Ui::NewVersionDialog),
    m_ctrl(_ctrl)
{
    ui->setupUi(this);

    m_version = m_ctrl->settings()->newVersion();

    ui->progressBar->setVisible(false);
    ui->label->setText(tr("A new version is available : %1").arg(m_version.first));
}

NewVersionDialog::~NewVersionDialog()
{
    delete ui;
}

void NewVersionDialog::on_updateButton_clicked()
{
    ui->updateButton->setVisible(false);
    ui->progressBar->setVisible(true);
    ui->buttonBox->setDisabled(true);

    m_file.setFileName("installer.exe");

    if (!m_file.open(QIODevice::WriteOnly))
    {
        qxtLog->error("Unable to open installer.exe");

        errorMessage();
    }

    ui->label->setText(tr("Downloading UMWP_Autochanger_%1_Setup.exe ...").arg(m_version.first));

    QNetworkAccessManager* manager = new QNetworkAccessManager();
    m_reply = manager->get(QNetworkRequest(QUrl(m_version.second)));

    connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotDataReady()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(slotDownloadFinished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));
}


void NewVersionDialog::errorMessage()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Unable to download installer file."));
    msgBox.exec();
    done(0);
}

void NewVersionDialog::slotDataReady()
{
    m_file.write(m_reply->readAll());
}

void NewVersionDialog::slotDownloadProgress(qint64 _received, qint64 _total)
{
    if (_total == -1) {
        _total = qMax(_received, (qint64) APP_INSTALLER_SIZE);
    }

    ui->progressBar->setMaximum(_total);
    ui->progressBar->setValue(_received);
}

void NewVersionDialog::slotDownloadFinished()
{
    slotDataReady();
    m_file.close();

    m_reply->manager()->deleteLater();
    m_reply->deleteLater();

    if (m_reply->error() != QNetworkReply::NoError)
    {
        qxtLog->error("Network error");

        m_file.remove();
        errorMessage();
    }
    else {
        qxtLog->trace("Download finished");

        ui->progressBar->setValue(100);
        ui->progressBar->setMaximum(100);

        m_ctrl->launchInstaller();
    }
}
