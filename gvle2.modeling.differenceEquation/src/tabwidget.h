/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2014 INRA
 *
 */
#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QObject>
#include <QWidget>

namespace Ui {
class TabWidget;
}

namespace vle {
namespace gvle2 {

class TabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = 0);
    ~TabWidget();

private:
    Ui::TabWidget *ui;
};
}}
#endif // TABWIDGET_H
