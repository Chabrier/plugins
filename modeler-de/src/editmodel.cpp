#include "editmodel.h"
#include "ui_editmodel.h"

EditModel::EditModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editModel)
{
    mModel = 0;

    ui->setupUi(this);

    QTableWidget *tProp;
    tProp = ui->tableParameters;
    tProp->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    tProp->horizontalHeader()->setStretchLastSection(true);
    //QObject::connect(tProp, SIGNAL(itemChanged(QTableWidgetItem *)),
    //                 this,  SLOT  (onPropertyChanged(QTableWidgetItem *)));
    //QObject::connect(tProp, SIGNAL(currentCellChanged(int, int, int, int)),
    //                 this,  SLOT  (onPropertySelected(int, int, int, int)));

}

EditModel::~EditModel()
{
    delete ui;
}

#include <QDebug>
void EditModel::setModel(vleVpzModel *model)
{
    mModel = model;

    sourceCpp *modelClass = model->getModelerClass();

    QString className = modelClass->getTemplate()->getTagValue("class");
    QString expName = className + model->getName();
    vpzExpCond *exp = model->getVpz()->getCondition(expName);
    if (exp == 0)
        return;

    sourceCppTemplate *tpl = modelClass->getTemplate();

    if (tpl->tagArrayLoad("par"))
    {
        int n = tpl->getTagArrayCount();
        ui->tableParameters->setRowCount(n);

        for (int i = 0; i < n; i++)
        {
            QString pname  = tpl->getTagArrayName(i);
            QString pvalue = tpl->getTagArrayValue(i);

            QTableWidgetItem *itemName = new QTableWidgetItem(pname);
            itemName->setFlags(Qt::ItemIsEnabled);
            ui->tableParameters->setItem(i, 0, itemName);

            QTableWidgetItem *itemDefault = new QTableWidgetItem(pvalue);
            itemDefault->setFlags(Qt::ItemIsEnabled);
            ui->tableParameters->setItem(i, 1, itemDefault);

            vpzExpCondPort  *eport = exp->getPort(pname);
            vpzExpCondValue *eval  = eport->getValue();
            QString curVal;
            curVal.setNum(eval->getDouble());
            QTableWidgetItem *itemValue = new QTableWidgetItem(curVal);
            ui->tableParameters->setItem(i, 2, itemValue);
        }
    }

}
