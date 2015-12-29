#ifndef MAIN_H
#define MAIN_H

#include <qt_windows.h>
#include <QtXml>
#include <QString>
#include <QVector>
#include <QsLog.h>

#include "constants.h"


namespace UM {
    // wallpaper arrangment
    enum WALLPAPER
    {
        W_DESKTOP = 0,
        W_MONITOR = 1,
        W_NONE = 16
    };

    // wallpaper layout
    enum IMAGE
    {
        IM_CENTER = 0,
        IM_TILE = 1,
        IM_STRETCH = 2,
        IM_STRETCH_PROP = 3,
        IM_FILL = 4,
        IM_CUSTOM = 15,
        IM_NONE = 16
    };

    // loop mode
    enum MODE
    {
        RANDOM = 0,
        SEQUENTIAL = 1,
        NONE = 16
    };

    // alignment
    enum ALIGN
    {
        ALIGN_TOP_LEFT = 0,
        ALIGN_TOP_CENTER = 1,
        ALIGN_TOP_RIGHT = 2,
        ALIGN_CENTER_LEFT = 3,
        ALIGN_CENTER_CENTER = 4,
        ALIGN_CENTER_RIGHT = 5,
        ALIGN_BOTTOM_LEFT = 6,
        ALIGN_BOTTOM_CENTER = 7,
        ALIGN_BOTTOM_RIGHT = 8
    };
}

// helper for monitors config
struct Monitor {
    bool enabled;
    COLORREF color;
    Monitor() : enabled(true), color(0x00000000) {}
};

Q_DECLARE_METATYPE(UM::WALLPAPER)
Q_DECLARE_METATYPE(UM::IMAGE)
Q_DECLARE_METATYPE(UM::MODE)

// flags for dialog without help button
const Qt::WindowFlags SimpleDialogFlag = Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint;

// common functions
void addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value);

#endif // MAIN_H
