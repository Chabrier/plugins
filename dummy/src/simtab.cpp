/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#include "simtab.h"
#include "ui_simtab.h"

SimTab::SimTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimTab)
{
    ui->setupUi(this);
}

SimTab::~SimTab()
{
    delete ui;
}
