/*!
 * \file     ProgressBarGraph.cpp
 * \brief    ProgressBarGraph Element Class
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
#include "headers/elements/progressbargraph.h"
#include "ui_progressbargraph.h"

// Display
#include <QTextStream>

// Data Processing
#include "headers/staticdependencies/valueconversion.h"
#include <math.h>

// Constructor
ProgressBarGraph::ProgressBarGraph(QString title, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressBarGraph)
{
    ui->setupUi(this);
    ui->valueLabel->setText(title);
    ui->pBar->setRange(0, 100);
    ui->pBar->setValue(0);
    ui->pBar->setTextVisible(false);

    ui->unit->hide();

    dStructure = 0;
    maxSourceValue = 32767;
}

// Destructor
ProgressBarGraph::~ProgressBarGraph()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

// Initialize name and value
void ProgressBarGraph::loadAttribute(QString title, float value) {

    ui->valueLabel->setText(title);
    setValue(value);

}

// Redraw the graph to show changes
void ProgressBarGraph::repaint() {

        int max = ui->pBar->maximum();
        int min = ui->pBar->minimum();

        float width=0;
        if(max != min)
            width = ((float)ui->pBar->width()/(max - min));

        if(width == 0)
            width = 1000;

//         QTextStream out(stdout);
//         out << "width pBar : " << width << endl;

        ui->pBar->setStyleSheet(QString("::chunk:horizontal { background: qlineargradient(x0: 0, y1: 0.5, x2: 1, y1: 0.5, stop: 0 #05B8CC, stop: 1 white );"
                                        "margin-right: 2px; width: "
                                        + QString().setNum(width) +
                                        "px; }"
                                        "QProgressBar:horizontal { border: 1px solid gray; border-radius: 3px; background: white; padding: 1px; }" ));

}

// Claculate percent of given 'value' out of 'pBar->maximum' value.
float ProgressBarGraph::getPercent(float value) {

    int max = ui->pBar->maximum();
    int min = ui->pBar->minimum();
    float percent = (value*100)/(max - min);
    return percent;

}

// Set new value to Bar. This function includes call to value conversion functions to calculate appropriate value.
void ProgressBarGraph::setValue(float val) {

    this->value = val;    // ui->pBar->setValue(getPercent(value));
    float newValue = val;

    if(dStructure != 0)
        newValue = convertCoeffForSensor_TimePlot(val, dStructure->dataType, maxSourceValue);

    ui->value->setText(QString().number(newValue, 'f', 4));
    ui->pBar->setValue(newValue);

}

/************************ Initializer Methods used by XML parser. ************************/

// Show / Hide text on graph
void ProgressBarGraph::setTextVisibility(char show) {
    if(show == 'Y' || show == 'y')
        ui->pBar->setTextVisible(true);
    else
        ui->pBar->setTextVisible(false);
}

// Setter : maxSourceValue
void ProgressBarGraph::setMaxValue(int value) {
    maxSourceValue = value;
}

// Setter : minBarValue
void ProgressBarGraph::setMinBarValue(int value) {

    ui->pBar->setMinimum(value);

    repaint();
}

// Setter : maxBarValue
void ProgressBarGraph::setMaxBarValue(int value) {

    ui->pBar->setMaximum(value);
    repaint();

}

// Setter : unit
void ProgressBarGraph::setUnit(QString unit) {

    ui->unit->setText(unit);
    if (unit.length() > 0)
    {
        ui->unit->show();
    }
    else
    {
        ui->unit->hide();
    }

}

// Set the indentification name of ProgressBarGraph
void ProgressBarGraph::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of ProgressBarGraph
QString ProgressBarGraph::getNameIdentifier() {
    return nameIdentifier;
}

// Set dataStructure for this element.
bool ProgressBarGraph::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

// Get dataStructure of this element.
DataStructure * ProgressBarGraph::getStructure() {
    return dStructure;
}

// Setter : \var index (!! NOT IMPLEMENTED !!)
void ProgressBarGraph::setIndex(int index) {
    this->index = index;
}

// Getter : \var index (!! NOT IMPLEMENTED !!)
int ProgressBarGraph::getIndex() {
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
void ProgressBarGraph::on_pBar_valueChanged(int value)
{
    Q_UNUSED(value)

    float percent = getPercent(this->value);

    QString text = QString().setNum(percent);
    text.remove(5,3);                           // floating point precision
    text.append("%");

    ui->pBar->setFormat(text);

}

// // Slots for response to external signals. // //

// Response to a signal from checkBox.
void ProgressBarGraph::disable()
{
    this->setDisabled(true);
}

// Response to a signal from checkBox.
void ProgressBarGraph::enable()
{
    this->setEnabled(true);
}

// Response to a signal from pushButton.
void ProgressBarGraph::refresh()
{
    setValue(value);
}

// Response to a signal from pushButton.
void ProgressBarGraph::clear()
{
    setValue(ui->pBar->minimum());
}

// Response to a signal from slideBar.
void ProgressBarGraph::updateValue(float value)
{
    setValue(value);
}

// Response to a signal from uiPainter. Updates the element to latest changes.
void ProgressBarGraph::updateEvent() {
    if(dStructure != 0) {
        setValue(dStructure->value);
    }
}

/*********************************************************************************************************************************************************/
