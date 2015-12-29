#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QsLog.h>
#include <QsLogDest.h>

#include "main.h"
#include "gui/mainwindow.h"
#include "controller.h"
#include "settings.h"
#include "environment.h"


int main(int argc, char *argv[])
{
    // ensure only one running instance
    HANDLE hMutexHandle = CreateMutex(NULL, TRUE, L"com.strangeplanet.umwp_autochanger");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    // logger
    QsLogging::Logger& logger = QsLogging::Logger::instance();

    if (argc > 1 && std::string(argv[1]) == "--log")
    {
        logger.setLoggingLevel(QsLogging::TraceLevel);

        logger.addDestination(QsLogging::DestinationFactory::MakeFileDestination(
                                  APP_LOG_FILENAME,
                                  QsLogging::EnableLogRotation,
                                  QsLogging::MaxSizeBytes(1024*1024*5)
                                  ));
        logger.addDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
    }
    else
    {
        logger.setLoggingLevel(QsLogging::OffLevel);
    }

    QLOG_TRACE() << "Starting =================================================";

    // create cache dir
    QDir dirHelper;
    if (!dirHelper.exists(APP_CACHE_DIR))
    {
        dirHelper.mkdir(APP_CACHE_DIR);
    }


    // init
    Settings settings;
    settings.load();

    Environment enviro(&settings);

    QString language = settings.get("language").toString();

    // default translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // app translations
    QTranslator appTranslator;
    appTranslator.load(":/lang/" + language + "/main");
    app.installTranslator(&appTranslator);


    // core
    Controller ctrl(&settings, &enviro);

    MainWindow window(&ctrl);
    window.init();

    ctrl.checkVersion();

    // fire update with delay
    QTimer::singleShot(500, &ctrl, SLOT(update()));

    // end
    int ret = app.exec();

    QLOG_TRACE() << "Finished";

    ReleaseMutex(hMutexHandle);
    CloseHandle(hMutexHandle);

    return ret;
}


/*
 * helper for simple XML text nodes
 */
void addSimpleTextNode(QDomDocument* _dom, QDomNode* _parent, const QString &_name, const QString &_value)
{
    QDomElement element = _dom->createElement(_name);
    element.appendChild(_dom->createTextNode(_value));
    _parent->appendChild(element);
}
