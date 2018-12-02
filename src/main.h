#ifndef MAIN_H
#define MAIN_H

#include <qt_windows.h>
#include <QtCore>
#include <QString>
#include <QVector>
#include <QSize>
#include <QHash>
#include <QVariant>
#include <QColor>
#include <QRect>
#include <QtXml>
#include <QsLog.h>

#include "constants.h"

enum TRI_BOOL {
    UNKNOW_BOOL = -1,
    FALSE_BOOL = FALSE,
    TRUE_BOOL = TRUE
};

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
        MODE_RANDOM = 0,
        MODE_SEQUENTIAL = 1,
        MODE_NONE = 16
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

    // lock mode
    enum LOCK_TYPE
    {
        LOCK_DISABLED = 0,
        LOCK_ALL = 1,
        LOCK_SETS = 2
    };

    // helper for monitors config
    struct Monitor {
        bool enabled;
        QRgb color;
        Monitor() : enabled(true), color(Qt::black) {}
    };

    // Store data about new version
    struct NewVersion {
        QString code;
        QString link;
        QString hash;
    };

    // flags for dialog without help button
    const Qt::WindowFlags SimpleDialogFlag = Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint;

    // algorythm used for password hash
    const QCryptographicHash::Algorithm PasswordHash = QCryptographicHash::Sha512;

    // config keys
    namespace CONF {
        const QString window_width = "window_width";
        const QString window_height = "window_height";
        const QString delay = "delay";
        const QString minimize = "minimize";
        const QString check_updates = "check_updates";
        const QString msgcount = "msgcount";
        const QString close_warning = "close_warning";
        const QString use_hotkeys = "use_hotkeys";
        const QString show_notifications = "show_notifications";
        const QString last_dir = "last_dir";
        const QString language = "language";
        const QString changelog_shown = "changelog_shown";
        const QString open_program = "open_program";
        const QString lock_enabled = "lock_enabled";
        const QString lock_password = "lock_password";
        const QString lock_startup = "lock_startup";
        const QString lock_minimize = "lock_minimize";
        const QString lock_type = "lock_type";
        const QString ignore_update = "ignore_update";
        const QString default_set = "default_set";

        namespace HOTKEY {
            const QString refresh = "refresh";
            const QString startpause = "startpause";
            const QString showhide = "showhide";
            const QString delay = "delay";
            const QString lockunlock = "lockunlock";
        }
    }
}

Q_DECLARE_METATYPE(UM::WALLPAPER)
Q_DECLARE_METATYPE(UM::IMAGE)
Q_DECLARE_METATYPE(UM::MODE)
Q_DECLARE_METATYPE(UM::ALIGN)
Q_DECLARE_METATYPE(UM::Monitor)
Q_DECLARE_METATYPE(UM::NewVersion)

#endif // MAIN_H
