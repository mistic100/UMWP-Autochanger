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
    short borderWidth;
    COLORREF borderColor;

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
        borderWidth(3),
        borderColor(0x000000)
    {}
};

#endif // CUSTOMLAYOUT_H

