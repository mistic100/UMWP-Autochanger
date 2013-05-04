#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSize>

#include "main.h"
#include "set.h"


/*
 * Name: Settings
 * Description: Class managing settings of the application
 */
class Settings
{
private:
    short int       m_iState;        // state if the installation
    bool            m_bUnsaved;      // true if there are unsaved data

    QString         m_sExePath;      // path to UltraMon.exe
    QString         m_sWallPath;     // path to folder containing *.wallpaper
    QString         m_sBMPPath;      // path to UltraMon Wallpaper.bmp
    QString         m_sUMVersion;    // UltraMon version
    QString         m_sStartLnkPath;

    QVector<Set*>   m_oSets;         // list of wallpaper sets

    short int       m_iNbMonitors;   // number of monitors
    short int       m_iNbWallpapers; // number of wallpapers used
    QSize           m_oWindowSize;   // size of the window
    short int       m_iDelay;        // delay of refresh
    bool            m_bMinimize;     // minimize on startup if sets are registered
    bool            m_bCheckFiles;   // periodically check files
    short int       m_iMsgCount;     // counter for tray messages

public:
    Settings();
    ~Settings();

    void vDumpConfig();

    // main methods
    void vInit();
    void vReadNbWalls();
    void vWriteXML();
    void vReadXML();

    // getters
    Set*        const poGetSet(int _i) const  { return m_oSets.at(_i); }
    short int   const iNbSets() const         { return m_oSets.size(); }
    short int   const &iState() const         { return m_iState; }
    bool        const &bIsUnsaved() const     { return m_bUnsaved; }
    QString     const &sExePath() const       { return m_sExePath; }
    QString     const &sUMVersion() const     { return m_sUMVersion; }
    QString     const &sWallPath() const      { return m_sWallPath; }
    QString     const &sBMPPath() const       { return m_sBMPPath; }
    short int   const &iNbMonitors() const    { return m_iNbMonitors; }
    short int   const &iNbWallpapers() const  { return m_iNbWallpapers; }
    short int   const &iDelay() const         { return m_iDelay; }
    QSize       const &oWindowSize() const    { return m_oWindowSize; }
    bool        const &bMinimize() const      { return m_bMinimize; }
    bool        const &bCheckFiles() const    { return m_bCheckFiles; }
    short int   const &iMsgCount() const      { return m_iMsgCount; }
    bool        const bCanAddShortcut() const { return !m_sStartLnkPath.isEmpty(); }
    bool        const bIsAutostart() const    { return bFileExists(m_sStartLnkPath.toStdString()); }
    int         const iNbFiles();

    // setters
    void vSetWindowSize(const QSize &_size) { m_oWindowSize=_size; }
    void vSetDelay(const short int &_delay) { m_iDelay=_delay; m_bUnsaved=true; }
    void vSetMinimize(bool _a)              { m_bMinimize=_a; m_bUnsaved=true; }
    void vSetCheckFiles(bool _a)            { m_bCheckFiles=_a; m_bUnsaved=true; }
    void vAddMsgCount()                     { m_iMsgCount++; }
    bool bSetExePath(const QString &_sPath);

    // sets management
    Set*    oAddSet(QString _sPath);
    Set*    oAddSet(QString _sPath, QString _sName, bool _bActive);
    void    vDeleteSet(short int _i);
    void    vDeleteSets(QList<int> _list);
    void    vDeleteAll();
    QString sRenameSet(short int _i, const QString &_name, bool _returnFull);
    void    vSetState(short int _i, bool _a);
    void    vUpdateSets();

    void vCreateShortcut();
    void vDeleteShortcut();
};

#endif // SETTINGS_H
