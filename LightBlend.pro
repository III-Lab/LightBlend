QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console release

TARGET = LightBlend
TEMPLATE = app
message("Current compiler: $$QMAKE_CC")
contains(QMAKE_CC, cl) {
message("Current compiler: MSVC")
    DESTDIR = $$PWD/msvc-build
}

contains(QMAKE_CC, gcc) {
message("Current compiler: MinGW")
    DESTDIR = $$PWD/mingw-build
}

OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/rcc
UI_DIR = $$DESTDIR/ui

INCLUDEPATH += $$PWD/items
include ($$PWD/python/python_thread.pri)

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    displaywidget.cpp \
    itemlabel.cpp \
    items/mitem.cpp \
    main.cpp \
    mainwindow.cpp \
    mscene.cpp \
    mview.cpp

HEADERS += \
    displaywidget.h \
    itemlabel.h \
    items/mitem.h \
    mainwindow.h \
    mscene.h \
    mview.h \
    reflect.h

FORMS += \
    mainwindow.ui

RC_ICONS = logo.ico

