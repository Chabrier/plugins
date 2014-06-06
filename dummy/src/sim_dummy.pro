#
#  This file is part of VLE, a framework for multi-modeling, simulation
#  and analysis of complex dynamical systems.
#  http://www.vle-project.org
#
#  Copyright (c) 2014 INRA
#
QT       += core gui

TEMPLATE = lib

TARGET = sim_dummy

CONFIG += plugin
INCLUDEPATH += ../../
DESTDIR = ../
CONFIG += qtestlib

SOURCES += sim_dummy.cpp \
    simtab.cpp \
    widtoolbar.cpp
HEADERS += sim_dummy.h \
    simtab.h \
    widtoolbar.h

FORMS += \
    simtab.ui \
    widtoolbar.ui
