/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014-2015 INRA http://www.inra.fr
 *
 * See the AUTHORS or Authors.txt file for copyright owners and
 * contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vlesm.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

namespace vle {
namespace gvle2 {

vleDomSm::vleDomSm(QDomDocument* doc): vleDomObject(doc)

{

}

vleDomSm::~vleDomSm()
{

}

QString
vleDomSm::getXQuery(QDomNode node)
{
    QString name = node.nodeName();
    if ((name == "compute") or
        (name == "parameters") or
        (name == "srcPlugin")) {
        return getXQuery(node.parentNode())+"/"+name;
    }
    //element identified by attribute name
    if ((name == "parameter")){
        return getXQuery(node.parentNode())+"/"+name+"[@name=\""
                +attributeValue(node,"name")+"\"]";
    }
    if (node.nodeName() == "vle_project_metadata") {
        return "./vle_project_metadata";
    }
    return "";
}

QDomNode
vleDomSm::getNodeFromXQuery(const QString& query,
        QDomNode d)
{
    //handle last
    if (query.isEmpty()) {
        return d;
    }

    QStringList queryList = query.split("/");
    QString curr = "";
    QString rest = "";
    int j=0;
    if (queryList.at(0) == ".") {
        curr = queryList.at(1);
        j=2;
    } else {
        curr = queryList.at(0);
        j=1;
    }
    for (int i=j; i<queryList.size(); i++) {
        rest = rest + queryList.at(i);
        if (i < queryList.size()-1) {
            rest = rest + "/";
        }
    }
    //handle first
    if (d.isNull()) {
        QDomNode rootNode = mDoc->documentElement();
        if (curr != "vle_project_metadata" or queryList.at(0) != ".") {
            return QDomNode();
        }
        return(getNodeFromXQuery(rest,rootNode));
    }

    //handle recursion with uniq node
    if ((curr == "parameters") or
        (curr == "compute") or
        (curr == "srcPlugin")){
        return getNodeFromXQuery(rest, obtainChild(d, curr, true));
    }
    //handle recursion with nodes identified by name
    std::vector<QString> nodeByNames;
    nodeByNames.push_back(QString("parameter"));
    QString selNodeByName ="";
    for (unsigned int i=0; i< nodeByNames.size(); i++) {
        if (curr.startsWith(nodeByNames[i])) {
            selNodeByName = nodeByNames[i];
        }
    }
    if (not selNodeByName.isEmpty()) {
        QStringList currSplit = curr.split("\"");
        QDomNode selMod = childWhithNameAttr(d, selNodeByName, currSplit.at(1));
        return getNodeFromXQuery(rest,selMod);
    }
    return QDomNode();
}

/************************************************************************/

vleSm::vleSm(const QString& srcpath, const QString& smpath):
    mDocSm(0), mFileNameSrc(srcpath), mFileNameSm(smpath), mVdoSm(0),
    undoStackSm(0), waitUndoRedoSm(false),
    oldValSm(), newValSm()
{
    qDebug() << "isisisisi " << mFileNameSm;


    QFile file(mFileNameSm);
    if (file.exists()) {
        mDocSm = new QDomDocument("vle_project_metadata");
        QXmlInputSource source(&file);
        QXmlSimpleReader reader;
        mDocSm->setContent(&source, &reader);
    } else {
        xCreateDom();
    }

    mVdoSm = new vleDomSm(mDocSm);
    undoStackSm = new vleDomDiffStack(mVdoSm);
    undoStackSm->init(*mDocSm);

    QObject::connect(undoStackSm,
                SIGNAL(undoRedoVdo(QDomNode, QDomNode)),
                this, SLOT(onUndoRedoSm(QDomNode, QDomNode)));
}

QString
vleSm::toQString(const QDomNode& node) const
{
    QString str;
    QTextStream stream(&str);
    node.save(stream, node.nodeType());
    return str;
}

void
vleSm::xCreateDom()
{
    if (not mDocSm) {
        mDocSm = new QDomDocument("vle_project_metadata");
        QDomProcessingInstruction pi;
        pi = mDocSm->createProcessingInstruction("xml",
                "version=\"1.0\" encoding=\"UTF-8\" ");
        mDocSm->appendChild(pi);

        QDomElement vpmRoot = mDocSm->createElement("vle_project_metadata");
        // Save VPZ file revision
        vpmRoot.setAttribute("version", "1.x");
        // Save the author name (if known)
        vpmRoot.setAttribute("author", "meto");
        QDomElement xCondPlug = mDocSm->createElement("parameters");
        vpmRoot.appendChild(xCondPlug);
        xCondPlug = mDocSm->createElement("compute");
        vpmRoot.appendChild(xCondPlug);
        xCondPlug = mDocSm->createElement("srcPlugin");
        xCondPlug.setAttribute("name", "DifferenceEquationMultiple@vle.extension.difference-equation");
        vpmRoot.appendChild(xCondPlug);
        mDocSm->appendChild(vpmRoot);
    }
}

void
vleSm::xReadDom()
{
    if (mDocSm) {
        QFile file(mFileNameSm);
        mDocSm->setContent(&file);
    }
}

void
vleSm::setParamToDoc(const QString& paramName, const QString& paramValue)
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode parametersNode =
            mDocSm->elementsByTagName("parameters").item(0);

    undoStackSm->snapshot(parametersNode);

    QDomNodeList parameterList =
            parametersNode.toElement().elementsByTagName("parameter");

    for (unsigned int i = 0; i< parameterList.length(); i++) {
        QDomNode param = parameterList.at(i);
        for (int j=0; j< param.attributes().size(); j++) {
            if ((param.attributes().item(j).nodeName() == "param") and
                    (param.attributes().item(j).nodeValue() == paramName))  {
                param.toElement().setAttribute("value", paramValue);

                emit modified();
                return;
            }
        }
    }
    QDomElement el = mDocSm->createElement("parameter");
    el.setAttribute("param", paramName);
    el.setAttribute("value", paramValue);
    parametersNode.appendChild(el);

    emit modified();
}

void
vleSm::rmParamToDoc(const QString& paramName)
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode parametersNode =
            mDocSm->elementsByTagName("parameters").item(0);

    undoStackSm->snapshot(parametersNode);

    QDomNodeList parameterList =
        parametersNode.toElement().elementsByTagName("parameter");

    for (unsigned int i = 0; i< parameterList.length(); i++) {
        QDomNode param = parameterList.at(i);
        for (int j = 0; j < param.attributes().size(); j++) {
            if ((param.attributes().item(j).nodeName() == "param") and
                    (param.attributes().item(j).nodeValue() == paramName))  {

                parametersNode.removeChild(param);
            }
        }
    }

    emit modified();
}


void
vleSm::renameParamToDoc(const QString& oldParamName, const QString& newParamName)
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode parametersNode =
            mDocSm->elementsByTagName("parameters").item(0);

    undoStackSm->snapshot(parametersNode);

    QString paramVal = getParamValue(oldParamName);
    if (not (oldParamName == newParamName)) {
        //update vpz with snapshot bu without signal
        bool oldBlock = undoStackSm->blockSignals(true);
        rmParamToDoc(oldParamName);
        setParamToDoc(newParamName, paramVal);
        undoStackSm->blockSignals(oldBlock);
    }
    emit modified();
}

QString
vleSm::getParamValue(const QString& paramName)
{
    QStringList res;
    if (mDocSm) {
        QDomElement docElem = mDocSm->documentElement();
        QDomNode parametersNode =
                mDocSm->elementsByTagName("parameters").item(0);
        QDomNodeList parameterList =
                parametersNode.toElement().elementsByTagName("parameter");
        for (unsigned int i =0; i< parameterList.length(); i++) {
            QDomNode param = parameterList.item(i);
            if (param.attributes().contains("param") and
                (param.attributes().namedItem("param").nodeValue() == paramName)) {
                return param.attributes().namedItem("value").nodeValue();
            }
        }
    }

return "";
}

void
vleSm::setComputeToDoc(const QString& computeBody)
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode computeNode =
            mDocSm->elementsByTagName("compute").item(0);

    undoStackSm->snapshot(computeNode);

    QDomNodeList childs = computeNode.childNodes();
    if (childs.length() == 0) {
        QDomCDATASection cdataCompute = mDocSm->createCDATASection(computeBody);
        computeNode.appendChild(cdataCompute);
    } else {
        QDomCDATASection cdataCompute = childs.at(0).toCDATASection();
        cdataCompute.setData(computeBody);
    }
    emit modified();
}

QString
vleSm::getComputeBody()
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode computeNode =
        mDocSm->elementsByTagName("compute").item(0);

    QDomNodeList childs = computeNode.childNodes();
    if (childs.length() == 0) {
        return "";
    } else {
        QDomCDATASection cdataCompute = childs.at(0).toCDATASection();
        return cdataCompute.data();
    }

}

QString
vleSm::getSrcPlugin()
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode srcPluginNode =
        mDocSm->elementsByTagName("srcPlugin").item(0);
    return srcPluginNode.attributes().namedItem("name").nodeValue();
}

void
vleSm::setClassNameToDoc(const QString& className)
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode srcPluginNode =
        mDocSm->elementsByTagName("srcPlugin").item(0);

    undoStackSm->snapshot(srcPluginNode);

    srcPluginNode.toElement().setAttribute("class", className);
    emit modified();
}

QString
vleSm::getClassName()
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode srcPluginNode =
        mDocSm->elementsByTagName("srcPlugin").item(0);
    return srcPluginNode.attributes().namedItem("class").nodeValue();
}

void
vleSm::setNamespaceToDoc(const QString& nm)
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode srcPluginNode =
        mDocSm->elementsByTagName("srcPlugin").item(0);

    undoStackSm->snapshot(srcPluginNode);

    srcPluginNode.toElement().setAttribute("namespace", nm);
    emit modified();
}

QString
vleSm::getNamespace()
{
    QDomElement docElem = mDocSm->documentElement();

    QDomNode srcPluginNode =
        mDocSm->elementsByTagName("srcPlugin").item(0);
    return srcPluginNode.attributes().namedItem("namespace").nodeValue();
}

QDomNodeList
vleSm::paramsFromDoc()
{
    QDomElement docElem = mDocSm->documentElement();
    QDomNode parametersNode =
        mDocSm->elementsByTagName("parameters").item(0);
    return parametersNode.toElement().elementsByTagName("parameter");
}

void
vleSm::setCurrentTab(QString tabName)
{
    undoStackSm->current_source = tabName;
}

void
vleSm::save()
{
    //save vpm
    QFile file(mFileNameSm);
    QFileInfo fileInfo(file);
    if (not fileInfo.dir().exists()) {
        if (not QDir().mkpath(fileInfo.dir().path())) {
            qDebug() << "Cannot create dir " << fileInfo.dir().path() ;
            return;
        }
    }
    if (not file.exists()) {
        if (not file.open(QIODevice::WriteOnly)) {
            qDebug() << "VPM File (" << mFileNameSm << ")can't be opened for write !";
            return;
        }
        file.close();
    }
    file.open(QIODevice::Truncate | QIODevice::WriteOnly);
    QByteArray xml = mDocSm->toByteArray();
    file.write(xml);
    file.close();
}

void
vleSm::undo()
{
    waitUndoRedoSm = true;
    undoStackSm->undo();
    emit modified();

}

void
vleSm::redo()
{
    waitUndoRedoSm = true;
    undoStackSm->redo();
    emit modified();
}

void
vleSm::onUndoRedoSm(QDomNode oldSm, QDomNode newSm)
{
    qDebug() <<"vleSm::onUndoRedoSm";

    oldValSm = oldSm;
    newValSm = newSm;
    waitUndoRedoSm = false;
    emit undoRedo(oldValSm, newValSm);
}


}}//namespaces
