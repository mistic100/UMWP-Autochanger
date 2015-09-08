#ifndef MAIN_H
#define MAIN_H

#include <qt_windows.h>
#include <QtXml>
#include <QString>
#include <QVector>
#include <QsLog.h>

#include "constants.h"


namespace UM {
    // types for wallpaper file
    enum WALLPAPER
    {
        W_DESKTOP = 0,
        W_MONITOR = 1,
        W_NONE = 16
    };
    enum BACKGROUND
    {
        BG_SOLID = 0,
        BG_GRAD_HORZ = 1,
        BG_GRAD_VERT = 2
    };
    enum IMAGE
    {
        IM_CENTER = 0,
        IM_TILE = 1,
        IM_STRETCH = 2,
        IM_STRETCH_PROP = 3,
        IM_FILL = 4,
        IM_NONE = 16
    };
    struct WP_MONITOR_FILE
    {
        BACKGROUND bgType;
        COLORREF color1;
        COLORREF color2;
        IMAGE imgStyle;
        wchar_t imgFile[260];
    };

    // other enums
    enum MODE
    {
        RANDOM = 0,
        SEQUENTIAL = 1,
        NONE = 16
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
void setDomNodeValue(QDomDocument* _dom, QDomNode* _node, const QString &_value);
void addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value);

#endif // MAIN_H
