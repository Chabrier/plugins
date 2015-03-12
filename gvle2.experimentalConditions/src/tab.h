/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2015 INRA
 *
 */
#ifndef TAB_H
#define TAB_H

#include <QObject>
#include <QWidget>
#include <vle/gvle2/vpzexpcond.h>

namespace Ui {
class MainTab;
}

class MainTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainTab(QWidget *parent = 0);
    ~MainTab();
    void setExpCond(vpzExpCond *cond);

signals:
    void valueChanged(vpzExpCond *cond);

private slots:
    void dateSelected(QDate date);

private:
    Ui::MainTab *ui;
    vpzExpCond  *mExpCond;
private:
    vpzExpCondValue *mEcValueDay;
    vpzExpCondValue *mEcValueMonth;
    vpzExpCondValue *mEcValueYear;
};

#endif // TAB_H
