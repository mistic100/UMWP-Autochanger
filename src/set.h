#ifndef SET_H
#define SET_H

#include "main.h"


/**
 * @brief An object representing a single wallpaper folder
 */
class Set
{
private :
    QString             m_sPath;
    QString             m_sName;
    UM::WALLPAPER       m_iType;
    UM::IMAGE           m_iStyle;
    QString             m_sUID;
    bool                m_bActive;
    QVector<QString>    m_vFiles;
    double              m_dLastModif;

public:
    Set(const QString &_path, const QString &_name, const bool _active);

    const QString       &path() const { return m_sPath; }
    const QString       &name() const { return m_sName; }
    const UM::WALLPAPER type() const  { return m_iType; }
    const UM::IMAGE     style() const { return m_iStyle; }
    const int           count() const { return m_vFiles.size(); }
    const bool          isActive() const { return m_bActive; }

    void    vSetActive(const bool _a)           { m_bActive=_a; }
    void    vSetName(const QString &_name)      { m_sName=_name; }
    void    vSetType(const UM::WALLPAPER _type) { m_iType=_type; }
    void    vSetStyle(const UM::IMAGE _style)   { m_iStyle=_style; }

    double  dReadLastModif(QString _sChild="", int _iLevel=0);
    void    vPopulateFiles(QString _sChild="", int _iLevel=0);

    void    vReadCache();
    void    vWriteCache();
    void    vDeleteCache();

    const QString fullName() const;
    const QString sGetFile(int _i) const;
};

#endif // SET_H
