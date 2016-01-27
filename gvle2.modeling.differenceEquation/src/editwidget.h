/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QTreeWidget>
#include <vle/gvle/vlepackage.h>
#include "vlesm.h"

namespace Ui {
class EditClass;
}

namespace vle {
namespace gvle {

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = 0);
    ~EditWidget();
    QString getClassName();
    void setClassName(QString name);
    void setModeNew();
    void setModeEdit();
    void setModeEdit2();
    void setParameters();
    void setCompute();
    void setPackage(vle::utils::Package *pkg)
    {mPkg = pkg;};
    void setSm(vleSm *sm)
    {mSm = sm;};
    vleSm* getSm()
    {return mSm;}
    void    addParameter(QString name, double v);
    bool    isValidParameterName(const QString parameterName) const;
    bool    isUniqParameterName(const QString parameterName) const;
    bool    allowClose();

public slots:
    void onModified();
    void onUndoRedoSm(QDomNode oldValSm, QDomNode newValSm);
    void onAddParameter();
    void onParameterModified(QString text);
    void onRemoveParameter();
    void onRenameSave();
    void onSaveClass();
    void onSaveParameter();
    void onSetCompute();
    void onSelectParameter(QTreeWidgetItem * current, QTreeWidgetItem * previous);

signals:
    void nameChanged(QString name);
    void saveClass();
    void newClass(QString name);

private:
    bool                saved;
    vleSm               * mSm;
    vle::utils::Package * mPkg;
    Ui::EditClass       * ui;
    QString             mClassName;
};

}}

#endif // EDITWIDGET_H
