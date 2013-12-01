#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

#include "main.h"
#include "mainwindow.h"
#include "controller.h"
#include "settings.h"


int main(int argc, char *argv[])
{
    HANDLE hMutexHandle = CreateMutex(NULL, TRUE, L"com.strangeplanet.umwp_autochanger");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    QApplication a(argc, argv);

    // default translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    // app translations
    QTranslator myappTranslator;
    myappTranslator.load(":/lang/" + QLocale::system().name());
    a.installTranslator(&myappTranslator);


    Settings oData;
    Controller oCtrl(&oData);
    MainWindow oWindow(&oCtrl);

    oWindow.vInit();

    int ret = a.exec();

    ReleaseMutex(hMutexHandle);
    CloseHandle(hMutexHandle);

    return ret;
}


/*
 * check if a file exists
 */
bool bFileExists(QString const &_sPath, bool _bCheckWrite)
{
    DWORD dAtt = GetFileAttributesA(_sPath.toStdString().c_str());

    return dAtt!=INVALID_FILE_ATTRIBUTES
           && !(dAtt&FILE_ATTRIBUTE_DIRECTORY)
           && (!(dAtt&FILE_ATTRIBUTE_READONLY) || !_bCheckWrite);
}

/*
 * check if a directory exists
 */
bool bDirectoryExists(const QString &_sPath)
{
    DWORD dAtt = GetFileAttributesA(_sPath.toStdString().c_str());
    return dAtt!=INVALID_FILE_ATTRIBUTES && (dAtt&FILE_ATTRIBUTE_DIRECTORY);
}

/*
 * helpers for simple XML text nodes
 */
void setDomNodeValue(QDomDocument* _poDoc, QDomNode* _poNode, const QString &_sValue)
{
    _poNode->appendChild(_poDoc->createTextNode(_sValue));
}

void addSimpleTextNode(QDomDocument* _poDoc, QDomNode* _poParent, const QString &_sName, const QString &_sValue)
{
    QDomElement element = _poDoc->createElement(_sName);
    setDomNodeValue(_poDoc, &element, _sValue);
    _poParent->appendChild(element);
}

/*
 * add trailling slash to a path if not present
 */
QString sAddTrailingSlash(const QString &_sPath)
{
    if (!_sPath.isEmpty() && !_sPath.endsWith('\\'))
    {
        return _sPath+"\\";
    }
    return _sPath;
}

/*
 * get dir name from path
 */
QString sGetDirName(QString const &_sPath)
{
    return _sPath.section('\\', -1, -1, QString::SectionSkipEmpty);
}

/*
 * determine is a filename corresponds to an image (jpg, bmp, png, gif)
 */
bool bIsImageFile(QString const &_sFilename)
{
    return _sFilename.endsWith(".jpg", Qt::CaseInsensitive)
        || _sFilename.endsWith(".jpeg", Qt::CaseInsensitive)
        || _sFilename.endsWith(".bmp", Qt::CaseInsensitive)
        || _sFilename.endsWith(".png", Qt::CaseInsensitive)
        || _sFilename.endsWith(".gif", Qt::CaseInsensitive);
}
