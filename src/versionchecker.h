#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "main.h"
#include "environment.h"


/**
 * @brief Used to download version file and compare to current version
 */
class VersionChecker : public QObject
{
    Q_OBJECT

public:
    VersionChecker(QObject* _parent = nullptr) : QObject(_parent) {}

public slots:
    void run()
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

        QNetworkRequest request(QUrl(APP_VERSION_URL));
        if (QString(APP_VERSION_URL).startsWith("https"))
        {
            request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        }

        manager->get(request);
    }

private slots:
    void downloadFinished(QNetworkReply* _reply)
    {
        if (_reply->error() == QNetworkReply::NoError)
        {
            UM::NewVersion version;
            version.code = _reply->readLine().trimmed();
            version.link = _reply->readLine().trimmed();
            version.hash = _reply->readLine().trimmed();
            if (QString(APP_VERSION_URL).startsWith("https"))
            {
                version.link = version.link.replace("http://", "https://");
            }

            if (version.code.compare(APP_VERSION) > 0)
            {
                QLOG_DEBUG() << "New version detected: " << version.code;
                emit newVersionAvailable(version);
            }
            else
            {
                QLOG_DEBUG() << "Server version is" << version.code;
            }
        }
        else
        {
            QLOG_ERROR() << _reply->errorString();
        }

        _reply->manager()->deleteLater();
        _reply->deleteLater();

        emit finished();
    }
    
signals:
    void newVersionAvailable(const UM::NewVersion);
    void finished();
};

#endif // VERSIONCHECKER_H
