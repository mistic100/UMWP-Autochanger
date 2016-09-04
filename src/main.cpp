#include <QApplication>
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

extern Q_CORE_EXPORT QBasicAtomicInt qt_qhash_seed;

int main(int argc, char *argv[])
{
    // ensure only one running instance
    HANDLE hMutexHandle = CreateMutex(NULL, TRUE, APP_MUTEX);
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    // lock QHash seed in order to have a deterministic attributes order in the XML settings file
    qputenv("QT_HASH_SEED", "0");
    qt_qhash_seed.store(0);

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


    // init
    Environment enviro;

    Settings settings(&enviro);
    settings.load();

    QString language = settings.param(UM::CONF::language).toString();

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
