#include <ctime>
#include <QPainter>

#include "controller.h"
#include "wallpapergenerator.h"
#include "lib/Crc32.h"


/**
 * @brief WallpaperGenerator::WallpaperGenerator
 * @param Controller* _ctrl
 */
WallpaperGenerator::WallpaperGenerator(Controller* _ctrl) :
    QObject(_ctrl),
    m_ctrl(_ctrl),
    m_enviro(_ctrl->enviro()),
    m_settings(_ctrl->settings())
{
    m_randomEngine.seed((unsigned int)time(NULL));
}

/**
 * @brief Get a random Set among all active sets
 * @return Set*
 */
Set* WallpaperGenerator::getRandomSet()
{
    int total = m_settings->nbActiveSets(true);

    if (total == 0)
    {
        return NULL;
    }

    if (total == 1)
    {
        return m_settings->activeSet(0);
    }

    uniform_int<int> unif(0, total-1);
    int counter = unif(m_randomEngine);

    return m_settings->activeSet(counter);
}

/**
 * @brief Get files URI from a Set. Contains empty strings for disabled monitors.
 * @param Set* _set
 * @return string[]
 */
QVector<QString> WallpaperGenerator::getFiles(Set* _set)
{
    QVector<QString> files;

    if (_set->type() == UM::W_MONITOR)
    {
        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                if (_set->mode() == UM::RANDOM)
                {
                    files.append(getRandomFile(_set, files));
                }
                else
                {
                    files.append(getNextFile(_set));
                }
            }
            else
            {
                files.append("");
            }
        }
    }
    else
    {
        if (_set->mode() == UM::RANDOM)
        {
            files.append(getRandomFile(_set, files));
        }
        else
        {
            files.append(getNextFile(_set));
        }
    }

    if (_set->mode() == UM::SEQUENTIAL)
    {
        // save current file info
        _set->writeCache();
    }

    return files;
}

/**
 * @brief Get next file in Set
 * @param Set* _set
 * @return string
 */
QString WallpaperGenerator::getNextFile(Set* _set)
{
    Set::Current current = _set->current();
    int index = 0;

    if (!current.file.isEmpty())
    {
        index = _set->fileIndex(current.file);

        if (index == -1)
        {
            index = current.index;
        }
        else
        {
            index++;
        }

        if (index >= _set->count())
        {
            index = 0;
        }
    }

    QString file = _set->file(index);

    current.file = file;
    current.index = index;
    _set->setCurrent(current);

    return file;
}

/**
 * @brief Get a random file within a Set
 * @param Set* _set
 * @param string[] _files
 * @return string
 */
QString WallpaperGenerator::getRandomFile(Set* _set, const QVector<QString> &_files)
{
    int total = _set->count();

    if (total == 0)
    {
        return "";
    }

    // only one file in the set ?!
    if (total == 1)
    {
        return _set->file(0);
    }

    uniform_int<int> unif(0, total-1);

    // search a random unused file
    short loop = 10;
    QString file;

    while (loop > 0)
    {
        loop--;

        int counter = unif(m_randomEngine);
        file = _set->file(counter);

        if (!_files.contains(file))
        {
            loop = 0;
        }
    }

    return file;
}

/**
 * @brief Resize image files and return an array of new paths in cache folder.
 * The result may be a mix of original and cached files.
 * @param Set* _set
 * @param string[] _files
 * @return string[]
 */
QVector<QString> WallpaperGenerator::adaptFiles(const Set* _set, const QVector<QString> &_files)
{
    QVector<QString> newFiles;

    if (_set->type() == UM::W_DESKTOP)
    {
        QRect rect = QRect(QPoint(), getDesktopEnabledRect().size());

        newFiles.append(adaptFileToMonitor(_files.at(0), -1, rect, rect, _set));
    }
    else
    {
        for (int i=0, l=_files.size(); i<l; i++)
        {
            if (!_files.at(i).isEmpty())
            {
                QRect scrRect = m_enviro->wpSize(i);
                QRect wpRect(QPoint(), scrRect.size());

                newFiles.append(adaptFileToMonitor(_files.at(i), i, scrRect, wpRect, _set));
            }
            else
            {
                newFiles.append(_files.at(i));
            }
        }
    }

    return newFiles;
}

/**
 * @brief Compute an image to fit in the monitor with a particular style, with cache management
 * @param string _file
 * @param int _idx
 * @param QRect _scrRect
 * @param QRect _wpRect
 * @param Set* _set
 * @return string
 */
QString WallpaperGenerator::adaptFileToMonitor(const QString &_file, int _idx, const QRect &_scrRect, const QRect &_wpRect, const Set* _set)
{
    QString key;
    if (_set->type() == UM::W_DESKTOP)
    {
        key = getDesktopWallpaperKey(_set->style());
    }
    else
    {
        key = QString::number(_set->style());
        key+= QString::number(m_settings->monitor(_idx).color);
    }

    QString cachePath = getCacheFilename(_file, _scrRect, key, _set->uuid());

    if (QFile::exists(cachePath))
    {
        return cachePath;
    }

    QImage source(_file);
    QRect srcRect = QRect(QPoint(), source.size());

    QLOG_DEBUG() << "Resizing image. Screen:" << _scrRect << "WP:" << _wpRect << "Image:" << srcRect;

    if (_set->style() == UM::IM_STRETCH) // stretching is done when generating the final wallpaper
    {
        return _file;
    }
    else if (_set->style() == UM::IM_CENTER || _set->style() == UM::IM_TILE) // for tile and center styles, compare the sizes
    {
        if (qAbs(srcRect.width() - _wpRect.width()) < 5 && qAbs(srcRect.height() - _wpRect.height()) < 5)
        {
            return _file;
        }
    }
    else // for every other styles, compare the ratios
    {
        double imageRatio = (double)srcRect.width() / srcRect.height();
        double screenRatio = (double)_wpRect.width() / _wpRect.height();

        if (qAbs(imageRatio - screenRatio) < 0.02)
        {
            return _file;
        }
    }

    QImage image(_scrRect.size(), QImage::Format_RGB32);
    QPainter painter(&image);

    // draw background color of enabled monitors
    if (_set->type() == UM::W_DESKTOP)
    {
        image.fill(Qt::black);

        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                QColor color((QRgb) m_settings->monitor(i).color);
                QRect rect = m_enviro->wpSize(i).translated(-_scrRect.topLeft());

                painter.setBrush(QBrush(color));
                painter.drawRect(rect);
            }
        }
    }
    else
    {
        QColor color((QRgb) m_settings->monitor(_idx).color);
        image.fill(color);
    }

    // draw image depending on style
    switch (_set->style())
    {
    case UM::IM_CENTER:
    {
        QRect zoneTarget(0, 0,
                   qMin(_wpRect.width(), srcRect.width()),
                   qMin(_wpRect.height(), srcRect.height())
        );
        zoneTarget.moveCenter(_wpRect.center());

        QRect zoneSource = zoneTarget;
        zoneSource.moveCenter(srcRect.center());

        painter.drawImage(zoneTarget, source, zoneSource);

        break;
    }
    case UM::IM_TILE:
    {
        QPoint pos = _wpRect.topLeft();

        do {
            painter.drawImage(pos, source);

            pos.rx()+= srcRect.width();

            if (!_wpRect.contains(pos))
            {
                pos.setX(_wpRect.left());
                pos.ry()+= srcRect.height();
            }
        }
        while (_wpRect.contains(pos));

        break;
    }
    case UM::IM_STRETCH:
    {
        source = source.scaled(_wpRect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawImage(_wpRect.topLeft(), source);
        break;
    }
    case UM::IM_STRETCH_PROP:
    {
        source = source.scaled(_wpRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        srcRect = QRect(QPoint(), source.size());

        QRect zoneTarget(
                    (_wpRect.width()-srcRect.width()) / 2,
                    (_wpRect.height()-srcRect.height()) / 2,
                     srcRect.width(),
                     srcRect.height()
        );
        zoneTarget.translate(_wpRect.topLeft());

        painter.drawImage(zoneTarget, source, srcRect);

        break;
    }
    case UM::IM_FILL:
    {
        source = source.scaled(_wpRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        srcRect = QRect(QPoint(), source.size());

        QRect zoneSource(
                    (srcRect.width()-_wpRect.width()) / 2,
                    (srcRect.height()-_wpRect.height()) / 2,
                     _wpRect.width(),
                     _wpRect.height()
        );

        painter.drawImage(_wpRect, source, zoneSource);

        break;
    }
    }

    painter.end();

    image.save(cachePath, 0, 90);

    return cachePath;
}

/**
 * @brief Generate the final wallpaper file
 * @param Set* _set
 * @param string[] _files
 * @return string
 */
QString WallpaperGenerator::generateFile(const Set *_set, const QVector<QString> &_files)
{
    QRect rect = m_enviro->wpSize(-1);
    QPoint offset = rect.topLeft();

    QImage image(rect.size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    image.fill(Qt::black);

    if (_set->type() == UM::W_DESKTOP)
    {
        QImage source(_files.at(0));
        QRect wpRect = getDesktopEnabledRect().translated(-offset);

        source = source.scaled(wpRect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter.drawImage(wpRect.topLeft(), source);
    }
    else
    {
        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                QImage source(_files.at(i));
                QRect wpRect = m_enviro->wpSize(i).translated(-offset);

                source = source.scaled(wpRect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

                painter.drawImage(wpRect.topLeft(), source);
            }
        }
    }

    // draw background color of disabled monitors
    for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
    {
        if (!m_settings->monitor(i).enabled)
        {
            QColor color((QRgb) m_settings->monitor(i).color);
            QRect wpRect = m_enviro->wpSize(i).translated(-offset);

            painter.setBrush(QBrush(color));
            painter.drawRect(wpRect);
        }
    }

    painter.end();

    QString filename = QDir::toNativeSeparators(QDir::tempPath() + "/" + QString::fromAscii(APP_WALLPAPER_FILE));

    // shift the tile if desktop origin is not at wallpaper origin
    if (offset != QPoint(0,0))
    {
        QImage image2(rect.size(), QImage::Format_RGB32);
        QPainter painter2(&image2);

        // main
        QRect targRect = rect.translated(-offset);
        QRect srcRect = rect.translated(-2*offset);
        painter2.drawImage(targRect, image, srcRect);

        // left
        targRect = QRect(rect.width()+rect.left(), 0, -rect.left(), rect.height()+rect.top());
        srcRect = QRect(0, -rect.top(), -rect.left(), rect.height()+rect.top());
        painter2.drawImage(targRect, image, srcRect);

        // top
        targRect = QRect(0, rect.height()+rect.top(), rect.width()+rect.left(), -rect.top());
        srcRect = QRect(-rect.left(), 0, rect.width()+rect.left(), -rect.top());
        painter2.drawImage(targRect, image, srcRect);

        // left-top
        targRect = QRect(rect.width()+rect.left(), rect.height()+rect.top(), -rect.left(), -rect.top());
        srcRect = QRect(0, 0, -rect.left(), -rect.top());
        painter2.drawImage(targRect, image, srcRect);

        painter2.end();

        image2.save(filename, 0, 90);
    }
    else
    {
        image.save(filename, 0, 90);
    }

    return filename;
}

/**
 * @brief Compute the total size of enabled monitors
 * @return  QRect
 */
QRect WallpaperGenerator::getDesktopEnabledRect()
{
    // compute enabled zone
    int minX=0, maxX=0, minY=0, maxY=0;

    for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
    {
        if (!m_settings->monitor(i).enabled)
        {
            continue;
        }

        QRect rect = m_enviro->wpSize(i);

        minX = qMin(minX, rect.left());
        minY = qMin(minY, rect.top());
        maxX = qMax(maxX, rect.left()+rect.width());
        maxY = qMax(maxY, rect.top()+rect.height());
    }

    return QRect(minX, minY, maxX-minX, maxY-minY);
}

/**
 * @brief Returns a unique key depending on monitors size, position, state and color
 * @param int _style
 * @return string
 */
QString WallpaperGenerator::getDesktopWallpaperKey(UM::IMAGE _style)
{
    QString str = QString::number(_style);

    for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
    {
        QRect screen = m_enviro->wpSize(i);
        Monitor monitor = m_settings->monitor(i);

        str+= QString::number(screen.left());
        str+= QString::number(screen.top());
        str+= QString::number(screen.width());
        str+= QString::number(screen.height());
        str+= QString::number(monitor.enabled);
        str+= QString::number(monitor.color);
    }

    return str;
}

/**
 * @brief Returns the cache path
 * @param string _originalFile
 * @param rect _rect
 * @param string _key
 * @return string
 */
QString WallpaperGenerator::getCacheFilename(const QString &_file, const QRect &_rect, const QString &_key1, const QString &_key2)
{
    uint32_t crc1 = crc32_1byte(_file.toStdString().c_str(), _file.size());
    uint32_t crc2 = crc32_1byte(_key1.toStdString().c_str(), _key1.size());

    return QDir::toNativeSeparators(QFileInfo(QString::fromAscii(APP_CACHE_DIR)).absoluteFilePath()+"/")
            + QString::number(crc1, 16) + "-"
            + QString::number(crc2, 16) + "-"
            + _key2 + "-"
            + QString::number(_rect.width()) + "x" + QString::number(_rect.height())
            + ".jpg";
}
