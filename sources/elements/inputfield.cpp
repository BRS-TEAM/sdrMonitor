/*!
 * \file     inputfield.cpp
 * \brief    LineEdit Element Class
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
#include "headers/elements/inputfield.h"
#include "ui_inputfield.h"

// Text Handler
#include <QTextStream>

// Data Processing
#include "headers/staticdependencies/valueconversion.h"
#include "headers/staticdependencies/datatypeconversion.h"

// Parent
#include "headers/core/tabcontroller.h"

// Constructor
InputField::InputField(TabController *parent) :
    QWidget((QWidget*)parent),
    ui(new Ui::InputField)
{
    ui->setupUi(this);
    ui->valueLineEdit->setValidator(0);
    defaultValue = "";
    dStructure = 0;
    outputType = "decimal";
    disabledValue = "0";
    resetValue = "RESETVALUE";
    oldValue = -1;
    tabControllerParent = (TabController*) parent;
}

// Destructor
InputField::~InputField()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

// Get the value of lineEdit field
QString InputField::getValueLineEdit() {

    QString str;

    if(this->isEnabled()){
        bool ok = false;
        float value = preprocessFieldText(QString().number(ui->valueLineEdit->text().toFloat(&ok), 'f', 6).toFloat(&ok));
        str = QString().setNum(value);
        if(!halfFormatInterpretation.isEmpty())
            str = QString().setNum((int)value,10);

        if(outputType == "hex") {
            QByteArray bA = ui->valueLineEdit->text().toLatin1();
            char * cStr = bA.data();
            str = QString().setNum(atohex(cStr));
        } else if(outputType == "float") {
            str = QString().setNum(float2single(value));
        }

    } else {
        str = disabledValue;
    }

    return str;
}

// Set the value of lineEdit field
void InputField::setValueLineEdit(QString value) {
    ui->valueLineEdit->setText(value);
}
void InputField::saveValueLineEdit(QString value) {
    bool ok;
    dStructure->value = value.toUInt(&ok,10);
}

float InputField::preprocessFieldText(float src) {

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

// Set Mask
/*
 * Numeric :
 *  - Accept any 'double' value of length upto 100 digits before decimal and 5 digits after decimal
 */
void InputField::setMask(QString mask){
    if(mask == "Numeric")
        ui->valueLineEdit->setValidator(new QDoubleValidator(0, 100, 5, this));
    /*
     * Add more mask here
     */
    else
        ui->valueLineEdit->setValidator(0);
}

// Set place holder text
void InputField::setPlaceHolder(QString placeHolder){
    ui->valueLineEdit->setPlaceholderText(placeHolder);
}

// Set Mode
void InputField::setMode(QString mode) {
    if(mode == "readOnly")
        ui->valueLineEdit->setReadOnly(true);
    else
        ui->valueLineEdit->setReadOnly(false);
}

// Set the indentification name of lineEdit
void InputField::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

// Setter : valueLabel
void InputField::setValueLabel(QString label) {
    valueLabel = label;
    ui->valueLabel->setText(valueLabel);

}

// Setter : defaultValue
void InputField::setDefaultValue(QString defaultValue) {
    ui->valueLineEdit->insert(defaultValue);
    this->defaultValue = defaultValue;
}

// Setter : disabledValue
void InputField::setDisabledValue(QString value) {
    this->disabledValue = value;
}

// Setter : halfFormatInterpretation
void InputField::setHalfFormatInterpretation(QString format) {
    halfFormatInterpretation = format;
}

// Setter : outputType
void InputField::setOutputType(QString type) {
    outputType = type;
}

/************************ Getters & Setters ************************/

// Get the indentification name of lineEdit
QString InputField::getNameIdentifier() {
    return nameIdentifier;
}

// Getter : valueLabel
QString InputField::getValueLabel() {
    return valueLabel;
}

// Setter : dStructure
bool InputField::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

// Getter : dStructure
DataStructure * InputField::getStructure() {
    return dStructure;
}

// Setter : resetValue
void InputField::setResetValue(QString value)
{
    resetValue = value;
}

// Setter : disabledValue
QString InputField::getDisabledValue() const
{
    return disabledValue;
}

// Setter : index (!! NOT IMPLEMENTED !!)
void InputField::setIndex(int index) {
    this->index = index;
}

// Getter : index (!! NOT IMPLEMENTED !!)
int InputField::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/
/*
 *
 * Add more methods here
 *
 */


/************************ Slots are special methods which execute in response to the connected signal. ************************/

// Response to a signal from pushButton
void InputField::clear()
{
    ui->valueLineEdit->clear();
}

// Response to a signal from pushButton
void InputField::reset()
{
    clear();
    ui->valueLineEdit->insert(defaultValue);
}

// Response to a signal from checkBox
void InputField::disable()
{
    //this->hide();
    this->setDisabled(true);

    if (dStructure != 0) {
        oldValue = dStructure->value;
        setValueLineEdit(disabledValue);
        //saveValueLineEdit(oldValue);

        QList<DataStructure *> dS;
        QList<QString> values;
        dS.append(getStructure());

        if(outputType == "hex") {
            bool ok = false;
            //int value = oldValue;
            int value = disabledValue.toInt(&ok, 16);
            values.append(QString().setNum(value));
        }
        else
            values.append(disabledValue);

        if(disabledValue != "DEFAULTVALUE")
            tabControllerParent->initHMIWriteReq(dS, values);
    }
}

// Response to a signal from checkBox
void InputField::enable()
{
    //this->show();
    this->setEnabled(true);

    if (dStructure != 0) {

        bool ok;
        if(resetValue != "RESETVALUE" && oldValue == disabledValue.toInt(&ok, 16))
            oldValue = resetValue.toInt(&ok,16);

        QTextStream out(stdout);
        out << "Old :: " << oldValue << ", Reset :: " << resetValue << ", Disable :: " << disabledValue << endl;
        out << "Old :: " << oldValue << ", Reset (int) :: " << resetValue.toInt(&ok,16) << ", Disable (int) :: " << disabledValue.toInt(&ok,16) << endl;

        float value = convertCoeffForTextField_TimePlot(oldValue, halfFormatInterpretation);
        setValueLineEdit(QString().number(value, 'f', 2));
        if(outputType == "hex") {
            setValueLineEdit(QString().setNum(oldValue, 16).right(dStructure->size/4));
        } else if(outputType == "float") {
            setValueLineEdit(QString().setNum(single2float((int)value), 'f', 4));
        }
        QList<DataStructure *> dS;
        QList<QString> values;
        dS.append(getStructure());
        values.append(getValueLineEdit());

        tabControllerParent->initHMIWriteReq(dS, values);
    }
}

// Response to a signal from checkBox
void InputField::select()                     // Implementation to be considered
{

}

// Response to a signal from checkBox
void InputField::deselect()                     // Implementation to be considered
{

}

// Updates the element to latest changes.
void InputField::updateEvent() {
    if(dStructure != 0) {
        // setValueLineEdit(QString().setNum(convertValueHere(dStructure->value)));
        float value = convertCoeffForTextField_TimePlot(dStructure->value, halfFormatInterpretation);
        QTextStream out(stdout);
        if(halfFormatInterpretation == "UPPER") {
        }
        setValueLineEdit(QString().number(value, 'f', 2));
        if(outputType == "hex") {
            setValueLineEdit(QString().setNum((int)dStructure->value, 16).right(dStructure->size/4));
        } else if(outputType == "float") {
            setValueLineEdit(QString().setNum(single2float((int)value), 'f', 4));
        }
    }
}

//float value = convertCoeffForTextField_TimePlot(oldValue, halfFormatInterpretation);
//setValueLineEdit(QString().number(value, 'f', 2));
//out << "Befor float outtype :: " << QString().number(value, 'f', 2) << endl;
//if(outputType == "hex") {
//    setValueLineEdit(QString().setNum(oldValue, 16).right(dStructure->size/4));
//} else if(outputType == "float") {
//    setValueLineEdit(QString().number(single2float((int)value), 'e', 2));

//    out << QString().number(single2float((int)value), 'e', 2) << endl;
//}
/*********************************************************************************************************************************************************/


//void InputField::setHorizontalSizeLimit(int size) {
//    this->setMaximumWidth(size);
//    this->repaint();
//}

// Slots


/*
 *
float InputField::convertCoeffForTextField_TimePlot(int value) {

    if(halfFormatInterpretation == "upper") {
        double result = half2double((short)(value >> 16));
        return result;
    } else if(halfFormatInterpretation == "lower") {
        double result = half2double((short)(value & 0x0000ffff));
        return result;
    } else return (int)value;
}
 *
 */
