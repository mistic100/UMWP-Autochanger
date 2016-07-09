#include <Shlobj.h>
#include <atlbase.h>
#include "lib/createshortcut.h"

#include "environment.h"


QString Environment::APPDATA_DIR = "";
QString Environment::EXPLORER_PATH = "";

/**
 * @brief Environment::Environment
 * @param Settings* _settings
 */
Environment::Environment()
{
    qRegisterMetaType<UM::NewVersion>("UM::NewVersion");

    // DETECT IF THE APP IS INSTALLED
    QDir dirHelper;
    m_isPortable = !dirHelper.exists(APP_UNINSTALLER_FILENAME);

    // CREATE CACHE DIR
    Environment::APPDATA_DIR = getAppDataFolder();

    if (!Environment::APPDATA_DIR.isEmpty())
    {
        QLOG_DEBUG() << "== APPDATA" << Environment::APPDATA_DIR;

        if (!dirHelper.exists(Environment::APPDATA_DIR + APP_CACHE_DIR))
        {
            dirHelper.mkpath(Environment::APPDATA_DIR + APP_CACHE_DIR);
        }
    }
    else
    {
        QLOG_FATAL() << "Cannot cache directory";
    }

    // LIST LANGUAGES
    QDirIterator it(":/lang", QDir::Dirs);
    while (it.hasNext())
    {
        it.next();
        m_languages.append(it.fileName());
    }
    qSort(m_languages);

    QLOG_DEBUG() << "== LANGUAGES" << m_languages;

    // SEARCH SHORTCUT FILE
    wchar_t* value;
    HRESULT result;

    value = (wchar_t*) malloc(256);
    result = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &value);

    if (result == S_OK)
    {
        m_shortcutPath = QString::fromWCharArray(value);
        m_shortcutPath.append("\\UMWP Autochanger.lnk");
    }
    else
    {
        QLOG_FATAL() << "Cannot read FOLDERID_Startup";
    }

    CoTaskMemFree(value);

    // LOCATE EXPLORER.exe
    char* value2 = getenv("windir");

    if (value2 != NULL)
    {
        Environment::EXPLORER_PATH = QString::fromLatin1(value2);
        Environment::EXPLORER_PATH.append("\\explorer.exe");
    }
    else
    {
        QLOG_FATAL() << "Something went really wrong!";
    }
}

/**
 * @brief Dump whole env in the log
 */
void Environment::log()
{
    QList<QString> sizes;
    for (QHash<int, QRect>::const_iterator it=m_wpSizes.begin(); it!=m_wpSizes.end(); ++it)
    {
        sizes.append(
            QString::number(it.key()) +": "+ QString::number(it.value().width()) +"x"+ QString::number(it.value().height())
            +" at "+ QString::number(it.value().left()) +"x"+ QString::number(it.value().top())
        );
    }

    QLOG_DEBUG() << "== MONITORS" << sizes;
}

/**
 * Callback for EnumDisplayMonitors
 */
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData)
{
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

    QList<RECT>* monitors = reinterpret_cast<QList<RECT>*>(dwData);
    monitors->append(mi.rcMonitor);

    return true;
}

/**
 * @brief Get monitors positions and sizes
 * @param QRect[] _sizes
 * @return bool
 */
void Environment::refreshMonitors()
{
    m_wpSizes.clear();

    QList<RECT> monitors;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitors));

    if (monitors.size() > 0)
    {
        int i=0, minX=0, maxX=0, minY=0, maxY=0;

        foreach (const RECT rect, monitors)
        {
            m_wpSizes.insert(i++, QRect(
                             rect.left,
                             rect.top,
                             rect.right-rect.left,
                             rect.bottom-rect.top
            ));

            minX = qMin(minX, (int)rect.left);
            minY = qMin(minY, (int)rect.top);
            maxX = qMax(maxX, (int)rect.right);
            maxY = qMax(maxY, (int)rect.bottom);
        }

        // store whole desktop size with its top-left-most position
        m_wpSizes.insert(-1, QRect(minX, minY, maxX-minX, maxY-minY));
    }
    else
    {
        QLOG_FATAL()<<"Unable to query monitors";
    }

    if (QsLogging::Logger::instance().loggingLevel() != QsLogging::OffLevel)
    {
        log();
    }
}

/**
 * @brief Refresh Windows wallpaper
 * @param string _file
 */
void Environment::setWallpaper(const QString &_file)
{
    CRegKey regKey;
    LONG result;

    result = regKey.Open(HKEY_CURRENT_USER, L"Control Panel\\Desktop", KEY_READ | KEY_WRITE);

    if (result == ERROR_SUCCESS)
    {
        result = regKey.SetStringValue(L"WallpaperStyle", L"0");
    }

    if (result == ERROR_SUCCESS)
    {
        result = regKey.SetStringValue(L"TileWallpaper", L"1");
    }

    if (result == ERROR_SUCCESS)
    {
        if (!SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)_file.toStdWString().c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE))
        {
            result = ERROR_FILE_NOT_FOUND;
        }
    }

    if (result != ERROR_SUCCESS)
    {
        QLOG_ERROR() << "Failed to set new wallpaper";
    }

    regKey.Close();
}

/**
 * Can we create the shortcut ?
 */
const bool Environment::canAddShortcut() const
{
    return !m_shortcutPath.isNull();
}

/**
 * @brief Does the shortcut exists ?
 */
const bool Environment::isAutostart() const
{
    if (!QFile::exists(m_shortcutPath))
    {
        return false;
    }

    wchar_t* path = (wchar_t*) malloc(256);
    ResolveShortcut((LPCWSTR)m_shortcutPath.utf16(), path);
    QString startlinkpath = QString::fromWCharArray(path);
    free(path);

    return startlinkpath.compare(QDir::toNativeSeparators(QCoreApplication::applicationFilePath())) == 0;
}

/**
 * @brief Create the startup shortcut
 */
void Environment::createShortcut()
{
    if (canAddShortcut())
    {
        QLOG_TRACE() << "Attempt to create shortcut";

        CreateShortcut(
            (LPCWSTR)QDir::toNativeSeparators(QCoreApplication::applicationFilePath()).utf16(),
            (LPCWSTR)QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).utf16(),
            (LPCWSTR)m_shortcutPath.utf16()
        );
    }
}

/**
 * @brief Delete the startup shortcut
 */
void Environment::deleteShortcut()
{
    if (isAutostart())
    {
        QLOG_TRACE() << "Remove shortcut";

        QFile::remove(m_shortcutPath);
    }
}

/**
 * @brief Return the application data location depending portability or not
 * @return string
 */
const QString Environment::getAppDataFolder() const
{
    if (m_isPortable)
    {
        return QDir::toNativeSeparators(QDir::currentPath() + "/");
    }
    else
    {
        return QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/");
    }
}
