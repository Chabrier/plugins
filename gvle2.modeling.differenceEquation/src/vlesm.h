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

#ifndef GVLE2_VLESM_H
#define GVLE2_VLESM_H

#include <QDebug>


#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QStaticText>
#include <QStyleOption>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNamedNodeMap>
#include <QXmlDefaultHandler>
#include <vle/value/Value.hpp>
#include <vle/value/Map.hpp>
#include <vle/gvle2/vleDomDiffStack.h>


namespace vle {
namespace gvle2 {

/**
 * @brief Class that implements vleDomObject especially for vleSm
 */
class vleDomSm : public vleDomObject
{
public:
    vleDomSm(QDomDocument* doc);
    ~vleDomSm();
    QString  getXQuery(QDomNode node);
    QDomNode getNodeFromXQuery(const QString& query, QDomNode d=QDomNode());
};

class vleSm : public QObject
{
    Q_OBJECT
public:
    vleSm(const QString& srcpath, const QString& smpath);
    QString getSrcPath() const
    {return mFileNameSrc;};
    QString getSmPath() const
    {return mFileNameSm;};
    void setSrcPath(const QString& name)
    {mFileNameSrc = name;};
    void setSmPath(const QString& name)
    {mFileNameSm = name;};
    QString toQString(const QDomNode& node) const;
    void xCreateDom();
    void xReadDom();
    QString getSrcPlugin();
    void setParamToDoc(const QString& paramName, const QString& paramValue);
    void renameParamToDoc(const QString& oldParamName, const QString& newParamName);
    void rmParamToDoc(const QString& paramName);
    QString getParamValue(const QString& paramName);
    void setComputeToDoc(const QString& computeBody);
    QString getComputeBody();
    void setClassNameToDoc(const QString& className);
    QString getClassName();
    void setNamespaceToDoc(const QString& nm);
    QString getNamespace();

    QDomNodeList paramsFromDoc();

    void setCurrentTab(QString tabName);

    void save();

    void undo();
    void redo();

public slots:
    void onUndoRedoSm(QDomNode oldValSm, QDomNode newValSm);

signals:
    void undoRedo(QDomNode oldValSm, QDomNode newValSm);
    void modified();

private:
    QDomDocument*    mDocSm;
    QString          mFileNameSrc;
    QString          mFileNameSm;
    vleDomSm*        mVdoSm;
    vleDomDiffStack* undoStackSm;
    bool             waitUndoRedoSm;
    QDomNode         oldValSm;
    QDomNode         newValSm;
};

}}//namespaces

#endif
