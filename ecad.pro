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
    $$PWD/gui/entities \
    $$PWD/gui/commands

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
    gui/commands/cadcommandadd.h \
    gui/commands/cadcommanddelete.h \
    gui/commands/cadcommandmove.h \
    gui/entities/mtext.h

FORMS += \
    resources/ui/mainwindow.ui

RESOURCES += \
    resources/images.qrc
