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
#include "dummy_modeler.h"
#include "tabwidget.h"
#include <iostream>
#include "vle/vpz/AtomicModel.hpp"

ModelerDummy::ModelerDummy() {
    mLogger    = 0;
    mSettings  = 0;
    mWidgetTab = 0;
    mWidgetEdit.clear();
    mName = "Dummy";
}

ModelerDummy::~ModelerDummy() {
    // Nothing to do ...
}

/**
 * @brief ModelerDummy::getname
 *        Return the plugin name
 */
QString ModelerDummy::getname() {
    return mName;
}

/**
 * @brief ModelerDummy::setSettings
 *        Save a pointer to the configuration file wrapper
 */
void ModelerDummy::setSettings(QSettings *s)
{
    mSettings = s;
}

/**
 * @brief ModelerDummy::setLogger
 *        Save a pointer to the logger instance of main app
 */
void ModelerDummy::setLogger(Logger *logger)
{
    mLogger = logger;
}

bool ModelerDummy::useCustomMainTab()
{
    return false;
}

/**
 * @brief ModelerDummy::getWidget
 *        Create the plugin GUI (widget inserted into main app tab)
 */
QWidget *ModelerDummy::getMainTabWidget()
{
    // If the widget has already been allocated
    if (mWidgetTab)
        // return saved pointer
        return mWidgetTab;

    // Allocate (and return) a new Simulation Tab widget
    mWidgetTab = new TabWidget();
    return mWidgetTab;
}
void ModelerDummy::delWidget()
{
    return;
}

/**
 * @brief ModelerDummy::addNewWidget
 *        Create a new tab to create and configure a new class
 */
QWidget *ModelerDummy::addNewWidget()
{
    EditWidget *newTab = new EditWidget();
    newTab->setModeNew();
    mWidgetEdit.append(newTab);

    QObject::connect(newTab, SIGNAL(nameChanged(QString)),
                     this,   SLOT  (onNameChange(QString)));
    QObject::connect(newTab, SIGNAL(saveClass()),
                     this,   SLOT  (onSaveClass()));

    return newTab;
}

/**
 * @brief ModelerDummy::addEditWidget
 *        Create a new tab to edit an existing class
 */
QWidget *ModelerDummy::addEditWidget()
{
    EditWidget *newTab = new EditWidget();
    mWidgetEdit.append(newTab);
    return newTab;
}

void ModelerDummy::onNameChange(QString name)
{
    // Search the caller tab
    QObject *senderObject = QObject::sender();
    EditWidget *tabWidget = qobject_cast<EditWidget *>(senderObject);
    if (tabWidget == 0)
        return;

    tabWidget->setClassName(name);
    tabWidget->setModeEdit();

    // Forward this event to main app
    emit nameChanged(tabWidget, name);
}

void ModelerDummy::onSaveClass()
{
    QObject *senderObject = QObject::sender();
    EditWidget *tabWidget = qobject_cast<EditWidget *>(senderObject);
    if (tabWidget == 0)
        return;

    // Forward this event to main app
    emit saveClass(tabWidget->getClassName());
}

Q_EXPORT_PLUGIN2(modeler_dummy, ModelerDummy)
