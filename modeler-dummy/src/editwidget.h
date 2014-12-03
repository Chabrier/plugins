/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QObject>
#include <QWidget>

namespace Ui {
class EditWidget;
}

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = 0);
    ~EditWidget();
    QString getClassName();
    void setClassName(QString name);
    void setModeNew();
    void setModeEdit();

public slots:
    void onRenameSave();
    void onSaveClass();

signals:
    void nameChanged(QString name);
    void saveClass();

private:
    Ui::EditWidget *ui;
    QString         mClassName;
};

#endif // EDITWIDGET_H
