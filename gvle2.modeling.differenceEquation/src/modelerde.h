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
#include <vle/gvle2/plugin_modeler.h>
#include <vle/gvle2/logger.h>
#include "editwidget.h"
#include "editmodel.h"
#include "vlesm.h"

namespace vle {
namespace gvle2 {

class sourceCpp;

class ModelerDifferenceEquation : public PluginModeler
{
    Q_OBJECT
    Q_INTERFACES(vle::gvle2::PluginModeler)

public:
    ModelerDifferenceEquation();
    ~ModelerDifferenceEquation();
    QString getname();
    QString getData(QString className);
    QString getData(vleSm* sm);
public:
    bool     useCustomMainTab();
    QWidget *openNewClass();
    QWidget *openEditClass(sourceCpp *src);
    QWidget *getEditClass(sourceCpp *src);
    void     closeEditClass(sourceCpp *src);
    //QWidget *addEditModel(vleVpzModel *model);
    //void     initExpCond(vpzExpCond *exp, sourceCpp *src);
    void rename(QString oldName, QString newName);
    void setSettings(QSettings *s);
    void setLogger(Logger *logger);
    void setPackage(vle::utils::Package *pkg);
    void undo(QWidget *w)
    {  qobject_cast<EditWidget *>(w)->getSm()->undo(); };
    void redo(QWidget *w)
    {  qobject_cast<EditWidget *>(w)->getSm()->redo(); };
    void save(QWidget *w)
    {  qobject_cast<EditWidget *>(w)->onSaveClass(); };
    bool allowClose(QWidget *w)
    {  return qobject_cast<EditWidget *>(w)->allowClose(); };
    void cloneSrc(const QString from, const QString to);


protected:
public slots:
    void onNameChange(QString name);
    void onSaveClass();
    void onEditDeleted(QObject *obj);
    void onModelChanged();
signals:
    void nameChanged(QWidget *tab, QString name);
    void saveClass(QString name);
    //void expCondChanged(vpzExpCond *exp);

private:
    vle::utils::Package * mPkg;
    QString             mName;
    QSettings           * mSettings;
    Logger              * mLogger;
    QList<EditWidget *> mWidgetEdit;
};

}}

#endif // MODELER_DE_H
