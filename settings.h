#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSize>
#include "main.h"
#include "set.h"

class Settings
{
private:
    int             m_iState;        // state if the installation
    bool            m_bIsWow64;      // true if running on x64 system
    bool            m_bUnsaved;      // true if there are unsaved data
    QString         m_sExePath;      // path to UltraMon.exe
    QString         m_sWallPath;     // path to folder containing *.wallpaper
    QString         m_sBMPPath;      // path to UltraMon Wallpaper.bmp
    QString         m_sUMVersion;    // UltraMon version
    QVector<Set*>   m_oSets;         // list of wallpaper sets
    int             m_iNbMonitors;   // number of monitors
    int             m_iNbWallpapers; // number of wallpapers used
    QSize           m_oWindowSize;   // size of the window
    int             m_iDelay;        // delay of refresh
    bool            m_bMinimize;     // minimize on startup if sets are registered
    bool            m_bCheckFiles;   // periodically check files

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
    int         const iNbSets()         { return m_oSets.size(); }
    int         const &iState()         { return m_iState; }
    bool        const &bIsUnsaved()     { return m_bUnsaved; }
    QString     const &sExePath()       { return m_sExePath; }
    QString     const &sUMVersion()     { return m_sUMVersion; }
    QString     const &sWallPath()      { return m_sWallPath; }
    QString     const &sBMPPath()       { return m_sBMPPath; }
    int         const &iNbMonitors()    { return m_iNbMonitors; }
    int         const &iNbWallpapers()  { return m_iNbWallpapers; }
    int         const &iDelay()         { return m_iDelay; }
    QSize       const &oWindowSize()    { return m_oWindowSize; }
    bool        const &bMinimize()      { return m_bMinimize; }
    bool        const &bCheckFiles()    { return m_bCheckFiles; }
    int         const iNbFiles();

    // setters
    void vSetWindowSize(QSize _size)    { m_oWindowSize=_size; }
    void vSetDelay(int _delay)          { m_iDelay=_delay; m_bUnsaved=true; }
    void vSetMinimize(bool _a)          { m_bMinimize=_a; m_bUnsaved=true; }
    void vSetCheckFiles(bool _a)        { m_bCheckFiles=_a; m_bUnsaved=true; }
    bool bSetExePath(QString _sPath);

    // sets management
    Set*    oAddSet(QString _sPath);
    Set*    oAddSet(QString _sPath, QString _sName, bool _bActive);
    void    vDeleteSet(int _i);
    void    vDeleteAll();
    bool    const bSwitchSet(int _i);
    QString sRenameSet(int _i, const QString &_name, bool _returnFull);
    void    vUpdateSets();
};

#endif // SETTINGS_H
