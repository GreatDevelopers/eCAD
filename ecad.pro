#-------------------------------------------------
#
# Project created by QtCreator 2014-06-23T00:27:11
#
#-------------------------------------------------
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

QT += core gui printsupport x11extras opengl

#greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets

TARGET = eCAD
TEMPLATE = app

LIBS += -L/usr/local/lib -lOgreMain

DEPENDPATH += .

INCLUDEPATH += \
    $$PWD/gui \
    $$PWD/gui/entities \
    $$PWD/gui/ogre \
    /usr/include/OGRE

SOURCES += \
    main.cpp\
    gui/mainwindow.cpp \
    gui/cadgraphicsscene.cpp \
    gui/entities/circle.cpp \
    gui/entities/ellipse.cpp \
    gui/entities/point.cpp \
    gui/entities/line.cpp \
    gui/cadgraphicsview.cpp \
    gui/ogre/cadogrewidget.cpp \
    gui/ogre/cadogrebasewidget.cpp

HEADERS += \
    gui/mainwindow.h \
    gui/cadgraphicsscene.h\
    gui/entities/circle.h \
    gui/entities/ellipse.h \
    gui/entities/point.h \
    gui/entities/line.h \
    gui/cadgraphicsview.h \
    gui/ogre/cadogrewidget.h \
    gui/ogre/cadogrebasewidget.h \
    gui/ogre/eventhandler.h \
    gui/ogre/cadcameraman.h

FORMS += \
    resources/ui/mainwindow.ui

RESOURCES += \
    resources/images.qrc
