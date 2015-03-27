#include <Shlobj.h>
#include <atlbase.h>
#include "lib/createshortcut.h"

#include "environment.h"
#include "sysreader.h"

extern short UMWP_STATE;


/**
 * @brief Environment::Environment
 * @param Settings* _settings
 */
Environment::Environment(Settings* _settings) :
    m_settings(_settings)
{
    qRegisterMetaType<NewVersion>("NewVersion");

    m_env["wallpath"] = QVariant();
    m_env["bmppath"] = QVariant();
    m_env["umversion"] = "";
    m_env["startlinkpath"] = QVariant();
    m_env["nb_monitors"] = 0;

    QDirIterator it(":/lang");
    while (it.hasNext())
    {
        it.next();
        m_languages.append(it.fileName());
    }
    qSort(m_languages);
}

/**
 * @brief Dump whole env in the log
 */
void Environment::log()
{
    QList<QString> env;
    for (QHash<QString, QVariant>::const_iterator it=m_env.begin(); it!=m_env.end(); ++it)
    {
        env.append(it.key() +": "+ it.value().toString());
    }

    QList<QString> sizes;
    for (QHash<int, QRect>::const_iterator it=m_wpSizes.begin(); it!=m_wpSizes.end(); ++it)
    {
        sizes.append(
            QString::number(it.key()) +": "+ QString::number(it.value().width()) +"x"+ QString::number(it.value().height())
            +" at "+ QString::number(it.value().left()) +"x"+ QString::number(it.value().top())
        );
    }

    QList<QString> langs;
    foreach (const QString lang, m_languages)
    {
        langs.append(lang);
    }

    QLOG_DEBUG() << "== ENVIRONEMENT";
    QLOG_DEBUG() << env;
    QLOG_DEBUG() << "== MONITORS";
    QLOG_DEBUG() << sizes;
    QLOG_DEBUG() << "== LANGUAGES";
    QLOG_DEBUG() << langs;
}

/**
 * @brief Init environnement variables
 */
void Environment::init()
{
    bool ok;

    // SEARCH ULTRAMON EXE
    QString umpath = m_settings->get("umpath").toString();
    if (umpath.isEmpty() || !QFile::exists(umpath))
    {
        umpath = SysReader::searchUMDexe(ok);

        if (!ok)
        {
            QLOG_ERROR() << "UltraMonDesktop.exe not found";
            UMWP_STATE|= UMWP::NOT_INSTALLED;
        }
        else
        {
            m_settings->setOpt("umpath", umpath);
            m_settings->save();
        }
    }

    // SEARCH ULTRAMON VERSION
    m_env["umversion"] = SysReader::getUMversion(ok);

    if (!ok)
    {
        QLOG_ERROR() << "Unknown UltraMon version";
        UMWP_STATE|= UMWP::BAD_VERSION;
    }
    else if (m_env["umversion"].toString().compare(QString::fromAscii(APP_MIN_UM_VERSION)) < 0)
    {
        QLOG_ERROR() << "Invalid UltraMon version";
        UMWP_STATE|= UMWP::BAD_VERSION;
    }

    // SEARCH BMP PATH
    m_env["bmppath"] = SysReader::buildBMPpath(ok);

    if (!ok) {
        UMWP_STATE|= UMWP::UNKNOWN_ERROR;
    }

    // SEARCH WALLPAPER FOLDER
    if (!(UMWP_STATE & UMWP::BAD_VERSION))
    {
        m_env["wallpath"] = SysReader::buildUMwallpaperPath(m_env["umversion"].toString(), ok);

        if (!ok)
        {
            UMWP_STATE|= UMWP::UNKNOWN_ERROR;
        }
        else if (!refreshMonitors())
        {
            UMWP_STATE|= UMWP::COM_ERROR;
        }
    }

    // SEARCH SHORTCUT FILE
    m_env["startlinkpath"] = SysReader::buildShortcutPath();
}

/**
 * @brief Read monitors config from default file or UltraMon API
 */
bool Environment::refreshMonitors()
{
    bool ok;

    SysReader::queryMonitors(m_wpSizes, m_header, ok);

    if (!ok)
    {
        QLOG_ERROR() << "Unable to query UltraMon API";
        return false;
    }

    m_env["nb_monitors"] = m_wpSizes.size() - 1; // an extra entry stores full available size

    checkSettings();

    return true;
}

/**
 * @brief Ensure that the settings are coherent with some environnement params
 */
void Environment::checkSettings()
{
    m_settings->setNbMonitors(m_env["nb_monitors"].toInt());
}

/**
 * Can we create the shortcut ?
 */
const bool Environment::canAddShortcut() const
{
    return !m_env["startlinkpath"].isNull();
}

/**
 * @brief Does the shortcut exists ?
 */
const bool Environment::isAutostart() const
{
    if (!QFile::exists(m_env["startlinkpath"].toString()))
    {
        return false;
    }

    wchar_t* path = (wchar_t*) malloc(256);
    ResolveShortcut((LPCWSTR)m_env["startlinkpath"].toString().utf16(), path);
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
            (LPCWSTR)m_env["startlinkpath"].toString().utf16()
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

        QFile::remove(m_env["startlinkpath"].toString());
    }
}
