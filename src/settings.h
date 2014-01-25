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

    short           m_state;   // state of the installation
    bool            m_unsaved; // true if there are unsaved data

    QVector<Set*>   m_apSets;    // list of wallpaper sets

public:
    Settings();
    ~Settings();

    // main methods
    void init();
    void readNbMonitors();

    void writeXML(QString _filename = "");
    bool readXML(QString _filename = "");

    void updateSets();
    void upgradeHotkeys(int WinMod);

    // getters
    const short     state() const     { return m_state; }
    const bool      isUnsaved() const { return m_unsaved; }

    const QString   sParam(const QString &_key) const;
    const bool      bParam(const QString &_key) const;
    const int       iParam(const QString &_key) const;

    const QString   sEnv(const QString &_key) const;
    const bool      bEnv(const QString &_key) const;
    const int       iEnv(const QString &_key) const;

    const int       hotkey(const QString &_key) const;

    Set*            pGetSet(int _i) const  { return m_apSets.at(_i); }
    Set*            pGetActiveSet(int _i) const;
    const int       nbSets() const         { return m_apSets.size(); }
    const int       nbActiveSets(bool _withFiles=false) const;

    const QSize     windowSize() const;

    // setters
    void setParam(const QString &_key, const QVariant &_val);
    void setHotkey(const QString &_key, const int &_val);

    void setWindowSize(const QSize &_size);
    void addMsgCount();
    bool setExePath(const QString &_sPath);

    // sets management
    Set* pAddSet(const QString &_path);
    Set* pAddSet(const QString &_path, QString &_name);

    void deleteSet(int _i);
    void clearSets();

    void editSet(int _i, const QString &_name, const UM::WALLPAPER _type, const UM::IMAGE _style, const int _hotkey);
    void setState(int _i, bool _state);

    void moveSet(int _from, int _to);

    // shortcut
    const bool isAutostart() const;
    const bool canAddShortcut() const;
    void createShortcut();
    void deleteShortcut();
};

#endif // SETTINGS_H
