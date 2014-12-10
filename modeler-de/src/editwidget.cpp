/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#include "editwidget.h"
#include "ui_editclass.h"

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditClass)
{
    ui->setupUi(this);
    ui->parameterEditBox->setEnabled(false);

    QObject::connect(ui->buttonRenameSave, SIGNAL(clicked()),
                     this,             SLOT  (onRenameSave()));
    QObject::connect(ui->buttonSave,   SIGNAL(clicked()),
                     this,             SLOT  (onSaveClass()));
    // Parameters widgets
    QObject::connect(ui->parametersList,  SIGNAL(currentRowChanged(int)),
                     this,                SLOT  (onSelectParameter(int)));
    QObject::connect(ui->buttonAddParameter, SIGNAL(clicked()),
                     this,                SLOT  (onAddParameter()));
    QObject::connect(ui->buttonRemoveParameter, SIGNAL(clicked()),
                     this,                SLOT  (onRemoveParameter()));
    QObject::connect(ui->buttonParameter, SIGNAL(clicked()),
                     this,                SLOT  (onSaveParameter()));
    QObject::connect(ui->parameterName,  SIGNAL(textChanged(QString)),
                     this,               SLOT  (onParameterModified(QString)));
    QObject::connect(ui->parameterValue, SIGNAL(textChanged(QString)),
                     this,               SLOT  (onParameterModified(QString)));
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

void EditWidget::addParameter(QString name, double v)
{
    // Insert a new parameter at the top position
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(name);
    item->setData(Qt::UserRole, QVariant(v));
    ui->parametersList->insertItem(0, item);
    // then, select it
    ui->parametersList->setCurrentRow(0);
}

void EditWidget::onRenameSave()
{
    emit nameChanged( ui->lineEditName->text() );
}

void EditWidget::onSaveClass()
{
    emit saveClass();
}

/**
 * @brief EditWidget::onSelectParameter (slot)
 *        Called when an item is selected into parameter list
 *
 */
#include <QDebug>
void EditWidget::onSelectParameter(int row)
{
    ui->buttonParameter->setEnabled(false);
    if (row < 0)
    {
        ui->parameterEditBox->setEnabled(false);
        return;
    }
    // Get the seleted item from list
    QListWidgetItem *item = ui->parametersList->item(row);
    // Update "edit" widgets according to item values
    QString strValue;
    double value = item->data(Qt::UserRole).toDouble();
    strValue.setNum(value);
    ui->parameterName->setText(item->text());
    ui->parameterValue->setText(strValue);
    ui->parameterEditBox->setEnabled(true);
}

/**
 * @brief EditWidget::onAddParameter (slot)
 *        Called when "Add Parameter" button is clicked
 *
 */
void EditWidget::onAddParameter()
{
    addParameter(tr("NewParameter"), 0);
}

/**
 * @brief EditWidget::onRemoveParameter (slot)
 *        Called when "Remove Parameter" button is clicked
 *
 */
void EditWidget::onRemoveParameter()
{
    QListWidgetItem *item = ui->parametersList->currentItem();
    if (item == 0)
        return;
    ui->parametersList->removeItemWidget(item);
}

/**
 * @brief EditWidget::onParameterModified (slot)
 *        Called when parameter name editbox or value editbox is modified
 *
 */
void EditWidget::onParameterModified(QString text)
{
    (void)text;

    QListWidgetItem *item = ui->parametersList->currentItem();
    if (item == 0)
        return;

    // Test if the parameter name has been modified
    bool nameModified = false;
    if (item->text() != ui->parameterName->text())
        nameModified = true;

    // Test if the parameter value has been modified
    bool valueModified = false;
    double oldValue = item->data(Qt::UserRole).toDouble();
    double newValue = ui->parameterValue->text().toDouble();
    if (newValue != oldValue)
        valueModified = true;

    if (nameModified || valueModified)
        ui->buttonParameter->setEnabled(true);
    else
        ui->buttonParameter->setEnabled(false);
}

void EditWidget::onSaveParameter()
{
    QListWidgetItem *item = ui->parametersList->currentItem();
    if (item == 0)
        return;
    QString paramName  = ui->parameterName->text();
    double  paramValue = ui->parameterValue->text().toDouble();
    item->setText(paramName);
    item->setData(Qt::UserRole, QVariant(paramValue));
}

int EditWidget::getParameterCount()
{
    return ui->parametersList->count();
}

QString EditWidget::getParameterName(int row)
{
    // Get the seleted item from list
    QListWidgetItem *item = ui->parametersList->item(row);

    return item->text();
}

double EditWidget::getParameterValue(int row)
{
    // Get the seleted item from list
    QListWidgetItem *item = ui->parametersList->item(row);

    QVariant vValue = item->data(Qt::UserRole);
    if (vValue.isValid() == false)
        return 0;

    return vValue.toDouble();
}

