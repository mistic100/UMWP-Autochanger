QT += core gui widgets xml network

DEFINES += NOMINMAX
CONFIG += c++11

TARGET = UMWP_Autochanger
TEMPLATE = app

RC_FILE = UMWP_Autochanger.rc

include(QsLog/QsLog.pri)

SOURCES += \
    ext/qglobalshortcut.cpp \
    main.cpp \
    set.cpp \
    controller.cpp \
    settings.cpp \
    environment.cpp \
    gui/listdelegate.cpp \
    gui/mainwindow.cpp \
    gui/mainwidget.cpp \
    gui/customstyledialog.cpp \
    gui/seteditdialog.cpp \
    gui/configdialog.cpp \
    gui/previewdialog.cpp \
    gui/newversiondialog.cpp \
    gui/menubar.cpp \
    gui/trayicon.cpp \
    gui/setcontextmenu.cpp \
    gui/screensdialog.cpp \
    wallpapergenerator.cpp \
    customlayoutgenerator.cpp

HEADERS += \
    ext/qlineedithotkey.h \
    ext/qtoolbarext.h \
    ext/qhotkeywidget.h \
    ext/qwidgetblinker.h \
    ext/qcomboboxext.h \
    ext/qglobalshortcut.h \
    ext/qlineeditcolor.h \
    constants.h \
    main.h \
    set.h \
    controller.h \
    settings.h \
    environment.h \
    versionchecker.h \
    gui/listdelegate.h \
    gui/listproxystyle.h \
    globalshortcut.h \
    gui/mainwindow.h \
    gui/mainwidget.h \
    gui/customstyledialog.h \
    gui/seteditdialog.h \
    gui/configdialog.h \
    gui/previewdialog.h \
    gui/newversiondialog.h \
    gui/menubar.h \
    gui/trayicon.h \
    gui/setcontextmenu.h \
    gui/screensdialog.h \
    wallpapergenerator.h \
    gui/changelogdialog.h \
    customlayout.h \
    customlayoutgenerator.h \
    ext/variable2dmatrix.h

FORMS += \
    gui/customstyledialog.ui \
    gui/seteditdialog.ui \
    gui/configdialog.ui \
    gui/newversiondialog.ui \
    gui/screensdialog.ui \
    gui/changelogdialog.ui

RESOURCES += \
    UMWP_Autochanger.qrc

TRANSLATIONS += \
    lang/en_GB/main.ts \
    lang/fr_FR/main.ts

OTHER_FILES += \
    UMWP_Autochanger.rc
