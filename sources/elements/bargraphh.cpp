/*!
 * \file     bargraphh.cpp
 * \brief    BarGraphH Element Class
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
#include "headers/elements/bargraphh.h"
#include "ui_bargraphh.h"

// Display
#include <QTextStream>

// Data Processing
#include "headers/staticdependencies/valueconversion.h"
#include <math.h>

// Constructor
BarGraphH::BarGraphH(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarGraphH)
{
    ui->setupUi(this);
    ui->valueLabel->setText("bGraph");
    // ui->pBar->setRange(0, 100);

    ui->pBar->setValue(0);
    ui->pBar->setTextVisible(false);



    barVal0to1 =false;
    ui->unit->hide();

    dStructure = 0;
    maxSourceValue = 32767;
    minSourceValue = 0;
    maxBarValue = 100;
    minBarValue = 0;
    peakValue = 100;
    multiplicatorValue = 1.0;
    ui->pBar->setMaximum(100);
    ui->pBar->setMinimum(0);
}

// Destructor
BarGraphH::~BarGraphH()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

// Initialize name and value
void BarGraphH::loadAttribute(QString title, double value) {

    ui->valueLabel->setText(title);
    setValue(value);

}

// Redraw the graph to show changes
void BarGraphH::repaint() {



    if(barValue > peakValue) {

        //int max = barValue;
        //int min = minBarValue;
        int peakPoint = getPercent(peakValue);

        ui->pBar->setStyleSheet(QString("::chunk:horizontal { background-color: qlineargradient(x0: 0, x2: 1, stop: 0 #05B8CC, stop: "
                                        + QString().setNum(peakPoint) +
                                        " #05B8CC, stop: "
                                        + QString().setNum(peakPoint + 0.00001) +
                                        " red, stop: 1 red )}"
                                        "::chunk { background-color: #05B8CC; margin-top: 0.5px; margin-bottom: 0.5px; border-radius: 5px; }"
                                        "QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                                        "QProgressBar:horizontal { border: 2px solid gray; border-radius: 3px; background: white; }"
                                        "::chunk:vertical { background-color: qlineargradient(y0: 0, y2: 1, stop: 1 #05B8CC, stop: "
                                        + QString().setNum(1 - peakPoint) +
                                        " #05B8CC, stop: "
                                        + QString().setNum(1 - peakPoint - 0.00001) +
                                        " red, stop: 0 red )}"
                                        "QProgressBar:vertical { border: 2px solid gray; border-radius: 3px; background: white;}"));

    } else {
        ui->pBar->setStyleSheet(QString("::chunk:horizontal { background-color: qlineargradient(x0: 0, x2: 1, stop: 0 #05B8CC, stop: 1 #05B8CC )}"
                                        "::chunk {background-color: #05B8CC; margin-top: 0.5px; margin-bottom: 0.5px; border-radius: 5px; }"
                                        "QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                                        "QProgressBar:horizontal { border: 2px solid gray; border-radius: 3px; background: white; }"
                                        "::chunk:vertical { background-color: qlineargradient(y0: 1, y2: 0, stop: 1 #05B8CC, stop: 0 #05B8CC )}"
                                        "QProgressBar:vertical { border: 2px solid gray; border-radius: 3px; background: white;}"));
    }
}

// Claculate percent of given 'value' out of 'pBar->maximum' value.
double BarGraphH::getPercent(double value) {

    double percent = ((value-minBarValue)/(maxBarValue - minBarValue))*100;
    return percent;

}

// Set new value to Bar. This function includes call to value conversion functions to calculate appropriate value.
void BarGraphH::setValue(double val) {

    this->value = val; // ui->pBar->setValue(getPercent(value)); // ui->pBar->setValue(value);
    double newValue = val;

    if(dStructure != 0) {
        val = val * multiplicatorValue;
        if(val < minSourceValue)
        {
            val = minSourceValue;
        }
        newValue = convertCoeffForSensor_TimePlot(val, dStructure->dataType, maxSourceValue);
    }

    ui->value->setText(QString().number(newValue, 'f', 2)); // ui->value->setNum(value);

    barValue = newValue;
    ui->pBar->setValue((int)getPercent(barValue));


    repaint();

}

/************************ Initializer Methods used by XML parser. ************************/

// Show / Hide text on graph
void BarGraphH::setTextVisibility(char show) {
    if(show == 'Y' || show == 'y')
        ui->pBar->setTextVisible(true);
    else
        ui->pBar->setTextVisible(false);
}

// Show / Hide Level Bar
void BarGraphH::setBarVisiblity(char show) {

    if(show == 'N' || show == 'n')
        ui->pBar->setVisible(false);
    else
        ui->pBar->setVisible(true);

}

// Setter : peakValue
void BarGraphH::setPeakValue(double value) {

    peakValue = value;
    repaint();

}

// Setter : maxSourceValue
void BarGraphH::setMaxValue(double value) {
    maxSourceValue = value;
}

// Setter : minBarValue
void BarGraphH::setMinBarValue(double value) {

    minBarValue = value;
    //ui->pBar->setMinimum(value);

}

// Setter : maxBarValue
void BarGraphH::setMaxBarValue(double value) {

    if (maxBarValue == peakValue)
    {
        peakValue = value;
    }
    maxBarValue = value;

}

// Setter : unit
void BarGraphH::setUnit(QString unit) {

    ui->unit->setText(unit);

}

// Set the indentification name of BarGraphH
void BarGraphH::setNameIdentifier(QString name) {
    nameIdentifier = name;
}


/************************ Getters & Setters ************************/

// Get the indentification name of BarGraphH
QString BarGraphH::getNameIdentifier() {
    return nameIdentifier;
}

// Set dataStructure for this element.
bool BarGraphH::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
    {
        if (dStructure->dataType == "REGULAR")
        {
            //Value will be converted as % so we need to set the
        }

        return true;
    }
    return false;
}

// Get dataStructure of this element.
DataStructure * BarGraphH::getStructure() {
    return dStructure;
}

// Setter : \var index (!! NOT IMPLEMENTED !!)
void BarGraphH::setIndex(int index) {
    this->index = index;
}

// Getter : \var index (!! NOT IMPLEMENTED !!)
int BarGraphH::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/
/*
 *
 * Add methods here
 *
 */

/************************ Slots are special methods which execute in response to the connected signal. ************************/


// // Slots for response to internal signals // //

// Response to 'valueChanged(int)' action signal.
void BarGraphH::on_pBar_valueChanged(int value)
{
    Q_UNUSED(value)

    double percent = getPercent(this->value);

    QString text = QString().setNum(percent);
    text.remove(5,3);                           // floating point precision
    text.append("%");

    ui->pBar->setFormat(text);

}

// // Slots for response to external signals. // //

// Response to a signal from checkBox.
void BarGraphH::disable()
{
    this->setDisabled(true);
}

// Response to a signal from checkBox.
void BarGraphH::enable()
{
    this->setEnabled(true);
}

// Response to a signal from pushButton.
void BarGraphH::refresh()
{
    setValue(value);
}

// Response to a signal from pushButton.
void BarGraphH::clear()
{
    setValue(ui->pBar->minimum());
}

// Response to a signal from slideBar.
void BarGraphH::updateValue(double value)
{
    setValue(value);
}

// Response to a signal from slideBar.
void BarGraphH::updatePeakValue(double value)
{
    setPeakValue(value);
}

// Response to a signal from uiPainter. Updates the element to latest changes.
void BarGraphH::updateEvent() {
    if(dStructure != 0) {
        setValue(dStructure->value);
    }
}

/*********************************************************************************************************************************************************/


void BarGraphH::setBarOrientation(QString align) {

    if(align == "V" || align == "v") {
        QWidget::setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
        QWidget::setMinimumSize(25,25);
        QWidget::setMaximumSize(600,400);
        ui->pBar->setOrientation(Qt::Vertical);
        ui->pBar->setMinimumSize(25,150);
        ui->pBar->setMaximumSize(25,150);
        ui->Layout->setDirection(QBoxLayout::TopToBottom);
        //ui->Layout->addWidget(ui->valueLabel);
        //ui->Layout->addWidget(ui->pBar);
        //ui->Layout->addWidget(ui->value);
        //ui->Layout->addWidget(ui->unit);
        ui->Layout->setAlignment(ui->valueLabel,Qt::AlignHCenter);
        ui->Layout->setAlignment(ui->pBar,Qt::AlignHCenter);
        ui->Layout->setAlignment(ui->value,Qt::AlignHCenter);
        ui->Layout->setAlignment(ui->unit,Qt::AlignHCenter);
        ui->value->setMinimumWidth(55);
        ui->value->setMaximumWidth(79);
        ui->value->setAlignment(Qt::AlignHCenter);
        ui->valueLabel->setMinimumWidth(0);
        ui->valueLabel->setMinimumHeight(25);
        ui->valueLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        ui->unit->setMinimumWidth(0);
        ui->unit->setAlignment(Qt::AlignHCenter);
    }

}

void BarGraphH::setMinValue(double value)
{
    minSourceValue = value;
}

void BarGraphH::setMultiplicatorValue(double value)
{
    multiplicatorValue = value;
}

