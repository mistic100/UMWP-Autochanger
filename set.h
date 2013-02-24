#ifndef SET_H
#define SET_H

#include "main.h"

/*
 * an object representing a single wallpaper folder
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

    QString const &path() { return m_sPath; }
    QString const &name() { return m_sName; }
    int     const count() { return m_vFiles.size(); }
    bool    const &isActive() { return m_bActive; }

    void    vSetName(const QString &_name) { m_sName=_name; }
    bool    const bSwitchState();

    double  dReadLastModif(QString _sub="", int _level=0);
    void    vPopulateFiles(QString _sub="", int _level=0);

    QString const sGetFullName();
    QString const sGetFile(int _i);
};

#endif // SET_H
