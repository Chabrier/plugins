/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#include "editwidget.h"
#include "ui_editwidget.h"

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonRenameSave, SIGNAL(clicked()),
                     this,             SLOT  (onRenameSave()));
    QObject::connect(ui->buttonSave,   SIGNAL(clicked()),
                     this,             SLOT  (onSaveClass()));
}

EditWidget::~EditWidget()
{
    delete ui;
}

QString EditWidget::getClassName()
{
    return mClassName;
}

void EditWidget::setClassName(QString name)
{
    mClassName = name;
    // Update ui
    ui->labelEditName->setText(mClassName);
}

void EditWidget::setModeNew()
{
    ui->content->setEnabled(false);
    ui->lineEditName->setText(mClassName);
    ui->headerStack->setCurrentIndex(1);
}
void EditWidget::setModeEdit()
{
    ui->headerStack->setCurrentIndex(0);
    ui->content->setEnabled(true);
    ui->buttonSave->setEnabled(true);
}

void EditWidget::onRenameSave()
{
    emit nameChanged( ui->lineEditName->text() );
}

void EditWidget::onSaveClass()
{
    emit saveClass();
}
