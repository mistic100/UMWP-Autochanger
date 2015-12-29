#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSize>
#include <QHash>
#include <QVariant>

#include "main.h"
#include "set.h"


/**
 * @brief Class managing settings of the application
 */
class Settings
{
private:
    QHash<QString, QVariant> m_options; // configurable options
    QHash<QString, int>      m_hotkeys;
    QVector<UM::Monitor>     m_monitors;
    QVector<Set*>            m_sets; // list of wallpaper sets

public:
    Settings();
    ~Settings();

    // main methods
    void log();

    bool save(QString _filename = "");
    bool load(QString _filename = "");

    void updateSets();

    // getters
    const QVariant  param(const QString &_key) const  { return m_options.value(_key); }
    const int       hotkey(const QString &_key) const { return m_hotkeys.value(_key); }
    const UM::Monitor monitor(const int _key) const   { return m_monitors.at(_key);   }
    const int       nbEnabledMonitors() const;
    const QSize     windowSize() const;

    Set*            set(int _i) const { return m_sets.at(_i); }
    const int       nbSets() const    { return m_sets.size(); }
    Set*            activeSet(int _i, bool _withFiles=false) const;
    const int       nbActiveSets(bool _withFiles=false) const;

    // setters
    void setParam(const QString &_key, const QVariant &_val) { m_options[_key] = _val; }
    void setHotkey(const QString &_key, const int &_val)     { m_hotkeys[_key] = _val; }
    void setMonitor(const int _key, const UM::Monitor &_val) { m_monitors[_key] = _val; }
    void setNbMonitors(const int _size) { if (_size>m_monitors.size()) m_monitors.resize(_size); }

    void setWindowSize(const QSize &_size);

    // sets management
    Set* addSet(const QString &_path);
    void deleteSets(const QList<Set*> &_sets);
    void clearSets();

    void activateSets(const QList<Set*> &_sets);
    void unactivateSets(const QList<Set*> &_sets);
    void setActiveSets(const QList<Set *> &_sets);

    void editSets(const QList<Set*> _sets, const Set &_data);
    void moveSet(int _from, int _to);

private:
    // migration methods
    void upgradeHotkeys(int WinMod);
    void upgradeMode(UM::MODE _mode);
};

#endif // SETTINGS_H
