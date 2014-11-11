#-------------------------------------------------
#
# Project created by QtCreator 2014-06-23T00:27:11
#
#-------------------------------------------------
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

QT += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eCAD
TEMPLATE = app

INCLUDEPATH += \
    $$PWD/gui \
    $$PWD/gui/entities

SOURCES += \
    main.cpp\
    gui/mainwindow.cpp \
    gui/cadgraphicsscene.cpp \
    gui/entities/circle.cpp \
    gui/entities/ellipse.cpp \
    gui/entities/point.cpp \
    gui/entities/line.cpp \
    gui/cadgraphicsview.cpp \
    gui/entities/mtext.cpp

HEADERS += \
    gui/mainwindow.h \
    gui/cadgraphicsscene.h\
    gui/entities/circle.h \
    gui/entities/ellipse.h \
    gui/entities/point.h \
    gui/entities/line.h \
    gui/cadgraphicsview.h \
    gui/cadcommands/cadcommandadd.h \
    gui/cadcommands/cadcommanddelete.h \
    gui/cadcommands/cadcommandmove.h \
    gui/entities/mtext.h \
    gui/gEntity.h \
    gui/clipboard.h

FORMS += \
    resources/ui/mainwindow.ui

RESOURCES += \
    resources/images.qrc
