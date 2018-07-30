/*!
 * \file     checkbox.cpp
 * \brief    CheckBox Element Class
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
#include "headers/elements/checkbox.h"
#include "ui_checkbox.h"

// Connections
#include "headers/elements/inputfield.h"
#include "headers/elements/button.h"
#include "headers/elements/slidebar.h"
#include "headers/elements/bargraphh.h"
#include "headers/elements/progressbargraph.h"
#include "headers/elements/xyplot.h"
#include "headers/elements/timevalueplot.h"

// Parent
#include "headers/core/tabcontroller.h"

// Constructor
CheckBox::CheckBox(TabController *parent) :
    QWidget((QWidget*)parent),
    ui(new Ui::CheckBox)
{
    ui->setupUi(this);
    ui->checkBox->setCheckable(true);
    ui->checkBox->setCheckState(Qt::Unchecked);

    checkedValue = 0;
    uncheckedValue = 0x7ff;
    dStructure = 0;
    updateFlag = false;
    tabControllerParent = (TabController*) parent;

    connect(this, SIGNAL(checked()), this, SLOT(checkedEvent()));
    connect(this, SIGNAL(unchecked()), this, SLOT(uncheckedEvent()));
}

// Destructor
CheckBox::~CheckBox()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

// Check the checkBox state
bool CheckBox::isCheckBoxChecked() {
    if(ui->checkBox->isChecked())
    return true;
    else
    return false;
}

/************************ Initializer Methods used by XML parser. ************************/

// Set the checkBox title
void CheckBox::setCheckBoxTitle(QString label) {
    ui->checkBox->setText(label);
}

// Set the checkBox state
void CheckBox::setCheckBoxState(QString state) {
    if(state == "checked")
    ui->checkBox->setCheckState(Qt::Checked);
    else
    ui->checkBox->setCheckState(Qt::Unchecked);
}

// Set the indentification name of BarGraph
void CheckBox::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

// Set the default value that should be transmitted to the target when the CheckBox is Checked
void CheckBox::setCheckedValue(int value) {
    checkedValue = value;
}

// Set the default value that should be transmitted to the target when the CheckBox is Unchecked
void CheckBox::setUncheckedValue(int value) {
    uncheckedValue = value;
}

/************************ Getters & Setters ************************/

// Get the indentification name of checkBox
QString CheckBox::getNameIdentifier() {
    return nameIdentifier;
}

// Setter : dStructure
bool CheckBox::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

// Getter : dStructure
DataStructure * CheckBox::getStructure() {
        return dStructure;
}

// Setter : index (!! NOT IMPLEMENTED !!)
void CheckBox::setIndex(int index) {
    this->index = index;
}

// Getter : index (!! NOT IMPLEMENTED !!)
int CheckBox::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/

// Connect checkBox to lineEdit element with given action
void CheckBox::initConnection(InputField * connector, QString action) {

    if(action == "disable") {
        connect(this, SIGNAL(checked()), connector, SLOT(disable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(enable()));
    } else if(action == "enable") {
        connect(this, SIGNAL(checked()), connector, SLOT(enable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(disable()));
    } else if(action == "select") {
        connect(this, SIGNAL(checked()), connector, SLOT(select()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(deselect()));
    }
}

// Connect checkBox to pushButton element with given action
void CheckBox::initConnection(Button * connector, QString action) {

    if(action == "disable") {
        connect(this, SIGNAL(checked()), connector, SLOT(disable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(enable()));
    } else if(action == "enable") {
        connect(this, SIGNAL(checked()), connector, SLOT(enable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(disable()));
    }
}

// Connect checkBox to SlideBar element with given action
void CheckBox::initConnection(SlideBar * connector, QString action) {

    if(action == "disable") {
        connect(this, SIGNAL(checked()), connector, SLOT(disable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(enable()));
    } else if(action == "enable") {
        connect(this, SIGNAL(checked()), connector, SLOT(enable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(disable()));
    }
    else if(action == "RD45")
    {
        connect(this, SIGNAL(checked()), connector, SLOT(RD45()));
    
    }
    else if(action == "Wave")
    {
        connect(this, SIGNAL(checked()), connector, SLOT(Wave()));
       
    }
    else if(action == "Default")
    {
        connect(this, SIGNAL(checked()), connector, SLOT(Default()));
        
    }
}

// Connect checkBox to BarGraph element with given action
void CheckBox::initConnection(BarGraphH * connector, QString action) {

    if(action == "disable") {
        connect(this, SIGNAL(checked()), connector, SLOT(disable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(enable()));
    } else if(action == "enable") {
        connect(this, SIGNAL(checked()), connector, SLOT(enable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(disable()));
    }
}

// Connect checkBox to ProgressBarGraph element with given action
void CheckBox::initConnection(ProgressBarGraph * connector, QString action) {

    if(action == "disable") {
        connect(this, SIGNAL(checked()), connector, SLOT(disable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(enable()));
    } else if(action == "enable") {
        connect(this, SIGNAL(checked()), connector, SLOT(enable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(disable()));
    }
}

// Connect checkBox to XYPlot element with given action
void CheckBox::initConnection(XYPlot * connector, QString action) {

    if(action == "disable") {
        connect(this, SIGNAL(checked()), connector, SLOT(disable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(enable()));
    } else if(action == "enable") {
        connect(this, SIGNAL(checked()), connector, SLOT(enable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(disable()));
    }
}

// Connect checkBox to TimeValuePlot element with given action
void CheckBox::initConnection(TimeValuePlot * connector, QString action) {

    if(action == "disable") {
        connect(this, SIGNAL(checked()), connector, SLOT(disable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(enable()));
    } else if(action == "enable") {
        connect(this, SIGNAL(checked()), connector, SLOT(enable()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(disable()));
    } else if(action == "hide") {
        connect(this, SIGNAL(checked()), connector, SLOT(hideWidget()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(showWidget()));
    } else if(action == "show") {
        connect(this, SIGNAL(checked()), connector, SLOT(showWidget()));
        connect(this, SIGNAL(unchecked()), connector, SLOT(hideWidget()));
    }
}

/************************ Slots are special methods which execute in response to the connected signal. ************************/

// Response to 'stateChanged()' action signal.
void CheckBox::on_checkBox_stateChanged(int state)
{
    if(state == Qt::Checked) { emit checked(); }
    else { emit unchecked(); }
}

// Response to 'checked()' action signal.
void CheckBox::checkedEvent()
{    
    if (dStructure != 0 && !updateFlag) {
        QList<DataStructure *> dS;
        dS.append(getStructure());

        QList<QString> value;
        value.append(QString().setNum(checkedValue));
        tabControllerParent->initHMIWriteReq(dS, value);
    } else {
        updateFlag = false;
    }
}

// Response to 'unchecked()' action signal.
void CheckBox::uncheckedEvent()
{

    if (dStructure != 0 && !updateFlag) {
        QList<DataStructure *> dS;
        dS.append(getStructure());

        QList<QString> value;
        value.append(QString().setNum(uncheckedValue));
        tabControllerParent->initHMIWriteReq(dS, value);
    } else {
        updateFlag = false;
    }
}

// Updates the element to latest changes.
void CheckBox::updateEvent() {

    if (dStructure != 0) {

        updateFlag = true;

        if(dStructure->value == checkedValue) {
            ui->checkBox->setChecked(true);
            emit checked();
        } else if(dStructure->value == uncheckedValue) {
            ui->checkBox->setChecked(false);
            emit unchecked();
        } else {
            updateFlag = false;
        }
    }

}

/*********************************************************************************************************************************************************/
