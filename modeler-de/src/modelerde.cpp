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
#include "modelerde.h"
#include <iostream>
#include <sstream>
#include <vle/gvle2/sourcecpp.h>
#include <vle/gvle2/sourcecpptemplate.h>
#include <vle/utils/Template.hpp>
#include <vle/utils/Tools.hpp>

ModelerDifferenceEquation::ModelerDifferenceEquation() {
    mLogger    = 0;
    mSettings  = 0;
    mWidgetEdit.clear();
    mName = "DifferenceEquationMultiple";
}

ModelerDifferenceEquation::~ModelerDifferenceEquation() {
    // Nothing to do ...
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
    EditWidget *w;
    for (int i = 0; i < mWidgetEdit.count(); i++)
    {
        w = mWidgetEdit.at(i);
        if (w->getClassName() == className)
            break;
        w = 0;
    }
    if (w == 0)
        return QString();

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
    vleTpl.stringSymbol().append("classname", className.toStdString());
    vleTpl.stringSymbol().append("namespace", "namespace_debug");

    vleTpl.listSymbol().append("par");
    vleTpl.listSymbol().append("val");
    for(int i = 0; i < w->getParameterCount(); i++)
    {
        double      value = w->getParameterValue(i);
        std::string paramName  = w->getParameterName(i).toStdString();
        std::string paramValue = vle::utils::toScientificString(value, false);
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
    mWidgetEdit.append(newTab);

    QObject::connect(newTab, SIGNAL(destroyed(QObject*)),
                     this,   SLOT  (onEditDeleted(QObject *)));
    QObject::connect(newTab, SIGNAL(nameChanged(QString)),
                     this,   SLOT  (onNameChange(QString)));
    QObject::connect(newTab, SIGNAL(saveClass()),
                     this,   SLOT  (onSaveClass()));

    return newTab;
}

/**
 * @brief ModelerDifferenceEquation::openEditClass
 *        Create a new tab to edit an existing class
 */
QWidget *ModelerDifferenceEquation::openEditClass(sourceCpp *src)
{
    EditWidget *newTab = new EditWidget();
    newTab->setModeEdit();
    mWidgetEdit.append(newTab);

    sourceCppTemplate *tpl = src->getTemplate();

    // Search the class name from tag
    newTab->setClassName( tpl->getTagValue("class") );

    if (tpl->tagArrayLoad("par"))
    {
        int n = tpl->getTagArrayCount();
        for (int i = 0; i < n; i++)
        {
            QString pname  = tpl->getTagArrayName(i);
            QString pvalue = tpl->getTagArrayValue(i);
            newTab->addParameter(pname, pvalue.toDouble());
        }
    }
    QObject::connect(newTab, SIGNAL(destroyed(QObject*)),
                     this,   SLOT  (onEditDeleted(QObject *)));
    // Set handler for the "Save" button
    QObject::connect(newTab, SIGNAL(saveClass()),
                     this,   SLOT  (onSaveClass()));

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
 *        Create a new tab to edit an existing class
 */
QWidget *ModelerDifferenceEquation::addEditModel(vleVpzModel *model)
{
    EditModel *widget = new EditModel();
    widget->setModel(model);

    return widget;
}

/**
 * @brief ModelerDifferenceEquation::initExpCond
 *        Init an exprimental condition according to template parameters
 */
void ModelerDifferenceEquation::initExpCond(vpzExpCond *exp, sourceCpp *src)
{
    sourceCppTemplate *tpl = src->getTemplate();

    if (tpl->tagArrayLoad("par"))
    {
        int n = tpl->getTagArrayCount();
        for (int i = 0; i < n; i++)
        {
            QString pname  = tpl->getTagArrayName(i);
            QString pvalue = tpl->getTagArrayValue(i);

            // Create a new port for this parameter
            vpzExpCondPort  *port = new vpzExpCondPort(exp);
            port->setName(pname);
            // Insert a double value to the port
            vpzExpCondValue *v = port->createValue(vpzExpCondValue::TypeDouble);
            v->setDouble( pvalue.toDouble() );
            // Then, insert the new port to experimental condition
            exp->addPort(port);
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

    tabWidget->setClassName(name);
    tabWidget->setModeEdit();

    // Forward this event to main app
    emit nameChanged(tabWidget, name);
}

/**
 * @brief ModelerDifferenceEquation::onSaveClass (slot)
 *        Called when a class tab request to be saved
 */
void ModelerDifferenceEquation::onSaveClass()
{
    QObject *senderObject = QObject::sender();
    EditWidget *tabWidget = qobject_cast<EditWidget *>(senderObject);
    if (tabWidget == 0)
        return;

    // Forward this event to main app
    emit saveClass(tabWidget->getClassName());
}

Q_EXPORT_PLUGIN2(modeler_de, ModelerDifferenceEquation)
