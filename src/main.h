#ifndef MAIN_H

#include <Windows.h>
#include <QtXml>
#include <QString>
#include <QVector>
#include <QxtLogger>


#define APP_NAME "UMWP Autochanger"
#define APP_VERSION "1.4.1"
#define APP_MIN_UM_VERSION "3.2.0"
#define APP_CONFIG_FILE "settings.xml"
#define APP_CACHE_DIR "cache/"
#define APP_WALLPAPER_FILE "AutoChanger.wallpaper"
#define APP_HOMEPAGE "http://www.strangeplanet.fr/work/umwp-autochanger"
#define APP_VERSION_URL L"http://www.strangeplanet.fr/work/umwp-autochanger/last-version.txt"
#define APP_MAX_TRAVERSAL 3


// state values
namespace UMWP {
    const short OK = 0;
    const short NOT_INSTALLED = 1;
    const short BAD_VERSION = 2;
    const short FILE_NOT_FOUND = 4;
    const short SETTINGS_FILE_ERROR = 8;
    const short UNKNOWN_ERROR = 256;
}


// types for wallpaper file
namespace UM {
    enum WALLPAPER
    {
        W_DESKTOP = 0,
        W_MONITOR = 1
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
        IM_STRETCH_PROP = 3
//        IM_FILL = 4
    };
    struct WP_MONITOR_FILE
    {
        BACKGROUND bgType;
        COLORREF color1;
        COLORREF color2;
        IMAGE imgStyle;
        wchar_t imgFile[260];
    };
}

Q_DECLARE_METATYPE(UM::WALLPAPER)
Q_DECLARE_METATYPE(UM::IMAGE)


// common functions
bool directoryExists(const QString &_path);
bool isImageFile(const QString &_filename);

void setDomNodeValue(QDomDocument* _dom, QDomNode* _node, const QString &_value);
void addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value);

const QList<QVariant> hashToList(const QHash<QString, QVariant> &hash);
const QList<QVariant> hashToList(const QHash<QString, int> &hash);

#define MAIN_H
#endif // MAIN_H
