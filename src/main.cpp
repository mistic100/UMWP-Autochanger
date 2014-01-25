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

    QApplication app(argc, argv);

    // default translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // app translations
    QTranslator appTranslator;
    appTranslator.load(":/lang/" + QLocale::system().name());
    app.installTranslator(&appTranslator);


    Settings settings;
    Controller ctrl(&settings);
    MainWindow window(&ctrl);

    window.init();

    int ret = app.exec();

    ReleaseMutex(hMutexHandle);
    CloseHandle(hMutexHandle);

    return ret;
}


/*
 * check if a file exists
 */
bool fileExists(QString const &_path, bool _checkWrite)
{
    DWORD dAtt = GetFileAttributesA(_path.toStdString().c_str());

    return dAtt!=INVALID_FILE_ATTRIBUTES
           && !(dAtt&FILE_ATTRIBUTE_DIRECTORY)
           && (!(dAtt&FILE_ATTRIBUTE_READONLY) || !_checkWrite);
}

/*
 * check if a directory exists
 */
bool directoryExists(const QString &_path)
{
    DWORD dAtt = GetFileAttributesA(_path.toStdString().c_str());
    return dAtt!=INVALID_FILE_ATTRIBUTES && (dAtt&FILE_ATTRIBUTE_DIRECTORY);
}

/*
 * helpers for simple XML text nodes
 */
void setDomNodeValue(QDomDocument* _pDoc, QDomNode* _pNode, const QString &_value)
{
    _pNode->appendChild(_pDoc->createTextNode(_value));
}

void addSimpleTextNode(QDomDocument* _poDoc, QDomNode* _poParent, const QString &_name, const QString &_value)
{
    QDomElement element = _poDoc->createElement(_name);
    setDomNodeValue(_poDoc, &element, _value);
    _poParent->appendChild(element);
}

/*
 * add trailling slash to a path if not present
 */
QString addTrailingSlash(const QString &_path)
{
    if (!_path.isEmpty() && !_path.endsWith('\\'))
    {
        return _path+"\\";
    }
    return _path;
}

/*
 * get dir name from path
 */
QString getDirectoryName(QString const &_path)
{
    return _path.section('\\', -1, -1, QString::SectionSkipEmpty);
}

/*
 * determine is a filename corresponds to an image (jpg, bmp, png, gif)
 */
bool isImageFile(QString const &_filename)
{
    return _filename.endsWith(".jpg", Qt::CaseInsensitive)
        || _filename.endsWith(".jpeg", Qt::CaseInsensitive)
        || _filename.endsWith(".bmp", Qt::CaseInsensitive)
        || _filename.endsWith(".png", Qt::CaseInsensitive)
        || _filename.endsWith(".gif", Qt::CaseInsensitive);
}
