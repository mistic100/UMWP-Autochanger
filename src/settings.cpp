#include "settings.h"
#include "ext/qhotkeywidget.h"


/**
 * @brief Settings::Settings
 */
Settings::Settings()
{
    // default configuration
    m_options["window_width"] = 440;
    m_options["window_height"] = 240;
    m_options["delay"] = 60;
    m_options["minimize"] = true;
    m_options["check_updates"] = true;
    m_options["msgcount"] = 0;
    m_options["close_warning"] = true;
    m_options["use_hotkeys"] = false;
    m_options["show_notifications"] = true;
    m_options["last_dir"] = QDir::homePath();
    m_options["language"] = QLocale::system().name();
    m_options["default_mode"] = UM::RANDOM;
    m_options["default_type"] = UM::W_MONITOR;
    m_options["default_style"] = UM::IM_STRETCH_PROP;
    m_options["changelog_shown"] = "0.0.0";

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
void Settings::setWindowSize(const QSize &_size)
{
    m_options["window_width"] = _size.width();
    m_options["window_height"] = _size.height();
}

/**
 * @brief Return the number of enabled monitors
 */
const int Settings::nbEnabledMonitors() const
{
    int n = 0;

    foreach (Monitor mon, m_monitors)
    {
        if (mon.enabled) n++;
    }

    return n;
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
    QList<QString> options;
    for (QHash<QString, QVariant>::const_iterator it=m_options.begin(); it!=m_options.end(); ++it)
    {
        options.append(it.key() +": "+ it.value().toString());
    }

    QList<QString> hotkeys;
    for (QHash<QString, int>::const_iterator it=m_hotkeys.begin(); it!=m_hotkeys.end(); ++it)
    {
        hotkeys.append(it.key() +": "+ QString::number(it.value()));
    }

    QList<QString> sets;
    for (QVector<Set*>::const_iterator it = m_sets.constBegin(); it != m_sets.constEnd(); ++it)
    {
        sets.append((*it)->name()+": "+((*it)->isActive()?"active":"inactive"));
    }

    QLOG_DEBUG() << "== OPTIONS" << options;
    QLOG_DEBUG() << "== HOTKEYS" << hotkeys;
    QLOG_DEBUG() << "== SETS" << sets;
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
        _filename = APP_CONFIG_FILE;
    }

    // open xml file
    QFile file(_filename);

    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        QLOG_ERROR() << "Unable to load config file";
        return false;
    }

    // initialize domdocument
    QDomDocument dom;
    if (!dom.setContent(file.readAll()))
    {
        QLOG_ERROR() << "Unable to load config file";
        return false;
    }

    file.close();

    clearSets();

    // read settings node
    QDomElement settingsNode = dom.documentElement().firstChild().toElement();

    int newHotkey = 0;
    UM::MODE newMode = UM::NONE;
    bool updated = false;

    while (!settingsNode.isNull())
    {
        // config node
        if (settingsNode.tagName() == "config")
        {
            QDomElement configNode = settingsNode.firstChild().toElement();

            while (!configNode.isNull())
            {
                // migration to 1.3
                if (configNode.tagName() == "hotkey")
                {
                    newHotkey = configNode.text().toInt();
                }
                // migration to 1.9
                else if (configNode.tagName() == "mode")
                {
                    newMode = configNode.text() == "random" ? UM::RANDOM : UM::SEQUENTIAL;
                    m_options["default_mode"] = newMode;
                }
                else if (m_options.contains(configNode.tagName()))
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
                    m_sets.append(new Set(&setNode));
                }

                setNode = setNode.nextSibling().toElement();
            }
        }
        // monitors node
        else if (settingsNode.tagName() == "monitors")
        {
            QDomElement monNode = settingsNode.firstChild().toElement();

            m_monitors.clear();

            while (!monNode.isNull())
            {
                Monitor mon;
                mon.enabled = (bool) monNode.attribute("enabled").toInt();
                mon.color = (COLORREF) monNode.attribute("color").toInt();

                m_monitors.append(mon);

                monNode = monNode.nextSibling().toElement();
            }
        }

        settingsNode = settingsNode.nextSibling().toElement();
    }

    QLOG_TRACE() << "Config loaded from file";

    if (newHotkey > 0)
    {
        QLOG_INFO() << "Need to update hotkeys";
        upgradeHotkeys(newHotkey);
        updated = true;
    }

    if (newMode != UM::NONE)
    {
        QLOG_INFO() << "Need to update modes";
        upgradeMode(newMode);
        updated = true;
    }

    if (updated)
    {
        QLOG_INFO() << "Settings file format changed";
        save();
    }

    if (QsLogging::Logger::instance().loggingLevel() != QsLogging::OffLevel)
    {
        log();
    }

    for (QVector<Set*>::const_iterator it = m_sets.constBegin(); it != m_sets.constEnd(); ++it)
    {
        (*it)->init();
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
        _filename = APP_CONFIG_FILE;
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
        Set* set = *it;

        // set node
        QDomElement setNode = dom.createElement("set");

        set->writeXml(&setNode, &dom);

        setsNode.appendChild(setNode);
    }

    mainNode.appendChild(setsNode);

    // monitors node
    QDomElement monsNode = dom.createElement("monitors");

    for (QVector<Monitor>::const_iterator it = m_monitors.constBegin(); it != m_monitors.constEnd(); ++it)
    {
        Monitor mon = *it;

        // monitor node
        QDomElement monNode = dom.createElement("monitor");
        monNode.setAttribute("enabled", mon.enabled);
        monNode.setAttribute("color", (int)mon.color);

        monsNode.appendChild(monNode);
    }

    mainNode.appendChild(monsNode);

    dom.appendChild(mainNode);

    // save file
    QFile file(_filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        dom.save(stream, 2);

        file.close();

        QLOG_TRACE() << "Config file saved";

        return true;
    }

    QLOG_ERROR() << "Unable to save config file";

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
    QLOG_DEBUG() << "New set: " << _path;

    QDir dir = QDir(_path);
    if (!dir.exists())
    {
        QLOG_ERROR() << "Invalid set path";
        return NULL;
    }

    Set* set = new Set(_path, _name);
    if (_name.isEmpty())
    {
        set->setName(dir.dirName());
    }

    set->setMode(static_cast<UM::MODE>(m_options["default_mode"].toInt()));
    set->setStyle(static_cast<UM::IMAGE>(m_options["default_style"].toInt()));
    set->setType(static_cast<UM::WALLPAPER>(m_options["default_type"].toInt()));

    m_sets.append(set);
    save();

    return set;
}

/**
 * @brief Delete sets
 * @param int[] _sets
 */
void Settings::deleteSets(const QList<Set*> &_sets)
{
    foreach (Set* set, _sets)
    {
        QLOG_DEBUG() << "Delete set: " << set->name();

        set->deleteCache();
        m_sets.removeAll(set);
        delete set;
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
void Settings::activateSets(const QList<Set*> &_sets)
{
    foreach(Set* set, _sets)
    {
        set->setActive(true);

        QLOG_DEBUG() << "Activate set: " << set->name();
    }

    save();
}

/**
 * @brief Unactivate sets
 * @param int[] _sets
 */
void Settings::unactivateSets(const QList<Set*> &_sets)
{
    foreach(Set* set, _sets)
    {
        set->setActive(false);

        QLOG_DEBUG() << "Unactive set: " << set->name();
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

    if (QsLogging::Logger::instance().loggingLevel() != QsLogging::OffLevel)
    {
        QList<QString> sets;
        for (QVector<Set*>::const_iterator it = m_sets.constBegin(); it != m_sets.constEnd(); ++it)
        {
            sets.append((*it)->name()+", "+((*it)->isActive()?"active":"inactive"));
        }

        QLOG_DEBUG() << "Change sets state:";
        QLOG_DEBUG() << sets;
    }

    save();
}

/**
 * @brief Edit multiple sets
 * @param Set*[] _sets
 * @param string _name - only if there is one set edited
 * @param UM::WALLPAPER _type
 * @param UM::IMAGE _style
 * @param UM::MODE _mode
 * @param int _hotkey
 * @param CustomLayout _layout
 */
void Settings::editSets(const QList<Set*> _sets, const QString &_name, const UM::WALLPAPER _type, const UM::IMAGE _style, const UM::MODE _mode, const int _hotkey, const CustomLayout &_layout)
{
    foreach (Set* set, _sets)
    {
        if (_sets.size() == 1)     set->setName(_name);
        if (_type != UM::W_NONE)   set->setType(_type);
        if (_style != UM::IM_NONE) set->setStyle(_style);
        if (_mode != UM::NONE)     set->setMode(_mode);
        if (_hotkey != QHotKeyWidget::KEEP_KEY) set->setHotkey(_hotkey);
        if (_style != UM::IM_NONE) set->setCustLayout(_layout);

        QLOG_DEBUG() << "Edit set: " << set->name();
    }

    save();
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

    QLOG_DEBUG() << "Move set: " << set->name() << ", " << _from << "->" << _to;

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

/**
 * @brief Apply global mode to each set, migration to 1.9
 * @param int _mode
 */
void Settings::upgradeMode(UM::MODE _mode)
{
    foreach (Set* set, m_sets)
    {
        set->setMode(_mode);
    }
}
