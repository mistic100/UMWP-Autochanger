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
 * @brief Read monitors dimensions from UltraMon API
 */
void SysReader::queryMonitors(QHash<int, QScreen> &_sizes, QByteArray &_header, bool &_ok)
{
    _sizes.clear();
    _header.clear();
    _ok = false;

    HRESULT result;

    CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    IUltraMonSystem2* pSys = 0;
    result = CoCreateInstance(CLSID_UltraMonSystem, 0, CLSCTX_ALL, IID_IUltraMonSystem2, reinterpret_cast<void**>(&pSys));

    if (result == S_OK)
    {
        // READ DATA FROM UM API
        IUltraMonMonitors* pMons = 0;
        pSys->get_Monitors(&pMons);

        // number of monitors
        long nbMonitors = 0;
        pMons->get_Count(&nbMonitors);

        VARIANT vt = { 0 };
        vt.vt = VT_I4;

        // cycle monitors
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

                _sizes.insert(i, QScreen(
                                  (int)left,
                                  (int)top,
                                  (int)width,
                                  (int)height
                ));

                minX = qMin(minX, (int)left);
                minY = qMin(minY, (int)top);
                maxX = qMax(maxX, (int)left+(int)width);
                maxY = qMax(maxY, (int)top+(int)height);
            }

            pMon->Release();
        }

        // CREATE .wallpaper FILE HEADER
        const int version = 2;
        const int activedesktop = 0;
        nbMonitors = _sizes.size();

        if (nbMonitors > 0)
        {
            _ok = true;

            _header.append("UMWP", 4); // signature
            _header.append((char*)&version, 2); // version
            _header.append((char*)&activedesktop, 1); // active desktop
            _header.append((char*)&nbMonitors, 4); // nb monitors

            // rect monitors
            for (int i=0; i<nbMonitors; i++)
            {
                QScreen screen = _sizes.value(i);
                RECT rect;
                rect.right = screen.width();
                rect.bottom = screen.height();
                rect.left = screen.left();
                rect.top = screen.top();
                _header.append((char*)&rect, sizeof(RECT));
            }

            // store whoel desktop size with its top-left-most position
            _sizes.insert(-1, QScreen(minX, minY, maxX-minX, maxY-minY));
        }

        pMons->Release();
        pSys->Release();
    }

    CoUninitialize();
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
