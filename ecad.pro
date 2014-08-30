#-------------------------------------------------
#
# Project created by QtCreator 2014-06-23T00:27:11
#
#-------------------------------------------------
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eCAD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    circle.cpp \
    ellipse.cpp \
    point.cpp \
    line.cpp \
    cadgraphicsscene.cpp

HEADERS  += mainwindow.h \
    circle.h \
    ellipse.h \
    point.h \
    line.h \
    cadgraphicsscene.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
