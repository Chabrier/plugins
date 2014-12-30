/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef MODELER_DE_H
#define MODELER_DE_H

#include <QObject>
#include <QSettings>
#include <vle/gvle2/sourcecpp.h>
#include <vle/gvle2/vpzexpcond.h>
#include <vle/gvle2/vlevpz.h>
#include <vle/gvle2/plugin_modeler.h>
#include <vle/gvle2/logger.h>
#include "editwidget.h"
#include "editmodel.h"

class ModelerDifferenceEquation : public PluginModeler
{
    Q_OBJECT
    Q_INTERFACES(PluginModeler)

public:
    ModelerDifferenceEquation();
    ~ModelerDifferenceEquation();
    QString getname();
    QString getData(QString className);
public:
    bool     useCustomMainTab();
    QWidget *addNewWidget();
    QWidget *addEditWidget(sourceCpp *src);
    QWidget *getEditClass(sourceCpp *src);
    void     closeEditClass(sourceCpp *src);
    QWidget *addEditModel(vleVpzModel *model);
    void     initExpCond(vpzExpCond *exp, sourceCpp *src);
    void setSettings(QSettings *s);
    void setLogger(Logger *logger);

public slots:
    void onNameChange(QString name);
    void onSaveClass();
    void onEditDeleted(QObject *obj);
signals:
    void nameChanged(QWidget *tab, QString name);
    void saveClass(QString name);

private:
    QString             mName;
    QSettings         * mSettings;
    Logger            * mLogger;
    QList<EditWidget *> mWidgetEdit;
};

#endif // MODELER_DE_H
