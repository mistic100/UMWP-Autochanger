#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSize>
#include <QHash>
#include <QVariant>

#include "main.h"
#include "set.h"


/*
 * Name: Settings
 * Description: Class managing settings of the application
 */
class Settings
{
private:
    QHash<QString, QVariant> m_options; // configurable options
    QHash<QString, QVariant> m_env;     // environnement variables

    short           m_iState;   // state if the installation
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
    short const     &iState() const         { return m_iState; }
    bool const      &bIsUnsaved() const     { return m_bUnsaved; }

    QString const   sParam(QString const _key) const;
    bool const      bParam(QString const _key) const;
    int const       iParam(QString const _key) const;


    QString const   sEnv(QString const _key) const;
    bool const      bEnv(QString const _key) const;
    int const       iEnv(QString const _key) const;

    Set*            poGetSet(int _i) const  { return m_oSets.at(_i); }
    Set*            poGetActiveSet(int _i) const;
    int const       iNbSets() const         { return m_oSets.size(); }
    int const       iNbActiveSets(bool _bWithFiles) const;
    int const       iNbFiles();

    bool const      bIsAutostart() const;
    QSize const     oWindowSize() const;

    // setters
    void vSetParam(QString const _key, QVariant _val);
    void vSetWindowSize(QSize const &_size);
    void vAddMsgCount();
    bool bSetExePath(QString const &_sPath);

    // sets management
    Set*    oAddSet(QString _sPath);
    Set*    oAddSet(QString _sPath, QString &_sName, bool _bActive);

    void    vDeleteSet(int _i);
    void    vDeleteAll();

    void    vEditSet(int _i, QString const &_sName, int const _iType, int const _iStyle);
    void    vSetState(int _i, bool _a);

    void    vMoveSet(int _from, int _to);

    // shortcut
    bool const bCanAddShortcut() const;
    void vCreateShortcut();
    void vDeleteShortcut();
};

#endif // SETTINGS_H
