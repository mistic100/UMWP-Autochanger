#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#pragma comment(lib, "urlmon.lib")

#include <UrlMon.h>
#include <QObject>

#include "main.h"


/**
 * @brief Used to download version file and compare to current version
 */
class VersionChecker : public QObject
{
    Q_OBJECT

public:
    VersionChecker(QObject* _parent) : QObject(_parent) {}

public slots:
    void run()
    {
        HRESULT res = URLDownloadToFile(NULL, APP_VERSION_URL, L"version.txt", 0, NULL);

        if (res == S_OK )
        {
            QFile file("version.txt");
            if (file.open(QIODevice::ReadOnly))
            {
                QString ver = file.readLine();
                file.close();
                file.remove();


                if (ver.compare(QString::fromAscii(APP_VERSION)) > 0)
                {
                    qxtLog->debug("New version detected: "+ver);
                    emit newVersionAvailable(ver);
                }
            }
        }

        emit finished();
    }
    
signals:
    void newVersionAvailable(const QString);
    void finished();
    
};

#endif // VERSIONCHECKER_H
