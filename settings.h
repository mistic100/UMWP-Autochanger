#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSize>
#include "main.h"
#include "set.h"

class Settings
{
private:
    short int       m_iState;        // state if the installation
    bool            m_bUnsaved;      // true if there are unsaved data
    QString         m_sExePath;      // path to UltraMon.exe
    QString         m_sWallPath;     // path to folder containing *.wallpaper
    QString         m_sBMPPath;      // path to UltraMon Wallpaper.bmp
    QString         m_sUMVersion;    // UltraMon version
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
    Set*        const poGetSet(int _i)  { return m_oSets.at(_i); }
    short int   const iNbSets()         { return m_oSets.size(); }
    short int   const &iState()         { return m_iState; }
    bool        const &bIsUnsaved()     { return m_bUnsaved; }
    QString     const &sExePath()       { return m_sExePath; }
    QString     const &sUMVersion()     { return m_sUMVersion; }
    QString     const &sWallPath()      { return m_sWallPath; }
    QString     const &sBMPPath()       { return m_sBMPPath; }
    short int   const &iNbMonitors()    { return m_iNbMonitors; }
    short int   const &iNbWallpapers()  { return m_iNbWallpapers; }
    short int   const &iDelay()         { return m_iDelay; }
    QSize       const &oWindowSize()    { return m_oWindowSize; }
    bool        const &bMinimize()      { return m_bMinimize; }
    bool        const &bCheckFiles()    { return m_bCheckFiles; }
    short int   const &iMsgCount()      { return m_iMsgCount; }
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
    void    vDeleteAll();
    bool    const bSwitchSet(short int _i);
    QString sRenameSet(short int _i, const QString &_name, bool _returnFull);
    void    vUpdateSets();
};

#endif // SETTINGS_H
