#ifndef SET_H
#define SET_H

#include "main.h"


/*
 * Name: Set
 * Description: An object representing a single wallpaper folder
 */
class Set
{
private :
    QString             m_sPath;
    QString             m_sName;
    int                 m_iType;
    int                 m_iStyle;
    QString             m_sUID;
    bool                m_bActive;
    QVector<QString>    m_vFiles;
    double              m_dLastModif;

public:
    Set(QString const &_path, QString const &_name, bool _active);

    QString const &path() const { return m_sPath; }
    QString const &name() const { return m_sName; }
    int     const &type() const  { return m_iType; }
    int     const &style() const { return m_iStyle; }
    int     const count() const { return m_vFiles.size(); }
    bool    const &isActive() const { return m_bActive; }

    void    vSetActive(bool const _a) { m_bActive=_a; }
    void    vSetName(QString const &_name) { m_sName=_name; }
    void    vSetType(int const _type) { m_iType=_type; }
    void    vSetStyle(int const _style) { m_iStyle=_style; }

    double  dReadLastModif(QString _sub="", int _level=0);
    void    vPopulateFiles(QString _sub="", int _level=0);

    void    vReadCache();
    void    vWriteCache();
    void    vDeleteCache();

    QString const fullName() const;
    QString const sGetFile(int _i) const;
};

#endif // SET_H
