#include <ctime>
#include <QPainter>

#include "controller.h"
#include "wallpapergenerator.h"
#include "lib/Crc32.h"
#include "umutils.h"

QT_BEGIN_NAMESPACE
    extern Q_GUI_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
QT_END_NAMESPACE


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

    Set* tempSet = getRandomSet();

    if (tempSet == NULL)
    {
        QLOG_WARN() << "No active set";
        return result;
    }

    result.type = tempSet->type();

    QVector<QString> tempFiles;

    if (result.type == UM::W_MONITOR)
    {
        for (int i=0, l=m_enviro->nbMonitors(); i<l; i++)
        {
            if (m_settings->monitor(i).enabled)
            {
                Set* set = getRandomSet(i, result.sets);

                if (set == NULL)
                {
                    QLOG_WARN() << "No active set on monitor" << i;

                    result.sets.append(NULL);
                    tempFiles.append("");
                }
                else
                {
                    result.sets.append(getRandomSet(i, result.sets));
                    tempFiles.append(getFile(i, &result));
                }
            }
            else
            {
                result.sets.append(NULL);
                tempFiles.append("");
            }
        }
    }
    else
    {
        result.sets.append(tempSet);
        tempFiles.append(getFile(0, &result));
    }

    m_enviro->setWallpaper(generateFile(tempFiles, &result));
    cleanCustLayoutTemp();

    QLOG_DEBUG() << "Current type:" << result.type;
    QLOG_DEBUG() << "Current sets:" << result.sets;
    QLOG_DEBUG() << "Current files:" << result.files;
    QLOG_INFO() << "Wallpaper generated in" << timer.elapsed() << "milliseconds";

    return result;
}

/**
 * @brief Get a random Set among all active sets
 * @param int _monitor -1 to search on all monitors
 * @param Set*[] _existingSets for collision detection
 * @return Set*
 */
Set* WallpaperGenerator::getRandomSet(const int _monitor, const QVector<Set*> &_existingSets)
{
    QVector<Set*> sets;

    for (int i = 0; i < m_settings->nbSets(); i++)
    {
        Set* set = m_settings->set(i);

        // the set must be
        //      - active
        //      - valid
        //      - with files
        //      - active on a monitor if _monitor is provided
        if (set->isActive() && set->isValid() && set->nbFiles() > 0 && (_monitor == -1 || set->type() == UM::W_MONITOR && set->isActiveOnMonitor(_monitor)))
        {
            sets.append(set);
        }
    }

    int nbSets = sets.size();

    if (nbSets == 0)
    {
        return NULL;
    }

    if (nbSets == 1)
    {
        return sets.at(0);
    }

    // build list of total nb of walls * frequency
    QList<int> nbWalls({0});
    for (int i=0; i<nbSets; i++)
    {
        Set* set = sets.at(i);

        int setWeight = qRound(set->nbFiles() * set->frequency());
        if (set->type() == UM::W_DESKTOP && set->style() != UM::IM_CUSTOM)
        {
            setWeight*= m_settings->nbEnabledMonitors();
        }

        nbWalls.append(nbWalls.last() + setWeight);
    }

    // generate random number between 0 and total weight
    std::uniform_int<int> unif(0, nbWalls.last()-1);

    // search a random unused set
    short loop = 5; // the collisions detection will only try 5 times
    Set* set = NULL;

    // if all sets are already used, disable collisions detection
    if (nbSets <= _existingSets.size())
    {
        loop = 1;
    }

    while (loop > 0)
    {
        loop--;

        int counter = unif(m_randomEngine);
        set = NULL;

        // choose the set containing the generated number
        for (int i=0; i<nbSets; i++)
        {
            if (counter >= nbWalls.at(i) && counter < nbWalls.at(i+1))
            {
                set = sets.at(i);
                break;
            }
        }

        // should never get here
        if (set == NULL)
        {
            return set;
        }

        if (!_existingSets.contains(set))
        {
            loop = 0;
        }
    }

    return set;
}

/**
 * @brief Get a file for set at position _idx
 * @param int _idx
 * @param Result* _context
 * @return string
 */
QString WallpaperGenerator::getFile(int _idx, Result* _context)
{
    Set* set = _context->sets.at(_idx);

    if (set->style() == UM::IM_CUSTOM)
    {
        return generateCustomFile(_idx, _context);
    }
    else
    {
        return getAndAdaptFile(_idx, _context);
    }
}

/**
 * @brief Get a random folder inside a set
 * @param Set* _set
 * @return string
 */
QString WallpaperGenerator::getRandomFolder(Set* _set)
{
    int nbFolders = _set->nbFolders();

    if (nbFolders == 0)
    {
        return "";
    }

    if (nbFolders == 1)
    {
        return _set->folder(0);
    }

    std::uniform_int<int> unif(0, nbFolders-1);
    int counter = unif(m_randomEngine);

    return _set->folder(counter);
}

/**
 * @brief Get a file in set a position _idx and adapt it for the monitor
 * @param int _idx
 * @param Result* _context
 * @return string
 */
QString WallpaperGenerator::getAndAdaptFile(int _idx, Result* _context)
{
    Set* set =  _context->sets.at(_idx);

    QString file;

    if (set->mode() == UM::MODE_RANDOM)
    {
        file = getRandomFile(set);
    }
    else
    {
        file = getNextFile(set);
    }

    _context->files.append(file);

    if (file.isEmpty())
    {
        return "";
    }
    else
    {
        return adaptFileToMonitor(file, _idx, _context);
    }
}

/**
 * @brief Get X files from a set
 * @param Set* _set
 * @param int _nb
 * @param Result* _context
 * @return string[]
 */
QVector<QString> WallpaperGenerator::getFilesForCustom(Set* _set, int _nb, Result* _context)
{
    QVector<QString> files;

    CustomLayout layout = _set->custLayout();

    if (_set->mode() == UM::MODE_RANDOM)
    {
        QString folder;
        if (layout.perFolder)
        {
            folder = getRandomFolder(_set);
        }

        for (int i=0; i<_nb; i++)
        {
            files.append(getRandomFile(_set, folder, files));
        }

        if (layout.perFolder)
        {
            _context->files.append(folder);
        }
        else
        {
            _context->files += files;
        }
    }
    else
    {
        for (int i=0; i<_nb; i++)
        {
            files.append(getNextFile(_set));
        }

        _context->files += files;
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
 * @brief Get a random file within a Set.
 * @param Set* _set
 * @param string _folder optional
 * @return string
 */
QString WallpaperGenerator::getRandomFile(Set* _set, const QString &_folder, const QVector<QString> &_files)
{
    int totalFiles = _set->nbFilesInFolder(_folder);

    if (totalFiles == 0)
    {
        return "";
    }

    // only one file in the set ?!
    if (totalFiles == 1)
    {
        return _set->fileInFolder(_folder, 0);
    }

    std::uniform_int<int> unif(0, totalFiles-1);

   // search a random unused file
   short loop = 10; // the collisions detection will only try 10 times
   QString file;

   // if all files are already used, disable collisions detection
   if (totalFiles <= _files.size())
   {
       loop = 1;
   }

   while (loop > 0)
   {
       loop--;

       int counter = unif(m_randomEngine);
       file = _set->fileInFolder(_folder, counter);

       if (!_files.contains(file))
       {
           loop = 0;
       }
   }

   return file;
}

/**
 * @brief Generate one custom layout file for the set at position _idx
 * @param int _idx
 * @param Result* _context
 * @return string
 */
QString WallpaperGenerator::generateCustomFile(int _idx, WallpaperGenerator::Result* _context)
{
    Set* set = _context->sets.at(_idx);

    QRect scrRect;
    if (_context->type == UM::W_DESKTOP)
    {
        scrRect = QRect(QPoint(), getDesktopEnabledRect().size());
    }
    else
    {
        scrRect = QRect(QPoint(), m_enviro->wpSize(_idx).size());
    }

    CustomLayout layout = set->custLayout();
    QList<QRect> rawBlocks = m_custGenerator->generate(layout);
    QList<QRect> blocks;

    if (rawBlocks.empty())
    {
        return "";
    }

    // scale blocks to wallpaper size
    double wRatio =  scrRect.width() / (double) layout.cols;
    double hRatio =  scrRect.height() / (double) layout.rows;

    foreach (const QRect &block, rawBlocks)
    {
        QRect newBlock = UM::scaledRect(block, wRatio, hRatio);

        // make sure the rect touch the border of the screen
        if (qAbs(newBlock.right() - scrRect.width()) <= 3)
        {
            newBlock.setRight(scrRect.width()-1);
        }

        if (qAbs(newBlock.bottom() - scrRect.height()) <= 3)
        {
            newBlock.setBottom(scrRect.height()-1);
        }

        blocks.append(newBlock);
    }

    // get necessary files from set
    QVector<QString> files = getFilesForCustom(set, blocks.size(), _context);

    QLOG_DEBUG()<<"Custom sources:"<<files;

    // init random engines
    if (layout.variationEnabled)
    {
        std::shuffle(blocks.begin(), blocks.end(), m_randomEngine);
    }

    std::normal_distribution<double> randomAngle(0.0, 0.3 + 30.0 * layout.angleVariation);
    std::normal_distribution<double> randomSize(0.2 * layout.sizeVariation, 0.002 + 0.2 * layout.sizeVariation);
    std::normal_distribution<double> randomPos(0.0, 0.001 + 0.1 * layout.posVariation);

    // init painter
    QImage image(scrRect.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // draw background
    QColor monitorColor = QColor(m_settings->monitor(_idx).color);
    painter.setBrush(QBrush(monitorColor));
    painter.drawRect(scrRect);

    QPen borderPen(Qt::NoPen);
    QBrush shadowBrush(Qt::NoBrush);

    if (layout.borderEnabled)
    {
        borderPen.setStyle(Qt::SolidLine);
        borderPen.setColor(QColor(layout.borderColor));
        borderPen.setJoinStyle(Qt::MiterJoin);
        borderPen.setWidth(layout.borderWidth / 2);
    }

    if (layout.shadowEnabled)
    {
        QColor color(layout.shadowColor);
        color.setAlpha(10);
        shadowBrush.setStyle(Qt::SolidPattern);
        shadowBrush.setColor(color);
    }

    // draw each block
    for (int i=0, l=blocks.size(); i<l; i++)
    {
        QRect block = blocks.at(i);
        QImage source(files.at(i));

        // alter the block
        double angle = 0;
        QPoint offset;
        if (layout.variationEnabled)
        {
            int newWidth = block.width() * (1 + randomSize(m_randomEngine));
            int newHeight = block.height() * (1 + randomSize(m_randomEngine));
            int newLeft = block.left() + (block.width() - newWidth) / 2 + randomPos(m_randomEngine) * newWidth;
            int newTop = block.top() + (block.height() - newHeight) / 2 + randomPos(m_randomEngine) * newHeight;
            angle = randomAngle(m_randomEngine);

            block = QRect(newLeft, newTop, newWidth, newHeight);
            offset = block.center();

            block.translate(-offset);
        }

        // draw the drop shadow (from QPixmapDropShadowFilter implementation)
        if (layout.shadowEnabled)
        {
            QImage tmp(scrRect.size(), QImage::Format_ARGB32_Premultiplied);
            tmp.fill(0);

            QPainter tmpPainter(&tmp);

            if (layout.variationEnabled)
            {
                tmpPainter.translate(offset);
                tmpPainter.rotate(angle);
            }

            tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
            tmpPainter.setBrush(QBrush(Qt::white));
            tmpPainter.drawRect(block);
            tmpPainter.end();

            QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
            blurred.fill(0);

            tmpPainter.begin(&blurred);
            qt_blurImage(&tmpPainter, tmp, layout.shadowWidth, false, true);

            tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            tmpPainter.fillRect(tmp.rect(), QColor(layout.shadowColor));
            tmpPainter.end();

            painter.drawImage(QPoint(), blurred);
        }

        // transform the painter
        if (layout.variationEnabled)
        {
            painter.translate(offset);
            painter.rotate(angle);
        }

        // draw the image
        source = source.scaled(block.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QRect srcRect = QRect(QPoint(), source.size());

        QRect zoneSource(
                    (srcRect.width()-block.width()) / 2,
                    (srcRect.height()-block.height()) / 2,
                     block.width(),
                     block.height()
                    );

        painter.drawImage(block, source, zoneSource);

        // draw the borders
        if (layout.borderEnabled)
        {
            // 1px offset to overlap image limit
            QRect borderRect = block.adjusted(
                        borderPen.width()/2 - 1,
                        borderPen.width()/2 - 1,
                        - borderPen.width()/2 + 1,
                        - borderPen.width()/2 + 1
                        );

            painter.setPen(borderPen);
            painter.setBrush(Qt::NoBrush);

            painter.drawRect(borderRect);
        }

        painter.resetTransform();
    }

    painter.end();

    // save in temp folder
    QString filename = getCustLayoutTempFilename(_idx, set);

    image.save(filename, 0, 100);

    return filename;
}

/**
 * @brief Compute an image to fit in the monitor with a particular style, with cache management
 * @param string _file
 * @param int _idx
 * @param Result* _context
 * @return string
 */
QString WallpaperGenerator::adaptFileToMonitor(const QString &_file, int _idx, Result* _context)
{
    Set* set = _context->sets.at(_idx);

    QString key = QString::number(set->style());
    QRect wpRect;

    if (_context->type == UM::W_DESKTOP)
    {
        key+= getDesktopWallpaperKey();

        wpRect.setSize(getDesktopEnabledRect().size());
    }
    else
    {
        key+= QString::number(m_settings->monitor(_idx).color);

        wpRect.setSize(m_enviro->wpSize(_idx).size());
    }

    QString cachePath = getCacheFilename(_file, wpRect, key, set->uuid());

    if (QFile::exists(cachePath))
    {
        return cachePath;
    }

    QImage source(_file);
    QRect srcRect(QPoint(), source.size());

    QLOG_DEBUG() << "Resizing image. Screen:" << wpRect.size() << "Image:" << srcRect.size();

    if (set->style() == UM::IM_STRETCH) // stretching is done when generating the final wallpaper
    {
        return _file;
    }
    else if (set->style() == UM::IM_CENTER || set->style() == UM::IM_TILE) // for tile and center styles, compare the sizes
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
    if (set->type() == UM::W_DESKTOP)
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
    switch (set->style())
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
 * @param string[] _files
 * @param Result* _context
 * @return string
 */
QString WallpaperGenerator::generateFile(const QVector<QString> &_files, Result* _context)
{
    QRect rect = m_enviro->wpSize(-1);
    QPoint offset = rect.topLeft();

    QImage image(rect.size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    image.fill(Qt::black);

    // draw each image
    if (_context->type == UM::W_DESKTOP)
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
 * @return QRect
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
        maxX = qMax(maxX, rect.left() + rect.width());
        maxY = qMax(maxY, rect.top() + rect.height());
    }

    return QRect(minX, minY, maxX-minX, maxY-minY);
}

/**
 * @brief Returns a unique key depending on monitors size, position, state and color
 * @return string
 */
QString WallpaperGenerator::getDesktopWallpaperKey()
{
    QString str;

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
 * @param int _idx
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

/**
 * @brief Deletes all temp files for custom layouts
 */
void WallpaperGenerator::cleanCustLayoutTemp()
{
    QDir cache = QDir::tempPath();
    QStringList files = cache.entryList(QStringList()<<QString(APP_CUSTOM_PREFIX)+"*", QDir::Files);

    foreach (QString file, files)
    {
        QFile::remove(cache.absoluteFilePath(file));
    }
}
