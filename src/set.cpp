#include <QKeySequence>

#include "set.h"
#include "environment.h"
#include "umutils.h"


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

    // added in 2.2
    if (_dom->hasAttribute("frequency"))
    {
        m_frequency = _dom->attribute("frequency").toDouble();
    }

    // added in 2.2.1
    if (_dom->hasAttribute("lock"))
    {
        m_lock = static_cast<TRI_BOOL>(QVariant(_dom->attribute("lock")).toBool());
    }

    // added in 2.2
    if (_dom->hasAttribute("monitors") && !_dom->attribute("monitors").isEmpty())
    {
        QStringList monitors = _dom->attribute("monitors").split(",");
        foreach (QString monitor, monitors)
        {
            m_monitors.append(monitor.toInt());
        }
    }

    // before 1.4 format
    if (_dom->hasAttribute("hotkey_mod"))
    {
        m_hotkey = _dom->attribute("hotkey").toInt() + _dom->attribute("hotkey_mod").toInt();
    }
    // since 1.4 format
    else if (_dom->hasAttribute("hotkey"))
    {
        // stored in string representation since 2.2.1
        bool isInt;
        m_hotkey = _dom->attribute("hotkey").toInt(&isInt);

        if (!isInt)
        {
            m_hotkey = UM::keySequenceToInt(QKeySequence(_dom->attribute("hotkey")));
        }
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
                m_custLayout = CustomLayout(&element);
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
    _writer->writeAttribute("hotkey", QKeySequence(m_hotkey).toString());
    _writer->writeAttribute("frequency", QString::number(m_frequency));
    _writer->writeAttribute("lock", QVariant((bool) m_lock).toString());

    QStringList monitors;
    foreach (int monitor, m_monitors)
    {
        monitors.append(QString::number(monitor));
    }
    _writer->writeAttribute("monitors", monitors.join(","));

    _writer->writeTextElement("path", m_path);

    if (m_style == UM::IM_CUSTOM)
    {
        m_custLayout.writeXml(_writer);
    }

    _writer->writeEndElement();
}

void Set::init()
{
    readCache();
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
    if (_folder.isEmpty())
    {
        return nbFiles();
    }

    int total = 0;
    foreach (const QString _path, m_files)
    {
        if (_path.startsWith(_folder + QDir::separator()))
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
    if (_i < 0)
    {
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
    if (_folder.isEmpty())
    {
        return file(_i);
    }

    if (_i < 0)
    {
        _i = 0;
    }

    int i = 0;
    foreach (const QString _path, m_files)
    {
        if (_path.startsWith(_folder + QDir::separator()))
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
    if (_i < 0)
    {
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
    return m_valid;
}

/**
 * @brief Update and cache files list
 * @param double _lastModif
 * @param string[] _files
 * @param string[] _folders
 */
void Set::update(const double _lastModif, const QVector<QString> &_files, const QVector<QString> _folders)
{
    m_lastModif = _lastModif;
    m_files = _files;
    m_folders = _folders;
    writeCache();
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
void Set::deleteCache()
{
    QDir cache(Environment::APPDATA_DIR + APP_CACHE_DIR);
    QStringList files = cache.entryList(QStringList()<<"*"+m_uuid+"*", QDir::Files);

    foreach (QString file, files)
    {
        QFile::remove(cache.absoluteFilePath(file));
    }

    m_lastModif = 0;
    m_current = Current();
    m_files.clear();
    m_folders.clear();
}
