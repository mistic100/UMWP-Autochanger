#ifndef MAIN_H

#include <Windows.h>
#include <QtXml>
#include <QString>
#include <QVector>

#include <QDebug>


#define APP_NAME "UMWP Autochanger"
#define APP_VERSION "1.2"
#define APP_CONFIG_FILE "settings.xml"
#define APP_WALLPAPER_FILE "AutoChanger.wallpaper"
#define APP_HOMEPAGE "http://www.strangeplanet.fr/work/umwp-autochanger"
#define APP_VERSION_URL L"http://www.strangeplanet.fr/work/umwp-autochanger/last-version.txt"


// state values
#define UM_OK 0
#define UM_NOT_INSTALLED 1
#define UM_BAD_VERSION 2
#define UM_FILE_NOT_FOUND 4
#define UM_SETTINGS_FILE_ERROR 8


// types for wallpaper file
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


// common functions
bool bFileExists(QString const &_sPath, bool _bCheckWrite=true);
bool bDirectoryExists(QString const &_sPath);

QString sGetDirName(const QString &_sPath);
void vAddTrailingSlash(QString* _psPath);
bool bIsImageFile(QString const &_sFilename);

void setDomNodeValue(QDomDocument* _pDoc, QDomNode* _pNode, QString const &_value);
void addSimpleTextNode(QDomDocument* _pDoc, QDomNode* _parent, QString const &_name, QString const &_value);

#define MAIN_H
#endif // MAIN_H
