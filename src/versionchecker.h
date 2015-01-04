#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>
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
    VersionChecker(QObject* _parent=0) : QObject(_parent) {}

public slots:
    void run()
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager();
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl(QString::fromAscii(APP_VERSION_URL))));
    }

private slots:
    void downloadFinished(QNetworkReply* _reply)
    {
        if (_reply->error() == QNetworkReply::NoError)
        {
            NewVersion version;
            version.code = _reply->readLine().trimmed();
            version.link = _reply->readLine().trimmed();
            version.hash = _reply->readLine().trimmed();

            if (version.code.compare(QString::fromAscii(APP_VERSION)) > 0)
            {
                qxtLog->debug("New version detected: "+version.code);
                emit newVersionAvailable(version);
            }
        }

        _reply->manager()->deleteLater();
        _reply->deleteLater();

        emit finished();
    }
    
signals:
    void newVersionAvailable(const NewVersion);
    void finished();
    
};

#endif // VERSIONCHECKER_H
