#
#  This file is part of VLE, a framework for multi-modeling, simulation
#  and analysis of complex dynamical systems.
#  http://www.vle-project.org
#
#  Copyright (c) 2014 INRA
#
QT       += core gui

TEMPLATE = lib

TARGET = modeler-difference-equation

CONFIG += plugin
INCLUDEPATH += ../../
DESTDIR = ../
CONFIG += qtestlib

SOURCES += modelerde.cpp \
    editwidget.cpp
HEADERS += modelerde.h \
    editwidget.h

FORMS += tabwidget.ui \
    editclass.ui
