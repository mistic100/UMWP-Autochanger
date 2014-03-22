#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QxtBasicFileLoggerEngine>

#include "main.h"
#include "mainwindow.h"
#include "controller.h"
#include "settings.h"

extern short UMWP_STATE = 0;


int main(int argc, char *argv[])
{
    HANDLE hMutexHandle = CreateMutex(NULL, TRUE, L"com.strangeplanet.umwp_autochanger");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    QApplication app(argc, argv);

    app.setQuitOnLastWindowClosed(false);

    // logger
    if (argc > 1 && std::string(argv[1]) == "--log")
    {
        QxtBasicFileLoggerEngine* debug = new QxtBasicFileLoggerEngine("debug.log");
        qxtLog->addLoggerEngine("debug", debug);
        qxtLog->enableAllLogLevels();
    }
    else
    {
        qxtLog->disableAllLogLevels();
    }

    qxtLog->trace("Starting =================================================");

    // default translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // app translations
    QTranslator appTranslator;
    appTranslator.load(":/lang/" + QLocale::system().name());
    app.installTranslator(&appTranslator);

    QDir dirHelper;
    if (!dirHelper.exists(APP_CACHE_DIR))
    {
        dirHelper.mkdir(APP_CACHE_DIR);
    }

    Settings settings;
    Controller ctrl(&settings);
    MainWindow window(&ctrl);

    window.init();

    int ret = app.exec();

    qxtLog->trace("Finished");

    ReleaseMutex(hMutexHandle);
    CloseHandle(hMutexHandle);

    return ret;
}


/**
 * @brief check if a directory exists
 * @param string _path
 * @return bool
 */
bool directoryExists(const QString &_path)
{
    DWORD att = GetFileAttributesA(_path.toStdString().c_str());
    return att!=INVALID_FILE_ATTRIBUTES && (att&FILE_ATTRIBUTE_DIRECTORY);
}

/**
 * @brief determine is a filename corresponds to an image (jpg, bmp, png, gif)
 * @param string_filename
 * @return bool
 */
bool isImageFile(QString const &_filename)
{
    return _filename.endsWith(".jpeg", Qt::CaseInsensitive)
        || _filename.endsWith(".jpg", Qt::CaseInsensitive)
        || _filename.endsWith(".bmp", Qt::CaseInsensitive)
        || _filename.endsWith(".png", Qt::CaseInsensitive)
        || _filename.endsWith(".gif", Qt::CaseInsensitive);
}


/*
 * helpers for simple XML text nodes
 */
void setDomNodeValue(QDomDocument* _dom, QDomNode* _node, const QString &_value)
{
    _node->appendChild(_dom->createTextNode(_value));
}

void addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value)
{
    QDomElement element = _dom->createElement(_name);
    setDomNodeValue(_dom, &element, _value);
    _parent->appendChild(element);
}

/*
 * helpers for logger
 */
const QList<QVariant> hashToList(const QHash<QString, QVariant> &hash)
{
    QList<QVariant> list;

    for (QHash<QString, QVariant>::const_iterator it=hash.begin(); it!=hash.end(); ++it)
    {
        list.push_back(it.key() +": "+ it.value().toString());
    }

    return list;
}

const QList<QVariant> hashToList(const QHash<QString, int> &hash)
{
    QList<QVariant> list;

    for (QHash<QString, int>::const_iterator it=hash.begin(); it!=hash.end(); ++it)
    {
        list.push_back(it.key() +": "+ QString::number(it.value()));
    }

    return list;
}

const QList<QVariant> hashToList(const QHash<int, QSize> &hash)
{
    QList<QVariant> list;

    for (QHash<int, QSize>::const_iterator it=hash.begin(); it!=hash.end(); ++it)
    {
        list.push_back(QString::number(it.key()) +": "+ QString::number(it.value().width()) +"x"+ QString::number(it.value().height()));
    }

    return list;
}
