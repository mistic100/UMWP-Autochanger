#include "settings.h"
#include "ext/qhotkeywidget.h"
#include "umutils.h"

/**
 * @brief Settings::Settings
 */
Settings::Settings(Environment* _enviro) :
    m_enviro(_enviro)
{
    // default configuration
    m_options[UM::CONF::window_width] = APP_MIN_WIDTH;
    m_options[UM::CONF::window_height] = APP_MIN_HEIGHT;
    m_options[UM::CONF::delay] = 60;
    m_options[UM::CONF::minimize] = true;
    m_options[UM::CONF::check_updates] = true;
    m_options[UM::CONF::msgcount] = 0;
    m_options[UM::CONF::close_warning] = true;
    m_options[UM::CONF::use_hotkeys] = false;
    m_options[UM::CONF::show_notifications] = true;
    m_options[UM::CONF::last_dir] = QDir::homePath();
    m_options[UM::CONF::language] = QLocale::system().name();
    m_options[UM::CONF::changelog_shown] = "0.0.0";
    m_options[UM::CONF::open_program] = "";
    m_options[UM::CONF::lock_enabled] = false;
    m_options[UM::CONF::lock_password] = "";
    m_options[UM::CONF::lock_startup] = true;
    m_options[UM::CONF::lock_minimize] = true;
    m_options[UM::CONF::lock_type] = UM::LOCK_ALL;
    m_options[UM::CONF::ignore_update] = "0.0.0";
    m_options[UM::CONF::default_set] = "";

    m_hotkeys[UM::CONF::HOTKEY::refresh] = 0;
    m_hotkeys[UM::CONF::HOTKEY::startpause] = 0;
    m_hotkeys[UM::CONF::HOTKEY::showhide] = 0;
    m_hotkeys[UM::CONF::HOTKEY::delay] = 0;
    m_hotkeys[UM::CONF::HOTKEY::lockunlock] = 0;
}

/**
 * @brief Settings::~Settings
 */
Settings::~Settings()
{
    clearSets(); // clear memory but do not save changes !
}

/**
 * @brief Ensure that the settings are coherent with some environnement params
 */
void Settings::check()
{
    int size = m_enviro->nbMonitors();
    if (size > m_monitors.size())
    {
        m_monitors.resize(size);
    }

    foreach (Set* set, m_sets)
    {
        set->check();
    }
}

/**
 * @brief Get saved window size
 */
const QSize Settings::windowSize() const
{
    return QSize(
            m_options[UM::CONF::window_width].toInt(),
            m_options[UM::CONF::window_height].toInt()
            );
}

/**
 * @brief Save new window size
 */
void Settings::setWindowSize(const QSize &_size)
{
    m_options[UM::CONF::window_width] = _size.width();
    m_options[UM::CONF::window_height] = _size.height();
}

/**
 * @brief Return the number of enabled monitors
 */
const int Settings::nbEnabledMonitors() const
{
    int n = 0;

    foreach (const UM::Monitor &mon, m_monitors)
    {
        if (mon.enabled) n++;
    }

    return n;
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

    QLOG_DEBUG() << "== OPTIONS" << options;
    QLOG_DEBUG() << "== HOTKEYS" << hotkeys;
    QLOG_DEBUG() << "== SETS";

    foreach (Set* set, m_sets)
    {
        QLOG_DEBUG()<<"   "<<set->name()<<(set->isActive()?"active":"inactive")<<","<<set->nbFiles()<<"files,"<<set->nbFolders()<<"folders";
    }
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
        if (!m_enviro->isPortable() && !QFile(Environment::APPDATA_DIR + APP_CONFIG_FILE).exists() && QFile(APP_CONFIG_FILE).exists())
        {
            moveAppData();
        }

        _filename = Environment::APPDATA_DIR + APP_CONFIG_FILE;
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
    UM::MODE newMode = UM::MODE_NONE;
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
                    newMode = configNode.text() == "random" ? UM::MODE_RANDOM : UM::MODE_SEQUENTIAL;
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
                    // stored in string representation since 2.2.1
                    bool isInt;
                    m_hotkeys[ hotkeyNode.tagName() ] = hotkeyNode.text().toInt(&isInt);

                    if (!isInt)
                    {
                        m_hotkeys[ hotkeyNode.tagName() ] = UM::keySequenceToInt(QKeySequence(hotkeyNode.text()));
                    }
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
                    try {
                        m_sets.append(new Set(&setNode));
                    }
                    catch(const std::exception &e) {
                        QLOG_ERROR()<<e.what();
                    }
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
                UM::Monitor mon;
                mon.enabled = (bool) monNode.attribute("enabled").toInt();
                mon.color = QColor(monNode.attribute("color")).rgb();

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

    if (newMode != UM::MODE_NONE)
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

    foreach (Set* set, m_sets)
    {
        set->init();
    }

    if (QsLogging::Logger::instance().loggingLevel() != QsLogging::OffLevel)
    {
        log();
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
        _filename = Environment::APPDATA_DIR + APP_CONFIG_FILE;
    }

    // open file
    QFile file(_filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QLOG_ERROR() << "Unable to save config file";
        return false;
    }

    // initialize document
    QXmlStreamWriter writer(&file);
    writer.setAutoFormattingIndent(2);
    writer.setAutoFormatting(true);
    writer.setCodec("UTF-8");
    writer.writeStartDocument();

    // settings node
    writer.writeStartElement("settings");

    // config node
    writer.writeStartElement("config");

    for (QHash<QString, QVariant>::const_iterator it = m_options.constBegin(); it != m_options.constEnd(); ++it)
    {
        writer.writeTextElement(it.key(), it.value().toString());
    }

    writer.writeEndElement();

    // hotkeys node
    writer.writeStartElement("hotkeys");

    for (QHash<QString, int>::const_iterator it = m_hotkeys.constBegin(); it != m_hotkeys.constEnd(); ++it)
    {
        writer.writeTextElement(it.key(), QKeySequence(it.value()).toString());
    }

    writer.writeEndElement();

    // sets node
    writer.writeStartElement("sets");

    foreach (const Set* set, m_sets)
    {
        set->writeXml(&writer);
    }

    writer.writeEndElement();

    // monitors node
    writer.writeStartElement("monitors");

    foreach (const UM::Monitor &mon, m_monitors)
    {
        writer.writeStartElement("monitor");
        writer.writeAttribute("enabled", QString::number(mon.enabled));
        writer.writeAttribute("color", QColor(mon.color).name());
        writer.writeEndElement();
    }

    writer.writeEndElement();

    // settings node
    writer.writeEndElement();

    writer.writeEndDocument();

    // close file
    file.close();

    QLOG_TRACE() << "Config file saved";

    return true;
}

/**
 * @brief Add a new set from path
 * @param string _path
 * @return Set*
 */
Set* Settings::addSet(const QString &_path)
{
    QLOG_DEBUG() << "New set: " << _path;

    QDir dir = QDir(_path);
    if (!dir.exists())
    {
        QLOG_ERROR() << "Invalid set path";
        return NULL;
    }

    Set* set = new Set(_path, dir.dirName());
    set->init();

    set->setMode(UM::MODE_RANDOM);
    set->setStyle(UM::IM_STRETCH_PROP);
    set->setType(UM::W_MONITOR);

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
void Settings::setActiveSets(const QList<Set*> &_sets)
{
    foreach(Set* set, m_sets)
    {
        set->setActive(false);
    }

    foreach(Set* set, _sets)
    {
        set->setActive(true);
    }

    if (QsLogging::Logger::instance().loggingLevel() != QsLogging::OffLevel)
    {
        QList<QString> sets;
        foreach (const Set* set, m_sets)
        {
            sets.append(set->name()+", "+(set->isActive()?"active":"inactive"));
        }

        QLOG_DEBUG() << "Change sets state:";
        QLOG_DEBUG() << sets;
    }

    save();
}

/**
 * @brief Edit multiple sets
 * @param Set*[] _sets
 * @param Set _data
 */
void Settings::editSets(const QList<Set*> _sets, const Set &_data)
{
    foreach (Set* set, _sets)
    {
        if (_sets.size() == 1)                          set->setName(_data.name());
        if (_data.type() != UM::W_NONE)                 set->setType(_data.type());
        if (_data.style() != UM::IM_NONE)               set->setStyle(_data.style());
        if (_data.mode() != UM::MODE_NONE)              set->setMode(_data.mode());
        if (_data.hotkey() != QHotKeyWidget::KEEP_KEY)  set->setHotkey(_data.hotkey());
        if (_data.frequency() != 0)                     set->setFrequency(_data.frequency());
        if (_data.lock() != UNKNOW_BOOL)                set->setLock(_data.lock());
        if (_data.style() == UM::IM_CUSTOM)             set->setCustLayout(_data.custLayout());
        else if (_data.style() != UM::IM_NONE)          set->setCustLayout(CustomLayout());
        if (!_data.monitors().contains(-1))             set->setMonitors(_data.monitors());

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
 * @brief Returns a set by it's uuid
 * @param string _uuid
 * @return Set*
 */
Set* Settings::setByUuid(const QString &_uuid) const
{
    foreach (Set* set, m_sets)
    {
        if (set->uuid() == _uuid)
        {
            return set;
        }
    }

    return NULL;
}

/**
 * @brief Get one of the active sets
 * @param int _i - position in the sub-vector of active sets
 * @return Set*
 */
Set* Settings::activeSet(int _i) const
{
    QVector<Set*> activeSets;

    foreach (Set* set, m_sets)
    {
        if (set->isValid() && set->isActive())
        {
            activeSets.append(set);
        }
    }

    return activeSets.at(_i);
}

/**
 * @brief Get the number of active sets
 * @return int
 */
int const Settings::nbActiveSets() const
{
    int totalSets = 0;

    foreach (const Set* set, m_sets)
    {
        if (set->isValid() && set->isActive())
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
    foreach (Set* set, m_sets)
    {
        set->setHotkey(key + QtMod);

        key++;
        if (key > Qt::Key_9) break;
    }

    m_options[UM::CONF::use_hotkeys] = true;
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

/**
 * @brief Move app data to Local/Data if installed version, migration to 2.1
 */
void Settings::moveAppData()
{
    QFile::copy(APP_CONFIG_FILE, Environment::APPDATA_DIR + APP_CONFIG_FILE);
    QDir(APP_CACHE_DIR).removeRecursively();
}
