#-------------------------------------------------
#
# Project created by QtCreator 2014-06-23T00:27:11
#
#-------------------------------------------------

# eCAD.pro
#
# Copyright (C) 2014  GreatDevelopers
#
# eCAD is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# eCAD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

QT += core gui printsupport qml

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
    gui/entities/text.cpp \
    gui/cadscriptwidget.cpp \
    gui/cadcommandwidget.cpp \
    gui/entities/arc.cpp \
    gui/entities/image.cpp \
    gui/entities/arrow.cpp \
    gui/entities/dimhorizontal.cpp \
    gui/entities/dimvertical.cpp \
    gui/entities/dimradial.cpp \
    gui/entities/dimdiametric.cpp

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
    gui/entities/text.h \
    gui/cadscriptwidget.h \
    gui/cadcommandwidget.h \
    gui/getEntity.h \
    gui/clipboardstack.h \
    gui/entities/arc.h \
    gui/entities/image.h \
    gui/entities/arrow.h \
    gui/entities/dimhorizontal.h \
    gui/entities/dimvertical.h \
    gui/entities/dimradial.h \
    gui/entities/dimdiametric.h

FORMS += \
    resources/ui/mainwindow.ui \
    resources/ui/aboutdialog.ui

RESOURCES += \
    resources/images.qrc
