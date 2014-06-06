/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef SIM_DUMMY_H
#define SIM_DUMMY_H

#include <QObject>
#include <QSettings>
#include <vle/gvle2/plugin_sim.h>
#include <vle/gvle2/logger.h>
#include <vle/gvle2/vlevpz.h>
#include "simtab.h"
#include "widtoolbar.h"

class SimDummy : public QObject, public PluginSimulator
{
    Q_OBJECT
    Q_INTERFACES(PluginSimulator)

public:
    SimDummy();
    ~SimDummy();
    QString getname();
    QWidget *getWidget();
    void     delWidget();
    QWidget *getWidgetToolbar();
    void     delWidgetToolbar();
    void setSettings(QSettings *s);
    void setLogger(Logger *logger);
    void setVpz(vleVpz*vpz);
    void *getVpz();
    void setPackage(vle::utils::Package *pkg);

private:
    QString name;
    QSettings             *  mSettings;
    Logger                *  mLogger;
    SimTab                *  mWidgetTab;
    widToolbar            *  mWidgetToolbar;
};

#endif // SIM_DUMMY_H
