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
    bool             m_active = true;
    bool             m_valid = true;
    QVector<QString> m_files;
    Current          m_current;
    double           m_lastModif = 0;
    int              m_hotkey = 0;
    CustomLayout     m_custLayout;

public:
    Set() {} // only used for temp items
    Set(const QString &_path, const QString &_name);
    Set(const QDomElement* _dom);

    const QString       &path() const       { return m_path; }
    const QString       &name() const       { return m_name; }
    const UM::WALLPAPER type() const        { return m_type; }
    const UM::IMAGE     style() const       { return m_style; }
    const UM::MODE      mode() const        { return m_mode; }
    const int           count() const       { return m_files.size(); }
    const bool          isActive() const    { return m_active; }
    const bool          isValid() const     { return m_valid; }
    const int           hotkey() const      { return m_hotkey; }
    const QString       uuid() const        { return m_uuid; }
    const Current       current() const     { return m_current; }
    const CustomLayout  custLayout() const  { return m_custLayout; }
    const QString       file(int _i) const;

    void setActive(const bool _a)           { m_active=_a; }
    void setName(const QString &_name)      { m_name=_name; }
    void setType(const UM::WALLPAPER _type) { m_type=_type; }
    void setStyle(const UM::IMAGE _style)   { m_style=_style; }
    void setMode(const UM::MODE _mode)      { m_mode=_mode; }
    void setHotkey(const int _hotkey)       { m_hotkey=_hotkey; }
    void setCurrent(const Current &_curr)   { m_current=_curr; }
    void setCustLayout(const CustomLayout &_layout) { m_custLayout=_layout; }

    void init();
    bool check();
    double lastChange();
    void populateFiles();

    void readCache();
    void writeCache() const;
    void deleteCache() const;

    void writeXml(QXmlStreamWriter *_writer) const;

    const QString fullName() const;
    const QString hotkeyStr() const;
    const int fileIndex(const QString &_file) const { return m_files.indexOf(_file); }

private:
    double lastChangeRecur(const QString &_child, const int _level=0);
    void populateFilesRecur(const QString &_child, const int _level=0);
};

#endif // SET_H
