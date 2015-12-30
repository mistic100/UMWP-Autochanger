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

    m_uuid = QString(QCryptographicHash::hash(m_path.toUtf8(), QCryptographicHash::Md5).toHex());
}

/**
 * @brief Set::Set
 * @param QDomElement _dom
 */
Set::Set(const QDomElement* _dom)
{
    m_path = _dom->text().trimmed();
    m_name = _dom->attribute("name");
    m_active = _dom->attribute("active").toInt();

    if (!m_path.endsWith('\\'))
    {
        m_path.append('\\');
    }

    m_uuid = QString(QCryptographicHash::hash(m_path.toUtf8(), QCryptographicHash::Md5).toHex());


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

    // 1.3 format
    if (_dom->hasAttribute("hotkey_mod"))
    {
        m_hotkey = _dom->attribute("hotkey").toInt() + _dom->attribute("hotkey_mod").toInt();
    }
    // 1.4 format
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
            if (element.tagName() == "customLayout")
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
                m_custLayout.borderWidth = element.attribute("borderWidth").toShort();
                m_custLayout.borderColor = element.attribute("borderColor").toInt();
            }
        }

        node = node.nextSibling();
    }
}

/**
 * @brief Write the set in a DOM element
 * @param QDomElement _dom
 * @param QDomDocument _document
 */
void Set::writeXml(QDomElement* _dom, QDomDocument* _document) const
{
    _dom->setAttribute("name", m_name);
    _dom->setAttribute("type", m_type);
    _dom->setAttribute("style", m_style);
    _dom->setAttribute("mode", m_mode);
    _dom->setAttribute("active", m_active);
    _dom->setAttribute("hotkey", m_hotkey);

    _dom->appendChild(_document->createTextNode(m_path));

    if (m_style == UM::IM_CUSTOM)
    {
        QDomElement layout = _document->createElement("customLayout");

        layout.setAttribute("rows", m_custLayout.rows);
        layout.setAttribute("cols", m_custLayout.cols);
        layout.setAttribute("minRows", m_custLayout.minRows);
        layout.setAttribute("minCols", m_custLayout.minCols);
        layout.setAttribute("maxRows", m_custLayout.maxRows);
        layout.setAttribute("maxCols", m_custLayout.maxCols);
        layout.setAttribute("mainEnabled", m_custLayout.mainEnabled);
        layout.setAttribute("mainRows", m_custLayout.mainRows);
        layout.setAttribute("mainCols", m_custLayout.mainCols);
        layout.setAttribute("mainPos", m_custLayout.mainPos);
        layout.setAttribute("borderEnabled", m_custLayout.borderEnabled);
        layout.setAttribute("borderWidth", m_custLayout.borderWidth);
        layout.setAttribute("borderColor", m_custLayout.borderColor);

        _dom->appendChild(layout);
    }
}

void Set::init()
{
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
    QDir dir(_path);
    QStringList files = dir.entryList(FILES_FILTER,
                                      QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot,
                                      QDir::Name | QDir::DirsLast | QDir::IgnoreCase
                                      );

    foreach (const QString path, files)
    {
        if (QFileInfo(_path + path).isDir())
        {
            if (_level+1 < APP_MAX_TRAVERSAL)
            {
                populateFilesRecur(_path + path + "/", _level+1);
            }
        }
        else
        {
            m_files.append(_path + path);
        }
    }
}

/**
 * @brief Read cached data
 */
void Set::readCache()
{
    QFile file(APP_CACHE_DIR + m_uuid);

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

        file.close();
    }
}

/**
 * @brief Write cache data
 */
void Set::writeCache() const
{
    QFile file(APP_CACHE_DIR + m_uuid);

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);

        out<<m_lastModif;
        out<<m_files;
        out<<m_current.file;
        out<<m_current.index;

        file.close();
    }
}

/**
 * @brief Delete cache files
 */
void Set::deleteCache() const
{
    QDir cache(APP_CACHE_DIR);
    QStringList files = cache.entryList(QStringList()<<"*"+m_uuid+"*", QDir::Files);

    foreach (QString file, files)
    {
        QFile::remove(cache.absoluteFilePath(file));
    }
}
