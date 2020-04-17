#ifndef SET_H
#define SET_H

#include "main.h"
#include "customlayout.h"


/**
 * @brief An object representing a single wallpaper folder
 */
class Set
{
public:
    struct Current {
        QString file;
        int index;
        Current() : file(""), index(0) {}
    };

private:
    QString          m_path;
    QString          m_name;
    QString          m_uuid;
    UM::WALLPAPER    m_type = UM::W_MONITOR;
    UM::IMAGE        m_style = UM::IM_FILL;
    UM::MODE         m_mode = UM::MODE_RANDOM;
    TRI_BOOL         m_recent = FALSE_BOOL;
    double           m_frequency = 1.0;
    TRI_BOOL         m_lock = FALSE_BOOL;
    bool             m_active = true;
    bool             m_valid = true;
    QVector<QString> m_files;
    QVector<QString> m_folders;
    Current          m_current;
    double           m_lastModif = 0;
    bool             m_filesByDate = false;
    int              m_hotkey = 0;
    CustomLayout     m_custLayout;
    QVector<int>     m_monitors;

public:
    Set() {} // only used for temp items
    Set(const QString &_path, const QString &_name);
    Set(const QDomElement* _dom);

    const QString       &path() const       { return m_path; }
    const QString       &name() const       { return m_name; }
    UM::WALLPAPER       type() const        { return m_type; }
    UM::IMAGE           style() const       { return m_style; }
    UM::MODE            mode() const        { return m_mode; }
    TRI_BOOL            recent() const      { return m_recent; }
    double              frequency() const   { return m_frequency; }
    TRI_BOOL            lock() const        { return m_lock; }
    double              lastModif() const   { return m_lastModif; }
    bool                filesByDate() const { return m_filesByDate; }
    int                 nbFiles() const     { return m_files.size(); }
    int                 nbFolders() const   { return m_folders.size(); }
    int                 nbFilesInFolder(const QString &_folder) const;
    bool                isActive() const    { return m_active; }
    bool                isValid() const     { return m_valid; }
    int                 hotkey() const      { return m_hotkey; }
    const QString       uuid() const        { return m_uuid; }
    const Current       current() const     { return m_current; }
    const CustomLayout  custLayout() const  { return m_custLayout; }
    const QVector<int>  monitors() const    { return m_monitors; }
    bool                isActiveOnMonitor(int _i) const { return m_monitors.size() == 0 || m_monitors.contains(_i); }
    const QString       file(int _i) const;
    const QString       fileInFolder(const QString &_folder, int _i) const;
    const QString       folder(int _i) const;

    void setActive(const bool _a)           { m_active=_a; }
    void setName(const QString &_name)      { m_name=_name; }
    void setType(const UM::WALLPAPER _type) { m_type=_type; }
    void setStyle(const UM::IMAGE _style)   { m_style=_style; }
    void setMode(const UM::MODE _mode)      { m_mode=_mode; }
    void setRecent(const TRI_BOOL _r)       { m_recent=_r; }
    void setFrequency(const double _freq)   { m_frequency=_freq; }
    void setLock(const TRI_BOOL _lock)      { m_lock=_lock; }
    void setHotkey(const int _hotkey)       { m_hotkey=_hotkey; }
    void setCurrent(const Current &_curr)   { m_current=_curr; writeCache(); }
    void setCustLayout(const CustomLayout &_layout) { m_custLayout=_layout; }
    void setMonitors(const QVector<int> &_monitors) { m_monitors=_monitors; }

    void init();
    bool check();
    void update(const double _lastModif, const QVector<QString> &_files, const QVector<QString> _folders);

    void readCache();
    void writeCache() const;
    void deleteCache();

    void writeXml(QXmlStreamWriter *_writer) const;

    const QString fullName() const;
    const QString hotkeyStr() const;
    const int fileIndex(const QString &_file) const { return m_files.indexOf(_file); }

};

#endif // SET_H
