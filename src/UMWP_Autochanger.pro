#-------------------------------------------------
#
# Project created by QtCreator 2013-02-13T09:38:53
#
#-------------------------------------------------

QT += core gui xml
CONFIG += qxt
QXT += core gui

TARGET = UMWP_Autochanger
TEMPLATE = app

RC_FILE = UMWP_Autochanger.rc


SOURCES += \
    main.cpp\
    mainwindow.cpp \
    controller.cpp \
    set.cpp \
    listdelegate.cpp \
    settings.cpp \
    errorwidget.cpp \
    mainwidget.cpp \
    seteditdialog.cpp \
    configdialog.cpp \
    qlineedithotkey.cpp \
    qtoolbarext.cpp \
    previewdialog.cpp \
    sysreader.cpp \
    reader.cpp

HEADERS += \
    lib/createshortcut.h \
    lib/UltraMon.h \
    mainwindow.h \
    controller.h \
    set.h \
    main.h \
    listdelegate.h \
    settings.h \
    errorwidget.h \
    versionchecker.h \
    listproxystyle.h \
    mainwidget.h \
    seteditdialog.h \
    configdialog.h \
    qlineedithotkey.h \
    globalshortcut.h \
    qtoolbarext.h \
    previewdialog.h \
    constants.h \
    widgetblinker.h \
    sysreader.h \
    qscreen.h

RESOURCES += \
    UMWP_Autochanger.qrc

TRANSLATIONS += \
    lang/en_GB/main.ts \
    lang/fr_FR/main.ts

OTHER_FILES += \
    UMWP_Autochanger.rc

FORMS += \
    mainwidget.ui \
    seteditdialog.ui \
    configdialog.ui \
    previewdialog.ui
