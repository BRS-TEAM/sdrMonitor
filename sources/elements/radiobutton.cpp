/*!
 * \file     radiobutton.cpp
 * \brief    Radio Button Group Element Class
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
#include "headers/elements/radiobutton.h"
#include "ui_radiobutton.h"

#include <QDebug>
#include <QRadioButton>
#include <QButtonGroup>
#include "headers/elements/timevalueplot.h"
#include "headers/elements/slidebar.h"
// Parent
#include "headers/core/tabcontroller.h"

// Constructor
RadioButton::RadioButton(TabController *parent) :
    QWidget(parent),
    ui(new Ui::RadioButton)
{
    ui->setupUi(this);

    buttonGroup = new QButtonGroup(this);

    dStructure = 0;
    updateFlag = false;
    tabControllerParent = (TabController*) parent;

    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(initConnection(SlideBar*,QString)));
    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onRadioButtonClicked(int)));


}


// Destructor
RadioButton::~RadioButton()
{
    delete ui;

    for(int i = 0; i < rButton.count(); i++)
        delete rButton.at(i);
    rButton.clear();

    delete buttonGroup;
    //connector->deleteLater();
}

/************************ General Methods specific to this class. ************************/


/************************ Initializer Methods used by XML parser. ************************/

void RadioButton::setLabelName(const QString &value)
{
    ui->radioButtonGroupLabel->setText(value);
}

void RadioButton::addNewButton(QString label) {

    rButton.append(new QRadioButton(label));

    buttonGroup->addButton(rButton.last());
    buttonGroup->setId(rButton.last(), rButton.count()-1);

    ui->radioButtonLayout->addWidget(rButton.last());

    if(rButton.size() == 1) {
        updateFlag = true;
        rButton.first()->setChecked(true);
        updateFlag = false;
    }
}

void RadioButton::setButtonAttributes(QString value, QString isEnable, QString isSelected) {
    if(value != "DEFAULT")
        rButtonValue.append(value.toInt());
    else
        rButtonValue.append(rButtonValue.size());

    if(isEnable == "N" || isEnable == "n")
        rButton.last()->setDisabled(true);

    if(isSelected == "Y" || isSelected == "y")
        rButton.last()->setChecked(true);
}

// Set the indentification name of RadioButton
void RadioButton::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of RadioButton
QString RadioButton::getNameIdentifier() {
    return nameIdentifier;
}

bool RadioButton::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

DataStructure * RadioButton::getStructure() {
    return dStructure;
}

void RadioButton::setIndex(int index) {
    this->index = index;
}

int RadioButton::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/

// Connect radioButton to TimeValuePlot element with given action
void RadioButton::initConnection(TimeValuePlot * connector, QString action) {

    if(action == "selectSourceMode") {
        connect(this, SIGNAL(radioButtonClicked(QString)), connector, SLOT(selectSourceMode(QString)));
    }
}
void RadioButton::initConnection(SlideBar* connector, QString action)
{
    if(action == "RD45")
    {
        connect(this, SIGNAL(radioButtonClicked(QString)), connector, SLOT(RD45()));

    }
    else if(action == "Wave")
    {
        connect(this, SIGNAL(radioButtonClicked(QString)), connector, SLOT(Wave()));

    }
    else if(action == "Default")
    {
        connect(this, SIGNAL(radioButtonClicked(QString)), connector, SLOT(Default()));

    }
}
/************************ Slots are special methods which execute in response to the connected signal. ************************/

void RadioButton::onRadioButtonClicked(int id)
{
    qDebug() << "button " << id << " " << buttonGroup->button(id)->text() << " is " << buttonGroup->button(id)->isChecked();
    emit radioButtonClicked(buttonGroup->button(id)->text());

    if (dStructure != 0 && !updateFlag) {
        QList<DataStructure *> dS;
        dS.append(getStructure());

        QList<QString> value;
        value.append(QString().setNum(rButtonValue.at(id)));
        tabControllerParent->initHMIWriteReq(dS, value);
    } else {
        updateFlag = false;
    }

//        connector->RD45();
//        connect(this, SIGNAL(radioButtonClicked(QString)), connector, SLOT(RD45()));


//    else if(action == "Wave")
//    {
//        connect(this, SIGNAL(radioButtonClicked(QString)), connector, SLOT(Wave()));

//    }
//    else if(action == "Default")
//    {
//        connect(this, SIGNAL(radioButtonClicked(QString)), connector, SLOT(Default()));

//    }

    if (id == 0)
    {
        connector = new SlideBar;
        connector->RD45();
    }
    if (id == 1)
    {
        connector = new SlideBar;
        connector->Wave();
    }
    if (id == 2)
    {
        connector = new SlideBar;
        connector->Default();
    }



}


void RadioButton::updateEvent() {

    if (dStructure != 0) {
        qDebug() << "upodate Value";
        updateFlag = true;
        for(int i = 0; i < rButton.count(); i++)
            if(dStructure->value == rButtonValue.at(i)) {
                buttonGroup->button(i)->setChecked(true);
                break;
            }
        updateFlag = false;
    }
}

/*********************************************************************************************************************************************************/
