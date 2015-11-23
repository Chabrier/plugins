/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#include <QObject>
#include <QtPlugin>
#include "modelerde.h"
#include "vlesm.h"
#include <iostream>
#include <sstream>
#include <vle/gvle2/sourcecpp.h>
#include <vle/gvle2/sourcecpptemplate.h>
#include <vle/utils/Template.hpp>
#include <vle/utils/Tools.hpp>

namespace vle {
namespace gvle2 {

ModelerDifferenceEquation::ModelerDifferenceEquation() {
    mPkg       = 0;
    mLogger    = 0;
    mSettings  = 0;
    mWidgetEdit.clear();
    mName = "DifferenceEquationMultiple";
}

ModelerDifferenceEquation::~ModelerDifferenceEquation() {
}

/**
 * @brief ModelerDifferenceEquation::getname
 *        Return the plugin name
 */
QString ModelerDifferenceEquation::getname()
{
    return mName;
}

QString ModelerDifferenceEquation::getData(QString className)
{
    qDebug() << "ModelerDifferenceEquation::getData " << className;


    EditWidget *w;
    vleSm *sm;

    for (int i = 0; i < mWidgetEdit.count(); i++)
    {
        w = mWidgetEdit.at(i);
        if (w->getClassName() == className) {
            sm = w->getSm();
            break;
        }
        w = 0;
    }
    if (w == 0)
        return QString();

    return getData(sm);
}

QString ModelerDifferenceEquation::getData(vleSm * sm)
{
    QString tpl = "/**\n"                                               \
        "  * @file {{classname}}.cpp\n"                                 \
        "  * @author ...\n"                                             \
        "  * ...\n"                                                     \
        "  * @@tag DifferenceEquationMultiple@vle.extension.difference-equation @@" \
        "namespace:{{namespace}};"                                      \
        "class:{{classname}};par:"                                      \
        "{{for i in par}}"                                              \
        "{{par^i}},{{val^i}}|"                                          \
        "{{end for}}"                                                   \
        ";sync:"                                                        \
        "{{for i in sync}}"                                             \
        "{{sync^i}}|"                                                   \
        "{{end for}}"                                                   \
        ";nosync:"                                                      \
        "{{for i in nosync}}"                                           \
        "{{nosync^i}}|"                                                 \
        "{{end for}}"                                                   \
        "@@end tag@@\n"                                                 \
        "  */\n\n"                                                      \
        "#include <vle/extension/DifferenceEquation.hpp>\n\n"           \
        "//@@begin:includes@@\n"                                        \
        "{{includes}}"                                                  \
        "//@@end:includes@@\n\n"                                        \
        "namespace vd = vle::devs;\n"                                   \
        "namespace ve = vle::extension;\n"                              \
        "namespace vv = vle::value;\n\n"                                \
        "namespace {{namespace}} {\n\n"                                 \
        "class {{classname}} : public ve::DifferenceEquation::Multiple\n" \
        "{\n"                                                           \
        "public:\n"                                                     \
        "    {{classname}}(\n"                                          \
        "       const vd::DynamicsInit& atom,\n"                        \
        "       const vd::InitEventList& evts)\n"                       \
        "        : ve::DifferenceEquation::Multiple(atom, evts)\n"      \
        "    {\n"                                                       \
        "//@@begin:construct@@\n"                                       \
        "{{construct}}"                                                 \
        "//@@end:construct@@\n\n"                                       \
        "{{for i in par}}"                                              \
        "        {{par^i}} = vv::toDouble(evts.get(\"{{par^i}}\"));\n"  \
        "{{end for}}"                                                   \
        "{{for i in var}}"                                              \
        "        {{var^i}} = createVar(\"{{var^i}}\");\n"               \
        "{{end for}}"                                                   \
        "{{for i in sync}}"                                             \
        "        {{sync^i}} = createSync(\"{{sync^i}}\");\n"            \
        "{{end for}}"                                                   \
        "{{for i in nosync}}"                                           \
        "        {{nosync^i}} = createNosync(\"{{nosync^i}}\");\n"      \
        "{{end for}}"                                                   \
        "    }\n"                                                       \
        "\n"                                                            \
        "    virtual ~{{classname}}()\n"                                \
        "    {}\n"                                                      \
        "\n"                                                            \
        "//@@begin:compute@@\n"                                         \
        "{{compute}}"                                                   \
        "//@@end:compute@@\n\n"                                         \
        "//@@begin:initValue@@\n"                                       \
        "{{initValue}}"                                                 \
        "//@@end:initValue@@\n\n"                                       \
        "private:\n"                                                    \
        "//@@begin:user@@\n"                                            \
        "{{userFunctions}}"                                             \
        "//@@end:user@@\n\n"                                            \
        "{{for i in par}}"                                              \
        "    double {{par^i}};\n"                                       \
        "{{end for}}"                                                   \
        "{{for i in var}}"                                              \
        "    Var {{var^i}};\n"                                          \
        "{{end for}}"                                                   \
        "{{for i in sync}}"                                             \
        "    Sync {{sync^i}};\n"                                        \
        "{{end for}}"                                                   \
        "{{for i in nosync}}"                                           \
        "    Nosync {{nosync^i}};\n"                                    \
        "{{end for}}"                                                   \
        "};\n\n"                                                        \
        "} // namespace {{namespace}}\n\n"                              \
        "DECLARE_DYNAMICS({{namespace}}::{{classname}})\n\n";


    vle::utils::Template vleTpl(tpl.toStdString());
    vleTpl.stringSymbol().append("classname", sm->getClassName().toStdString());
    vleTpl.stringSymbol().append("namespace", sm->getNamespace().toStdString());

    vleTpl.listSymbol().append("par");
    vleTpl.listSymbol().append("val");

    QDomNodeList paramsXml = sm->paramsFromDoc();
    for (unsigned int i = 0; i < paramsXml.length(); i++) {
        QDomNode param = paramsXml.item(i);
        std::string paramName = param.attributes().namedItem("param").nodeValue().toStdString();
        std::string paramValue = param.attributes().namedItem("value").nodeValue().toStdString();

        vleTpl.listSymbol().append("par", paramName);
        vleTpl.listSymbol().append("val", paramValue);
    }

    vleTpl.listSymbol().append("var");
    vleTpl.listSymbol().append("sync");
    vleTpl.listSymbol().append("nosync");
    vleTpl.stringSymbol().append("includes", "");
    vleTpl.stringSymbol().append("compute",
                                 "virtual void compute(const vd::Time& /*time*/)\n" \
                                 "{ }\n");
    vleTpl.stringSymbol().append("initValue",
                                 "virtual void initValue(const vd::Time& /*time*/)\n" \
                                 "{ }\n");
    vleTpl.stringSymbol().append("userFunctions", "");
    vleTpl.stringSymbol().append("construct",
                                 "// available constructor parameters :\n" \
                                 "//  * const vd::DynamicsInit& atom\n" \
                                 "//  * const vd::InitEventList& evts");

    std::ostringstream out;
    vleTpl.process(out);

    return QString( out.str().c_str() );
}

/**
 * @brief ModelerDifferenceEquation::setSettings
 *        Save a pointer to the configuration file wrapper
 */
void ModelerDifferenceEquation::setSettings(QSettings *s)
{
    mSettings = s;
}

/**
 * @brief ModelerDifferenceEquation::setLogger
 *        Save a pointer to the logger instance of main app
 */
void ModelerDifferenceEquation::setLogger(Logger *logger)
{
    mLogger = logger;
}

void ModelerDifferenceEquation::setPackage(vle::utils::Package *pkg)
{
    mPkg = pkg;
}

bool ModelerDifferenceEquation::useCustomMainTab()
{
    return false;
}

void ModelerDifferenceEquation::onEditDeleted(QObject *obj)
{
    for (int i = 0; i < mWidgetEdit.count(); i++)
    {
        EditWidget *ew = mWidgetEdit.at(i);
        if (ew != obj)
            continue;

        mWidgetEdit.removeAt(i);
        break;
    }
}

/**
 * @brief ModelerDifferenceEquation::openNewClass
 *        Create a new tab to create and configure a new class
 */
QWidget *ModelerDifferenceEquation::openNewClass()
{
    EditWidget *newTab = new EditWidget();
    newTab->setModeNew();
    newTab->setPackage(mPkg);

    mWidgetEdit.append(newTab);

    QObject::connect(newTab, SIGNAL(destroyed(QObject*)),
                     this,   SLOT  (onEditDeleted(QObject *)));
    QObject::connect(newTab, SIGNAL(nameChanged(QString)),
                     this,   SLOT  (onNameChange(QString)));
    QObject::connect(newTab, SIGNAL(saveClass()),
                     this,   SLOT  (onSaveClass()));
    QObject::connect(newTab, SIGNAL(newClass(QString)),
                     this,   SLOT  (onNewClass(QString)));

    return newTab;
}

/**
 * @brief ModelerDifferenceEquation::openEditClass
 *        Create a new tab to edit an existing class
 */
QWidget *ModelerDifferenceEquation::openEditClass(sourceCpp *src)
{
    qDebug() << "ModelerDifferenceEquation::openEditClass";

    EditWidget *newTab = new EditWidget();

    newTab->setPackage(mPkg);
    mWidgetEdit.append(newTab);

    sourceCppTemplate *tpl = 0;
    tpl = src->getTemplate();
    qDebug() << tpl;

    // Search the class name from tag
    newTab->setClassName(tpl->getTagValue("class"));
    newTab->setParameters();
    newTab->setCompute();

    newTab->setModeEdit2();

    qDebug() << "the name " << tpl->getTagValue("class");


    // if (tpl->tagArrayLoad("par"))
    // {
    //     int n = tpl->getTagArrayCount();
    //     for (int i = 0; i < n; i++)
    //     {
    //         QString pname  = tpl->getTagArrayName(i);
    //         QString pvalue = tpl->getTagArrayValue(i);
    //         newTab->addParameter(pname, pvalue.toDouble());
    //     }
    // }
    QObject::connect(newTab, SIGNAL(destroyed(QObject*)),
                     this,   SLOT  (onEditDeleted(QObject *)));
    // Set handler for the "Save" button
    QObject::connect(newTab, SIGNAL(saveClass()),
                     this,   SLOT  (onSaveClass()));
    QObject::connect(newTab, SIGNAL(nameChanged(QString)),
                     this,   SLOT  (onNameChange(QString)));

    return newTab;
}

/**
 * @brief ModelerDifferenceEquation::getEditClass
 *        Search and return an opened class widget
 */
QWidget *ModelerDifferenceEquation::getEditClass(sourceCpp *src)
{
    sourceCppTemplate *tpl = src->getTemplate();
    QString srcClassName = tpl->getTagValue("class");

    for (int i = 0; i < mWidgetEdit.count(); i++)
    {
        EditWidget *ew = mWidgetEdit.at(i);
        if (ew->getClassName() == srcClassName)
            return ew;
    }
    return 0;
}

void ModelerDifferenceEquation::closeEditClass(sourceCpp *src)
{
    sourceCppTemplate *tpl = src->getTemplate();
    QString srcClassName = tpl->getTagValue("class");

    for (int i = 0; i < mWidgetEdit.count(); i++)
    {
        EditWidget *ew = mWidgetEdit.at(i);
        if (ew->getClassName() == srcClassName)
        {
            mWidgetEdit.removeAt(i);
            delete ew;
            break;
        }
    }
}

/**
 * @brief ModelerDifferenceEquation::addEditModel
 *        Create a new tab to edit a model
 */
// QWidget *ModelerDifferenceEquation::addEditModel(vleVpzModel *model)
// {
//     EditModel *widget = new EditModel();

//     QObject::connect(widget, SIGNAL(valueChanged()),
//                      this,   SLOT  (onModelChanged()));

//     widget->setModel(model);

//     return widget;
// }

/**
 * @brief ModelerDifferenceEquation::initExpCond
 *        Init an exprimental condition according to template parameters
 */
// void ModelerDifferenceEquation::initExpCond(vpzExpCond *exp, sourceCpp *src)
// {
//     sourceCppTemplate *tpl = src->getTemplate();

//     if (tpl->tagArrayLoad("par"))
//     {
//         int n = tpl->getTagArrayCount();
//         for (int i = 0; i < n; i++)
//         {
//             QString pname  = tpl->getTagArrayName(i);
//             QString pvalue = tpl->getTagArrayValue(i);

//             // Create a new port for this parameter
//             vpzExpCondPort  *port = new vpzExpCondPort(exp);
//             port->setName(pname);
//             // Insert a double value to the port
//             vpzExpCondValue *v = port->createValue(vpzExpCondValue::TypeDouble);
//             v->setDouble( pvalue.toDouble() );
//             // Then, insert the new port to experimental condition
//             exp->addPort(port);
//         }
//     }
// }

/**
 * @brief ModelerDifferenceEquation::rename
 *        Used to change the name of an opened class
 */
void ModelerDifferenceEquation::rename(QString oldName, QString newName)
{
    for (int i = 0; i < mWidgetEdit.count(); i++)
    {
        EditWidget *ew = mWidgetEdit.at(i);
        if (ew->getClassName() == oldName)
        {
            ew->setClassName(newName);
            break;
        }
    }
}

/**
 * @brief ModelerDifferenceEquation::onNameChange (slot)
 *        Called when the name of an opened class has changed
 */
void ModelerDifferenceEquation::onNameChange(QString name)
{
    // Search the caller tab
    QObject *senderObject = QObject::sender();
    EditWidget *tabWidget = qobject_cast<EditWidget *>(senderObject);
    if (tabWidget == 0)
        return;

    // Forward this event to main app
    emit nameChanged(tabWidget, name);
}

/**
 * @brief ModelerDifferenceEquation::onSaveClass (slot)
 *        Called when a class tab request to be saved
 */
void ModelerDifferenceEquation::onSaveClass()
{
    qDebug() << "ModelerDifferenceEquation::onSaveClass" ;

    QObject *senderObject = QObject::sender();
    EditWidget *tabWidget = qobject_cast<EditWidget *>(senderObject);
    if (tabWidget == 0)
        return;

    QString fileName = tabWidget->getSm()->getSrcPath();

    vleSm* sm = tabWidget->getSm();

    QFile file(sm->getSrcPath());
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << getData(sm->getClassName());
    file.close();
}

/**
 * @brief ModelerDifferenceEquation::onValueChanged (slot)
 *        Called when a model tab has changed
 */
void ModelerDifferenceEquation::onModelChanged()
{
    QObject *senderObject = QObject::sender();
    EditModel *tab = qobject_cast<EditModel *>(senderObject);
    if (tab == 0)
        return;

//    vpzExpCond *exp = tab->getExpCond();

//    emit expCondChanged(exp);
}

void ModelerDifferenceEquation::cloneSrc(const QString from, const QString to)
{
    qDebug() << "ModelerDifferenceEquation::cloneSrc" << " " << from << " " << to;

    QString basepath = mPkg->getDir(vle::utils::PKG_SOURCE).c_str();
    vleSm* sm = new vleSm(basepath +"/src/"+ from + ".cpp",
                          basepath + "/metadata/src/" + from + ".sm");

    QString completeNameToSrc = basepath + "/src/"+ to + ".cpp";
    QString completeNameToSm = basepath + "/metadata/src/"+ to + ".sm";
    sm->setClassNameToDoc(to);
    sm->setSrcPath(completeNameToSrc);
    sm->setSmPath(completeNameToSm);

    sm->save();

    QFile file(sm->getSrcPath());
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << getData(sm);
    file.close();
}
}}

Q_EXPORT_PLUGIN2(modeler_de, vle::gvle2::ModelerDifferenceEquation)
