#include "customlayoutgenerator.h"

/**
 * @brief CustomLayoutGenerator::CustomLayoutGenerator
 */
CustomLayoutGenerator::CustomLayoutGenerator()
{
    std::random_device rd;
    m_randomEngine.seed(rd());
}

/**
 * @brief generates the tiles
 * @param CustomLayout* _conf
 * @return QRect[]
 */
QList<QRect> CustomLayoutGenerator::generate(const CustomLayout &_conf)
{
    if (QsLogging::Logger::instance().loggingLevel() != QsLogging::OffLevel)
    {
        QList<QString> log({
           "grid: " + QString::number(_conf.rows) + "x" + QString::number(_conf.cols),
           "tiles: " + QString::number(_conf.minRows) + "x" + QString::number(_conf.minCols) + " -> " + QString::number(_conf.maxRows) + "x" + QString::number(_conf.maxCols),
           "main: " + QString(_conf.mainEnabled ? "y" : "n") + " " + QString::number(_conf.mainRows) + "x" + QString::number(_conf.mainCols),
           "random: " + QString(_conf.variationEnabled ? "y" : "n") + " angle: " + QString::number(_conf.angleVariation) + " size: " + QString::number(_conf.sizeVariation) + " pos: " + QString::number(_conf.posVariation),
           "border: " + QString(_conf.borderEnabled ? "y" : "n") + " " + QString::number(_conf.borderWidth) + "px " + QColor(_conf.borderColor).name(),
           "shadow: " + QString(_conf.shadowEnabled ? "y" : "n") + " " + QString::number(_conf.shadowWidth) + "px " + QColor(_conf.shadowColor).name()
        });
        QLOG_DEBUG() << "== LAYOUT" << log;
    }

    if (_conf.cols < 1 || _conf.rows < 1 ||
            _conf.minCols > _conf.cols || _conf.maxCols > _conf.cols || _conf.minCols > _conf.maxCols ||
            _conf.minRows > _conf.rows || _conf.maxRows > _conf.rows || _conf.minRows > _conf.maxRows ||
            _conf.mainEnabled && (_conf.mainCols > _conf.cols || _conf.mainRows > _conf.rows)
            )
    {
        QLOG_FATAL()<<"CustomLayoutGenerator: Dimensions out of bounds";
        return QList<QRect>();
    }

    m_conf = _conf;

    m_matrix = Variable2DMatrix<int>(m_conf.cols, m_conf.rows);
    m_matrix.fill(0);

    m_blocks.clear();

    // total surface minus the main block if enabled
    int totalSurface = m_conf.cols * m_conf.rows;
    if (m_conf.mainEnabled)
    {
        totalSurface-= m_conf.mainCols * m_conf.mainRows;
    }

    // add the big item at defined position
    if (m_conf.mainEnabled)
    {
        QRect main(0, 0, m_conf.mainCols, m_conf.mainRows);

        // compute top position
        switch(m_conf.mainPos)
        {
        case UM::ALIGN_TOP_LEFT: case UM::ALIGN_TOP_CENTER: case UM::ALIGN_TOP_RIGHT:
            main.moveTop(0); break;

        case UM::ALIGN_CENTER_LEFT: case UM::ALIGN_CENTER_CENTER: case UM::ALIGN_CENTER_RIGHT:
            main.moveTop(qFloor((m_conf.rows - main.height()) / 2)); break;

        case UM::ALIGN_BOTTOM_LEFT: case UM::ALIGN_BOTTOM_CENTER: case UM::ALIGN_BOTTOM_RIGHT:
            main.moveTop(m_conf.rows - main.height()); break;
        }

        // compute left position
        switch(m_conf.mainPos)
        {
        case UM::ALIGN_TOP_LEFT: case UM::ALIGN_CENTER_LEFT: case UM::ALIGN_BOTTOM_LEFT:
            main.moveLeft(0); break;

        case UM::ALIGN_TOP_CENTER: case UM::ALIGN_CENTER_CENTER: case UM::ALIGN_BOTTOM_CENTER:
            main.moveLeft(qFloor((m_conf.cols - main.width()) / 2)); break;

        case UM::ALIGN_TOP_RIGHT: case UM::ALIGN_CENTER_RIGHT: case UM::ALIGN_BOTTOM_RIGHT:
            main.moveLeft(m_conf.cols - main.width()); break;
        }

        addBlock(main);
    }

    // fill the space with items as configured
    QList<QRect> pool;

    // create a pool of items that can fit in the remaining space
    // and with sizes bounded by the config
    for (int i=m_conf.minCols; i<=m_conf.maxCols; i++)
        for (int j=m_conf.minRows; j<=m_conf.maxRows; j++)
            if (canFitBlock(i, j) && !(i == m_conf.maxCols && j == m_conf.maxRows))
                pool.append(QRect(0, 0, i, j));

    // compute the surface occupied by all pool items
    int poolSurface = 0;
    foreach (const QRect &block, pool)
        poolSurface+= block.width() * block.height();

    // compute how many times each item must be duplicated in the pool
    // in order to fill the whole surface
    int nbPool = poolSurface==0 ? 0 : qCeil(totalSurface / poolSurface);

    // actually duplicate the items
    foreach (const QRect &block, pool)
        for (int k=0; k<nbPool; k++)
            pool.append(block);

    usePool(pool);

    // complete with smaller items
    if (m_conf.minCols != 1 || m_conf.minRows != 1)
    {
        pool.clear();

        // create a new pool of smaller items
        // from 1*1 to the minimum configured size
        for (int i=1; i<=m_conf.minCols; i++)
            for (int j=1; j<=m_conf.minRows; j++)
                if (!(i == m_conf.minCols && j == m_conf.minRows))
                    for (int k=0; k<=nbPool; k++)
                        pool.append(QRect(0, 0, i, j));

        usePool(pool);
    }

    // complete with 1*1 blocks
    for (int i = 0; i < m_conf.cols; i++)
        for (int j = 0; j < m_conf.rows; j++)
            if (m_matrix(i, j) == 0)
                addBlock(QRect(i, j, 1, 1));

    return m_blocks;
}

/**
 * @brief Test if a block can fit anywhere in the matrix
 * @param int w
 * @param int h
 * @return bool
 */
bool CustomLayoutGenerator::canFitBlock(int w, int h)
{
    for (int i=0; i <= m_conf.cols-w; i++)
        for (int j=0; j <= m_conf.rows-h; j++)
            if (canAddBlock(i, j, w, h))
                return true;

    return false;
}

/**
 * @brief Test if a block can fit at a particular position in the matrix
 * @param int x
 * @param int y
 * @param int w
 * @param int h
 * @return bool
 */
bool CustomLayoutGenerator::canAddBlock(int x, int y, int w, int h)
{
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            if (i >= m_conf.cols || j >= m_conf.rows)
                return false;
            else if (m_matrix(i, j) == 1)
                return false;

    return true;
}

/**
 * @brief Add a block to the matrix and the block list
 * @param QRect _block
 */
void CustomLayoutGenerator::addBlock(const QRect &_block)
{
    for (int i = _block.left(); i < _block.left() + _block.width(); i++)
        for (int j = _block.top(); j < _block.top() + _block.height(); j++)
            m_matrix(i, j) = 1;

    m_blocks.append(_block);
}

/**
 * @brief Randomize a pool of blocks and try to fit each item in the matrix
 * @param QRect[] _pool
 */
void CustomLayoutGenerator::usePool(QList<QRect> &_pool)
{
    if (_pool.size() > 0)
    {
        std::shuffle(_pool.begin(), _pool.end(), m_randomEngine);

        do {
            QRect block = _pool.first();

            for (int j = 0; j < m_conf.rows; j++)
                for (int i = 0; i < m_conf.cols; i++)
                    if (canAddBlock(i, j, block.width(), block.height()))
                    {
                        block.moveTo(i, j);
                        addBlock(block);
                        goto matrix_loop;
                    }

            matrix_loop:
            _pool.removeFirst();
        }
        while (_pool.size() > 0);
    }
}
