#include <Shlobj.h>
#include <Knownfolders.h>
#include <atlbase.h>
#include "lib/UltraMon.h"
#include "lib/UltraMon_i.c"

#include "sysreader.h"
#include "main.h"


static LPCTSTR UM_UNINSTALL_REGKEY = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{ED7FE81C-378C-411D-B5B4-509B978BA204}";
static LPCTSTR UM_INSTALL_REGKEY = L"SOFTWARE\\Realtime Soft\\UltraMon";
static LPCTSTR MS_CURRENTVERSION_REGKEY = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion";
static QString UM_DIRECTORY_1 = "\\Realtime Soft\\UltraMon\\UltraMonDesktop.exe";
static QString UM_DIRECTORY_2 = "\\UltraMon\\UltraMonDesktop.exe";
static QString UM_BMP_PATH = "\\Realtime Soft\\UltraMon\\UltraMon Wallpaper.bmp";
static QString UM_WP_PATH = "\\Realtime Soft\\UltraMon\\%1\\Wallpapers\\";


/**
 * @brief Search UltraMonDesktop.exe in various folders
 */
QString SysReader::searchUMDexe(bool &_ok)
{
    CRegKey regKey;
    wchar_t* value;
    LONG result;
    DWORD wordLen;
    QList<QString> path;

    _ok = false;


    value = (wchar_t*) malloc(256);
    wordLen = 256;
    result = regKey.Open(HKEY_LOCAL_MACHINE, UM_UNINSTALL_REGKEY, KEY_READ | KEY_WOW64_64KEY);

    if (result == ERROR_SUCCESS)
    {
        result = regKey.QueryStringValue(_T("InstallLocation"), value, &wordLen);
        if (result == ERROR_SUCCESS)
        {
            QString umPath = QString::fromWCharArray(value, wordLen-1); // remove the \0 termination
            path.append(umPath + "UltraMonDesktop.exe");
        }
    }

    regKey.Close();
    free(value);


    // cannot use SHGetKnownFolderPath because we are a 32bits application and are looking for the x64 Program Files directory
    value = (wchar_t*) malloc(256);
    wordLen = 256;
    result = regKey.Open(HKEY_LOCAL_MACHINE, MS_CURRENTVERSION_REGKEY, KEY_READ | KEY_WOW64_64KEY);

    if (result == ERROR_SUCCESS)
    {
        result = regKey.QueryStringValue(_T("ProgramFilesDir"), value, &wordLen);
        if (result == ERROR_SUCCESS)
        {
             QString pfPath = QString::fromWCharArray(value, wordLen-1); // remove the \0 termination
             path.append(pfPath + UM_DIRECTORY_1);
             path.append(pfPath + UM_DIRECTORY_2);
        }
    }

    regKey.Close();
    free(value);


    for (QList<QString>::iterator it = path.begin(); it != path.end(); ++it)
    {
        if (QFile::exists(*it))
        {
            _ok = true;
            return *it;
        }
    }

    if (path.isEmpty())
    {
        return "";
    }
    else
    {
        return path.back();
    }
}

/**
 * @brief Read UltraMon version from registry
 */
QString SysReader::getUMversion(bool &_ok)
{
    CRegKey regKey;
    wchar_t* value;
    LONG result;
    DWORD wordLen;
    QString version;

    _ok = false;


    value = (wchar_t*) malloc(16);
    wordLen = 16;
    result = regKey.Open(HKEY_LOCAL_MACHINE, UM_INSTALL_REGKEY, KEY_READ | KEY_WOW64_64KEY);

    if (result == ERROR_SUCCESS)
    {
        result = regKey.QueryStringValue(_T("CurrentVersion"), value, &wordLen);
        if (result == ERROR_SUCCESS)
        {
            _ok = true;
            version = QString::fromWCharArray(value, wordLen-1); // remove the \0 termination
        }
    }

    regKey.Close();
    free(value);

    return version;
}

/**
 * @brief Build UltraMon Wallpaper.bmp path
 */
QString SysReader::buildBMPpath(bool &_ok)
{
    wchar_t* value;
    HRESULT result;
    QString path;

    _ok = false;


    value = (wchar_t*) malloc(256);
    result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &value);

    if (result == S_OK)
    {
        _ok = true;
        path = QString::fromWCharArray(value) + UM_BMP_PATH;
    }
    else
    {
        qxtLog->fatal("Something went really wrong!");
    }

    CoTaskMemFree(value);


    return path;
}

/**
 * @brief Build .wallpaper folder
 */
QString SysReader::buildUMwallpaperPath(const QString &_version, bool &_ok)
{
    wchar_t* value;
    HRESULT result;
    QString path;

    _ok = false;


    value = (wchar_t*) malloc(256);
    result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &value);

    if (result == S_OK)
    {
        _ok = true;
        path = QString::fromWCharArray(value) + UM_WP_PATH.arg(_version);
    }
    else
    {
        qxtLog->fatal("Something went really wrong!");
    }

    CoTaskMemFree(value);


    return path;
}

/**
 * @brief Create default.wallpaper file with monitors dimensions
 */
void SysReader::createUMwallpaper(const QString &_path, const QHash<int, QScreen> &_sizes, bool&_ok)
{
    _ok = false;
    const int version = 2;
    const int activedesktop = 0;
    int nbMonitors = _sizes.size()-1;

    QString filename = _path + "default.wallpaper";
    QByteArray buffer;

    buffer.append("UMWP", 4); // signature
    buffer.append((char*)&version, 2); // version
    buffer.append((char*)&activedesktop, 1); // active desktop
    buffer.append((char*)&nbMonitors, 4); // nb monitors

    // rect monitors
    for (int i=0; i<nbMonitors; i++)
    {
        QScreen screen = _sizes.value(i);
        RECT rect;
        rect.right = screen.width();
        rect.bottom = screen.height();
        rect.left = screen.left();
        rect.top = screen.top();
        buffer.append((char*)&rect, sizeof(RECT));
    }

    // wp style
    UM::WALLPAPER wpstyle = UM::W_DESKTOP;
    buffer.append((char*)&wpstyle, sizeof(UM::WALLPAPER));

    buffer.append((char*)&nbMonitors, 4); // nb walls

    // wallpaper files
    for (int i=0; i<nbMonitors; i++)
    {
        UM::WP_MONITOR_FILE wall;
        wall.bgType = UM::BG_SOLID;
        wall.color1 = 0x00000000;
        wall.color2 = 0x00000000;
        wall.imgStyle = UM::IM_CENTER;
        memset(wall.imgFile, 0, 260*sizeof(wchar_t));
        buffer.append((char*)&wall, sizeof(UM::WP_MONITOR_FILE));
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        _ok = true;
        file.write(buffer);
        file.close();
    }
}

/**
 * @brief Read monitors dimensions from default.wallpaper file
 */
void SysReader::readMonitors(const QString &_path, QHash<int, QScreen> &_sizes, QByteArray &_header, bool &_ok)
{
    _sizes.clear();
    _header.clear();
    _ok = false;

    QString filename = _path + "default.wallpaper";
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly)) {
        _ok = true;

        file.seek(7); // "UMWP",  version, activedesktop

        // number of monitors
        int nbMonitors;
        file.read((char*)&nbMonitors, sizeof(DWORD));

        // rect monitors
        int minX=0, maxX=0, minY=0, maxY=0;
        for (int i=0; i<nbMonitors; i++)
        {
            RECT monitor;
            file.read((char*)&monitor, sizeof(RECT));

            _sizes.insert(i, QScreen(
                               (int)monitor.right,
                               (int)monitor.bottom,
                               (int)monitor.top,
                               (int)monitor.left
                            ));

            minX = qMin(minX, (int)monitor.left);
            minY = qMin(minY, (int)monitor.top);
            maxX = qMax(maxX, (int)monitor.left+(int)monitor.right);
            maxY = qMax(maxY, (int)monitor.top+(int)monitor.bottom);
        }

        // store the header
        int headerSize = 7 + sizeof(DWORD) + nbMonitors*sizeof(RECT);
        file.reset();
        _header = file.read(headerSize);

        _sizes.insert(-1, QScreen(maxX-minX, maxY-minY));

        file.close();
    }
}

/**
 * @brief Read monitors dimensions from UltraMon API
 */
QHash<int, QScreen> SysReader::queryMonitors(bool &_ok)
{
    _ok = false;

    HRESULT result;
    QHash<int, QScreen> sizes;

    CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    IUltraMonSystem2* pSys = 0;
    result = CoCreateInstance(CLSID_UltraMonSystem, 0, CLSCTX_ALL, IID_IUltraMonSystem2, reinterpret_cast<void**>(&pSys));

    if (result == S_OK)
    {
        _ok = true;

        IUltraMonMonitors* pMons = 0;
        pSys->get_Monitors(&pMons);

        // number of monitors
        long nbMonitors = 0;
        pMons->get_Count(&nbMonitors);

        VARIANT vt = { 0 };
        vt.vt = VT_I4;

        // monitors
        int minX=0, maxX=0, minY=0, maxY=0;
        for (int i=0; i<nbMonitors; ++i)
        {
            IUltraMonMonitor* pMon = 0;
            vt.lVal = i;
            pMons->Item(vt, &pMon);

            VARIANT_BOOL enabled = VARIANT_FALSE;
            pMon->get_Enabled(&enabled);

            if (enabled == VARIANT_TRUE)
            {
                long width = 0, height = 0, left = 0, top = 0;
                pMon->get_Width(&width);
                pMon->get_Height(&height);
                pMon->get_Left(&left);
                pMon->get_Top(&top);

                sizes.insert(i, QScreen(
                                   (int)width,
                                   (int)height,
                                   (int)top,
                                   (int)left
                                ));

                minX = qMin(minX, (int)left);
                minY = qMin(minY, (int)top);
                maxX = qMax(maxX, (int)left+(int)width);
                maxY = qMax(maxY, (int)top+(int)height);
            }

            pMon->Release();
        }

        sizes.insert(-1, QScreen(maxX-minX, maxY-minY));

        pMons->Release();
        pSys->Release();
    }

    CoUninitialize();

    return sizes;
}

/**
 * @brief Build UMWP Autochanger.lnk path
 */
QString SysReader::buildShortcutPath()
{
    wchar_t* value;
    HRESULT result;
    QString path;


    value = (wchar_t*) malloc(256);
    result = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &value);

    if (result == S_OK)
    {
        path = QString::fromWCharArray(value);
        path.append("\\UMWP Autochanger.lnk");
    }
    else
    {
        qxtLog->fatal("Something went really wrong!");
    }

    CoTaskMemFree(value);


    return path;
}
