#include <ctime>
#include <QPainter>

#include "controller.h"
#include "wallpapergenerator.h"
#include "lib/Crc32.h"
#include "umutils.h"


/**
 * @brief WallpaperGenerator::WallpaperGenerator
 * @param Controller* _ctrl
 */
WallpaperGenerator::WallpaperGenerator(Controller* _ctrl) :
    QObject(_ctrl),
    m_enviro(_ctrl->enviro()),
    m_settings(_ctrl->settings())
{
    std::random_device rd;
    m_randomEngine.seed(rd());

    m_custGenerator = new CustomLayoutGenerator();
}

/**
 * @brief WallpaperGenerator::~WallpaperGenerator
 */
WallpaperGenerator::~WallpaperGenerator()
{
    delete m_custGenerator;
}

/**
 * @brief Main generation task. Called asynchronously from Controller
 * @param Set* _set
 * @return Result
 */
WallpaperGenerator::Result WallpaperGenerator::generate()
{
    QTime timer;
    timer.start();

    WallpaperGenerator::Result result;

    // get random files
    result.set = getRandomSet();

    if (result.set == NULL)
    {
        QLOG_WARN() << "No active set";
        return result;
    }

    QLOG_DEBUG() << "Current set:" << result.set->name() << "Type:" << result.set->type() << "Style:" << result.set->style();

    QVector<QString> tempFiles;
    if (result.set->style() == UM::IM_CUSTOM)
    {
        tempFiles = getCustomFiles(&result);
    }
    else
    {
        result.files += getFiles(&result);

        tempFiles = adaptFiles(result.set, result.files);
    }

    QLOG_DEBUG() << "Current files:" << result.files;

    QString filepath = generateFile(result.set, tempFiles);

    m_enviro->setWallpaper(filepath);

    QLOG_INFO() << "Wallpaper generated in" << timer.elapsed() << "milliseconds";

    return result;
}

/**
 * @brief Get a random Set among all active sets
 * @return Set*
 */
Set* WallpaperGenerator::getRandomSet()
{
    int nbSets = m_settings->nbActiveSets(true);

    if (nbSets == 0)
    {
        return NULL;
    }

    if (nbSets == 1)
    {
        return m_settings->activeSet(0, true);
    }

    // build list of total nb of walls
    QList<int> nbWalls({0});
    for (int i=0; i<nbSets; i++)
    {
        Set* set = m_settings->activeSet(i, true);
        int setWeight = qRound(set->nbFiles() * set->frequency());
        nbWalls.append(nbWalls.last() + setWeight);
    }

    // generate random number between 0 and total nb of walls
    std::uniform_int<int> unif(0, nbWalls.last()-1);
    int counter = unif(m_randomEngine);

    // choose the set containing the generated number
    for (int i=0; i<nbSets; i++)
    {
        if (counter >= nbWalls.at(i) && counter < nbWalls.at(i+1))
        {
            return m_settings->activeSet(i, true);
        }
    }

    // should never get here
    return NULL;
}

/**
 * @brief Get a random folder inside a set
 * @param Set* _set
 * @return string
 */
QString WallpaperGenerator::getRandomFolder(Set *_set)
{
    int nbFolders = _set->nbFolders();

    if (nbFolders == 0)
    {
        return QString();
    }

    if (nbFolders == 1)
    {
        return _set->folder(0);
    }

    std::uniform_int<int> unif(0, nbFolders-1);

    return _set->folder(unif(m_randomEngine));
}

/**
 * @brief Get files URI from a Set. Contains empty strings for disabled monitors.
 * @param Set* _set
 * @return string[]
 */
QVector<QString> WallpaperGenerator::getFiles(WallpaperGenerator::Result* _context)
{
    QVector<QString> files;

    if (_context->set->perFolder())
    {
        _context->folder = getRandomFolder(_context->set);
    }

    if (_context->set->type() == UM::W_DESKTOP)
    {
        if (_context->set->mode() == UM::MODE_RANDOM)
        {
            files.append(getRandomFile(_context, files));
        }
        else
        {
            files.append(getNextFile(_context->set));
        }
    }
    else
    {
        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                if (_context->set->mode() == UM::MODE_RANDOM)
                {
                    files.append(getRandomFile(_context, files));
                }
                else
                {
                    files.append(getNextFile(_context->set));
                }
            }
            else
            {
                files.append("");
            }
        }
    }

    if (_context->set->mode() == UM::MODE_SEQUENTIAL)
    {
        // save current file info
        _context->set->writeCache();
    }

    return files;
}

/**
 * @brief Get X files URI from a Set.
 * @param Set* _set
 * @param int _nb
 * @return
 */
QVector<QString> WallpaperGenerator::getFiles(WallpaperGenerator::Result* _context, int _nb)
{
    QVector<QString> files;

    if (_context->set->perFolder())
    {
        _context->folder = getRandomFolder(_context->set);
    }

    if (_context->set->mode() == UM::MODE_RANDOM)
    {
        for (int i=0; i<_nb; i++)
            files.append(getRandomFile(_context, files));
    }
    else
    {
        for (int i=0; i<_nb; i++)
            files.append(getNextFile(_context->set));

        // save current file info
        _context->set->writeCache();
    }

    return files;
}

/**
 * @brief Get next file in Set and store the loop state.
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

        if (index >= _set->nbFiles())
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
 * @brief Get a random file within a Set. Will try to return a file not already in the list.
 * @param Set* _set
 * @param string[] _files
 * @return string
 */
QString WallpaperGenerator::getRandomFile(WallpaperGenerator::Result* _context, const QVector<QString> &_files)
{
    int total = _context->set->perFolder() ? _context->set->nbFilesInFolder(_context->folder) : _context->set->nbFiles();

    if (total == 0)
    {
        return "";
    }

    // only one file in the set ?!
    if (total == 1)
    {
        return _context->set->perFolder() ? _context->set->fileInFolder(_context->folder, 0) : _context->set->file(0);
    }

    std::uniform_int<int> unif(0, total-1);

    // search a random unused file
    short loop = 10; // the collisions detection will only try 10 times
    QString file;

    // if all files are already used, disable collisions detection
    if (total <= _files.size())
    {
        loop = 1;
    }

    while (loop > 0)
    {
        loop--;

        int counter = unif(m_randomEngine);
        file = _context->set->perFolder() ? _context->set->fileInFolder(_context->folder, counter) : _context->set->file(counter);

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
QVector<QString> WallpaperGenerator::adaptFiles(Set* _set, const QVector<QString> &_files)
{
    QVector<QString> newFiles;

    if (_set->type() == UM::W_DESKTOP)
    {
        newFiles.append(adaptFileToMonitor(_files.at(0), -1, _set));
    }
    else
    {
        for (int i=0, l=_files.size(); i<l; i++)
        {
            if (!_files.at(i).isEmpty())
            {
                newFiles.append(adaptFileToMonitor(_files.at(i), i, _set));
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
 * @brief Get temporary files URI for custom layout set. Contains empty strings for disabled monitors.
 * @param Set* _set
 * @return string[]
 */
QVector<QString> WallpaperGenerator::getCustomFiles(WallpaperGenerator::Result* _context)
{
    QVector<QString> files;

    if (_context->set->type() == UM::W_DESKTOP)
    {
        files.append(generateCustomFile(-1, _context));
    }
    else
    {
        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                files.append(generateCustomFile(i, _context));
            }
            else
            {
                files.append("");
            }
        }
    }

    return files;
}

/**
 * @brief Generate one custom layout file in temp folder.
 * @param QRect _scrRect
 * @param Set* _set
 * @return string
 */
QString WallpaperGenerator::generateCustomFile(int _idx, WallpaperGenerator::Result* _context)
{
    QRect scrRect;
    if (_context->set->type() == UM::W_DESKTOP)
    {
        scrRect = QRect(QPoint(), getDesktopEnabledRect().size());
    }
    else
    {
        scrRect = QRect(QPoint(), m_enviro->wpSize(_idx).size());
    }

    CustomLayout layout = _context->set->custLayout();
    QList<QRect> rawBlocks = m_custGenerator->generate(layout);
    QList<QRect> blocks;

    // scale blocks to wallpaper size
    double wRatio =  scrRect.width() / (double) layout.cols;
    double hRatio =  scrRect.height() / (double) layout.rows;

    foreach (const QRect block, rawBlocks)
    {
        QRect newBlock = UM::scaledRect(block, wRatio, hRatio);

        if (qAbs(newBlock.left() - scrRect.width()) <= 3)
        {
            newBlock.setLeft(scrRect.width()-1);
        }

        if (qAbs(newBlock.bottom() - scrRect.height()) <= 3)
        {
            newBlock.setBottom(scrRect.height()-1);
        }

        blocks.append(newBlock);
    }

    // get necessary files from set
    QVector<QString> files = getFiles(_context, blocks.size());
    _context->files += files;

    QImage image(scrRect.size(), QImage::Format_RGB32);
    QPainter painter(&image);

    // draw each block
    for (int i=0, l=blocks.size(); i<l; i++)
    {
        QRect block = blocks.at(i);
        QImage source(files.at(i));

        source = source.scaled(block.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QRect srcRect = QRect(QPoint(), source.size());

        QRect zoneSource(
                    (srcRect.width()-block.width()) / 2,
                    (srcRect.height()-block.height()) / 2,
                     block.width(),
                     block.height()
        );

        painter.drawImage(block, source, zoneSource);
    }

    // draw borders
    if (layout.borderEnabled)
    {
        QPen pen;
        pen.setColor(QColor(layout.borderColor));
        pen.setJoinStyle(Qt::MiterJoin);
        pen.setWidth(layout.borderWidth);
        painter.setPen(pen);

        foreach (const QRect block, blocks)
        {
            foreach (const QLine line, UM::rectBorders(block))
            {
                if (
                        (line.x1()==0 && line.x2()==0) ||
                        (line.x1()==scrRect.width()-1 && line.x2()==scrRect.width()-1) ||
                        (line.y1()==0 && line.y2()==0) ||
                        (line.y1()==scrRect.height()-1 && line.y2()==scrRect.height()-1)
                        )
                {
                    // do not draw extreme borders
                }
                else
                {
                    painter.drawLine(line);
                }
            }
        }

        if (layout.borderScreenEnabled)
        {
            QRect borderRect(pen.width()/2, pen.width()/2, scrRect.width()-pen.width(), scrRect.height()-pen.width());
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(borderRect);
        }
    }

    painter.end();

    // save in temp folder
    QString filename = getCustLayoutTempFilename(_idx, _context->set);

    image.save(filename, 0, 100);

    return filename;
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
QString WallpaperGenerator::adaptFileToMonitor(const QString &_file, int _idx, Set* _set)
{
    QString key;
    QRect wpRect;

    if (_set->type() == UM::W_DESKTOP)
    {
        key = getDesktopWallpaperKey(_set->style());

        wpRect.setSize(getDesktopEnabledRect().size());
    }
    else
    {
        key = QString::number(_set->style());
        key+= QString::number(m_settings->monitor(_idx).color);

        wpRect.setSize(m_enviro->wpSize(_idx).size());
    }

    QString cachePath = getCacheFilename(_file, wpRect, key, _set->uuid());

    if (QFile::exists(cachePath))
    {
        return cachePath;
    }

    QImage source(_file);
    QRect srcRect(QPoint(), source.size());

    QLOG_DEBUG() << "Resizing image. Screen:" << wpRect.size() << "Image:" << srcRect.size();

    if (_set->style() == UM::IM_STRETCH) // stretching is done when generating the final wallpaper
    {
        return _file;
    }
    else if (_set->style() == UM::IM_CENTER || _set->style() == UM::IM_TILE) // for tile and center styles, compare the sizes
    {
        if (qAbs(srcRect.width() - wpRect.width()) < 5 && qAbs(srcRect.height() - wpRect.height()) < 5)
        {
            return _file;
        }
    }
    else // for every other styles, compare the ratios
    {
        double imageRatio = (double)srcRect.width() / srcRect.height();
        double screenRatio = (double)wpRect.width() / wpRect.height();

        if (qAbs(imageRatio - screenRatio) < 0.02)
        {
            return _file;
        }
    }

    QImage image(wpRect.size(), QImage::Format_RGB32);
    QPainter painter(&image);

    // draw background color of enabled monitors
    if (_set->type() == UM::W_DESKTOP)
    {
        image.fill(Qt::black);

        QPoint offset = getDesktopEnabledRect().topLeft();

        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                QColor color(m_settings->monitor(i).color);
                QRect rect = m_enviro->wpSize(i).translated(-offset);

                painter.setBrush(QBrush(color));
                painter.drawRect(rect);
            }
        }
    }
    else
    {
        QColor color(m_settings->monitor(_idx).color);
        image.fill(color);
    }

    // draw image depending on style
    switch (_set->style())
    {
    case UM::IM_CENTER:
    {
        QRect zoneTarget(0, 0,
                         qMin(wpRect.width(), srcRect.width()),
                         qMin(wpRect.height(), srcRect.height())
                         );
        zoneTarget.moveCenter(wpRect.center());

        QRect zoneSource = zoneTarget;
        zoneSource.moveCenter(srcRect.center());

        painter.drawImage(zoneTarget, source, zoneSource);

        break;
    }
    case UM::IM_TILE:
    {
        QPoint pos(0,0);

        do {
            painter.drawImage(pos, source);

            pos.rx()+= srcRect.width();

            if (!wpRect.contains(pos))
            {
                pos.setX(0);
                pos.ry()+= srcRect.height();
            }
        }
        while (wpRect.contains(pos));

        break;
    }
    case UM::IM_STRETCH:
    {
        source = source.scaled(wpRect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawImage(QPoint(), source);
        break;
    }
    case UM::IM_STRETCH_PROP:
    {
        source = source.scaled(wpRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        srcRect = QRect(QPoint(), source.size());

        QRect zoneTarget(
                    (wpRect.width()-srcRect.width()) / 2,
                    (wpRect.height()-srcRect.height()) / 2,
                     srcRect.width(),
                     srcRect.height()
                    );

        painter.drawImage(zoneTarget, source, srcRect);

        break;
    }
    case UM::IM_FILL:
    {
        source = source.scaled(wpRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        srcRect = QRect(QPoint(), source.size());

        QRect zoneSource(
                    (srcRect.width()-wpRect.width()) / 2,
                    (srcRect.height()-wpRect.height()) / 2,
                     wpRect.width(),
                     wpRect.height()
                    );

        painter.drawImage(wpRect, source, zoneSource);

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
QString WallpaperGenerator::generateFile(Set *_set, const QVector<QString> &_files)
{
    QRect rect = m_enviro->wpSize(-1);
    QPoint offset = rect.topLeft();

    QImage image(rect.size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    image.fill(Qt::black);

    // draw each image
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
            QColor color(m_settings->monitor(i).color);
            QRect wpRect = m_enviro->wpSize(i).translated(-offset);

            painter.setBrush(QBrush(color));
            painter.drawRect(wpRect);
        }
    }

    painter.end();

    QString filename = QDir::toNativeSeparators(QDir::tempPath() + "/" + APP_WALLPAPER_FILE);

    // shift the tile if desktop origin is not at wallpaper origin
    // only for Windows 7/8
    if (offset != QPoint(0,0) && QSysInfo::WindowsVersion < QSysInfo::WV_WINDOWS10)
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

        image2.save(filename, 0, 100);
    }
    else
    {
        image.save(filename, 0, 100);
    }

    return filename;
}

/**
 * @brief Compute the total size of enabled monitors
 * @return  QRect
 */
QRect WallpaperGenerator::getDesktopEnabledRect()
{
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
        UM::Monitor monitor = m_settings->monitor(i);

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

    return QDir::toNativeSeparators(Environment::APPDATA_DIR + APP_CACHE_DIR + "/")
            + QString::number(crc1, 16) + "-"
            + QString::number(crc2, 16) + "-"
            + _key2 + "-"
            + QString::number(_rect.width()) + "x" + QString::number(_rect.height())
            + ".jpg";
}

/**
 * @brief Return a temp file path for a custom layout
 * @param QRect _rect
 * @param Set* _set
 * @return string
 */
QString WallpaperGenerator::getCustLayoutTempFilename(int _idx, Set* _set)
{
    return QDir::toNativeSeparators(QDir::tempPath() + "/")
            + APP_CUSTOM_PREFIX
            + _set->uuid() + "-"
            + QString::number(_idx)
            + ".bmp";
}
