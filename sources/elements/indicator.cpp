/*!
 * \file     indicator.cpp
 * \brief    Indicator Element Class
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
#include "headers/elements/indicator.h"
#include "ui_indicator.h"


// Data Processing
#include "headers/staticdependencies/datatypeconversion.h"
#include "headers/staticdependencies/valueconversion.h"

// Constructor
Indicator::Indicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Indicator)
{
    ui->setupUi(this);
    activeState = "Active";
    progressiveState = "In process...";
    inactiveState = "Inactive";
    valueLabel = "N/A";

    changeState(NONE);

    dStructure = 0;
}

// Destructor
Indicator::~Indicator()
{
    delete ui;
}


/************************ General Methods specific to this class. ************************/


/************************ Initializer Methods used by XML parser. ************************/

void Indicator::setLabelName(const QString &value)
{
    valueLabel = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}

void Indicator::setInactiveStateLabel(const QString &value)
{
    inactiveState = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}

void Indicator::setProgressiveStateLabel(const QString &value)
{
    progressiveState = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}

void Indicator::setActiveStateLabel(const QString &value)
{
    activeState = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}


// Set the indentification name of Indicator
void Indicator::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of BarGraphH
QString Indicator::getNameIdentifier() {
    return nameIdentifier;
}

bool Indicator::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

DataStructure * Indicator::getStructure() {
    return dStructure;
}

void Indicator::setIndex(int index) {
    this->index = index;
}

int Indicator::getIndex() {
    return index;
}

/************************ Private Methods specific to this class. ************************/
void Indicator::setText(QString stateLabel) {
    ui->indicatorWidget->setText(stateLabel);
}

/************************ Slots are special methods which execute in response to the connected signal. ************************/

void Indicator::changeState(IndicatorStates indicatorState) {
    if(indicatorState == ACTIVE) {
        if(!activeStateColour.isEmpty())
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                       "background-color:" + activeStateColour+ "; }"));
        else
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                   "background-color: green; }"));

        setText(activeState);
    } else if(indicatorState == PROGRESSIVE) {
        if(!progressiveStateColour.isEmpty())
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                       "background-color:" + progressiveStateColour+ "; }"));
        else
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                   "background-color: yellow; }"));
        setText(progressiveState);
    } else if(indicatorState == INACTIVE) {
        if(!inactiveStateColour.isEmpty())
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                       "background-color:" + inactiveStateColour+ "; }"));
        else
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                   "background-color: red; }"));
        setText(inactiveState);
    } else {
        if(!noneStateColour.isEmpty())
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                       "background-color:" + noneStateColour+ "; }"));
        else
            ui->indicatorWidget->setStyleSheet(QString("QRadioButton::indicator { width: 23px; height: 23px; border-width: 2px; border-radius: 11px; border-color: black;"
                                                   "background-color: white; }"));
        setText(valueLabel);
    }
}


void Indicator::updateEvent() {
    if (dStructure != 0) {
        if(dStructure->value == ACTIVE)
            changeState(ACTIVE);
        else if(dStructure->value == PROGRESSIVE)
            changeState(PROGRESSIVE);
        else if(dStructure->value == INACTIVE)
            changeState(INACTIVE);
        else
            changeState(NONE);
    }
}

void Indicator::setInactiveStateColour(const QString &value)
{
    inactiveStateColour = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}

void Indicator::setProgressiveStateColour(const QString &value)
{
    progressiveStateColour = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}

void Indicator::setActiveStateColour(const QString &value)
{
    activeStateColour = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}

void Indicator::setNoneStateColour(const QString &value)
{
    noneStateColour = value;
    if (dStructure != 0)
    {
        changeState((IndicatorStates)dStructure->value);
    }
    else
    {
        changeState(NONE);
    }
}

/*********************************************************************************************************************************************************/
