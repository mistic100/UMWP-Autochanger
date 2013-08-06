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
    HANDLE hMutexHandle = CreateMutex( NULL, TRUE, L"com.strangeplanet.umwp_autochanger" );
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

    if (oData.iState() == UM_OK)
    {
        oWindow.vShowMain();
    }
    else
    {
        oWindow.vShowError();
    }


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
    DWORD att = GetFileAttributesA(_sPath.toStdString().c_str());
    if (_bCheckWrite)
        return att!=INVALID_FILE_ATTRIBUTES && !(att&FILE_ATTRIBUTE_READONLY) && !(att&FILE_ATTRIBUTE_DIRECTORY);
    else
        return att!=INVALID_FILE_ATTRIBUTES && !(att&FILE_ATTRIBUTE_DIRECTORY);
}

/*
 * check if a directory exists
 */
bool bDirectoryExists(QString const &_sPath)
{
    DWORD att = GetFileAttributesA(_sPath.toStdString().c_str());
    return att!=INVALID_FILE_ATTRIBUTES && (att&FILE_ATTRIBUTE_DIRECTORY);
}

/*
 * helpers for simple XML text nodes
 */
void setDomNodeValue(QDomDocument* _pDoc, QDomNode* _pNode, QString const &_value)
{
    _pNode->appendChild(_pDoc->createTextNode(_value));
}

void addSimpleTextNode(QDomDocument* _pDoc, QDomNode* _parent, QString const &_name, QString const &_value)
{
    QDomElement element = _pDoc->createElement(_name);
    setDomNodeValue(_pDoc, &element, _value);
    _parent->appendChild(element);
}

/*
 * add trailling slash to a path if not present
 */
void vAddTrailingSlash(QString* _psPath)
{
    if ( !_psPath->isEmpty() && !_psPath->endsWith('\\') )
    {
        _psPath->append('\\');
    }
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
