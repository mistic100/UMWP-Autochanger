QT += core gui xml network
CONFIG += qxt
QXT += core gui

TARGET = UMWP_Autochanger
TEMPLATE = app

RC_FILE = UMWP_Autochanger.rc


SOURCES += \
    main.cpp\
    set.cpp \
    controller.cpp \
    settings.cpp \
    environment.cpp \
    sysreader.cpp \
    gui/listdelegate.cpp \
    gui/mainwindow.cpp \
    gui/errorwidget.cpp \
    gui/mainwidget.cpp \
    gui/seteditdialog.cpp \
    gui/configdialog.cpp \
    gui/previewdialog.cpp \
    gui/newversiondialog.cpp \
    gui/menubar.cpp \
    gui/trayicon.cpp \
    gui/setcontextmenu.cpp \
    gui/screensdialog.cpp \
    wallpapergenerator.cpp

HEADERS += \
    ext/qlineedithotkey.h \
    ext/qtoolbarext.h \
    ext/qscreen.h \
    ext/qhotkeywidget.h \
    ext/qwidgetblinker.h \
    constants.h \
    main.h \
    set.h \
    controller.h \
    settings.h \
    environment.h \
    sysreader.h \
    versionchecker.h \
    listdelegate.h \
    listproxystyle.h \
    globalshortcut.h \
    gui/mainwindow.h \
    gui/errorwidget.h \
    gui/mainwidget.h \
    gui/seteditdialog.h \
    gui/configdialog.h \
    gui/previewdialog.h \
    gui/newversiondialog.h \
    gui/menubar.h \
    gui/trayicon.h \
    gui/setcontextmenu.h \
    gui/screensdialog.h \
    wallpapergenerator.h

FORMS += \
    ext/qhotkeywidget.ui \
    gui/mainwidget.ui \
    gui/seteditdialog.ui \
    gui/configdialog.ui \
    gui/newversiondialog.ui \
    gui/screensdialog.ui

RESOURCES += \
    UMWP_Autochanger.qrc

TRANSLATIONS += \
    lang/en_GB/main.ts \
    lang/fr_FR/main.ts

OTHER_FILES += \
    UMWP_Autochanger.rc
