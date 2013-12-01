#include "set.h"
#include "dirent.h"


/**
 * @brief Set::Set
 * @param string _path
 * @param string _name
 * @param bool _active
 */
Set::Set(const QString &_path, const QString &_name, const bool _active)
{
    m_sPath = _path;
    m_sName = _name;
    m_iType = UM::W_MONITOR;
    m_iStyle = UM::IM_CENTER;
    m_bActive = _active;
    m_dLastModif = 0;
    m_sUID = QString(QCryptographicHash::hash(m_sPath.toUtf8(), QCryptographicHash::Md5).toHex());

    vReadCache();
    vPopulateFiles();
}

/**
 * @brief Get name with file counter
 * @return string
 */
const QString Set::fullName() const
{
    return m_sName+" ("+QString::number(count())+")";
}

/**
 * @brief Get a file path
 * @param int _i
 * @return  string
 */
const QString Set::sGetFile(int _i) const
{
    if (_i < m_vFiles.size())
    {
        return m_vFiles.at(_i);
    }
    else
    {
        return m_vFiles.back();
    }
}

/**
 * @brief Recursively read the last modification date of a folder
 * must not be directly called with parameters
 * @param string _sChild
 * @param int _iLevel
 * @return double
 */
double Set::dReadLastModif(QString _sChild, int _iLevel)
{
    QString sPath = m_sPath+_sChild;

    double dDate;
    FILETIME ftWrite;
    SYSTEMTIME stWrite;

    HANDLE hDir = CreateFile(sPath.toStdWString().c_str(), GENERIC_READ,FILE_SHARE_READ,
                             NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if (!GetFileTime( hDir, NULL, NULL, &ftWrite ))
    {
        GetSystemTime(&stWrite);
        SystemTimeToVariantTime(&stWrite, &dDate);
        return dDate;
    }
    CloseHandle(hDir);

    FileTimeToSystemTime(&ftWrite, &stWrite);
    SystemTimeToVariantTime(&stWrite, &dDate);

    if (_iLevel < 2)
    {
        DIR* poDir = opendir(sPath.toStdString().c_str());
        struct dirent* poEntry = readdir(poDir);

        QString sFilename = QString(poEntry->d_name);

        while (poEntry != NULL)
        {
            if (poEntry->d_type==DT_DIR && sFilename.compare(".")!=0 && sFilename.compare("..")!=0)
            {
                double dSubDate = dReadLastModif(sFilename+"\\", _iLevel+1);
                if (dSubDate > dDate)
                {
                    dDate = dSubDate;
                }
            }

            poEntry = readdir(poDir);
            sFilename = QString(poEntry->d_name);
        }

        closedir(poDir);
    }

    return dDate;
}

/**
 * @brief Recursively construct the list of files (if changed)
 * must not be directly called with parameters
 * @param string _sChild
 * @param int _iLevel
 */
void Set::vPopulateFiles(QString _sChild, int _iLevel)
{
    if (_sChild.isEmpty())
    {
        double dNewDate = dReadLastModif();
        if (dNewDate <= m_dLastModif)
        {
            return;
        }

        m_dLastModif = dNewDate;
        m_vFiles.clear();
    }

    if (_iLevel < 2)
    {
        QString sPath = m_sPath+_sChild;
        DIR* poDir = opendir(sPath.toStdString().c_str());

        struct dirent* poEntry = readdir(poDir);
        QString sFilename = QString(poEntry->d_name);

        while (poEntry != NULL)
        {
            if (poEntry->d_type==DT_DIR && sFilename.compare(".")!=0 && sFilename.compare("..")!=0)
            {
                vPopulateFiles(sFilename+"\\", _iLevel+1);
            }
            else if (poEntry->d_type == DT_REG && bIsImageFile(sFilename))
            {
                m_vFiles.push_back(sPath+sFilename);
            }

            poEntry = readdir(poDir);
            sFilename = QString(poEntry->d_name);
        }

        closedir(poDir);
    }

    if (_sChild.isEmpty())
    {
        vWriteCache();
    }
}

/**
 * @brief Read cached data
 */
void Set::vReadCache()
{
    QFile oFile(QString::fromAscii(APP_CACHE_DIR)+m_sUID);
    if (oFile.exists())
    {
        oFile.open(QIODevice::ReadOnly);
        QDataStream in(&oFile);
        in>>m_dLastModif;
        in>>m_vFiles;
        oFile.close();
    }
}

/**
 * @brief Write cache data
 */
void Set::vWriteCache()
{
    QFile oFile(QString::fromAscii(APP_CACHE_DIR)+m_sUID);
    oFile.open(QIODevice::WriteOnly);
    QDataStream out(&oFile);
    out<<m_dLastModif;
    out<<m_vFiles;
    oFile.close();
}

/**
 * @brief Delete cache file
 */
void Set::vDeleteCache()
{
    QFile::remove(QString::fromAscii(APP_CACHE_DIR)+m_sUID);
}
