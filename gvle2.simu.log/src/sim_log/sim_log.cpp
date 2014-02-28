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
#include <QDebug>
#include "sim_log.h"
#include "simtab.h"
#include <iostream>
#include "vle/vpz/AtomicModel.hpp"

SimLog::SimLog() {
    mLogger    = 0;
    mThread    = 0;
    mSettings  = 0;
    mWidgetTab = 0;
    mCurrPackage   = 0;
    mWidgetToolbar = 0;
    mSimDataValid  = false;
    mDuration      = 0;
    mStepCount     = 0;
}

SimLog::~SimLog() {
    // Nothing to do ...
}

/**
 * @brief SimLog::getname
 *        Return the plugin name
 */
QString SimLog::getname() {
    QString name = "Log";
    return name;
}

/**
 * @brief SimLog::log
 *        Send a log message to main app
 */
void SimLog::log(QString message)
{
    if (mLogger)
        mLogger->log(message);
}

/**
 * @brief SimLog::setSettings
 *        Save a pointer to the configuration file wrapper
 */
void SimLog::setSettings(QSettings *s)
{
    mSettings = s;
}

/**
 * @brief SimLog::setLogger
 *        Save a pointer to the logger instance of main app
 */
void SimLog::setLogger(Logger *logger)
{
    mLogger = logger;
}

/**
 * @brief SimLog::setPackage
 *        Set the vle-package used for current simulation context
 */
void SimLog::setPackage(vle::utils::Package *pkg)
{
    mCurrPackage = pkg;
}

/**
 * @brief SimLog::getWidget
 *        Create the plugin GUI (widget inserted into main app tab)
 */
QWidget *SimLog::getWidget()
{
    // If the widget has already been allocated
    if (mWidgetTab)
        // return saved pointer
        return mWidgetTab;

    // Allocate (and return) a new Simulation Tab widget
    mWidgetTab = new SimTabLog();
    return mWidgetTab;
}

/**
 * @brief SimLog::getWidgetToolbar
 *        Create the plugin GUI toolbox (widget inserted into right column)
 */
QWidget *SimLog::getWidgetToolbar()
{
    if (mWidgetToolbar)
        return mWidgetToolbar;

    mWidgetToolbar = new widToolbar();
    return mWidgetToolbar;
}

/**
 * @brief SimPlot::addPlotSig
 *        Slot called when a new signal is enabled into the tree (toolbox)
 */
void SimLog::addPlotSig(QString view, QString port)
{
    mObsPorts.append(port);
    if (!mViewLastRow.contains(view)) {
        mViewLastRow.insert(view, 1);

        qDebug() << "addPlotSig " << view << " " << port;

    }
}

/**
 * @brief SimPlot::delPlotSig
 *        Slot called when a signal is disabled into the tree (toolbox)
 */
void SimLog::delPlotSig(QString port)
{
    mObsPorts.removeAll(port);
}

/**
 * @brief SimLog::setVpz
 *        Set the VPZ package used for the simulation
 */
void SimLog::setVpz(vle::vpz::Vpz *vpz)
{
    mVpz = vpz;
    vle::vpz::Observables curVpzObs;
    vle::vpz::Views       curVpzViews;

    curVpzViews  = mVpz->project().experiment().views();
    curVpzObs    = mVpz->project().experiment().views().observables();


    if ( getWidget() )
    {
        // Update the title (Experiment Name and VPZ file name)
        QString expName = mVpz->project().experiment().name().c_str();
        QString simTitle = QString("%1 (%2)").arg(expName).arg(mVpz->filename().c_str());
        mWidgetTab->setModelName(simTitle);

        QObject::connect(mWidgetTab, SIGNAL(doStartStop()),
                         this,       SLOT  (startStop()));
    }

    if ( getWidgetToolbar() )
    {
        mWidgetToolbar->buildTree(mVpz);

        QObject::connect(mWidgetToolbar, SIGNAL(addSig    (QString, QString)),
                         this,           SLOT  (addPlotSig(QString, QString)));
        QObject::connect(mWidgetToolbar, SIGNAL(delSig    (QString)),
                         this,           SLOT  (delPlotSig(QString)));
    }
}

void *SimLog::getVpz()
{
    return (void *)mVpz;
}

void SimLog::startStop()
{
    if (mThread == 0)
    {
        mWidgetTab->simStarted();
        mWidgetToolbar->simStarted();
        simulationStart();
    }
    else
    {
        if (mSimThread)
        {
            if (mSimThread->isPaused())
            {
                mSimThread->setPause(false);
                mWidgetTab->simStarted();
                mWidgetToolbar->simStarted();
            }
            else
            {
                mSimThread->setPause(true);
                mWidgetTab->simPaused();
                mWidgetToolbar->simPaused();
            }
        }
    }
}

void SimLog::simulationStart()
{
    mSimThread = new simPlotThread(mVpz);

    // Load and init the simulation into VLE
    try {
        if (mCurrPackage == 0)
            throw 0;
        mCurrPackage->addDirectory("","output",vle::utils::PKG_SOURCE);
        std::string path = mCurrPackage->getOutputDir(vle::utils::PKG_SOURCE);
        mSimThread->setOutputPath(path.c_str());
        mSimThread->load();
        mSimThread->init();
        mOutputs = mSimThread->getOutputs();
    } catch (...) {
        mWidgetTab->progressBar(tr("Fatal Error"));
        QString logMessage;
        logMessage = tr("Project error:") + mSimThread->getError();
        qDebug() << logMessage;
        log(logMessage);
        delete mSimThread;
        mSimThread = 0;
        return;
    }
    mSimDataValid = true;
    mStepCount   = 0;
    QMap<QString, int>::iterator i;
    for (i = mViewLastRow.begin(); i != mViewLastRow.end(); ++i)
        i.value() = 1;

    //ui->buttonGo->setIcon(*(new QIcon(":/icon/resources/control_pause_blue.png")));

    QString expName = mVpz->project().experiment().name().c_str();
    mDuration = mVpz->project().experiment().duration();
    QString startMessage;
    startMessage  = tr("Simulation started. ");
    startMessage += tr("Experiment name: ") + expName + " ";
    startMessage += QString(tr("Duration=%1")).arg(mDuration, 0, 'f', 4);
    log(startMessage);

    // Start the simulation thread
    mThread = new QThread();
    mSimThread->moveToThread(mThread);
    connect(mThread,    SIGNAL(started()), mSimThread, SLOT(run()));
    connect(mSimThread, SIGNAL(step()),    this, SLOT(simulationGetStep()));
    connect(mSimThread, SIGNAL(end()),     this, SLOT(simulationFinished()));
    mThread->start();
}

/**
 * @brief SimLog::simulationGetStep
 *        Called when thread emit a "step" signal (each run loop)
 *
 */
void SimLog::simulationGetStep()
{

    QString line;

    // Update the progress bar
    double debugTime = mSimThread->getCurrentTime();
    double debugElapsed = debugTime - mVpz->project().experiment().begin();
    double percent = (debugElapsed / mDuration) * 100.00;
    mWidgetTab->progressBar(percent);

    if (mOutputs)
    {
        try {
        uint i;
        vle::value::Map::iterator itb = mOutputs->begin();
        vle::value::Map::iterator ite = mOutputs->end();
        for (; itb != ite; itb++)
        {
            QString matViewName = QString::fromStdString(itb->first);
            // If the selected view isn't used by any plot, ignore it
            if ( ! mViewLastRow.contains(matViewName))
                continue;

            vle::value::Matrix* mat;
            mat = mSimThread->getMatrix(itb->second);
            if (mat == 0)
            {
                QString logMessage;
                logMessage  = tr("Project error: could not read Matrix on step ");
                logMessage += QString("%1").arg(mStepCount);
                log(logMessage);
                continue;
            }
            // If no result available, nothing more to do
            if (mat->rows() == 0)
            {
                delete mat;
                continue;
            }

            // Here, we know that :
            //   - selected view is valid and used by one or more sig,
            //   - datas are available

            vle::value::Value *headTest = mat->get(0, 0);
            int headType = headTest->getType();
            if (headType != 3)
            {
                if (mSimDataValid)
                {
                    QString logMessage;
                    logMessage = tr("Simulation plotting error (no header) ");
                    logMessage += QString("view: %1").arg(matViewName);
                    log(logMessage);
                    mSimDataValid = false;
                }
                delete mat;
                continue;
            }

            std::cout << " headers "<< std::endl;

            QList<QString> portNames;
            for (i = 0; i < mat->columns(); i++)
            {
                QString portName;
                vle::value::Value *vHead = mat->get(i, 0);
                QString colHeader = QString(vHead->toString().value().c_str());


                std::cout << colHeader.toStdString();

                QStringList list1 = colHeader.split(":");
                if (list1.size() > 1)
                    portName = list1[1];
                else
                    portName = colHeader;
                portNames.append(portName);
            }

            std::cout << "ooooooooooooooooooo"<< std::endl;

            for (i = mViewLastRow.value(matViewName); i < mat->rows(); i++)
            {

                std::cout << "================================================="<< std::endl;

                double t;
                try {
                    // Read the time position of the result sample
                    t = mat->getDouble(0, i);
                } catch (std::exception& e) {
                    QString logMessage= tr("Simulation: could not get current time of the sample : ");
                    logMessage += QString(e.what());
                    log(logMessage);
                    continue;
                }

                std::cout << "pos = " << i << " time =" << t;

                line = QString("pos = %1,  time = %2").arg(i).arg(t);

                for (uint j = 1; j < mat->columns(); j++)
                {
                    vle::value::Value *vCell;
                    QString portName = portNames.at(j);

                    if (mObsPorts.indexOf(portName) != -1) {
                        line = QString("%1, %2 = ").arg(line).arg(portName);
                    }

                    std::cout << " portname = " <<  portName.toStdString();

                    try {
                        vCell = mat->get(j, i);
                        line = QString("%1%2").arg(line).arg(QString((vCell->writeToString()).c_str()));
                    } catch (std::exception& e) {
                        QString logMessage= tr("Simulation: result read error : ");
                        logMessage += QString(e.what());
                        log(logMessage);
                        continue;
                    }

                    std::cout << std::endl;

                }
                QPlainTextEdit * qlog = mWidgetTab->getPlotWidget();
                qlog->appendPlainText(line);
            }
            mViewLastRow.operator[] (matViewName) = i;
            delete mat;
        } // for { } mOutputs iterator

        } catch(...) {
            //
        }
    }

    mStepCount++;
}

/**
 * @brief SimLog::simulationFinished
 *        Called when thread emit a "finish" signal (at the end of sim)
 *
 */
void SimLog::simulationFinished()
{
    try {
        // Call finish to close vle::devs job
        mSimThread->finish();
        // Terminate simulation thread
        mThread->quit();
        mThread->wait();
        delete mThread;
        mThread = 0;
        // Clean memory
        delete mSimThread;
        mSimThread = 0;
    } catch (...) {
        QString logMessage;
        logMessage = tr("Project error (finish):") + mSimThread->getError();
        log(logMessage);
    }
    log(tr("Simulation finished"));
    mWidgetTab->simStoped();
    mWidgetToolbar->simStoped();
}

Q_EXPORT_PLUGIN2(sim_log, SimLog)
