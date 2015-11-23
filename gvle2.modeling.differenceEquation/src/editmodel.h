/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef EDITMODEL_H
#define EDITMODEL_H

#include <QTableWidgetItem>
#include <QWidget>
#include <vle/gvle2/vlevpz.h>

namespace Ui {
class editModel;
}

namespace vle {
namespace gvle2 {
class sourceCpp;

class EditModel : public QWidget
{
    Q_OBJECT

public:
    explicit EditModel(QWidget *parent = 0);
    ~EditModel();
//    void setModel(vleVpzModel *model);
//    vpzExpCond *getExpCond();

signals:
    void valueChanged();

public slots:
    void onSaveParameters();
    void onParameterChanged(QTableWidgetItem *item);

private:
    Ui::editModel *ui;
//    vleVpzModel   *mModel;
    sourceCpp     *mSource;
};

}}

#endif // EDITMODEL_H
