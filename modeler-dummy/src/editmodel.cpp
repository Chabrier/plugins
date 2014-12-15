#include "editmodel.h"
#include "ui_editmodel.h"

EditModel::EditModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditModel)
{
    ui->setupUi(this);
}

EditModel::~EditModel()
{
    delete ui;
}
