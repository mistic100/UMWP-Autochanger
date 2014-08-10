#include "settings.h"

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

    m_hotkeys["refresh"] = 0;
    m_hotkeys["startpause"] = 0;
    m_hotkeys["showhide"] = 0;
}

/**
 * @brief Settings::~Settings
 */
Settings::~Settings()
{
    clearSets(); // clear memory but do not save changes !
}


/**
 * @brief Get saved window size
 */
const QSize Settings::windowSize() const
{
    return QSize(m_options["window_width"].toInt(), m_options["window_height"].toInt());
}

/**
 * @brief Save new window size
 */
void Settings::setWindowSize(const QSize &_size, bool _save)
{
    m_options["window_width"] = _size.width();
    m_options["window_height"] = _size.height();

    if (_save)
    {
        save();
    }
}

/**
 * @brief Increment tray tooltip count
 */
void Settings::incrementMsgCount()
{
    m_options["msgcount"] = m_options["msgcount"].toInt()+1;
}

/**
 * @brief Dump whole config in the log
 */
void Settings::log()
{
    QList<QVariant> options;
    options.append("== OPTIONS");
    for (QHash<QString, QVariant>::const_iterator it=m_options.begin(); it!=m_options.end(); ++it)
    {
        options.append(it.key() +": "+ it.value().toString());
    }

    QList<QVariant> hotkeys;
    hotkeys.append("== HOTKEYS");
    for (QHash<QString, int>::const_iterator it=m_hotkeys.begin(); it!=m_hotkeys.end(); ++it)
    {
        hotkeys.append(it.key() +": "+ QString::number(it.value()));
    }

    QList<QVariant> sets;
    sets.append("== SETS");
    for (QVector<Set*>::const_iterator it = m_sets.constBegin(); it != m_sets.constEnd(); ++it)
    {
        sets.append((*it)->name()+": "+((*it)->isActive()?"active":"inactive"));
    }

    qxtLog->debug(options);
    qxtLog->debug(hotkeys);
    qxtLog->debug(sets);
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

    if (!file.exists() || !file.open(QIODevice::ReadOnly))
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

    clearSets();

    // read settings node
    QDomElement settingsNode = dom.documentElement().firstChild().toElement();

    int newHotkey = 0;
    bool updated = false;

    while (!settingsNode.isNull())
    {
        // config node
        if (settingsNode.tagName() == "config")
        {
            QDomElement configNode = settingsNode.firstChild().toElement();

            while (!configNode.isNull())
            {
                // migration from 1.2
                if (configNode.tagName() == "hotkey")
                {
                    newHotkey = configNode.text().toInt();
                    updated = true;
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
                    updated = true;
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

                    // added in 1.3 - 1.3 format
                    if (setNode.hasAttribute("hotkey_mod"))
                    {
                        set->setHotkey(
                                    setNode.attribute("hotkey").toInt() +
                                    setNode.attribute("hotkey_mod").toInt()
                                    );
                        updated = true;
                    }
                    // 1.4 format
                    else if (setNode.hasAttribute("hotkey"))
                    {
                        set->setHotkey(setNode.attribute("hotkey").toInt());
                    }

                    m_sets.append(set);
                }

                setNode = setNode.nextSibling().toElement();
            }
        }

        settingsNode = settingsNode.nextSibling().toElement();
    }

    qxtLog->trace("Config loaded from file");

    if (newHotkey > 0)
    {
        qxtLog->info("Need to update hotkeys");
        upgradeHotkeys(newHotkey);
    }

    if (updated) {
        qxtLog->info("Settings file format changed");
        save();
    }

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

    for (QHash<QString, QVariant>::const_iterator it = m_options.constBegin(); it != m_options.constEnd(); ++it)
    {
        addSimpleTextNode(&dom, &configNode, it.key(), it.value().toString());
    }

    mainNode.appendChild(configNode);

    // hotkeys node
    QDomElement hotkeysNode = dom.createElement("hotkeys");

    for (QHash<QString, int>::const_iterator it = m_hotkeys.constBegin(); it != m_hotkeys.constEnd(); ++it)
    {
        addSimpleTextNode(&dom, &hotkeysNode, it.key(), QString::number(it.value()));
    }

    mainNode.appendChild(hotkeysNode);

    // sets node
    QDomElement setsNode = dom.createElement("sets");

    for (QVector<Set*>::const_iterator it = m_sets.constBegin(); it != m_sets.constEnd(); ++it)
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
            setOpt("umpath", _path);
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

    m_sets.append(set);
    save();

    return set;
}

/**
 * @brief Delete sets
 * @param int[] _sets
 */
void Settings::deleteSets(const QList<int> &_sets)
{
    int offset = 0;
    for (QList<int>::const_iterator i=_sets.constBegin(); i!=_sets.constEnd(); i++)
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
void Settings::activateSets(const QList<int> &_sets)
{
    for (QList<int>::const_iterator i=_sets.constBegin(); i!=_sets.constEnd(); i++)
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
void Settings::unactivateSets(const QList<int> &_sets)
{
    for (QList<int>::const_iterator i=_sets.constBegin(); i!=_sets.constEnd(); i++)
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
void Settings::setActiveSets(const QList<int> &_sets)
{
    for (int i=0, l=nbSets(); i<l; i++)
    {
        m_sets.at(i)->setActive(_sets.contains(i));
    }

    if (qxtLog->isLogLevelEnabled("debug", QxtLogger::DebugLevel))
    {
        QList<QVariant> sets;
        sets.append("Change sets state:");
        for (QVector<Set*>::const_iterator it = m_sets.constBegin(); it != m_sets.constEnd(); ++it)
        {
            sets.append((*it)->name()+", "+((*it)->isActive()?"active":"inactive"));
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
    if (_i < m_sets.size())
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
    for (QVector<Set*>::iterator it=m_sets.begin(); it!=m_sets.end(); ++it)
    {
        Set* set = *it;
        if (set->check())
        {
            set->populateFiles();
        }
    }
}

/**
 * @brief Get one of the active sets
 * @param int _i - position in the sub-vector of active sets
 * @return Set*
 */
Set* Settings::activeSet(int _i, bool _withFiles) const
{
    QVector<Set*> activeSets;

    for (QVector<Set*>::const_iterator it=m_sets.constBegin(); it!=m_sets.constEnd(); ++it)
    {
        Set* set = *it;
        if (set->isActive() && set->isValid() && (!_withFiles || set->count()>0))
        {
            activeSets.append(set);
        }
    }

    return activeSets.at(_i);
}

/**
 * @brief Get the number of active sets
 * @param bool _withFiles - if true, only non-empty sets are counted
 * @return int
 */
int const Settings::nbActiveSets(bool _withFiles) const
{
    int totalSets = 0;

    for (QVector<Set*>::const_iterator it=m_sets.constBegin(); it!=m_sets.constEnd(); ++it)
    {
        Set* set = *it;
        if (set->isActive() && set->isValid() && (!_withFiles || set->count()>0))
        {
            totalSets++;
        }
    }

    return totalSets;
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
    for (QVector<Set*>::const_iterator it=m_sets.constBegin(); it!=m_sets.constEnd(); ++it)
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
