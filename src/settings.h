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
    QHash<QString, QVariant> m_env;     // environnement variables
    QHash<QString, int>      m_hotkeys;

    QVector<Set*>   m_sets; // list of wallpaper sets

public:
    Settings();
    ~Settings();

    // main methods
    void init();
    void readNbMonitors();

    bool save(QString _filename = "");
    bool load(QString _filename = "");

    void updateSets();
    void upgradeHotkeys(int WinMod);

    // getters
    const QString sParam(const QString &_key) const;
    const bool    bParam(const QString &_key) const;
    const int     iParam(const QString &_key) const;

    const QString sEnv(const QString &_key) const;
    const bool    bEnv(const QString &_key) const;
    const int     iEnv(const QString &_key) const;

    const int     hotkey(const QString &_key) const;

    Set*          getSet(int _i) const { return m_sets.at(_i); }
    Set*          getActiveSet(int _i) const;
    const int     nbSets() const       { return m_sets.size(); }
    const int     nbActiveSets(bool _withFiles=false) const;

    const QSize   windowSize() const;

    // setters
    void setParam(const QString &_key, const QVariant &_val);
    void setHotkey(const QString &_key, const int &_val);

    void setWindowSize(const QSize &_size);
    void addMsgCount();
    bool setExePath(const QString &_sPath);

    // sets management
    Set* addSet(const QString &_path);
    Set* addSet(const QString &_path, const QString &_name);

    void deleteSets(const QList<int> _sets);
    void clearSets();

    void activateSets(const QList<int> _sets);
    void unactivateSets(const QList<int> _sets);
    void setActiveSets(const QList<int> _sets);
    void editSet(int _i, const QString &_name, const UM::WALLPAPER _type, const UM::IMAGE _style, const int _hotkey);

    void moveSet(int _from, int _to);

    // shortcut
    const bool isAutostart() const;
    const bool canAddShortcut() const;
    void createShortcut();
    void deleteShortcut();
};

#endif // SETTINGS_H
