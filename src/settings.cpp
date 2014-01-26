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
    m_state = UM_OK;

    m_options["umpath"] = QVariant();
    m_options["window_width"] = 440;
    m_options["window_height"] = 240;
    m_options["delay"] = 60;
    m_options["minimize"] = true;
    m_options["check"] = true;
    m_options["check_updates"] = true;
    m_options["msgcount"] = 0;
    m_options["use_hotkeys"] = false;
    m_options["show_notifications"] = true;
    m_options["last_dir"] = QDir::homePath();

    m_hotkeys["refresh"] = 0;
    m_hotkeys["startpause"] = 0;
    m_hotkeys["showhide"] = 0;

    m_env["wallpath"] = QVariant();
    m_env["bmppath"] = QVariant();
    m_env["umversion"] = "unknown";
    m_env["startlinkpath"] = QVariant();
    m_env["nb_monitors"] = 1;
    m_env["header_size"] = 7 + sizeof(DWORD) + sizeof(RECT);

    load();
    init();

    //qDebug()<<m_options;
    //qDebug()<<m_env;
}

/**
 * @brief Settings::~Settings
 */
Settings::~Settings()
{
    clearSets(); // clear memory but do not save changes !
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

const QSize Settings::windowSize() const
{
    return QSize(m_options["window_width"].toInt(), m_options["window_height"].toInt());
}

const bool Settings::canAddShortcut() const
{
    return !m_env["startlinkpath"].isNull();
}

const bool Settings::isAutostart() const
{
    return fileExists(m_env["startlinkpath"].toString());
}

const int Settings::hotkey(const QString &_key) const
{
    return m_hotkeys.value(_key, 0);
}


/*
 * setters
 */
void Settings::setParam(const QString &_key, const QVariant &_val)
{
    m_options[_key] = _val;
}

void Settings::setHotkey(const QString &_key, const int &_val)
{
    m_hotkeys[_key] = _val;
}

void Settings::setWindowSize(const QSize &_size)
{
    m_options["window_width"] = _size.width();
    m_options["window_height"] = _size.height();
}

void Settings::addMsgCount()
{
    m_options["msgcount"] = m_options["msgcount"].toInt()+1;
}


/**
 * @brief Init environnement variables
 */
void Settings::init()
{
    CRegKey regKey;
    int result;
    DWORD wordLen;
    wchar_t* value1;
    wchar_t* value2;


    // SEARCH ULTRAMON EXE
    if (m_options["umpath"].isNull())
    {
        value1 = (wchar_t*) malloc(256);
        wordLen = 256;

        result = regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{ED7FE81C-378C-411D-B5B4-509B978BA204}"), KEY_READ | KEY_WOW64_64KEY);
        if (result == ERROR_SUCCESS)
        {
            result = regKey.QueryStringValue(_T("InstallLocation"), value1, &wordLen);
            if (result == ERROR_SUCCESS)
            {
                 QString exePath = QString::fromWCharArray(value1, wordLen-1); // remove the \0 termination
                 exePath.append("UltraMonDesktop.exe");
                 m_options["umpath"] = exePath;
            }
        }
        regKey.Close();

        free(value1);
    }

    if (m_options["umpath"].isNull())
    {
        value1 = (wchar_t*) malloc(256);
        wordLen = 256;

        result = regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"), KEY_READ | KEY_WOW64_64KEY);
        if (result == ERROR_SUCCESS)
        {
            result = regKey.QueryStringValue(_T("ProgramFilesDir"), value1, &wordLen);
            if (result == ERROR_SUCCESS)
            {
                 QString exePath = QString::fromWCharArray(value1, wordLen-1); // remove the \0 termination
                 exePath.append("\\Realtime Soft\\UltraMon\\UltraMonDesktop.exe");
                 m_options["umpath"] = exePath;
            }
        }
        regKey.Close();

        free(value1);
    }

    if (m_options["umpath"].isNull() || !fileExists(m_options["umpath"].toString(), false))
    {
        m_state|= UM_NOT_INSTALLED;
    }


    // SEARCH ULTRAMON VERSION
    value1 = (wchar_t*) malloc(16);
    wordLen = 16;

    result = regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Realtime Soft\\UltraMon"), KEY_READ | KEY_WOW64_64KEY);
    if (result == ERROR_SUCCESS)
    {
        result = regKey.QueryStringValue(_T("CurrentVersion"), value1, &wordLen);
        if (result == ERROR_SUCCESS)
        {
            m_env["umversion"] = QString::fromWCharArray(value1, wordLen-1); // remove the \0 termination
        }
    }
    regKey.Close();

    free(value1);

    if (m_env["umversion"] == "unknown")
    {
        m_state|= UM_BAD_VERSION;
    }
    else if (m_env["umversion"].toString().compare(QString::fromAscii(APP_MIN_UM_VERSION)) < 0)
    {
        m_state|= UM_BAD_VERSION;
    }


    // SEARCH BMP PATH
    value2 = (wchar_t*) malloc(256);

    result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &value2);
    if (result == S_OK)
    {
        QString bmpPath = QString::fromWCharArray(value2);
        bmpPath.append("\\Realtime Soft\\UltraMon\\UltraMon Wallpaper.bmp");
        m_env["bmppath"] = bmpPath;
    }
    else
    {
        m_state|= UM_UNKNOWN_ERROR;
    }

    CoTaskMemFree(value2);


    // SEARCH WALLPAPER FOLDER
    if (!(m_state & UM_BAD_VERSION))
    {
        value2 = (wchar_t*) malloc(256);

        int iResult = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &value2);
        if (iResult == S_OK)
        {
            QString wallPath = QString::fromWCharArray(value2);
            wallPath.append("\\Realtime Soft\\UltraMon\\" + m_env["umversion"].toString() + "\\Wallpapers\\");
            m_env["wallpath"] = wallPath;
        }
        else
        {
            m_state|= UM_UNKNOWN_ERROR;
        }

        CoTaskMemFree(value2);
    }

    // CHECK WALLPAPER FILE
    if (m_env["wallpath"].isNull())
    {
        m_state|= UM_FILE_NOT_FOUND;
    }
    else
    {
        QString filename = m_env["wallpath"].toString().append("default.wallpaper");

        if (!fileExists(filename))
        {
            m_state|= UM_FILE_NOT_FOUND;
        }
        else
        {
            readNbMonitors();
        }
    }


    // SEARCH SHORTCUT FILE
    value2 = (wchar_t*) malloc(256);

    result = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &value2);
    if (result == S_OK)
    {
        QString startLinkPath = QString::fromWCharArray(value2);
        startLinkPath.append("\\UMWP Autochanger.lnk");
        m_env["startlinkpath"] = startLinkPath;
    }

    CoTaskMemFree(value2);
}

/**
 * @brief Read the number of monitors from .wallpaper file
 */
void Settings::readNbMonitors()
{
    QString filename = m_env["wallpath"].toString().append("default.wallpaper");
    std::ifstream ifs(filename.toStdString(), std::ios::in | std::ios::binary);

    ifs.ignore(7); // "UMWP",  version, activedesktop

    int nbMonitors;
    ifs.read((char*)&nbMonitors, sizeof(DWORD)); // number of monitors

    m_env["nb_monitors"] = nbMonitors;
    m_env["header_size"] = 7 + sizeof(DWORD) + nbMonitors*sizeof(RECT);

    ifs.close();
}


/**
 * @brief Load contents of the settings file
 */
bool Settings::load(QString _filename)
{
    if (_filename.isEmpty())
    {
        _filename = QString::fromAscii(APP_CONFIG_FILE);
    }

    // open xml file
    QFile file(_filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    // initialize domdocument
    QDomDocument dom;
    if (!dom.setContent(file.readAll()))
    {
        return false;
    }

    file.close();

    // read settings node
    QDomElement settingsNode = dom.documentElement().firstChild().toElement();

    clearSets();

    int newHotkey = 0;

    while (!settingsNode.isNull())
    {
        // config node
        if (settingsNode.tagName() == "config")
        {
            QDomElement configNode = settingsNode.firstChild().toElement();

            while (!configNode.isNull())
            {
                // migration from 1.3
                if (configNode.tagName() == "hotkey")
                {
                    newHotkey = configNode.text().toInt();
                }
                else
                {
                    m_options[ configNode.tagName() ] = configNode.text();
                }

                configNode = configNode.nextSibling().toElement();
            }
        }
        else if (settingsNode.tagName() == "hotkeys")
        {
            QDomElement hotkeyNode = settingsNode.firstChild().toElement();

            while (!hotkeyNode.isNull())
            {
                // 1.3 format
                if (hotkeyNode.hasAttribute("key"))
                {
                    m_hotkeys[ hotkeyNode.tagName() ] =
                                hotkeyNode.attribute("key").toInt() +
                                hotkeyNode.attribute("mod").toInt();
                }
                // 1.4 format
                else
                {
                    m_hotkeys[ hotkeyNode.tagName() ] = hotkeyNode.text().toInt();
                }

                hotkeyNode = hotkeyNode.nextSibling().toElement();
            }
        }
        // sets node
        else if (settingsNode.tagName() == "sets")
        {
            QDomElement setNode = settingsNode.firstChild().toElement();

            while (!setNode.isNull())
            {
                // set node
                if (setNode.tagName() == "set")
                {
                    QString path = setNode.text().trimmed();
                    if (directoryExists(path))
                    {
                        Set* pNewSet = addSet(path, setNode.attribute("name"));
                        pNewSet->setActive(setNode.attribute("active").toInt());

                        // added in 1.3
                        if (setNode.hasAttribute("type"))
                        {
                            UM::WALLPAPER wp_type = static_cast<UM::WALLPAPER>(setNode.attribute("type").toInt());
                            UM::IMAGE im_style = static_cast<UM::IMAGE>(setNode.attribute("style").toInt());
                            pNewSet->setType(wp_type);
                            pNewSet->setStyle(im_style);
                        }

                        // added in 1.4
                        if (setNode.hasAttribute("hotkey_mod"))
                        {
                            pNewSet->setHotkey(
                                        setNode.attribute("hotkey").toInt() +
                                        setNode.attribute("hotkey_mod").toInt()
                                        );
                        }
                        else if (setNode.hasAttribute("hotkey"))
                        {
                            pNewSet->setHotkey(setNode.attribute("hotkey").toInt());
                        }
                    }
                }

                setNode = setNode.nextSibling().toElement();
            }
        }

        settingsNode = settingsNode.nextSibling().toElement();
    }

    // migration from 1.3
    if (newHotkey > 0)
    {
        upgradeHotkeys(newHotkey);
        save();
    }

    return true;
}

/**
 * @brief Write parameters into the settings file
 */
void Settings::save(QString _filename)
{
    if (_filename.isEmpty())
    {
        _filename = QString::fromAscii(APP_CONFIG_FILE);
    }

    // initialize domdocument
    QDomDocument dom;
    QDomProcessingInstruction header = dom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    dom.appendChild(header);

    // settings node
    QDomElement mainNode = dom.createElement("settings");

    // config node
    QDomElement configNode = dom.createElement("config");

    for (QHash<QString, QVariant>::iterator it = m_options.begin(); it != m_options.end(); ++it)
    {
        addSimpleTextNode(&dom, &configNode, it.key(), it.value().toString());
    }

    mainNode.appendChild(configNode);

    // hotkeys node
    QDomElement hotkeysNode = dom.createElement("hotkeys");

    for (QHash<QString, int>::iterator it = m_hotkeys.begin(); it != m_hotkeys.end(); ++it)
    {
        addSimpleTextNode(&dom, &hotkeysNode, it.key(), QString::number(it.value()));
    }

    mainNode.appendChild(hotkeysNode);

    // sets node
    QDomElement setsNode = dom.createElement("sets");

    for (QVector<Set*>::iterator it = m_sets.begin(); it != m_sets.end(); ++it)
    {
        Set* pSet = *it;

        // set node
        QDomElement setNode = dom.createElement("set");
        setNode.setAttribute("name", pSet->name());
        setNode.setAttribute("type", pSet->type());
        setNode.setAttribute("style", pSet->style());
        setNode.setAttribute("active", pSet->isActive());
        setNode.setAttribute("hotkey", pSet->hotkey());
        setDomNodeValue(&dom, &setNode, pSet->path());

        setsNode.appendChild(setNode);
    }

    mainNode.appendChild(setsNode);

    dom.appendChild(mainNode);

    // save file
    QFile file(_filename);
    file.open(QIODevice::WriteOnly);

    QTextStream stream(&file);
    dom.save(stream, 2);

    file.close();
}


/**
 * @brief Change the path of UltraMonDesktop.exe
 * @param string _sPath
 * @return bool - true if the path is valid
 */
bool Settings::setExePath(const QString &_path)
{
    if (fileExists(_path))
    {
        QString filename = _path.section('\\', -1);
        if (filename.compare("UltraMonDesktop.exe")==0)
        {
            setParam("umpath", _path);
            save();

            m_state&= ~UM_NOT_INSTALLED;
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
Set* Settings::addSet(const QString &_path)
{
    return addSet(_path, getDirectoryName(_path));
}

/**
 * @brief Add a new set
 * @param string _sPath
 * @param string _sName
 * @param bool _bActive
 * @return Set*
 */
Set* Settings::addSet(const QString &_path, QString &_name)
{
    QString path = addTrailingSlash(_path);

    if (!directoryExists(path))
    {
        return NULL;
    }

    Set* pSet = new Set(path, _name);
    m_sets.push_back(pSet);
    save();

    return pSet;
}

/**
 * @brief Delete sets
 * @param int[] _ai
 */
void Settings::deleteSets(const QList<int> _aSets)
{
    int offset = 0;
    for (QList<int>::const_iterator i=_aSets.begin(); i!=_aSets.end(); i++)
    {
        m_sets.at(*i-offset)->deleteCache();
        delete m_sets.at(*i-offset);
        m_sets.remove(*i-offset);
        offset++;
    }

    save();
}

/**
 * @brief Unload all sets
 * Sets are only unloaded from memory, they are not deleted
 */
void Settings::clearSets()
{
    if (!m_sets.empty())
    {
        qDeleteAll(m_sets);
        m_sets.clear();
    }
}

/**
 * @brief Activate sets
 * @param int[] _ai
 */
void Settings::activateSets(const QList<int> _aSets)
{
    for (QList<int>::const_iterator i=_aSets.begin(); i!=_aSets.end(); i++)
    {
        m_sets.at(*i)->setActive(true);
    }

    save();
}

/**
 * @brief Unactivate sets
 * @param int[] _ai
 */
void Settings::unactivateSets(const QList<int> _aSets)
{
    for (QList<int>::const_iterator i=_aSets.begin(); i!=_aSets.end(); i++)
    {
        m_sets.at(*i)->setActive(false);
    }

    save();
}

/**
 * @brief Activate only some sets
 * @param int[] _ai
 */
void Settings::setActiveSets(const QList<int> _aSets)
{
    for (int i=0, l=nbSets(); i<l; i++)
    {
        m_sets.at(i)->setActive(_aSets.contains(i));
    }

    save();
}

/**
 * @brief Edit a set
 * @param int _i - position in vecto
 * @param stirng _sName
 * @param int _iType
 * @param int _iStyle
 */
void Settings::editSet(int _i, const QString &_name, const UM::WALLPAPER _type, const UM::IMAGE _style, const int _hotkey)
{
    if ( _i < m_sets.size())
    {
        Set* pSet = m_sets.at(_i);
        pSet->setName(_name);
        pSet->setType(_type);
        pSet->setStyle(_style);
        pSet->setHotkey(_hotkey);
    }
}

/**
 * @brief Move a set
 * @param int _from - positon in vector
 * @param int _to - new position in vector
 */
void Settings::moveSet(int _from, int _to)
{
    Set* pSet = m_sets.at(_from);
    m_sets.insert(_to, pSet);

    if (_from<_to)
    {
        m_sets.remove(_from);
    }
    else
    {
        m_sets.remove(_from+1);
    }
}

/**
 * @brief Remove unexisting sets and update file lists
 */
void Settings::updateSets()
{
    for (QVector<Set*>::iterator it=m_sets.begin(); it!=m_sets.end(); )
    {
        if (!directoryExists((*it)->path()))
        {
            m_sets.erase(it);
        }
        else
        {
            (*it)->populateFiles();
            it++;
        }
    }
}

/**
 * @brief Get one of the active sets
 * @param int _i - position in the sub-vector of active sets
 * @return Set*
 */
Set* Settings::getActiveSet(int _i) const
{
    QVector<Set*> apActiveSets;
    for (QVector<Set*>::const_iterator it=m_sets.begin(); it!=m_sets.end(); ++it)
    {
        if ((*it)->isActive())
        {
            apActiveSets.push_back((*it));
        }
    }
    return apActiveSets.at(_i);
}

/**
 * @brief Get the number of active sets
 * @param bool _bWithFiles - if true, only non-empty sets are counted
 * @return  int
 */
int const Settings::nbActiveSets(bool _withFiles) const
{
    int totalSets = 0;
    for (QVector<Set*>::const_iterator it=m_sets.begin(); it!=m_sets.end(); ++it)
    {
        if ((*it)->isActive() && (!_withFiles || (*it)->count()>0))
        {
            totalSets++;
        }
    }
    return totalSets;
}

/**
 * @brief Create the startup shortcut
 */
void Settings::createShortcut()
{
    if (!m_env["startlinkpath"].isNull())
    {
        wchar_t* path1 = (wchar_t*) malloc(256);
        wchar_t* path2 = (wchar_t*) malloc(256);

        GetModuleFileName(NULL, path1, 256);
        wcscpy(path2, path1);
        PathRemoveFileSpec(path2);

        CreateShortcut(path1, path2, (LPCWSTR)m_env["startlinkpath"].toString().utf16());

        free(path1);
        free(path2);
    }
}

/**
 * @brief Delete the startup shortcut
 */
void Settings::deleteShortcut()
{
    if (isAutostart())
    {
        QFile::remove(m_env["startlinkpath"].toString());
    }
}

/**
 * @brief Build sets hotkeys when migrating from version 1.3
 */
void Settings::upgradeHotkeys(int WinMod)
{
    int QtMod = 0;
    if (WinMod & MOD_SHIFT)
    {
        QtMod+= Qt::SHIFT;
    }
    if (WinMod & MOD_CONTROL)
    {
        QtMod+= Qt::CTRL;
    }
    if (WinMod & MOD_ALT)
    {
        QtMod+= Qt::ALT;
    }
    if (WinMod & MOD_WIN)
    {
        QtMod+= Qt::META;
    }

    int key = Qt::Key_1;
    for (QVector<Set*>::iterator it=m_sets.begin(); it!=m_sets.end(); ++it)
    {
        (*it)->setHotkey(key + QtMod);
        key++;
        if (key > Qt::Key_9)
        {
            break;
        }
    }

    m_options["use_hotkeys"] = true;
}
