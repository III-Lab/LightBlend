QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

TARGET = LightBlend
TEMPLATE = app
DESTDIR = $$PWD/bin

INCLUDEPATH += $$PWD/items

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

include ($$PWD/python/python_thread.pri)

RC_ICONS = logo.ico

