#ifndef SET_H
#define SET_H

#include "main.h"


/**
 * @brief An object representing a single wallpaper folder
 */
class Set
{
private :
    QString          m_path;
    QString          m_name;
    UM::WALLPAPER    m_type;
    UM::IMAGE        m_style;
    QString          m_UID;
    bool             m_active;
    QVector<QString> m_files;
    double           m_lastModif;
    int              m_hotkey;

public:
    Set(const QString &_path, const QString &_name);

    const QString       &path() const { return m_path; }
    const QString       &name() const { return m_name; }
    const UM::WALLPAPER type() const  { return m_type; }
    const UM::IMAGE     style() const { return m_style; }
    const int           count() const { return m_files.size(); }
    const bool          isActive() const { return m_active; }
    const int           hotkey() const { return m_hotkey; }

    void setActive(const bool _a)           { m_active=_a; }
    void setName(const QString &_name)      { m_name=_name; }
    void setType(const UM::WALLPAPER _type) { m_type=_type; }
    void setStyle(const UM::IMAGE _style)   { m_style=_style; }
    void setHotkey(const int _hotkey)       { m_hotkey=_hotkey; }

    double getLastModif(QString _child="", int _level=0);
    void populateFiles(QString _child="", int _level=0);

    void  readCache();
    void  writeCache();
    void  deleteCache();

    const QString fullName() const;
    const QString getFile(int _i) const;
};

#endif // SET_H
