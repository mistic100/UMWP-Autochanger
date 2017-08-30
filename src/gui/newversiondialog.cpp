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

    ui->blockDownloading->setVisible(false);
    resize(size().width(), 250);

    ui->title->setText(ui->title->text().arg(APP_NAME));
    ui->description->setText(ui->description->text().arg(APP_NAME, m_version.code, APP_VERSION));
    ui->titleDownloading->setText(ui->titleDownloading->text().arg(APP_NAME, m_version.code));

    downloadChangelog();

    setWindowFlags(UM::SimpleDialogFlag);
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
void NewVersionDialog::on_buttonUpdate_clicked()
{
    ui->blockInfo->setVisible(false);
    ui->blockDownloading->setVisible(true);
    resize(size().width(), 60);

    m_file.setFileName(QDir::toNativeSeparators(QDir::tempPath() + "/" + APP_INSTALLER_FILENAME));

    if (!m_file.open(QIODevice::WriteOnly))
    {
        QLOG_ERROR() << "Unable to open installer destination file.";

        errorMessage();
    }
    else
    {
        QLOG_DEBUG() << "Download" << m_version.link;

        QNetworkAccessManager* manager = new QNetworkAccessManager();

        QNetworkRequest request(QUrl(m_version.link));
        if (m_version.link.startsWith("https"))
        {
            request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        }

        m_reply = manager->get(request);

        connect(m_reply, SIGNAL(readyRead()), this, SLOT(onDataReady()));
        connect(m_reply, SIGNAL(finished()), this, SLOT(onDownloadFinished()));
        connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
    }
}

/**
 * @brief Display error dialog and close update dialog
 */
void NewVersionDialog::errorMessage()
{
    QMessageBox::critical(this, tr("Error"), tr("Unable to download installer file."), QMessageBox::Ok, QMessageBox::Ok);
    reject();
}

/**
 * @brief Download the changelog file
 */
void NewVersionDialog::downloadChangelog()
{
    QString url = QString(APP_CHANGELOG_URL).arg(m_version.code);

    QNetworkAccessManager* manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onChangelogFinished(QNetworkReply*)));

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    if (url.startsWith("https"))
    {
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    }

    manager->get(request);
}

/**
 * @brief Display the changelog file
 * @param _reply
 */
void NewVersionDialog::onChangelogFinished(QNetworkReply* _reply)
{
    QString html;

    if (_reply->error() == QNetworkReply::NoError)
    {
        // Load CSS
        html.append("<style>");

        QFile cssFile(":/lang/changelog.css");

        cssFile.open(QIODevice::ReadOnly);
        QTextStream cssContent(&cssFile);
        cssContent.setCodec("UTF-8");
        html.append(cssContent.readAll());
        cssFile.close();

        html.append("</style>");

        html.append(QString(_reply->readAll()));
    }
    else
    {
        QLOG_ERROR() << _reply->errorString();

        html.append(_reply->errorString());
    }

    ui->textBrowser->setHtml(html);

    _reply->manager()->deleteLater();
    _reply->deleteLater();
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
        QLOG_ERROR() << m_reply->errorString();

        m_file.remove();
        errorMessage();
    }
    else
    {
        m_file.open(QIODevice::ReadOnly);
        QByteArray data = m_file.readAll();
        QString hash = QString(QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex());
        m_file.close();

        if (hash.compare(m_version.hash, Qt::CaseInsensitive) != 0)
        {
            QLOG_ERROR() << "File corrupted";
            QLOG_DEBUG() << "Remote hash" << m_version.hash << "Local hash" << hash;

            m_file.remove();
            errorMessage();
        }
        else
        {
            QLOG_TRACE() << "Download finished";

            ui->progressBar->setValue(100);
            ui->progressBar->setMaximum(100);

            accept();
            m_ctrl->launchInstaller();
        }
    }
}
