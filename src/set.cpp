#include <QKeySequence>
#include "set.h"


static const QStringList FILES_FILTER = QStringList()<<"*.jpeg"<<"*.jpg"<<"*.bmp"<<"*.png"<<"*.gif";


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
    m_valid = true;
    m_lastModif = 0;
    m_hotkey = 0;
    m_uuid = QString(QCryptographicHash::hash(m_path.toUtf8(), QCryptographicHash::Md5).toHex());

    readCache();
    check();

    if (m_valid)
    {
        populateFiles();
    }
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
 * @brief Get the hotkey as readable string
 * @return string
 */
const QString Set::hotkeyStr() const
{
    return QKeySequence(m_hotkey).toString(QKeySequence::NativeText);
}

/**
 * @brief Get a file path
 * @param int _i
 * @return  string
 */
const QString Set::file(int _i) const
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
 * @brief Check is the directory still exists
 * @return bool
 */
bool Set::check()
{
    m_valid = QDir(m_path).exists();
    return m_valid;
}

/**
 * @brief Recursively read the last modification date of a folder
 * @return double
 */
double Set::lastChange()
{
    return lastChangeRecur(m_path);
}

/**
 * @brief Internal recursive function for lastChange()
 * @param string _path
 * @param int _level
 * @return double
 */
double Set::lastChangeRecur(const QString &_path, const int _level)
{
    double date = QFileInfo(_path).lastModified().toTime_t();

    if (_level < APP_MAX_TRAVERSAL)
    {
        QDirIterator dir(_path, QDir::AllDirs | QDir::NoDotAndDotDot);

        while (dir.hasNext())
        {
            QString path = dir.next();

            double subDate = lastChangeRecur(path, _level+1);

            if (subDate > date)
            {
                date = subDate;
            }
        }
    }

    return date;
}

/**
 * @brief Recursively construct the list of files (if changed)
 */
void Set::populateFiles()
{
    double date = lastChange();
    if (date <= m_lastModif)
    {
        return;
    }

    m_lastModif = date;
    m_files.clear();

    populateFilesRecur(m_path);

    writeCache();
}

/**
 * @brief Internal recursive function for populateFiles()
 * @param string _path
 * @param int _level
 */
void Set::populateFilesRecur(const QString &_path, const int _level)
{
    QDirIterator dir(_path, FILES_FILTER, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);

    while (dir.hasNext())
    {
        QString path = dir.next();

        if (dir.fileInfo().isDir())
        {
            if (_level+1 < APP_MAX_TRAVERSAL)
            {
                populateFilesRecur(path + "/", _level+1);
            }
        }
        else
        {
            m_files.append(path);
        }
    }
}

/**
 * @brief Read cached data
 */
void Set::readCache()
{
    QFile file(QString::fromAscii(APP_CACHE_DIR) + m_uuid);

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
    QFile file(QString::fromAscii(APP_CACHE_DIR) + m_uuid);

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out<<m_lastModif;
        out<<m_files;
        file.close();
    }
}

/**
 * @brief Delete cache files
 */
void Set::deleteCache()
{
    QDir cache(QString::fromAscii(APP_CACHE_DIR));
    QStringList files = cache.entryList(QStringList()<<"*"+m_uuid+"*", QDir::Files);

    foreach (QString file, files)
    {
        QFile::remove(cache.absoluteFilePath(file));
    }
}
