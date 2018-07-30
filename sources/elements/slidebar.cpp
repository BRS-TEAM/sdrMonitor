/*!
 * \file     slidebar.cpp
 * \brief    SlideBar Element Class
 * \author   CoC BRS
 *
 * \copyright
 * Copyright (c) Continental AG and subsidiaries 2015\n
 * All rights reserved\n
 * The reproduction, transmission or use of this document or its contents is
 * not permitted without express written authority.\n
 * Offenders will be liable for damages. All rights, including rights created
 * by patent grant or registration of a utility model or design, are reserved.
 */

// Essential Headers
#include "headers/elements/slidebar.h"
#include "ui_slidebar.h"

// Connections
#include "headers/elements/bargraphh.h"
#include "headers/elements/progressbargraph.h"

// Data Processing
#include "headers/staticdependencies/valueconversion.h"
#include "headers/staticdependencies/datatypeconversion.h"

// Dialog Display
#include <QTextStream>

// Parent
#include "headers/core/tabcontroller.h"

// Constructor
SlideBar::SlideBar(QString title, TabController *parent) :
    QWidget((QWidget*)parent),
    ui(new Ui::SlideBar)
{
    ui->setupUi(this);
    loadAttribute(title, 0);
    ui->slideBar->setMaximum(MAX_VALUE = 100);

    dStructure = 0;
    tabControllerParent = parent;
    outputType = "decimal";
}

// Destructor
SlideBar::~SlideBar()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

// Initialize name and value
void SlideBar::loadAttribute(QString title, int value) {

    ui->valueLabel->setText(title);
    setValue(value);

}

// Setter : value
void SlideBar::setValue(int value) {

    this->value = value;
    ui->value->setNum(value);
    ui->slideBar->setValue(value);

}

/************************ Initializer Methods used by XML parser. ************************/

// Set the minimum value
void SlideBar::setMinValue(int value) {

    ui->slideBar->setMinimum(value);
    MIN_VALUE =  value;
}


// Enable slidebar
void SlideBar::setEnable(QString state) {
    if(state == "disable") {
        this->setDisabled(true);
    }
    else {
        this->setEnabled(true);
    }
}

// Set the steps between consecutive slide values
void SlideBar::setStepValue(int value) {

    ui->slideBar->setSingleStep(value);

}

// Setter : maxValue
void SlideBar::setMaxValue(int value) {

    ui->slideBar->setMaximum(value);
    MAX_VALUE =  value;
}

// Set the indentification name of SlideBar to name
void SlideBar::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

// Setter : outputType
void SlideBar::setOutputType(QString type) {
    outputType = type;
}

/************************ Getters & Setters ************************/

// Get the indentification name of SlideBar
QString SlideBar::getNameIdentifier() {
    return nameIdentifier;
}

// Getter : value
int SlideBar::getValue() {
    return value;
}

// Setter : dStructure
bool SlideBar::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

// Getter : dStructure
DataStructure * SlideBar::getStructure() {
    return dStructure;
}

// Setter : index
void SlideBar::setIndex(int index) {
    this->index = index;
}

// Getter : index
int SlideBar::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/

// Connect slideBar to barGraph element with given action
void SlideBar::initConnection(BarGraphH * connector, QString action) {

    if(action == "setValue") {
        connect(this, SIGNAL(mySignal(float)), connector, SLOT(updateValue(float)));
    } else if(action == "setPeakValue") {
        connect(this, SIGNAL(mySignal(float)), connector, SLOT(updatePeakValue(float)));
    }

}

// Connect slideBar to progressBarGraph element with given action
void SlideBar::initConnection(ProgressBarGraph * connector, QString action) {

    if(action == "setValue") {
        connect(this, SIGNAL(mySignal(float)), connector, SLOT(updateValue(float)));
    } else if(action == "setPeakValue") {
        connect(this, SIGNAL(mySignal(float)), connector, SLOT(updatePeakValue(float)));
    }

}

/************************ Slots are special methods which execute in response to the connected signal. ************************/

// Response to 'valueChanged()' action signal.
void SlideBar::on_slideBar_valueChanged(int value)
{
    float value_flt = 0;
    int value_single = 0;
    if(ui->slideBar->isEnabled()) {
        if(outputType == "float") {
            value_flt = (float)(value)/(float)(MAX_VALUE-MIN_VALUE);
            value_single =float2single(value_flt);
         //   QTextStream out(stdout);
            //out << "slidebar debug : " << value_single << endl ;
        }
        else
        {
            value_single = value;
        }

        setValue(value);
        emit mySignal(value);

        if (dStructure != 0) {
            QList<DataStructure *> dS;
            dS.append(getStructure());

            QList<QString> values;
            values.append(QString().setNum(value_single));
            tabControllerParent->initHMIWriteReq(dS, values);
        }
    }
}

// Response to a signal from checkBox.
void SlideBar::disable()
{
    this->setDisabled(true);
}

// Response to a signal from checkBox.
void SlideBar::enable()
{
    this->setEnabled(true);
}

// Response to a signal from pushButton
void SlideBar::reset()
{
    setValue(ui->slideBar->minimum());
}

// Updates the element to latest changes.
void SlideBar::updateEvent() {
    int value_int = 0;
    float value_flt = 0;
    if(dStructure != 0 && ui->slideBar->isEnabled()) {
        if(outputType == "float") {
            value_flt = single2float(dStructure->value);
            value_int = (int)(value_flt*(MAX_VALUE-MIN_VALUE));
            value_int = qRound(double(value_int/3.0));
        }
        else
        {
            value_int = dStructure->value;
        }
        setValue(value_int);
    }
}
void SlideBar::RD45()
{
    //this->setValue(8);
    ui->slideBar->setValue(8);
}
void SlideBar::Wave()
{
    //setValue(16);
    ui->slideBar->setValue(16);
}
void SlideBar::Default()
{
    //setValue(14);
    ui->slideBar->setValue(14);
}

/*********************************************************************************************************************************************************/
