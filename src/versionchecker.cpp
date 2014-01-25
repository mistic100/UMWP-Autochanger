#pragma comment(lib, "urlmon.lib")

#include <UrlMon.h>

#include "versionchecker.h"


/**
 * @brief VersionChecker::VersionChecker
 * @param QWidget* _parent
 */
VersionChecker::VersionChecker(QObject* _parent) : QObject(_parent) {}

/**
 * @brief Tries to download the version file and compare version
 */
void VersionChecker::run()
{
    HRESULT res = URLDownloadToFile(NULL, APP_VERSION_URL, L"version.txt", 0, NULL);

    if (res == S_OK && fileExists("version.txt"))
    {
        QFile file("version.txt");
        file.open(QIODevice::ReadOnly);
        QString ver = file.readLine();
        file.close();
        file.remove();


        if (ver.compare(QString::fromAscii(APP_VERSION)) > 0)
        {
            emit newVersionAvailable(ver);
        }
    }

    emit finished();
}
