/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef WIDTOOLBAR_H
#define WIDTOOLBAR_H

#include <QObject>
#include <QWidget>

namespace Ui {
class widToolbar;
}

class widToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit widToolbar(QWidget *parent = 0);
    ~widToolbar();

private:
    Ui::widToolbar *ui;
};

#endif // WIDTOOLBAR_H
