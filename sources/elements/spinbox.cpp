/*!
 * \file     spinbox.cpp
 * \brief    Spin Box Element Class
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
#include "headers/elements/spinbox.h"
#include "ui_spinbox.h"

#include <QDebug>

// Data Processing
#include "headers/staticdependencies/valueconversion.h"
#include "headers/staticdependencies/datatypeconversion.h"

// Parent
#include "headers/core/tabcontroller.h"

// Constructor
SpinBox::SpinBox(TabController *parent) :
    QWidget((QWidget*)parent),
    ui(new Ui::SpinBox)
{
    ui->setupUi(this);
    dStructure = 0;
    disabledValue = 0;
    defaultValue = 0;
    tabControllerParent = (TabController*) parent;
}

// Destructor
SpinBox::~SpinBox()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

// Get the value of spinBox field
float SpinBox::getValueSpinBox() {

    if(this->isEnabled()) {
        return preprocessSpinBoxValue(ui->spinBox->value());
    } else {
        return disabledValue;
    }

}

// Set the value of spinBox field
void SpinBox::setValueSpinBox(float value) {
    ui->spinBox->setValue(value);
}

int SpinBox::preprocessSpinBoxValue(float src) {

    if(halfFormatInterpretation == "UPPER") {
        int result;
        u_int16_t convertOut = 0;
        convertOut = doubles2half(src);
        result = (convertOut & 0xffff) << 16; // convert to 16 bit half
        return result;
    } else if(halfFormatInterpretation == "LOWER") {
        int result;
        u_int16_t convertOut = 0;
        convertOut = doubles2half(src);
        result = (convertOut & 0xffff); // convert to 16 bit half
        return result;
    } else return src;

}

/************************ Initializer Methods used by XML parser. ************************/

void SpinBox::setLabelName(const QString &value)
{
    ui->spinBoxLabel->setText(value);
}


// Set the indentification name of SpinBox
void SpinBox::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

// Setter : defaultValue
void SpinBox::setDefaultValue(float defaultValue) {
    ui->spinBox->setValue(defaultValue);
    this->defaultValue = defaultValue;
}

// Setter : disabledValue
void SpinBox::setDisabledValue(float value) {
    this->disabledValue = value;
}

// Setter : halfFormatInterpretation
void SpinBox::setHalfFormatInterpretation(QString format) {
    halfFormatInterpretation = format;
}

// Set the minimum value of SpinBox
void SpinBox::setSpinBoxLowerRange(float value) {
    ui->spinBox->setMinimum(value);
}

// Set the maximum value of SpinBox
void SpinBox::setSpinBoxUpperRange(float value) {
    ui->spinBox->setMaximum(value);
}

/************************ Getters & Setters ************************/

// Get the indentification name of SpinBox
QString SpinBox::getNameIdentifier() {
    return nameIdentifier;
}

bool SpinBox::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

DataStructure * SpinBox::getStructure() {
    return dStructure;
}

void SpinBox::setIndex(int index) {
    this->index = index;
}

int SpinBox::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/

/************************ Slots are special methods which execute in response to the connected signal. ************************/

// Response to a signal from pushButton
void SpinBox::clear()
{
    ui->spinBox->clear();
}

// Response to a signal from pushButton
void SpinBox::reset()
{
    clear();
    ui->spinBox->setValue(defaultValue);
}

void SpinBox::updateEvent() {

    if (dStructure != 0) {
        float value = convertCoeffForTextField_TimePlot(dStructure->value, halfFormatInterpretation);
        setValueSpinBox(value);
    }
}

/*********************************************************************************************************************************************************/
