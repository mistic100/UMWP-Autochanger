#ifndef CUSTOMLAYOUT_H
#define CUSTOMLAYOUT_H

#include "main.h"


/**
 * @brief Structure holding the config of a custom layout
 */
struct CustomLayout {
    short rows;
    short cols;
    short maxRows;
    short maxCols;
    short minRows;
    short minCols;
    bool mainEnabled;
    short mainRows;
    short mainCols;
    UM::ALIGN mainPos;
    bool borderEnabled;
    bool borderScreenEnabled;
    short borderWidth;
    QRgb borderColor;
    bool perFolder;

    CustomLayout() :
        rows(4),
        cols(6),
        maxRows(2),
        maxCols(2),
        minRows(1),
        minCols(1),
        mainEnabled(true),
        mainRows(2),
        mainCols(4),
        mainPos(UM::ALIGN_CENTER_CENTER),
        borderEnabled(true),
        borderScreenEnabled(false),
        borderWidth(10),
        borderColor(Qt::black),
        perFolder(false)
    {}
};

#endif // CUSTOMLAYOUT_H

