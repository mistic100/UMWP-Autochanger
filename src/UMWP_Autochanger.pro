#-------------------------------------------------
#
# Project created by QtCreator 2013-02-13T09:38:53
#
#-------------------------------------------------

QT += core gui xml

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
    versionchecker.cpp \
    listproxystyle.cpp \
    mainwidget.cpp

HEADERS += \
    mainwindow.h \
    controller.h \
    dirent.h \
    set.h \
    main.h \
    listdelegate.h \
    settings.h \
    errorwidget.h \
    createshortcut.h \
    versionchecker.h \
    listproxystyle.h \
    mainwidget.h

RESOURCES += \
    UMWP_Autochanger.qrc

TRANSLATIONS += \
    lang/en.ts \
    lang/fr.ts

OTHER_FILES += \
    UMWP_Autochanger.rc

FORMS += \
    mainwidget.ui