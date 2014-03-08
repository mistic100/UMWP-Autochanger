#include <Shlobj.h>
#include <Knownfolders.h>
#include <atlbase.h>
#include <iostream>
#include <fstream>
#include <direct.h>

#include "settings.h"
#include "createshortcut.h"

extern short UMWP_STATE;


/**
 * @brief Settings::Settings
 */
Settings::Settings()
{
    // default configuration
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
    // m_options["hotkey"] PROHIBITED

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

    if (qxtLog->isLogLevelEnabled("debug", QxtLogger::DebugLevel))
    {
        qxtLog->debug(hashToList(m_options));
        qxtLog->debug(hashToList(m_hotkeys));
        qxtLog->debug(hashToList(m_env));
        qxtLog->debug("App state: "+ QString::number(UMWP_STATE));
        qxtLog->debug("Autostart: "+ QString::number(isAutostart()));

        QList<QVariant> sets;
        sets.push_back("Sets state:");
        for (QVector<Set*>::iterator it = m_sets.begin(); it != m_sets.end(); ++it)
        {
            Set* pSet = *it;
            sets.push_back(pSet->name()+", "+(pSet->isActive()?"active":"inactive"));
        }
        qxtLog->debug(sets);
    }
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
    return QFile::exists(m_env["startlinkpath"].toString());
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

                 qxtLog->info("UM path found in registry: "+ m_options["umpath"].toString());
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

                 qxtLog->info("UM path guessed: "+ m_options["umpath"].toString());
            }
        }
        regKey.Close();

        free(value1);
    }

    if (m_options["umpath"].isNull() || !QFile::exists(m_options["umpath"].toString()))
    {
        qxtLog->error("UM path empty or invalid");

        UMWP_STATE|= UMWP::NOT_INSTALLED;
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

    if (m_env["umversion"] == QVariant("unknown") ||
        m_env["umversion"].toString().compare(QString::fromAscii(APP_MIN_UM_VERSION)) < 0)
    {
        qxtLog->error("UM version unknown or invalid");

        UMWP_STATE|= UMWP::BAD_VERSION;
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
        qxtLog->fatal("Something went really wrong ! (unable to query registry)");

        UMWP_STATE|= UMWP::UNKNOWN_ERROR;
    }

    CoTaskMemFree(value2);


    // SEARCH WALLPAPER FOLDER
    if (!(UMWP_STATE & UMWP::BAD_VERSION))
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
            qxtLog->fatal("Something went really wrong ! (unable to query registry)");

            UMWP_STATE|= UMWP::UNKNOWN_ERROR;
        }

        CoTaskMemFree(value2);

        // CHECK WALLPAPER FILE
        if (!m_env["wallpath"].isNull())
        {
            QString filename = m_env["wallpath"].toString().append("default.wallpaper");

            if (!QFile::exists(filename))
            {
                qxtLog->error("Wallpaper folder invalid");

                UMWP_STATE|= UMWP::FILE_NOT_FOUND;
            }
            else
            {
                readNbMonitors();
            }
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
    else
    {
        qxtLog->error("Shortcut path can't be guessed");
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
 * @param string _filename
 * @return bool
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
        qxtLog->error("Unable to load config file");
        return false;
    }

    // initialize domdocument
    QDomDocument dom;
    if (!dom.setContent(file.readAll()))
    {
        qxtLog->error("Unable to load config file");
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

                    if (QDir(path).exists())
                    {
                        Set* set = new Set(path, setNode.attribute("name"));
                        set->setActive(setNode.attribute("active").toInt());

                        // added in 1.3
                        if (setNode.hasAttribute("type"))
                        {
                            UM::WALLPAPER wp_type = static_cast<UM::WALLPAPER>(setNode.attribute("type").toInt());
                            UM::IMAGE im_style = static_cast<UM::IMAGE>(setNode.attribute("style").toInt());
                            set->setType(wp_type);
                            set->setStyle(im_style);
                        }

                        // added in 1.4
                        if (setNode.hasAttribute("hotkey_mod"))
                        {
                            set->setHotkey(
                                        setNode.attribute("hotkey").toInt() +
                                        setNode.attribute("hotkey_mod").toInt()
                                        );
                        }
                        else if (setNode.hasAttribute("hotkey"))
                        {
                            set->setHotkey(setNode.attribute("hotkey").toInt());
                        }

                        m_sets.push_back(set);
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
        qxtLog->info("Need to update hotkeys");
        upgradeHotkeys(newHotkey);
        save();
    }

    qxtLog->trace("Config loaded from file");

    return true;
}

/**
 * @brief Write parameters into the settings file
 * @param string _filename
 * @return bool
 */
bool Settings::save(QString _filename)
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
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        dom.save(stream, 2);

        file.close();

        qxtLog->trace("Config file saved");

        return true;
    }

    qxtLog->error("Unable to save config file");

    return false;
}


/**
 * @brief Change the path of UltraMonDesktop.exe
 * @param string _path
 * @return bool - true if the path is valid
 */
bool Settings::setExePath(const QString &_path)
{
    if (QFile::exists(_path))
    {
        QString filename = _path.section('\\', -1);
        if (filename.compare("UltraMonDesktop.exe")==0)
        {
            setParam("umpath", _path);
            save();

            UMWP_STATE&= ~UMWP::NOT_INSTALLED;
            return true;
        }
    }

    return false;
}


/**
 * @brief Add a new set from path
 * @param string _path
 * @return Set*
 */
Set* Settings::addSet(const QString &_path)
{
    return addSet(_path, "");
}

/**
 * @brief Add a new set
 * @param string _path
 * @param string _name
 * @return Set*
 */
Set* Settings::addSet(const QString &_path, const QString &_name)
{
    qxtLog->debug("New set: "+_path);

    QDir dir = QDir(_path);
    if (!dir.exists())
    {
        qxtLog->error("Invalid set path");
        return NULL;
    }

    Set* set = new Set(_path, _name);
    if (_name.isEmpty())
    {
        set->setName(dir.dirName());
    }

    m_sets.push_back(set);
    save();

    return set;
}

/**
 * @brief Delete sets
 * @param int[] _sets
 */
void Settings::deleteSets(const QList<int> _sets)
{
    int offset = 0;
    for (QList<int>::const_iterator i=_sets.begin(); i!=_sets.end(); i++)
    {
        int pos = *i-offset;

        qxtLog->debug("Delete set: "+m_sets.at(pos)->name());

        m_sets.at(pos)->deleteCache();
        delete m_sets.at(pos);
        m_sets.remove(pos);

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
 * @param int[] _sets
 */
void Settings::activateSets(const QList<int> _sets)
{
    for (QList<int>::const_iterator i=_sets.begin(); i!=_sets.end(); i++)
    {
        m_sets.at(*i)->setActive(true);

        qxtLog->debug("Activate set: "+m_sets.at(*i)->name());
    }

    save();
}

/**
 * @brief Unactivate sets
 * @param int[] _sets
 */
void Settings::unactivateSets(const QList<int> _sets)
{
    for (QList<int>::const_iterator i=_sets.begin(); i!=_sets.end(); i++)
    {
        m_sets.at(*i)->setActive(false);

        qxtLog->debug("Unactive set: "+m_sets.at(*i)->name());
    }

    save();
}

/**
 * @brief Activate only some sets
 * @param int[] _sets
 */
void Settings::setActiveSets(const QList<int> _sets)
{
    for (int i=0, l=nbSets(); i<l; i++)
    {
        m_sets.at(i)->setActive(_sets.contains(i));
    }

    if (qxtLog->isLogLevelEnabled("debug", QxtLogger::DebugLevel))
    {
        QList<QVariant> sets;
        sets.push_back("Change sets state:");
        for (QVector<Set*>::iterator it = m_sets.begin(); it != m_sets.end(); ++it)
        {
            Set* pSet = *it;
            sets.push_back(pSet->name()+", "+(pSet->isActive()?"active":"inactive"));
        }
        qxtLog->debug(sets);
    }

    save();
}

/**
 * @brief Edit a set
 * @param int _i - position in vector
 * @param string _name
 * @param UM::WALLPAPER _type
 * @param UM::IMAGE _style
 * @param int _hotkey
 */
void Settings::editSet(int _i, const QString &_name, const UM::WALLPAPER _type, const UM::IMAGE _style, const int _hotkey)
{
    if ( _i < m_sets.size())
    {
        Set* set = m_sets.at(_i);
        set->setName(_name);
        set->setType(_type);
        set->setStyle(_style);
        set->setHotkey(_hotkey);

        qxtLog->debug("Edit set: "+set->name());

        save();
    }
}

/**
 * @brief Move a set
 * @param int _from - positon in vector
 * @param int _to - new position in vector
 */
void Settings::moveSet(int _from, int _to)
{
    Set* set = m_sets.at(_from);
    m_sets.insert(_to, set);

    if (_from<_to)
    {
        m_sets.remove(_from);
    }
    else
    {
        m_sets.remove(_from+1);
    }

    qxtLog->debug("Move set: "+set->name()+", "+_from+"->"+_to);

    save();
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
 * @param bool _withFiles - if true, only non-empty sets are counted
 * @return int
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
    if (canAddShortcut())
    {
        qxtLog->trace("Attempt to create shortcut");

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
        qxtLog->trace("Remove shortcut");

        QFile::remove(m_env["startlinkpath"].toString());
    }
}

/**
 * @brief Build sets hotkeys when migrating from version 1.3
 * @param int WinMod
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
