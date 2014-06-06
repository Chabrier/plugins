/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef SIMTAB_H
#define SIMTAB_H

#include <QObject>
#include <QWidget>

namespace Ui {
class SimTab;
}

class SimTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit SimTab(QWidget *parent = 0);
    ~SimTab();

private:
    Ui::SimTab *ui;
};

#endif // SIMTAB_H
