#ifndef CUSTOMLAYOUTGENERATOR_H
#define CUSTOMLAYOUTGENERATOR_H

#include <random>

#include "main.h"
#include "customlayout.h"
#include "ext/variable2dmatrix.h"


/**
 * @brief An utility to generate tiles based on a CustomLayout
 */
class CustomLayoutGenerator
{
private:
    CustomLayout           m_conf;
    QList<QRect>           m_blocks;
    Variable2DMatrix<int>  m_matrix;

    std::mt19937           m_randomEngine; // mersenne_twister

public:
    CustomLayoutGenerator();

    QList<QRect> generate(const CustomLayout &_conf);

private:
    bool canFitBlock(int w, int h);
    bool canAddBlock(int x, int y, int w, int h);
    void addBlock(const QRect &_block);
    void usePool(QList<QRect> &_pool);
};

#endif // CUSTOMLAYOUTGENERATOR_H
