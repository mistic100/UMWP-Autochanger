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

    short           m_iState;   // state of the installation
    bool            m_bUnsaved; // true if there are unsaved data

    QVector<Set*>   m_oSets;    // list of wallpaper sets

public:
    Settings();
    ~Settings();

    // main methods
    void vInit();
    void vReadNbMonitors();
    void vWriteXML();
    void vReadXML();
    void vUpdateSets();

    // getters
    const short     &iState() const     { return m_iState; }
    const bool      &bIsUnsaved() const { return m_bUnsaved; }

    const QString   sParam(const QString &_key) const;
    const bool      bParam(const QString &_key) const;
    const int       iParam(const QString &_key) const;

    const QString   sEnv(const QString &_key) const;
    const bool      bEnv(const QString &_key) const;
    const int       iEnv(const QString &_key) const;

    Set*            poGetSet(int _i) const  { return m_oSets.at(_i); }
    Set*            poGetActiveSet(int _i) const;
    const int       iNbSets() const         { return m_oSets.size(); }
    const int       iNbActiveSets(bool _bWithFiles) const;
    const int       iNbActiveFiles();

    const bool      bIsAutostart() const;
    const QSize     oWindowSize() const;

    // setters
    void vSetParam(const QString &_key, const QVariant &_val);
    void vSetWindowSize(const QSize &_size);
    void vAddMsgCount();
    bool bSetExePath(const QString &_sPath);

    // sets management
    Set*    poAddSet(const QString &_sPath);
    Set*    poAddSet(const QString &_sPath, QString &_sName, bool _bActive);

    void    vDeleteSet(int _i);
    void    vClearSets();

    void    vEditSet(int _i, const QString &_sName, const int _iType, const int _iStyle);
    void    vSetState(int _i, bool _bState);

    void    vMoveSet(int _from, int _to);

    // shortcut
    const bool bCanAddShortcut();
    void vCreateShortcut();
    void vDeleteShortcut();
};

#endif // SETTINGS_H
