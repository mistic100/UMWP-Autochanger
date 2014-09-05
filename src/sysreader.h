#ifndef SYSREADER_H
#define SYSREADER_H

#include <QHash>
#include <QVariant>

#include "main.h"
#include "ext/qscreen.h"

/**
 * @brief Utilities to access environnement configuration
 */
class SysReader
{
public:
    static QString searchUMDexe(bool &_ok);
    static QString getUMversion(bool &_ok);
    static QString buildBMPpath(bool &_ok);
    static QString buildShortcutPath();

    static QString buildUMwallpaperPath(const QString &_version, bool &_ok);
    static void queryMonitors(QHash<int, QScreen> &_sizes, QByteArray &_header, bool &_ok);
};

#endif // SYSREADER_H
