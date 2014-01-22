#include <Shlobj.h>
#include <Knownfolders.h>
#include <atlbase.h>
#include <iostream>
#include <fstream>
#include <direct.h>

#include "settings.h"
#include "createshortcut.h"


/**
 * @brief Settings::Settings
 */
Settings::Settings()
{
    // default configuration
    m_iState = UM_OK;
    m_bUnsaved = false;

    m_options["umpath"] = QVariant();
    m_options["window_width"] = 430;
    m_options["window_height"] = 240;
    m_options["delay"] = 60;
    m_options["minimize"] = true;
    m_options["check"] = true;
    m_options["check_updates"] = true;
    m_options["msgcount"] = 0;
    m_options["use_hotkeys"] = false;
    m_options["show_notifications"] = true;
    m_options["last_dir"] = QVariant();

    m_hotkeys["refresh"] = 0;
    m_hotkeys["startpause"] = 0;
    m_hotkeys["showhide"] = 0;

    m_env["wallpath"] = QVariant();
    m_env["bmppath"] = QVariant();
    m_env["umversion"] = "unknown";
    m_env["startlinkpath"] = QVariant();
    m_env["nb_monitors"] = 1;

    vReadXML();
    vInit();

    //qDebug()<<m_options;
    //qDebug()<<m_env;

    QDir oDirHelper;
    if (!oDirHelper.exists(APP_CACHE_DIR))
    {
        oDirHelper.mkdir(APP_CACHE_DIR);
    }
}

/**
 * @brief Settings::~Settings
 */
Settings::~Settings()
{
    vClearSets(); // clear memory but do not save changes !
}


/*
 * getters
 */
const bool Settings::bParam(QString const &_key) const
{
    return m_options.value(_key, false).toBool();
}

const int Settings::iParam(QString const &_key) const
{
    return m_options.value(_key, 0).toInt();
}

const QString Settings::sParam(QString const &_key) const
{
    return m_options.value(_key, "").toString();
}

const bool Settings::bEnv(QString const &_key) const
{
    return m_env.value(_key, false).toBool();
}

const int Settings::iEnv(QString const &_key) const
{
    return m_env.value(_key, 0).toInt();
}

const QString Settings::sEnv(QString const &_key) const
{
    return m_env.value(_key, "").toString();
}

const QSize Settings::oWindowSize() const
{
    return QSize(m_options["window_width"].toInt(), m_options["window_height"].toInt());
}

const bool Settings::bCanAddShortcut()
{
    return !m_env["startlinkpath"].isNull();
}

const bool Settings::bIsAutostart() const
{
    return bFileExists(m_env["startlinkpath"].toString());
}

const int Settings::iHotkey(const QString &_key) const
{
    return m_hotkeys.value(_key, 0);
}


/*
 * setters
 */
void Settings::vSetParam(const QString &_key, const QVariant &_val)
{
    m_options[_key] = _val;
    m_bUnsaved = true;
}

void Settings::vSetHotkey(const QString &_key, const int &_val)
{
    m_hotkeys[_key] = _val;
    m_bUnsaved = true;
}

void Settings::vSetWindowSize(const QSize &_size)
{
    m_options["window_width"] = _size.width();
    m_options["window_height"] = _size.height();
}

void Settings::vAddMsgCount()
{
    m_options["msgcount"] = m_options["msgcount"].toInt()+1;
}


/**
 * @brief Init environnement variables
 */
void Settings::vInit()
{
    CRegKey oReg;
    int iResult;
    DWORD dwLen;


    // SEARCH ULTRAMON EXE
    if (m_options["umpath"].isNull())
    {
        wchar_t* sPath1 = (wchar_t*) malloc(256);
        dwLen = 256;

        iResult = oReg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"), KEY_READ | KEY_WOW64_64KEY);
        if (iResult == ERROR_SUCCESS)
        {
            iResult = oReg.QueryStringValue(_T("ProgramFilesDir"), sPath1, &dwLen);
            if (iResult == ERROR_SUCCESS)
            {
                 QString sExePath = QString::fromWCharArray(sPath1, dwLen-1); // remove the \0 termination
                 sExePath.append("\\Realtime Soft\\UltraMon\\UltraMonDesktop.exe");
                 m_options["umpath"] = sExePath;
            }
        }
        oReg.Close();

        free(sPath1);
    }

    if (m_options["umpath"].isNull() || !bFileExists(m_options["umpath"].toString(), false))
    {
        m_iState|= UM_NOT_INSTALLED;
    }


    // SEARCH ULTRAMON VERSION
    wchar_t* sVer = (wchar_t*) malloc(16);
    dwLen = 16;

    iResult = oReg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Realtime Soft\\UltraMon"), KEY_READ | KEY_WOW64_64KEY);
    if (iResult == ERROR_SUCCESS)
    {
        iResult = oReg.QueryStringValue(_T("CurrentVersion"), sVer, &dwLen);
        if (iResult == ERROR_SUCCESS)
        {
            m_env["umversion"] = QString::fromWCharArray(sVer, dwLen-1); // remove the \0 termination
        }
    }
    oReg.Close();

    free(sVer);

    if (m_env["umversion"] == "unknown")
    {
        m_iState|= UM_BAD_VERSION;
    }
    // TODO : check version


    // SEARCH BMP PATH
    wchar_t* sPath2 = (wchar_t*) malloc(256);

    iResult = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &sPath2);
    if (iResult == S_OK)
    {
        QString sBMPPath = QString::fromWCharArray(sPath2);
        sBMPPath.append("\\Realtime Soft\\UltraMon\\UltraMon Wallpaper.bmp");
        m_env["bmppath"] = sBMPPath;
    }
    // TODO : error check

    CoTaskMemFree(sPath2);


    // SEARCH WALLPAPER FOLDER
    if (!(m_iState & UM_BAD_VERSION))
    {
        wchar_t* sPath3 = (wchar_t*) malloc(256);

        int iResult = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &sPath3);
        if (iResult == S_OK)
        {
            QString sWallPath = QString::fromWCharArray(sPath3);
            sWallPath.append("\\Realtime Soft\\UltraMon\\" + m_env["umversion"].toString() + "\\Wallpapers\\");
            m_env["wallpath"] = sWallPath;
        }

        CoTaskMemFree(sPath3);
    }

    // CHECK WALLPAPER FILE
    if (m_env["wallpath"].isNull())
    {
        m_iState|= UM_FILE_NOT_FOUND;
    }
    else
    {
        QString sFilename = m_env["wallpath"].toString().append("default.wallpaper");

        if (!bFileExists(sFilename))
        {
            m_iState|= UM_FILE_NOT_FOUND;
        }
        else
        {
            vReadNbMonitors();
        }
    }


    // SEARCH SHORTCUT FILE
    wchar_t* sPath4 = (wchar_t*) malloc(256);

    iResult = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &sPath4);
    if (iResult == S_OK)
    {
        QString sStartLnkPath = QString::fromWCharArray(sPath4);
        sStartLnkPath.append("\\UMWP Autochanger.lnk");
        m_env["startlinkpath"] = sStartLnkPath;
    }

    CoTaskMemFree(sPath4);
}

/**
 * @brief Read the number of monitors from .wallpaper file
 */
void Settings::vReadNbMonitors()
{
    QString sFilename = m_env["wallpath"].toString() + "default.wallpaper";
    std::ifstream ifs(sFilename.toStdString(), std::ios::in | std::ios::binary);

    ifs.ignore(7); // "UMWP",  version, activedesktop

    int iNbMonitors;
    ifs.read((char*)&iNbMonitors, sizeof(DWORD)); // number of monitors
    m_env["nb_monitors"] = iNbMonitors;

    ifs.close();
}


/**
 * @brief Load contents of the settings file
 */
void Settings::vReadXML()
{
    // open xml file
    QFile oXML(QString::fromAscii(APP_CONFIG_FILE));

    if (!oXML.open(QIODevice::ReadOnly))
    {
        return;
    }

    // initialize domdocument
    QDomDocument oDom;
    oDom.setContent(oXML.readAll());

    oXML.close();

    // read settings (main) node
    QDomElement main_node = oDom.documentElement();
    QDomElement setting_node = main_node.firstChild().toElement();

    vClearSets();

    int iNewHotkey = 0;

    while (!setting_node.isNull())
    {
        // config node
        if (setting_node.tagName() == "config")
        {
            QDomElement config_node = setting_node.firstChild().toElement();

            while (!config_node.isNull())
            {
                // migration from 1.3
                if (config_node.tagName() == "hotkey")
                {
                    iNewHotkey = config_node.text().toInt();
                }
                else
                {
                    m_options[ config_node.tagName() ] = config_node.text();
                }

                config_node = config_node.nextSibling().toElement();
            }
        }
        else if (setting_node.tagName() == "hotkeys")
        {
            QDomElement hotkey_node = setting_node.firstChild().toElement();

            while (!hotkey_node.isNull())
            {
                if (hotkey_node.hasAttribute("key"))
                {
                    m_hotkeys[ hotkey_node.tagName() ] =
                                hotkey_node.attribute("key").toInt() +
                                hotkey_node.attribute("mod").toInt();
                }
                else
                {
                    m_hotkeys[ hotkey_node.tagName() ] = hotkey_node.text().toInt();
                }

                hotkey_node = hotkey_node.nextSibling().toElement();
            }
        }
        // sets node
        else if (setting_node.tagName() == "sets")
        {
            QDomElement set_node = setting_node.firstChild().toElement();

            while (!set_node.isNull())
            {
                // set node
                if (set_node.tagName() == "set")
                {
                    QString sPath = set_node.text().trimmed();
                    if (bDirectoryExists(sPath))
                    {
                        Set* poNewSet = poAddSet(
                            sPath,
                            set_node.attribute("name"),
                            set_node.attribute("active").toInt()
                            );

                        // added in 1.3
                        if (set_node.hasAttribute("type"))
                        {
                            UM::WALLPAPER wp_type = static_cast<UM::WALLPAPER>(set_node.attribute("type").toInt());
                            UM::IMAGE im_style = static_cast<UM::IMAGE>(set_node.attribute("style").toInt());
                            poNewSet->vSetType(wp_type);
                            poNewSet->vSetStyle(im_style);
                        }

                        // added in 1.4
                        if (set_node.hasAttribute("hotkey_mod"))
                        {
                            poNewSet->vSetHotkey(
                                        set_node.attribute("hotkey").toInt() +
                                        set_node.attribute("hotkey_mod").toInt()
                                        );
                        }
                        else if (set_node.hasAttribute("hotkey"))
                        {
                            poNewSet->vSetHotkey(set_node.attribute("hotkey").toInt());
                        }
                    }
                }

                set_node = set_node.nextSibling().toElement();
            }
        }

        setting_node = setting_node.nextSibling().toElement();
    }

    if (iNewHotkey > 0)
    {
        vUpgradeHotkeys(iNewHotkey);
        vWriteXML();
    }

    m_bUnsaved = false;
}

/**
 * @brief Write parameters into the settings file
 */
void Settings::vWriteXML(QString _sFilename)
{
    if (_sFilename.isEmpty())
    {
        _sFilename = QString::fromAscii(APP_CONFIG_FILE);
    }

    // initialize domdocument
    QDomDocument oDom;
    QDomProcessingInstruction header = oDom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    oDom.appendChild(header);

    // settings (main) node
    QDomElement main_node = oDom.createElement("settings");

    // config node
    QDomElement config_node = oDom.createElement("config");

    for (QHash<QString, QVariant>::iterator it = m_options.begin(); it != m_options.end(); ++it)
    {
        addSimpleTextNode(&oDom, &config_node, it.key(), it.value().toString());
    }

    main_node.appendChild(config_node);

    // hotkeys node
    QDomElement hotkeys_node = oDom.createElement("hotkeys");

    for (QHash<QString, int>::iterator it = m_hotkeys.begin(); it != m_hotkeys.end(); ++it)
    {
        addSimpleTextNode(&oDom, &hotkeys_node, it.key(), QString::number(it.value()));
    }

    main_node.appendChild(hotkeys_node);

    // sets node
    QDomElement sets_node = oDom.createElement("sets");

    for (QVector<Set*>::iterator it = m_oSets.begin(); it != m_oSets.end(); ++it)
    {
        Set* poSet = *it;

        // set node
        QDomElement set = oDom.createElement("set");
        set.setAttribute("name", poSet->name());
        set.setAttribute("type", poSet->type());
        set.setAttribute("style", poSet->style());
        set.setAttribute("active", poSet->isActive());
        set.setAttribute("hotkey", poSet->hotkey());
        setDomNodeValue(&oDom, &set, poSet->path());

        sets_node.appendChild(set);
    }

    main_node.appendChild(sets_node);

    oDom.appendChild(main_node);

    // save file
    QFile oXML(_sFilename);
    oXML.open(QIODevice::WriteOnly);

    QTextStream stream(&oXML);
    oDom.save(stream, 2);

    oXML.close();

    m_bUnsaved = false;
}


/**
 * @brief Change the path of UltraMonDesktop.exe
 * @param string _sPath
 * @return bool - true if the path is valid
 */
bool Settings::bSetExePath(const QString &_sPath)
{
    if (bFileExists(_sPath))
    {
        QString sFilename = _sPath.section('\\', -1);
        if (sFilename.compare("UltraMonDesktop.exe")==0)
        {
            vSetParam("umpath", _sPath);
            vWriteXML();

            m_iState&= ~UM_NOT_INSTALLED;
            return true;
        }
    }

    return false;
}


/**
 * @brief Add a new set from path
 * @param string _sPath
 * @return Set*
 */
Set* Settings::poAddSet(const QString &_sPath)
{
    return poAddSet(_sPath, sGetDirName(_sPath), true);
}

/**
 * @brief Add a new set
 * @param string _sPath
 * @param string _sName
 * @param bool _bActive
 * @return Set*
 */
Set* Settings::poAddSet(const QString &_sPath, QString &_sName, bool _bActive)
{
    QString sPath = sAddTrailingSlash(_sPath);

    if (!bDirectoryExists(sPath))
    {
        return NULL;
    }

    m_bUnsaved = true;

    Set* poSet = new Set(sPath, _sName, _bActive);
    m_oSets.push_back(poSet);

    return poSet;
}

/**
 * @brief Delete a set
 * @param int _i - position in vector
 */
void Settings::vDeleteSet(int _i)
{
    if ( _i < m_oSets.size())
    {
        m_bUnsaved = true;

        m_oSets.at(_i)->vDeleteCache();
        delete m_oSets.at(_i);
        m_oSets.remove(_i);
    }
}

/**
 * @brief Unload all sets
 * Sets are only onloaded from memory, they are not deleted
 */
void Settings::vClearSets()
{
    if (!m_oSets.empty())
    {
        m_bUnsaved = true;

        qDeleteAll(m_oSets);
        m_oSets.clear();
    }
}

/**
 * @brief Edit a set
 * @param int _i - position in vecto
 * @param stirng _sName
 * @param int _iType
 * @param int _iStyle
 */
void Settings::vEditSet(int _i, const QString &_sName, const UM::WALLPAPER _iType, const UM::IMAGE _iStyle, const int _iHotkey)
{
    if ( _i < m_oSets.size())
    {
        m_bUnsaved = true;

        Set* poSet = m_oSets.at(_i);
        poSet->vSetName(_sName);
        poSet->vSetType(_iType);
        poSet->vSetStyle(_iStyle);
        poSet->vSetHotkey(_iHotkey);
    }
}

/**
 * @brief Set the state of a set
 * @param int _i - positon in vector
 * @param bool _bState
 */
void Settings::vSetState(int _i, bool _bState)
{
    m_bUnsaved = true;
    m_oSets.at(_i)->vSetActive(_bState);
}

/**
 * @brief Move a set
 * @param int _from - positon in vector
 * @param int _to - new position in vector
 */
void Settings::vMoveSet(int _from, int _to)
{
    Set* poSet = m_oSets.at(_from);
    m_oSets.insert(_to, poSet);

    if (_from<_to)
    {
        m_oSets.remove(_from);
    }
    else
    {
        m_oSets.remove(_from+1);
    }

    m_bUnsaved = true;
}

/**
 * @brief Remove unexisting sets and update file lists
 */
void Settings::vUpdateSets()
{
    for (QVector<Set*>::iterator it=m_oSets.begin(); it!=m_oSets.end(); )
    {
        if (!bDirectoryExists((*it)->path()))
        {
            m_bUnsaved = true;
            m_oSets.erase(it);
        }
        else
        {
            (*it)->vPopulateFiles();
            it++;
        }
    }
}

/**
 * @brief Get one of the active sets
 * @param int _i - position in the sub-vector of active sets
 * @return Set*
 */
Set* Settings::poGetActiveSet(int _i) const
{
    QVector<Set*> poActiveSets;
    for (QVector<Set*>::const_iterator it=m_oSets.begin(); it!=m_oSets.end(); ++it)
    {
        if ((*it)->isActive())
        {
            poActiveSets.push_back((*it));
        }
    }
    return poActiveSets.at(_i);
}

/**
 * @brief Get the number of active sets
 * @param bool _bWithFiles - if true, only non-empty sets are counted
 * @return  int
 */
int const Settings::iNbActiveSets(bool _bWithFiles) const
{
    int iTotalSets = 0;
    for (QVector<Set*>::const_iterator it=m_oSets.begin(); it!=m_oSets.end(); ++it)
    {
        if ((*it)->isActive() && (!_bWithFiles || (*it)->count()>0))
        {
            iTotalSets++;
        }
    }
    return iTotalSets;
}

/**
 * @brief Get total files among active sets
 * @return int
 */
int const Settings::iNbActiveFiles()
{
    int iTotalFiles = 0;
    for (QVector<Set*>::iterator it=m_oSets.begin(); it!=m_oSets.end(); ++it)
    {
        if ((*it)->isActive())
        {
            iTotalFiles+= (*it)->count();
        }
    }
    return iTotalFiles;
}


/**
 * @brief Create the startup shortcut
 */
void Settings::vCreateShortcut()
{
    if (!m_env["startlinkpath"].isNull())
    {
        wchar_t* sPath = (wchar_t*) malloc(256);
        wchar_t* sPathE = (wchar_t*) malloc(256);

        GetModuleFileName(NULL, sPath, 256);
        wcscpy(sPathE, sPath);
        PathRemoveFileSpec(sPathE);

        CreateShortcut(sPath, sPathE, (LPCWSTR)m_env["startlinkpath"].toString().utf16());

        free(sPath);
        free(sPathE);
    }
}

/**
 * @brief Delete the startup shortcut
 */
void Settings::vDeleteShortcut()
{
    if (bIsAutostart())
    {
        QFile::remove(m_env["startlinkpath"].toString());
    }
}

/**
 * @brief Build sets hotkeys when migrating from version 1.3
 */
void Settings::vUpgradeHotkeys(int iWinMod)
{
    int iQtMod = 0;
    if (iWinMod & MOD_SHIFT)
    {
        iQtMod+= Qt::SHIFT;
    }
    if (iWinMod & MOD_CONTROL)
    {
        iQtMod+= Qt::CTRL;
    }
    if (iWinMod & MOD_ALT)
    {
        iQtMod+= Qt::ALT;
    }
    if (iWinMod & MOD_WIN)
    {
        iQtMod+= Qt::META;
    }

    int iKey = Qt::Key_1;
    for (QVector<Set*>::iterator it=m_oSets.begin(); it!=m_oSets.end(); ++it)
    {
        (*it)->vSetHotkey(iKey + iQtMod);
        iKey++;
        if (iKey > Qt::Key_9)
        {
            break;
        }
    }

    m_options["use_hotkeys"] = true;
}
