/*
 * This file is part of VLE, a framework for multi-modeling, simulation
 * and analysis of complex dynamical systems.
 * http://www.vle-project.org
 *
 * Copyright (c) 2015 INRA
 *
 */
#include <QDebug>
#include "tab.h"
#include "ui_tab.h"

/**
 * @brief MainTab::MainTab
 *        Default constructor
 */
MainTab::MainTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainTab)
{
    mEcValueDay   = 0;
    mEcValueMonth = 0;
    mEcValueYear  = 0;

    ui->setupUi(this);
    QObject::connect(ui->calendar, SIGNAL(clicked(QDate)),
                     this,         SLOT(dateSelected(QDate)) );
}

/**
 * @brief MainTab::~MainTab
 *        Default destructor
 */
MainTab::~MainTab()
{
    delete ui;
}

/**
 * @brief MainTab::setExpCond
 *        Set the Experimental Condition to use
 */
void MainTab::setExpCond(vpzExpCond *cond)
{
    // Save it
    mExpCond = cond;

    // In case of a nul pointer, nothing to do
    if (cond == 0)
        return;

    vpzExpCondPort *datePort = 0;

    // Search the "date" port of the experimental condition
    QList <vpzExpCondPort *> *ports = cond->getPorts();
    for (int i = 0; i < ports->count(); i++)
    {
        vpzExpCondPort *port = ports->at(i);
        if (port->getName().toLower() == "date")
        {
            datePort = port;
            break;
        }
    }
    // If no "date" port found, create it !
    if (datePort == 0)
    {
        datePort = new vpzExpCondPort();
        datePort->setName("date");
        cond->addPort(datePort);
        qDebug() << "ExpCond-Dummy-Plugin: Add date port";
    }

    int vDay   = -1;
    int vMonth = -1;
    int vYear  = -1;

    vpzExpCondValue *v;
    for (v = datePort->getValue(); v != 0; v = datePort->getNextValue())
    {
        if (v->type() != vpzExpCondValue::TypeString)
            continue;

        QStringList strList = v->getString().split(":");

        if ( (strList.count() == 2) && (strList.at(0) == "day") )
        {
            vDay = strList.at(1).toInt();
            mEcValueDay = v;
        }
        if ( (strList.count() == 2) && (strList.at(0) == "month") )
        {
            vMonth = strList.at(1).toInt();
            mEcValueMonth = v;
        }
        if ( (strList.count() == 2) && (strList.at(0) == "year") )
        {
            vYear = strList.at(1).toInt();
            mEcValueYear = v;
        }
    }
    if (vDay < 0)
    {
        vpzExpCondValue *ecValue;
        ecValue = datePort->createValue(vpzExpCondValue::TypeString);
        // Set default test value to 8 June 1949 : George Orwell - 1984 ;-)
        ecValue->setString("day:8");
        vDay = 8;
        // Save a copy of this ExpCondValue
        mEcValueDay = ecValue;
        qDebug() << "ExpCond-Dummy-Plugin:   Add day value to date port: " << vDay;
    }
    if (vMonth < 0)
    {
        vpzExpCondValue *ecValue;
        ecValue = datePort->createValue(vpzExpCondValue::TypeString);
        ecValue->setString("month:6");
        vMonth = 6;
        mEcValueMonth = ecValue;
        qDebug() << "ExpCond-Dummy-Plugin:   Add month value to date port: " << vMonth;
    }
    if (vYear < 0)
    {
        vpzExpCondValue *ecValue;
        ecValue = datePort->createValue(vpzExpCondValue::TypeString);
        ecValue->setString("year:1949");
        vYear = 1949;
        mEcValueYear = ecValue;
        qDebug() << "ExpCond-Dummy-Plugin:   Add year value to date port: " << vYear;
    }

    QDate selDate;
    selDate.setDate(vYear, vMonth, vDay);

    ui->calendar->setSelectedDate(selDate);
}

/**
 * @brief MainTab::dateSelected (slot)
 *        Called when a new date is selected from calendar widget
 */
void MainTab::dateSelected(QDate date)
{
    // Update the day
    QString strDay = QString("day:%1").arg(date.day());
    mEcValueDay->setString(strDay);

    // Update the month
    QString strMonth = QString("month:%1").arg(date.month());
    mEcValueMonth->setString(strMonth);

    // Update the year
    QString strYear = QString("year:%1").arg(date.year());
    mEcValueYear->setString(strYear);

    emit valueChanged(mExpCond);
}
