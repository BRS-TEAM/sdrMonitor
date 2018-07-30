/*!
 * \file     button.cpp
 * \brief    Push Button Element Class
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
#include "headers/elements/button.h"
#include "ui_button.h"

// Connections
#include "headers/elements/xyplot.h"
#include "headers/elements/inputfield.h"
#include "headers/elements/spinbox.h"
#include "headers/elements/slidebar.h"
#include "headers/elements/bargraphh.h"
#include "headers/elements/progressbargraph.h"

// Dialog Display
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

// Parent
#include "headers/core/tabcontroller.h"

// Constructor
Button::Button(TabController *parent) :
    QWidget((QWidget*)parent),
    ui(new Ui::Button)
{
    ui->setupUi(this);
    dStructure = 0;
    tabControllerParent = (TabController*) parent;
}

// Destructor
Button::~Button()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

/*
 *
 * Add more methods here
 *
 */

/************************ Initializer Methods used by XML parser. ************************/

// Set the button title
void Button::setButtonTitle(QString label) {
    ui->pushButton->setText(label);
}

// Assign function to the button
void Button::setButtonFunction(QString function) {

    if (function == "save") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(saveValue()));
    } else if (function == "load") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(loadValue()));
    } else if (function == "reset") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(resetValue()));
    }
}

 // Setter : resetVal
void Button::setResetValue(int value) {
    resetVal = value;
}

// Set the indentification name of BarGraphH
void Button::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of Button
QString Button::getNameIdentifier() {
    return nameIdentifier;
}

// Fetch the button title
QString Button::getButtonTitle() {
    return ui->pushButton->text();
}

// Setter : dStructure
bool Button::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

// Getter : dStructure
DataStructure * Button::getStructure() {
    return dStructure;
}

// Setter : index
void Button::setIndex(int index) {
    this->index = index;
}

// Getter : index
int Button::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/

// Connect pushButton to lineEdit element with given action
void Button::initConnection(InputField * connector, QString action) {

    if(action == "clear") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(clear()));
    } else if(action == "reset") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(reset()));
    } else if(action == "load") {
        sifLoadConnector.append(connector);
        connect(this, SIGNAL(loadSignal()), connector, SLOT(updateEvent()));
    } else if(action == "save") {
        sifSaveConnector.append(connector);
    }

}

// Connect pushButton to spinBox element with given action
void Button::initConnection(SpinBox * connector, QString action) {

    if(action == "clear") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(clear()));
    } else if(action == "reset") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(reset()));
    } else if(action == "load") {
        spinBoxLoadConnector.append(connector);
        connect(this, SIGNAL(loadSignal()), connector, SLOT(updateEvent()));
    } else if(action == "save") {
        spinBoxSaveConnector.append(connector);
    }

}

// Connect pushButton to slideBar element with given action
void Button::initConnection(SlideBar * connector, QString action) {

    if(action == "reset") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(reset()));
    }

}

// Connect pushButton to barGraph element with given action
void Button::initConnection(BarGraphH * connector, QString action) {

    if(action == "refresh") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(refresh()));
    } else if(action == "clear") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(clear()));
    }

}

// Connect pushButton to ProgressBarGraph element with given action
void Button::initConnection(ProgressBarGraph * connector, QString action) {

    if(action == "refresh") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(refresh()));
    } else if(action == "clear") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(clear()));
    }

}

// Connect pushButton to xyPlot element with given action
void Button::initConnection(XYPlot * connector, QString action) {

    if(action == "refresh") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(on_refreshButton_clicked()));
    } else if(action == "clear" || action == "reset") {
        connect(ui->pushButton, SIGNAL(clicked(bool)), connector, SLOT(on_clearButton_clicked()));
    }

}

/************************ Slots are special methods which execute in response to the connected signal. ************************/

// Response to a signal from pushButton.
void Button::saveValue() {
    QList<DataStructure *> dS;
    QList<QString> values;

    // Collect Data From connected Input Fields
    for(int i = 0; i < sifSaveConnector.count(); i++) {
        dS.append(sifSaveConnector.at(i)->getStructure());
        values.append(sifSaveConnector.at(i)->getValueLineEdit());
    }

    if(dS.count()) {

        // Find Pairs
        // Needed for coefficient having halfFormatInterpretation
        QList<int> pairs;
        for(int i = 0; i < dS.count(); i++) {
            if(dS.at(i) != 0) {
                QString address = dS.at(i)->address;
                for(int j = i+1; j < dS.count(); j++ ) {
                    if(address == dS.at(j)->address) {
                        pairs.append(i);
                        pairs.append(j);
                        break;
                    }
                }
            }
        }

        // Merge Pair Values
        bool ok = false;
        for(int i = 0; i < pairs.count(); i=i+2) {
            int value = QString(values.at(pairs.at(i))).toInt(&ok,10) | QString(values.at(pairs.at(i+1))).toInt(&ok,10);
            values.replace(pairs.at(i), QString().setNum(value));
            values.replace(pairs.at(i+1), QString().setNum(value));
        }

        tabControllerParent->initHMIWriteReq(dS, values);
    }

    dS.clear();

    // Collect Data From connected SpinBox Fields
    for(int i = 0; i < spinBoxSaveConnector.count(); i++) {
        dS.append(spinBoxSaveConnector.at(i)->getStructure());
        values.append(QString().setNum(spinBoxSaveConnector.at(i)->getValueSpinBox()));
    }

    if(dS.count()) {
        tabControllerParent->initHMIWriteReq(dS, values);
    }

    dS.clear();

    //    QMessageBox message;
    //    message.setText("Save");
    //    message.exec();

}

// Response to a signal from pushButton.
void Button::loadValue() {

    // Collect Data From connected Input Fields
    QList<DataStructure *> dS;
    for(int i = 0; i < sifLoadConnector.count(); i++)
        dS.append(sifLoadConnector.at(i)->getStructure());

    if(dS.count()) {

        tabControllerParent->initHMIReadReq(dS);

        emit loadSignal();
//        for(int i = 0; i < sifLoadConnector.count(); i++)
//            sifLoadConnector.at(i)->updateValue();
    }

    dS.clear();

    // Collect Data From connected SpinBox Fields
    for(int i = 0; i < spinBoxLoadConnector.count(); i++)
        dS.append(spinBoxLoadConnector.at(i)->getStructure());

    if(dS.count()) {

        tabControllerParent->initHMIReadReq(dS);

        emit loadSignal();
    }

    dS.clear();
}

// Response to a signal from pushButton.
void Button::resetValue() {

    QList<DataStructure *> dS;
    dS.append(getStructure());

    QList<QString> values;
    values.append(QString().setNum(resetVal));

    tabControllerParent->initHMIWriteReq(dS, values);

}

// Response to a signal from checkBox.
void Button::disable() {
    this->setDisabled(true);
}

// Response to a signal from checkBox.
void Button::enable() {
    this->setEnabled(true);
}

/*********************************************************************************************************************************************************/
