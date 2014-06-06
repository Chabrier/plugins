/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#include <QObject>
#include <QtPlugin>
#include <QDebug>
#include "sim_dummy.h"
#include "simtab.h"
#include <iostream>
#include "vle/vpz/AtomicModel.hpp"

SimDummy::SimDummy() {
    mLogger    = 0;
    mSettings  = 0;
    mWidgetTab = 0;
    mWidgetToolbar = 0;
}

SimDummy::~SimDummy() {
    // Nothing to do ...
}

/**
 * @brief SimDummy::getname
 *        Return the plugin name
 */
QString SimDummy::getname() {
    QString name = "Dummy";
    return name;
}

/**
 * @brief SimDummy::setSettings
 *        Save a pointer to the configuration file wrapper
 */
void SimDummy::setSettings(QSettings *s)
{
    mSettings = s;
}

/**
 * @brief SimDummy::setLogger
 *        Save a pointer to the logger instance of main app
 */
void SimDummy::setLogger(Logger *logger)
{
    mLogger = logger;
}

/**
 * @brief SimDummy::setPackage
 *        Set the vle-package used for current simulation context
 */
void SimDummy::setPackage(vle::utils::Package *pkg)
{
    (void)pkg;
}

/**
 * @brief SimDummy::getWidget
 *        Create the plugin GUI (widget inserted into main app tab)
 */
QWidget *SimDummy::getWidget()
{
    // If the widget has already been allocated
    if (mWidgetTab)
        // return saved pointer
        return mWidgetTab;

    // Allocate (and return) a new Simulation Tab widget
    mWidgetTab = new SimTab();
    return mWidgetTab;
}

/**
 * @brief SimSpacial::delWidget
 *        Delete the main widget (when tab is closed without plugin unload)
 */
void SimDummy::delWidget()
{
    // If widget is not allocated, nothing to do
    if (mWidgetTab == 0)
        return;

    // Delete widget and clear pointer
    delete mWidgetTab;
    mWidgetTab = 0;
}

/**
 * @brief SimDummy::getWidgetToolbar
 *        Create the plugin GUI toolbox (widget inserted into right column)
 */
QWidget *SimDummy::getWidgetToolbar()
{
    if (mWidgetToolbar)
        return mWidgetToolbar;

    mWidgetToolbar = new widToolbar();
    return mWidgetToolbar;
}

/**
 * @brief SimSpacial::delWidgetToolbar
 *        Delete the toolbar widget (when tab is closed without plugin unload)
 */
void SimDummy::delWidgetToolbar()
{
    // If widget is not allocated, nothing to do
    if (mWidgetToolbar == 0)
        return;

    // Delete widget and clear pointer
    delete mWidgetToolbar;
    mWidgetToolbar = 0;
}

/**
 * @brief SimDummy::setVpz
 *        Set the VPZ package used for the simulation
 */
void SimDummy::setVpz(vleVpz *vpz)
{
    (void)vpz;
}

void *SimDummy::getVpz()
{
    return (void *)0;
}

Q_EXPORT_PLUGIN2(sim_dummy, SimDummy)
