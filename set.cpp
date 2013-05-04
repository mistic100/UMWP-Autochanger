#include <iostream>

#include "set.h"
#include "dirent.h"


/*
 * constructor
 */
Set::Set(QString _path, QString _name, bool _active)
{
    m_sPath = _path;
    m_sName = _name;
    m_bActive = _active;
    m_dLastModif = 0;
    vPopulateFiles();
}

/*
 * get name with file counter
 */
QString const Set::sGetFullName() const
{
    return m_sName+" ("+QString::number(count())+")";
}

/*
 * get a file
 */
QString const Set::sGetFile(int _i) const
{
    if (_i <m_vFiles.size())
    {
        return m_vFiles.at(_i);
    }
    else
    {
        return m_vFiles.back();
    }
}

/*
 * recursively read the last modificatio date of a folder
 * must not be directly called with parameters
 */
double Set::dReadLastModif(QString _sub, int _level)
{
    QString sPath = m_sPath+_sub;
    std::wstring stemp = sPath.toStdWString();

    double dDate;
    FILETIME ftWrite;
    SYSTEMTIME stWrite;

    HANDLE hDir = CreateFile( stemp.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL );
    if (!GetFileTime( hDir, NULL, NULL, &ftWrite ))
    {
        GetSystemTime(&stWrite);
        SystemTimeToVariantTime(&stWrite, &dDate);
        return dDate;
    }
    CloseHandle(hDir);

    FileTimeToSystemTime(&ftWrite, &stWrite);
    SystemTimeToVariantTime(&stWrite, &dDate);

    if (_level < 2)
    {
        DIR* dir = opendir(sPath.toStdString().c_str());
        struct dirent* entry = readdir(dir);

        QString filename = QString(entry->d_name);

        while (entry != NULL)
        {
            if (entry->d_type == DT_DIR && filename.compare(".")!=0 && filename.compare("..")!=0)
            {
                double dSubDate = dReadLastModif(filename+"\\", _level+1);
                if (dSubDate > dDate)
                {
                    dDate = dSubDate;
                }
            }

            entry = readdir(dir);
            filename = QString(entry->d_name);
        }

        closedir(dir);
    }

    return dDate;
}

/*
 * recursively construct the list of files (if changed)
 * must not be directly called with parameters
 */
void Set::vPopulateFiles(QString _sub, int _level)
{
    if (_sub.isEmpty())
    {
        double dNewDate = dReadLastModif();
        if (dNewDate <= m_dLastModif)
        {
            return;
        }

        m_dLastModif = dNewDate;
        m_vFiles.clear();
    }

    if (_level < 2)
    {
        QString sPath = m_sPath+_sub;
        DIR* dir = opendir(sPath.toStdString().c_str());

        struct dirent* entry = readdir(dir);
        QString filename = QString(entry->d_name);

        while (entry != NULL)
        {
            if (entry->d_type == DT_DIR && filename.compare(".")!=0 && filename.compare("..")!=0)
            {
                vPopulateFiles(filename+"\\", _level+1);
            }
            else if (entry->d_type == DT_REG && bIsImageFile(filename))
            {
                m_vFiles.push_back(sPath+filename);
            }

            entry = readdir(dir);
            filename = QString(entry->d_name);
        }

        closedir(dir);
    }
}
