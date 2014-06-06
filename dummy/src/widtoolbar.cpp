/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#include "widtoolbar.h"
#include "ui_widtoolbar.h"

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
