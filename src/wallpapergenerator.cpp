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
 * @brief Get random files URI from a Set. Contains empty strings for disabled monitors.
 * @param Set* _set
 * @return string[]
 */
QVector<QString> WallpaperGenerator::getFiles(const Set* _set)
{
    QVector<QString> files;

    if (_set->type() == UM::W_MONITOR)
    {
        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                files.append(getRandomFile(_set, files));
            }
            else
            {
                files.append("");
            }
        }
    }
    else
    {
        files.append(getRandomFile(_set, files));
    }

    return files;
}

/**
 * @brief Get a random file within a Set
 * @param Set* _set
 * @param string[] _files
 * @return string
 */
QString WallpaperGenerator::getRandomFile(const Set* _set, const QVector<QString> &_files)
{
    int total = _set->count();

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
    // desktop mode with disabled monitors
    if (_set->type() == UM::W_DESKTOP)
    {
        bool has_disabled = false;

        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            has_disabled|= !m_settings->monitor(i).enabled;
        }

        if (has_disabled)
        {
            return adaptFileDesktopWithDisabled(_set, _files);
        }
    }

    // fill mode not handled by UltraMon
    if (_set->style() == UM::IM_FILL)
    {
        return adaptFilesFillMode(_set, _files);
    }

    return _files;

}

/**
 * @brief Create a new wallpaper file when we are in desktop mode with disabled monitors.
 * @param Set* _set
 * @param string[] _files (one element)
 * @return string[] (one element)
 */
QVector<QString> WallpaperGenerator::adaptFileDesktopWithDisabled(const Set* _set, const QVector<QString> &_files)
{
    QRect desktop = m_enviro->wpSize(-1);
    QPoint offset = -desktop.topLeft();

    QImage image(desktop.size(), QImage::Format_RGB32);
    image.fill(Qt::black);

    QPainter painter(&image);

    int minX=0, maxX=0, minY=0, maxY=0;

    for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
    {
        if (!m_settings->monitor(i).enabled)
        {
            continue;
        }

        // draw background color of enabled monitors
        QColor color((QRgb) m_settings->monitor(i).color);
        QRect rect = m_enviro->wpSize(i);

        painter.setBrush(QBrush(color));
        painter.drawRect(rect.translated(offset));

        // compute enabled zone
        minX = qMin(minX, rect.left());
        minY = qMin(minY, rect.top());
        maxX = qMax(maxX, rect.left()+rect.width());
        maxY = qMax(maxY, rect.top()+rect.height());
    }

    // copy image with appropriate mode
    QRect enabledRect = QRect(minX, minY, maxX-minX, maxY-minY).translated(offset);
    QImage source(_files.at(0));
    QRect sourceRect = QRect(QPoint(), source.size());

    switch (_set->style())
    {
    case UM::IM_CENTER:
    {
        QRect zoneTarget(0, 0,
                   qMin(enabledRect.width(), sourceRect.width()),
                   qMin(enabledRect.height(), sourceRect.height())
        );
        zoneTarget.moveCenter(enabledRect.center());

        QRect zoneSource = zoneTarget;
        zoneSource.moveCenter(sourceRect.center());

        painter.drawImage(zoneTarget, source, zoneSource);
        break;
    }
    case UM::IM_TILE:
    {
        QPoint pos = enabledRect.topLeft();

        do {
            painter.drawImage(pos, source);

            pos.rx()+= sourceRect.width();

            if (!enabledRect.contains(pos))
            {
                pos.setX(enabledRect.left());
                pos.ry()+= sourceRect.height();
            }
        }
        while (enabledRect.contains(pos));
        break;
    }
    case UM::IM_STRETCH:
    {
        painter.drawImage(enabledRect, source, sourceRect);
        break;
    }
    case UM::IM_STRETCH_PROP:
    {
        source = source.scaled(enabledRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        sourceRect = QRect(QPoint(), source.size());

        QRect zoneTarget(
                    (enabledRect.width()-sourceRect.width()) / 2,
                    (enabledRect.height()-sourceRect.height()) / 2,
                     sourceRect.width(),
                     sourceRect.height()
        );
        zoneTarget.translate(offset);

        painter.drawImage(zoneTarget, source, sourceRect);

        break;
    }
    case UM::IM_FILL:
    {
        source = source.scaled(enabledRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        sourceRect = QRect(QPoint(), source.size());

        QRect zoneSource(
                    (sourceRect.width()-enabledRect.width()) / 2,
                    (sourceRect.height()-enabledRect.height()) / 2,
                     enabledRect.width(),
                     enabledRect.height()
        );

        painter.drawImage(enabledRect, source, zoneSource);

        break;
    }
    }

    // draw color of disabled monitors
    for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
    {
        if (m_settings->monitor(i).enabled)
        {
            continue;
        }

        QColor color((QRgb) m_settings->monitor(i).color);
        QRect rect = m_enviro->wpSize(i).translated(offset);

        painter.setBrush(QBrush(color));
        painter.drawRect(rect);
    }

    // save
    QString cachePath = getCacheFilename(_files.at(0), desktop, _set->uuid()+"-"+getConfigurationKey(_set->style()));

    image.save(cachePath, 0, 90);

    return QVector<QString>()<<cachePath;
}

/**
 * @brief Resize images for custom FILL mode
 * @param Set* _set
 * @param string[] _files
 * @return string[]
 */
QVector<QString> WallpaperGenerator::adaptFilesFillMode(const Set* _set, const QVector<QString> &_files)
{
    QVector<QString> newFiles;

    for (int i=0, l=_files.size(); i<l; i++)
    {
        if (!_files.at(i).isEmpty())
        {
            // target size
            QRect size;
            if (_set->type() == UM::W_DESKTOP)
            {
                size = m_enviro->wpSize(-1);
            }
            else
            {
                size = m_enviro->wpSize(i);
            }

            QString cachePath = getCacheFilename(_files.at(i), size, _set->uuid());

            if (!QFile::exists(cachePath))
            {
                QImage image(_files.at(i));

                // if image ratio is almost the same, do not waste time in image cropping
                double curRatio = (double)image.size().width()/image.size().height();
                double targetRatio = (double)size.width()/size.height();

                if (qAbs(curRatio - targetRatio) < 0.02)
                {
                    newFiles.append(_files.at(i));
                    continue;
                }

                // scale
                image = image.scaled(size.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

                // crop
                int diffW = image.width()-size.width();
                int diffH = image.height()-size.height();
                image = image.copy(diffW/2, diffH/2, size.width(), size.height());

                image.save(cachePath, 0, 90);
            }

            newFiles.append(cachePath);
        }
        else
        {
            newFiles.append(_files.at(i));
        }
    }

    return newFiles;
}

/**
 * @brief Returns a unique key depending on monitors size, position, state and color
 * @param int _style
 * @return string
 */
QString WallpaperGenerator::getConfigurationKey(UM::IMAGE _style)
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

    uint32_t crc = crc32_1byte(str.toStdString().c_str(), str.size());

    return QString::number(crc, 16);
}

/**
 * @brief Returns the cache path
 * @param string _originalFile
 * @param rect _rect
 * @param string _key
 * @return string
 */
QString WallpaperGenerator::getCacheFilename(const QString &_originalFile, const QRect &_rect, const QString &_key)
{
    QFileInfo file(_originalFile);

    return QDir::toNativeSeparators(QFileInfo(QString::fromAscii(APP_CACHE_DIR)).absoluteFilePath()+"/")
            + file.completeBaseName() + "-" + _key + "-"
            + QString::number(_rect.width()) + "x" + QString::number(_rect.height())
            + "." + file.suffix();
}
