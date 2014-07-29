#include "set.h"
#include "lib/dirent.h"


/**
 * @brief Set::Set
 * @param string _path
 * @param string _name
 */
Set::Set(const QString &_path, const QString &_name)
{
    m_path = _path;
    m_name = _name;

    if (!m_path.endsWith('\\'))
    {
        m_path.append('\\');
    }

    m_type = UM::W_MONITOR;
    m_style = UM::IM_FILL;
    m_active = true;
    m_lastModif = 0;
    m_hotkey = 0;
    m_UID = QString(QCryptographicHash::hash(m_path.toUtf8(), QCryptographicHash::Md5).toHex());

    readCache();
    populateFiles();
}

/**
 * @brief Get name with file counter
 * @return string
 */
const QString Set::fullName() const
{
    return m_name+" ("+QString::number(count())+")";
}

/**
 * @brief Get a file path
 * @param int _i
 * @return  string
 */
const QString Set::gile(int _i) const
{
    if (_i < m_files.size())
    {
        return m_files.at(_i);
    }
    else
    {
        return m_files.back();
    }
}

/**
 * @brief Recursively read the last modification date of a folder
 * must not be directly called with parameters
 * @param string _child
 * @param int _level
 * @return double
 */
double Set::lastChange(const QString &_child, int _level)
{
    QString path = m_path+_child;

    double date;
    FILETIME fileDate;
    SYSTEMTIME systemDate;

    HANDLE hDir = CreateFile(path.toStdWString().c_str(), GENERIC_READ,FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if (!GetFileTime(hDir, NULL, NULL, &fileDate))
    {
        GetSystemTime(&systemDate);
        SystemTimeToVariantTime(&systemDate, &date);
        return date;
    }
    CloseHandle(hDir);

    FileTimeToSystemTime(&fileDate, &systemDate);
    SystemTimeToVariantTime(&systemDate, &date);

    if (_level < APP_MAX_TRAVERSAL)
    {
        DIR* pDir = opendir(path.toStdString().c_str());
        struct dirent* pEntry = readdir(pDir);

        QString filename = QString(pEntry->d_name);

        while (pEntry != NULL)
        {
            if (pEntry->d_type==DT_DIR && filename.compare(".")!=0 && filename.compare("..")!=0)
            {
                double subDate = lastChange(_child+filename+"\\", _level+1);
                if (subDate > date)
                {
                    date = subDate;
                }
            }

            pEntry = readdir(pDir);
            filename = QString(pEntry->d_name);
        }

        closedir(pDir);
    }

    return date;
}

/**
 * @brief Recursively construct the list of files (if changed)
 * must not be directly called with parameters
 * @param string _child
 * @param int _level
 */
void Set::populateFiles(const QString &_child, int _level)
{
    if (_child.isEmpty())
    {
        double date = lastChange();
        if (date <= m_lastModif)
        {
            return;
        }

        m_lastModif = date;
        m_files.clear();
    }

    if (_level < APP_MAX_TRAVERSAL)
    {
        QString path = m_path+_child;

        DIR* pDir = opendir(path.toStdString().c_str());
        struct dirent* pEntry = readdir(pDir);

        QString filename = QString(pEntry->d_name);

        while (pEntry != NULL)
        {
            if (pEntry->d_type==DT_DIR && filename.compare(".")!=0 && filename.compare("..")!=0)
            {
                populateFiles(_child+filename+"\\", _level+1);
            }
            else if (pEntry->d_type == DT_REG && isImageFile(filename))
            {
                m_files.append(path+filename);
            }

            pEntry = readdir(pDir);
            filename = QString(pEntry->d_name);
        }

        closedir(pDir);
    }

    if (_child.isEmpty())
    {
        writeCache();
    }
}

/**
 * @brief Read cached data
 */
void Set::readCache()
{
    QFile file(QString::fromAscii(APP_CACHE_DIR)+m_UID);

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in>>m_lastModif;
        in>>m_files;
        file.close();
    }
}

/**
 * @brief Write cache data
 */
void Set::writeCache()
{
    QFile file(QString::fromAscii(APP_CACHE_DIR)+m_UID);

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out<<m_lastModif;
        out<<m_files;
        file.close();
    }
}

/**
 * @brief Delete cache file
 */
void Set::deleteCache()
{
    QFile::remove(QString::fromAscii(APP_CACHE_DIR)+m_UID);
}
