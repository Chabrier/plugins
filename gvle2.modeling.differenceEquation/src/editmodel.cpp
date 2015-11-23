/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#include "editmodel.h"
#include "ui_editmodel.h"

namespace vle {
namespace gvle2 {

/**
 * @brief EditModel::EditModel
 *        Default constructor - Show and edit expcond for a specific model
 *
 */
EditModel::EditModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editModel)
{
//    mModel = 0;

    ui->setupUi(this);

    QTableWidget *tParams;
    tParams = ui->tableParameters;
    tParams->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    tParams->horizontalHeader()->setStretchLastSection(true);

    QObject::connect(ui->buttonParameterSave, SIGNAL(clicked()),
                     this,                    SLOT  (onSaveParameters()));
}

/**
 * @brief EditModel::~EditModel
 *        Default destructor
 *
 */
EditModel::~EditModel()
{
    delete ui;
}

/**
 * @brief EditModel::getExpCond
 *        Get the Experimental Condition used by modeler
 *
 */
// vpzExpCond *EditModel::getExpCond()
// {
//     if (mModel == 0)
//         return 0;

//     mSource = mModel->getModelerClass();
//     QString className = mSource->getTemplate()->getTagValue("class");
//     QString expName = className + mModel->getName();
//     vpzExpCond *exp = mModel->getVpz()->getCondition(expName);

//     return exp;
// }

/**
 * @brief EditModel::setModel
 *        Set the model to be modeled
 *
 */
// void EditModel::setModel(vleVpzModel *model)
// {
//     mModel = model;
//     bool isAltered = false;

//     vpzExpCond *exp = getExpCond();
//     if ((exp == 0) || (mSource == 0))
//         return;

//     sourceCppTemplate *tpl = mSource->getTemplate();

//     if (tpl->tagArrayLoad("par"))
//     {
//         QStringList usedPortsNames;

//         int n = tpl->getTagArrayCount();
//         ui->tableParameters->setRowCount(n);

//         for (int i = 0; i < n; i++)
//         {
//             QString pname  = tpl->getTagArrayName(i);
//             QString pvalue = tpl->getTagArrayValue(i);

//             vpzExpCondPort  *eport = exp->getPort(pname);
//             if (eport == 0)
//             {
//                 // Create a new port
//                 vpzExpCondPort *newPort = new vpzExpCondPort(exp);
//                 newPort->setName(pname);
//                 // Insert a double value
//                 vpzExpCondValue *v = newPort->createValue(vpzExpCondValue::TypeDouble);
//                 v->setDouble( pvalue.toDouble() );
//                 // Insert the new port to experimental condition
//                 exp->addPort(newPort);
//                 eport = newPort;
//                 isAltered = true;
//             }

//             vpzExpCondValue *eval  = eport->getValue();
//             if (eval == 0)
//             {
//                 // Insert a double value
//                 vpzExpCondValue *v = eport->createValue(vpzExpCondValue::TypeDouble);
//                 v->setDouble( pvalue.toDouble() );
//                 eval = v;
//             }

//             usedPortsNames.append(pname);

//             QTableWidgetItem *itemName = new QTableWidgetItem(pname);
//             itemName->setFlags(Qt::ItemIsEnabled);
//             ui->tableParameters->setItem(i, 0, itemName);

//             QTableWidgetItem *itemDefault = new QTableWidgetItem(pvalue);
//             itemDefault->setFlags(Qt::ItemIsEnabled);
//             ui->tableParameters->setItem(i, 1, itemDefault);

//             QString curVal;
//             curVal.setNum(eval->getDouble());
//             QTableWidgetItem *itemValue = new QTableWidgetItem(curVal);
//             itemValue->setData(Qt::UserRole, QVariant(curVal));
//             ui->tableParameters->setItem(i, 2, itemValue);
//         }

//         QList <vpzExpCondPort *> *portList = exp->getPorts();
//         for (int i = 0; i < portList->count(); i++)
//         {
//             vpzExpCondPort *checkPort = portList->at(i);
//             bool known = usedPortsNames.contains( checkPort->getName() );
//             if (known)
//                 continue;
//             exp->removePort( checkPort );
//             isAltered = true;
//         }
//     }
//     QObject::connect(ui->tableParameters, SIGNAL(itemChanged(QTableWidgetItem *)),
//                      this,    SLOT(onParameterChanged(QTableWidgetItem *)));

//     if (isAltered)
//         // Inform that parameter value has been changed
//         emit valueChanged();
// }

/**
 * @brief EditModel::onParameterChanged (slot)
 *        Called when the value of a parameter is modified into table
 *
 */
void EditModel::onParameterChanged(QTableWidgetItem *item)
{
    QVariant vOldValue = item->data(Qt::UserRole);
    QString oldValue = vOldValue.toString();

    QColor normalColor(255, 255, 255);
    QColor modifiedColor(255, 180, 180);

    if (item->text() != oldValue)
    {

        item->setBackground(QBrush(modifiedColor));
        ui->buttonParameterSave->setEnabled(true);
    }
    else
        item->setBackground(QBrush(normalColor));
}

/**
 * @brief EditModel::onSaveParameters (slot)
 *        Called when the "save" button of the parameters tab is clicked
 *
 */
void EditModel::onSaveParameters()
{
    // vpzExpCond *exp = getExpCond();
    // if (exp == 0)
    //     return;

    // int n = ui->tableParameters->rowCount();

    // for (int i = 0; i < n; i++)
    // {
    //     // Get the ExpCond port of the parameter
    //     QTableWidgetItem *item = ui->tableParameters->item(i, 0);
    //     QString pname = item->text();
    //     vpzExpCondPort  *eport = exp->getPort(pname);

    //     // Get the value item for parameter
    //     item = ui->tableParameters->item(i, 2);

    //     // Update the ExpCond according to specified value
    //     vpzExpCondValue *value = eport->getValue();
    //     value->setDouble( item->text().toDouble() );

    //     // Reset background color
    //     item->setBackground(QBrush(QColor(255, 255, 255)));
    // }
    // Save button is now disabled until a parameter value change
    ui->buttonParameterSave->setEnabled(false);

    // Inform that parameter value has been changed
    emit valueChanged();
}
}
}
