#ifndef MAIN_H

#include <Windows.h>
#include <QtXml>
#include <QString>
#include <QVector>

#include <QDebug>


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
#define UM_OK 0
#define UM_NOT_INSTALLED 1
#define UM_BAD_VERSION 2
#define UM_FILE_NOT_FOUND 4
#define UM_SETTINGS_FILE_ERROR 8
#define UM_UNKNOWN_ERROR 256


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


// common functions
bool fileExists(const QString &_path, bool _checkWrite=true);
bool directoryExists(const QString &_path);
QString getDirectoryName(const QString &_path);
QString addTrailingSlash(const QString &_path);
bool isImageFile(const QString &_filename);

void setDomNodeValue(QDomDocument* _pDoc, QDomNode* _pNode, const QString &_value);
void addSimpleTextNode(QDomDocument* _pDoc, QDomNode* _pParent, const QString &_name, const QString &_value);

#define MAIN_H
#endif // MAIN_H
