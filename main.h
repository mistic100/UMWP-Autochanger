#ifndef MAIN_H

#include <Windows.h>
#include <string>
#include <QtXml>
#include <QString>
#include <QVector>


#define APP_NAME "UMWP Autochanger"
#define APP_VERSION "1.0"
#define APP_CONFIG_FILE "settings.xml"
#define APP_WALLPAPER_FILE "AutoChanger.wallpaper"


// state values
#define UM_OK 0
#define UM_NOT_INSTALLED 1
#define UM_BAD_VERSION 2
#define UM_FILE_NOT_FOUND 4


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
bool bFileExists(const std::string &_sPath, bool _bCheckWrite=true);
bool bDirectoryExists(const std::string &_sPath);

QString sGetDirName(const QString _sPath);
void vAddTrailingSlash(QString* _psPath);
bool bIsImageFile(const QString &_sFilename);

void setDomNodeValue(QDomDocument *_pDoc, QDomNode* _pNode, const QString &_value);
void addSimpleTextNode(QDomDocument* _pDoc, QDomNode* _parent, const QString &_name, const QString &_value);

#define MAIN_H
#endif // MAIN_H
