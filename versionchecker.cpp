#pragma comment(lib, "urlmon.lib")

#include <UrlMon.h>

#include "versionchecker.h"


VersionChecker::VersionChecker(QObject* _parent) : QObject(_parent) {}


void VersionChecker::doCheckVersion()
{
    HRESULT res = URLDownloadToFile( NULL, APP_VERSION_URL, L"version.txt", 0, NULL );

    if (res == S_OK && bFileExists("version.txt"))
    {
        QFile f("version.txt");
        f.open(QIODevice::ReadOnly);
        QString ver = f.readLine();
        f.close();
        f.remove();


        if (ver.compare(QString::fromAscii(APP_VERSION)) > 0)
        {
            emit newVersionAvailable(ver);
        }
    }
}
