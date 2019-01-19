QT += core gui widgets xml network concurrent

DEFINES += NOMINMAX
CONFIG += c++11

TARGET = UMWP_Autochanger
TEMPLATE = app

RC_FILE = UMWP_Autochanger.rc

include(QsLog/QsLog.pri)

SOURCES += \
    ext/qglobalshortcut.cpp \
    ext/qxtspanslider.cpp \
    main.cpp \
    set.cpp \
    controller.cpp \
    settings.cpp \
    environment.cpp \
    wallpapergenerator.cpp \
    umutils.cpp \
    customlayoutgenerator.cpp \
    gui/listdelegate.cpp \
    gui/mainwindow.cpp \
    gui/seteditdialog.cpp \
    gui/configdialog.cpp \
    gui/previewdialog.cpp \
    gui/newversiondialog.cpp \
    gui/menubar.cpp \
    gui/trayicon.cpp \
    gui/setcontextmenu.cpp \
    gui/screensdialog.cpp \
    gui/statusbar.cpp \
    gui/mainlist.cpp \
    gui/customlayoutdialog.cpp \
    gui/delaydialog.cpp \
    directoryscanner.cpp

HEADERS += \
    ext/qlineedithotkey.h \
    ext/qhotkeywidget.h \
    ext/qwidgetblinker.h \
    ext/qcomboboxext.h \
    ext/qglobalshortcut.h \
    ext/qlineeditcolor.h \
    ext/variable2dmatrix.h \
    ext/qxtspanslider.h \
    ext/qbuttongroupext.h \
    constants.h \
    main.h \
    set.h \
    controller.h \
    settings.h \
    environment.h \
    versionchecker.h \
    globalshortcut.h \
    wallpapergenerator.h \
    customlayout.h \
    customlayoutgenerator.h \
    umutils.h \
    gui/listdelegate.h \
    gui/listproxystyle.h \
    gui/mainwindow.h \
    gui/seteditdialog.h \
    gui/configdialog.h \
    gui/previewdialog.h \
    gui/newversiondialog.h \
    gui/menubar.h \
    gui/trayicon.h \
    gui/setcontextmenu.h \
    gui/screensdialog.h \
    gui/changelogdialog.h \
    gui/statusbar.h \
    gui/mainlist.h \
    gui/customlayoutdialog.h \
    gui/delaydialog.h \
    gui/previewwidget.h \
    ext/qtabwidgetext.h \
    ext/qchecklist.h \
    directoryscanner.h

FORMS += \
    gui/seteditdialog.ui \
    gui/configdialog.ui \
    gui/newversiondialog.ui \
    gui/screensdialog.ui \
    gui/changelogdialog.ui \
    gui/customlayoutdialog.ui \
    gui/delaydialog.ui

RESOURCES += \
    UMWP_Autochanger.qrc

TRANSLATIONS += \
    lang/en_GB/main.ts \
    lang/fr_FR/main.ts \
    lang/ru_RU/main.ts

OTHER_FILES += \
    UMWP_Autochanger.rc
