/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2015 INRA
 *
 */
#ifndef EXPCOND_DUMMY_H
#define EXPCOND_DUMMY_H

#include <QObject>
#include <QSettings>
#include <vle/gvle2/plugin_cond.h>
#include <vle/gvle2/logger.h>
#include <vle/gvle2/vlevpz.h>
#include <vle/gvle2/vpzexpcond.h>
#include "tab.h"
#include "toolbar.h"

class ExpCondDummy : public PluginExpCond
{
    Q_OBJECT
    Q_INTERFACES(PluginExpCond)

public:
    ExpCondDummy();
    ~ExpCondDummy();
    QString getname();
    QWidget *getWidget();
    void     delWidget();
    QWidget *getWidgetToolbar();
    void     delWidgetToolbar();
    void setExpCond(vpzExpCond *cond);
    void setSettings(QSettings *s);
    void setLogger(Logger *logger);
    void setVpz(vleVpz*vpz);
    void *getVpz();
    void setPackage(vle::utils::Package *pkg);

public slots:
    void onTabDeleted(QObject *obj);

private:
    QSettings     *mSettings;
    Logger        *mLogger;
    MainTab       *mWidgetTab;
    widToolbar    *mWidgetToolbar;
    vpzExpCond    *mExpCond;
};

#endif // EXPCOND_DUMMY_H
