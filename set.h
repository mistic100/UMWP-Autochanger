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
    bool                m_bActive;
    QVector<QString>    m_vFiles;
    double              m_dLastModif;

public:
    Set(QString _path, QString _name, bool _active);

    QString const &path() const { return m_sPath; }
    QString const &name() const { return m_sName; }
    int     const count() const { return m_vFiles.size(); }
    bool    const &isActive() const { return m_bActive; }

    void    vSetActive(bool _a) { m_bActive=_a; }
    void    vSetName(const QString &_name) { m_sName=_name; }

    double  dReadLastModif(QString _sub="", int _level=0);
    void    vPopulateFiles(QString _sub="", int _level=0);

    QString const fullName() const;
    QString const sGetFile(int _i) const;
};

#endif // SET_H
