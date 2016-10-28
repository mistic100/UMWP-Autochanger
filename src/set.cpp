#include <QKeySequence>

#include "set.h"
#include "environment.h"
#include "umutils.h"


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

    m_uuid = UM::hash(m_path, QCryptographicHash::Md5);
}

/**
 * @brief Set::Set
 * @param QDomElement _dom
 */
Set::Set(const QDomElement* _dom)
{
    // before 2.1
    m_path = _dom->text().trimmed();

    m_name = _dom->attribute("name");
    m_active = (bool) _dom->attribute("active").toInt();

    // added in 1.3
    if (_dom->hasAttribute("type"))
    {
        m_type = static_cast<UM::WALLPAPER>(_dom->attribute("type").toInt());
        m_style = static_cast<UM::IMAGE>(_dom->attribute("style").toInt());
    }

    // added in 1.9
    if (_dom->hasAttribute("mode"))
    {
        m_mode = static_cast<UM::MODE>(_dom->attribute("mode").toInt());
    }

    // added in 2.1.1
    if (_dom->hasAttribute("frequency"))
    {
        m_frequency = _dom->attribute("frequency").toDouble();
    }

    // added in 2.1.1
    if (_dom->hasAttribute("perFolder"))
    {
        m_perFolder = _dom->attribute("perFolder").toInt();
    }

    // before 1.4 format
    if (_dom->hasAttribute("hotkey_mod"))
    {
        m_hotkey = _dom->attribute("hotkey").toInt() + _dom->attribute("hotkey_mod").toInt();
    }
    // since 1.4 format
    else if (_dom->hasAttribute("hotkey"))
    {
        m_hotkey = _dom->attribute("hotkey").toInt();
    }

    // inner nodes
    QDomNode node = _dom->firstChild();

    while (!node.isNull())
    {
        if (node.isElement())
        {
            QDomElement element = node.toElement();

            // added in 2.1
            if (element.tagName() == "path")
            {
                m_path = element.text().trimmed();
            }
            // added in 2.1
            else if (element.tagName() == "customLayout")
            {
                m_custLayout.rows = element.attribute("rows").toShort();
                m_custLayout.cols = element.attribute("cols").toShort();
                m_custLayout.minRows = element.attribute("minRows").toShort();
                m_custLayout.minCols = element.attribute("minCols").toShort();
                m_custLayout.maxRows = element.attribute("maxRows").toShort();
                m_custLayout.maxCols = element.attribute("maxCols").toShort();
                m_custLayout.mainEnabled = (bool) element.attribute("mainEnabled").toInt();
                m_custLayout.mainRows = element.attribute("mainRows").toShort();
                m_custLayout.mainCols = element.attribute("mainCols").toShort();
                m_custLayout.mainPos = static_cast<UM::ALIGN>(element.attribute("mainPos").toInt());
                m_custLayout.borderEnabled = (bool) element.attribute("borderEnabled").toInt();
                m_custLayout.borderScreenEnabled = (bool) element.attribute("borderScreenEnabled").toInt();
                m_custLayout.borderWidth = element.attribute("borderWidth").toShort();
                m_custLayout.borderColor = element.attribute("borderColor").toUInt();
            }
        }

        node = node.nextSibling();
    }

    if (m_path.isEmpty())
    {
        throw std::logic_error("Set has no path defined");
    }

    if (!m_path.endsWith('\\'))
    {
        m_path.append('\\');
    }

    m_uuid = QString(QCryptographicHash::hash(m_path.toUtf8(), QCryptographicHash::Md5).toHex());
}

/**
 * @brief Write the set in a DOM element
 * @param QDomElement _dom
 * @param QDomDocument _document
 */
void Set::writeXml(QXmlStreamWriter* _writer) const
{
    _writer->writeStartElement("set");

    _writer->writeAttribute("name", m_name);
    _writer->writeAttribute("type", QString::number(m_type));
    _writer->writeAttribute("style", QString::number(m_style));
    _writer->writeAttribute("mode", QString::number(m_mode));
    _writer->writeAttribute("active", QString::number(m_active));
    _writer->writeAttribute("hotkey", QString::number(m_hotkey));
    _writer->writeAttribute("frequency", QString::number(m_frequency));
    _writer->writeAttribute("perFolder", QString::number(m_perFolder));

    _writer->writeTextElement("path", m_path);

    if (m_style == UM::IM_CUSTOM)
    {
        _writer->writeStartElement("customLayout");

        _writer->writeAttribute("rows", QString::number(m_custLayout.rows));
        _writer->writeAttribute("cols", QString::number(m_custLayout.cols));
        _writer->writeAttribute("minRows", QString::number(m_custLayout.minRows));
        _writer->writeAttribute("minCols", QString::number(m_custLayout.minCols));
        _writer->writeAttribute("maxRows", QString::number(m_custLayout.maxRows));
        _writer->writeAttribute("maxCols", QString::number(m_custLayout.maxCols));
        _writer->writeAttribute("mainEnabled", QString::number(m_custLayout.mainEnabled));
        _writer->writeAttribute("mainRows", QString::number(m_custLayout.mainRows));
        _writer->writeAttribute("mainCols", QString::number(m_custLayout.mainCols));
        _writer->writeAttribute("mainPos", QString::number(m_custLayout.mainPos));
        _writer->writeAttribute("borderEnabled", QString::number(m_custLayout.borderEnabled));
        _writer->writeAttribute("borderScreenEnabled", QString::number(m_custLayout.borderScreenEnabled));
        _writer->writeAttribute("borderWidth", QString::number(m_custLayout.borderWidth));
        _writer->writeAttribute("borderColor", QString::number(m_custLayout.borderColor));

        _writer->writeEndElement();
    }

    _writer->writeEndElement();
}

void Set::init()
{
    readCache();
    populateFiles();
    check();
}

/**
 * @brief Get name with file counter
 * @return string
 */
const QString Set::fullName() const
{
    return m_name+" ("+QString::number(nbFiles())+")";
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
 * @brief Get the number of files in a folder
 * @param string _folder
 * @return int
 */
const int Set::nbFilesInFolder(const QString &_folder) const
{
    int total = 0;
    foreach(const QString _path, m_files)
    {
        if (_path.startsWith(_folder))
        {
            total++;
        }
    }
    return total;
}

/**
 * @brief Get a file path
 * @param int _i
 * @return string
 */
const QString Set::file(int _i) const
{
    if (_i < 0) {
        return m_files.front();
    }
    else if (_i < m_files.size())
    {
        return m_files.at(_i);
    }
    else
    {
        return m_files.back();
    }
}

/**
 * @brief Get a file path inside a folder
 * @param string _folder
 * @param int _i
 * @return string
 */
const QString Set::fileInFolder(const QString &_folder, int _i) const
{
    if (_i < 0) {
        _i = 0;
    }

    int i = 0;
    foreach(const QString _path, m_files)
    {
        if (_path.startsWith(_folder))
        {
            if (i == _i)
            {
                return _path;
            }
            i++;
        }
    }

    return m_files.back();
}

/**
 * @brief Get a folder path
 * @param int _i
 * @return string
 */
const QString Set::folder(int _i) const
{
    if (_i < 0) {
        return m_folders.front();
    }
    else if (_i < m_folders.size())
    {
        return m_folders.at(_i);
    }
    else
    {
        return m_folders.back();
    }
}

/**
 * @brief Check is the directory still exists
 * @return bool
 */
bool Set::check()
{
    m_valid = QDir(m_path).exists();
    m_valid&= m_files.length() > 0;
    m_valid&= !m_perFolder || m_folders.length() > 0;
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
    if (!QDir(m_path).exists())
    {
        return;
    }

    bool forceCheck = m_files.length() == 0 || m_perFolder && m_folders.length() == 0;
    double date = lastChange();

    if (!forceCheck && date <= m_lastModif)
    {
        return;
    }

    m_lastModif = date;
    m_files.clear();
    m_folders.clear();

    // list non-empty sub-folders
    QDir dir(m_path);
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase);

    foreach (const QString path, folders)
    {
        if (QDir(m_path + path).entryList(FILES_FILTER, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot).count() > 0)
        {
            m_folders.append(m_path + path);
        }
    }

    // recursively list files
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
    if (_level < APP_MAX_TRAVERSAL)
    {
        QDir dir(_path);
        QStringList files = dir.entryList(FILES_FILTER,
                                          QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot,
                                          QDir::Name | QDir::DirsLast | QDir::IgnoreCase
                                          );

        foreach (const QString path, files)
        {
            if (QFileInfo(_path + path).isDir())
            {
                populateFilesRecur(_path + path + "/", _level+1);
            }
            else if (!m_perFolder || _level > 0) // do not list first level files if perFolder
            {
                m_files.append(_path + path);
            }
        }
    }
}

/**
 * @brief Read cached data
 */
void Set::readCache()
{
    QFile file(Environment::APPDATA_DIR + APP_CACHE_DIR + m_uuid);

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);

        in>>m_lastModif;
        in>>m_files;
        if (!in.atEnd())
        {
            in>>m_current.file;
            in>>m_current.index;
        }
        if (!in.atEnd())
        {
            in>>m_folders;
        }

        file.close();
    }
}

/**
 * @brief Write cache data
 */
void Set::writeCache() const
{
    QFile file(Environment::APPDATA_DIR + APP_CACHE_DIR + m_uuid);

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);

        out<<m_lastModif;
        out<<m_files;
        out<<m_current.file;
        out<<m_current.index;
        out<<m_folders;

        file.close();
    }
}

/**
 * @brief Delete cache files
 */
void Set::deleteCache() const
{
    QDir cache(Environment::APPDATA_DIR + APP_CACHE_DIR);
    QStringList files = cache.entryList(QStringList()<<"*"+m_uuid+"*", QDir::Files);

    foreach (QString file, files)
    {
        QFile::remove(cache.absoluteFilePath(file));
    }
}
