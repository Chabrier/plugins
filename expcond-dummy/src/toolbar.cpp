/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2015 INRA
 *
 */
#include "toolbar.h"
#include "ui_toolbar.h"

widToolbar::widToolbar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widToolbar)
{
    ui->setupUi(this);

}

widToolbar::~widToolbar()
{
    delete ui;
}
