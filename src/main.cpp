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
    appTranslator.load(":/lang/" + QLocale::system().name() + "/main");
    app.installTranslator(&appTranslator);

    QDir dirHelper;
    if (!dirHelper.exists(APP_CACHE_DIR))
    {
        dirHelper.mkdir(APP_CACHE_DIR);
    }

    Settings settings;
    settings.init();
    if (qxtLog->isLogLevelEnabled("debug", QxtLogger::DebugLevel))
    {
        settings.log();
    }

    Controller ctrl(&settings);

    MainWindow window(&ctrl);
    window.init();

    int ret = app.exec();

    qxtLog->trace("Finished");

    ReleaseMutex(hMutexHandle);
    CloseHandle(hMutexHandle);

    return ret;
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
