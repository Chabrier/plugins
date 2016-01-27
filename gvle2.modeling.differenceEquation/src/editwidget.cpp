/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */

#include <QMessageBox>

#include "editwidget.h"
#include "ui_editclass.h"
#include <boost/lexical_cast.hpp>

namespace vle {
namespace gvle {

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    saved(true),
    ui(new Ui::EditClass)
{
    ui->setupUi(this);
    ui->parameterEditBox->setEnabled(false);

    ui->parametersList->setColumnCount(2);
    QStringList ColumnNames;
    ColumnNames << tr("Name") << tr("Value");

    ui->parametersList->setHeaderLabels(ColumnNames);

    QObject::connect(ui->buttonRenameSave, SIGNAL(clicked()),
                     this,                 SLOT  (onRenameSave()));
    QObject::connect(ui->buttonSave,       SIGNAL(clicked()),
                     this,                 SLOT  (onSaveClass()));
    // Parameters widgets
    QObject::connect(ui->parametersList,   SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
                     this,                 SLOT  (onSelectParameter(QTreeWidgetItem*, QTreeWidgetItem*)));
    QObject::connect(ui->buttonAddParameter, SIGNAL(clicked()),
                     this,                 SLOT  (onAddParameter()));
    QObject::connect(ui->buttonRemoveParameter, SIGNAL(clicked()),
                     this,                 SLOT  (onRemoveParameter()));
    QObject::connect(ui->buttonParameter,  SIGNAL(clicked()),
                     this,                 SLOT  (onSaveParameter()));
    QObject::connect(ui->parameterName,    SIGNAL(textChanged(QString)),
                     this,                 SLOT  (onParameterModified(QString)));
    QObject::connect(ui->parameterValue,   SIGNAL(textChanged(QString)),
                     this,                 SLOT  (onParameterModified(QString)));

    QObject::connect(ui->buttonCompute,    SIGNAL(clicked()),
                     this,                 SLOT  (onSetCompute()));

}

EditWidget::~EditWidget()
{
    delete ui;
}

void
EditWidget::onUndoRedoSm(QDomNode /*oldValSm*/, QDomNode /*newValSm*/)
{
    qDebug() << "EditWidget::onUndoRedoSm";
    setParameters();
    setCompute();
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

    QString basepath = mPkg->getDir(vle::utils::PKG_SOURCE).c_str();
    QString nameSm = name + ".sm";
    mSm = new vleSm(basepath +"/src/"+ name + ".cpp",
                    basepath + "/metadata/src/" + nameSm);
    mSm->setClassNameToDoc(name);
    mSm->setNamespaceToDoc(mPkg->name().c_str());

    onModified();
    onSaveClass();

    QObject::connect(mSm,
                     SIGNAL(undoRedo(QDomNode, QDomNode)),
                     this,
                     SLOT(onUndoRedoSm(QDomNode, QDomNode)));

    QObject::connect(mSm,
                     SIGNAL(modified()),
                     this,
                     SLOT(onModified()));
}

void EditWidget::setModeNew()
{
    qDebug() << "EditWidget::setModeNew";

    ui->content->setEnabled(false);
    ui->lineEditName->setText(mClassName);
    ui->headerStack->setCurrentIndex(1);
}
void EditWidget::setModeEdit()
{
    qDebug() << "EditWidget::setModeEdit";

    ui->headerStack->setCurrentIndex(0);
    ui->content->setEnabled(true);
    ui->buttonSave->setEnabled(true);
}

void EditWidget::setModeEdit2()
{
    qDebug() << "EditWidget::setModeEdit";

    ui->headerStack->setCurrentIndex(0);
    ui->content->setEnabled(true);
    ui->buttonSave->setEnabled(false);
}

void EditWidget::setParameters()
{
    ui->parametersList->clear();
    QDomNodeList paramsXml = mSm->paramsFromDoc();
    for (int i = 0; i < paramsXml.length(); i++) {
        QDomNode param = paramsXml.item(i);
        addParameter(param.attributes().namedItem("param").nodeValue(),
                     param.attributes().namedItem("value").nodeValue().toDouble());
    }
}

void EditWidget::setCompute()
{
    ui->editCompute->clear();
    ui->editCompute->appendPlainText(mSm->getComputeBody());

    qDebug() << mSm->getComputeBody();

}

void EditWidget::addParameter(QString name, double v)
{
    // Insert a new parameter at the top position
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->parametersList);
    item->setText(0, name);
    item->setText(1, QString::number(v));
    item->setData(1, Qt::UserRole, QVariant(v));
    ui->parametersList->addTopLevelItem(item);
    // then, select it
    //ui->parametersList->setCurrentRow(0);
}

void EditWidget::onRenameSave()
{
    qDebug() << "EditWidget::onRenameSave";

    setClassName(ui->lineEditName->text());

    setModeEdit();
}

void EditWidget::onSaveClass()
{
    mSm->save();
    saved = true;
    emit nameChanged(mSm->getClassName());
    ui->buttonSave->setEnabled(false);
}

void EditWidget::onModified()
{
    emit nameChanged(mSm->getClassName() + " *");
    emit saveClass();
    ui->buttonSave->setEnabled(true);
    saved = false;
}
bool EditWidget::allowClose()
{
    qDebug() << "EditWidget::allowClose()";

    if (saved) {
        return true;
    } else {
        QString className = mSm->getClassName();
        QMessageBox msgBox;
        msgBox.setText(tr("Modified Atom Class: ") +
                       className + "\n" +
                       tr("Save before close ?"));
        msgBox.addButton(QMessageBox::Save);
        msgBox.addButton(QMessageBox::Discard);
        msgBox.addButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:
            onSaveClass();
            return true;
            break;
        case QMessageBox::Discard:
            delete(mSm);
            setClassName(className);
            return true;
            break;
        case QMessageBox::Cancel:
            return false;
            break;
        default:
            break;
        }
        return false;
    }
};




/**
 * @brief EditWidget::onSelectParameter (slot)
 *        Called when an item is selected into parameter list
 *
 */

    void EditWidget::onSelectParameter(QTreeWidgetItem * current, QTreeWidgetItem * /*previous*/)
{
    ui->buttonParameter->setEnabled(false);
    if (not current)
    {
        ui->parameterEditBox->setEnabled(false);
        return;
    }
    QString strValue;
    double value = current->data(1, Qt::UserRole).toDouble();
    strValue.setNum(value);
    ui->parameterName->setText(current->text(0));
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
    int i = 0;
    QString basename = tr("NewParameter");
    QString name = basename;
    while (not isUniqParameterName(name)) {
        name = basename + "_" + QString::number(i++);
    }
    mSm->setParamToDoc(name, QString::number(0));
    addParameter(name, 0);
}

/**
 * @brief EditWidget::onRemoveParameter (slot)
 *        Called when "Remove Parameter" button is clicked
 *
 */
void EditWidget::onRemoveParameter()
{
    QTreeWidgetItem *item = ui->parametersList->currentItem();
    if (item) {
        mSm->rmParamToDoc(item->text(0));
        delete item;
    }
}

bool EditWidget::isValidParameterName(const QString parameterName) const
{
    std::string name = parameterName.toStdString();

    size_t i = 0;
    while (i < name.length()) {
        if (!isalnum(name[i])) {
            return false;
        }
        i++;
    }

    try {
        boost::lexical_cast <double>(name);
        return false;
    } catch (boost::bad_lexical_cast e) {}

    if (name == "")
        return false;
    else
        return true;
}

bool EditWidget::isUniqParameterName(const QString parameterName) const
{
    QDomNodeList paramsXml = mSm->paramsFromDoc();
    for (int i = 0; i < paramsXml.length(); i++) {
        QDomNode param = paramsXml.item(i);
        if (param.attributes().namedItem("param").nodeValue() ==
            parameterName) {
            return false;
        }
    }
    return true;
}

void EditWidget::onParameterModified(QString text)
{
    (void)text;

    QTreeWidgetItem *item = ui->parametersList->currentItem();
    if (item == 0)
        return;

    // Test if the parameter name has been modified
    bool nameModified = false;
    bool nameOk;
    if (item->text(0) != ui->parameterName->text()) {
        nameModified = true;
    }
    nameOk = isValidParameterName(ui->parameterName->text()) &&
        isUniqParameterName(ui->parameterName->text());

    // Test if the parameter value has been modified
    bool valueModified = false;
    bool valueOk;

    double oldValue = item->data(1, Qt::UserRole).toDouble();
    double newValue = ui->parameterValue->text().toDouble(&valueOk);
    if (newValue != oldValue) {
        valueModified = true;
    }

    if ((nameModified && nameOk)|| (valueModified && valueOk))
        ui->buttonParameter->setEnabled(true);
    else
        ui->buttonParameter->setEnabled(false);
}

void EditWidget::onSaveParameter()
{
    QTreeWidgetItem *item = ui->parametersList->currentItem();
    if (item == 0)
        return;
    QString paramName  = ui->parameterName->text();
    double  paramValue = ui->parameterValue->text().toDouble();

    if (paramName != item->text(0)) {
        mSm->renameParamToDoc(item->text(0), paramName);
    }

    mSm->setParamToDoc(paramName, ui->parameterValue->text());

    item->setText(0, paramName);
    item->setText(1, ui->parameterValue->text());
    item->setData(1, Qt::UserRole, QVariant(paramValue));
}

void EditWidget::onSetCompute()
{
    QString computeBody = ui->editCompute->toPlainText();

    mSm->setComputeToDoc(computeBody);
}

}}
