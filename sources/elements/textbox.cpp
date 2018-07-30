/*!
 * \file     textbox.cpp
 * \brief    Text Box Element Class
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
#include "headers/elements/textbox.h"
#include "ui_textbox.h"

#include <stdio.h>
#include <QDebug>
#include "headers/staticdependencies/valueconversion.h"

// Constructor
TextBox::TextBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextBox)
{
    ui->setupUi(this);
}

// Destructor
TextBox::~TextBox()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

void TextBox::setDataSourceIdentifier(QString label) {
    dSourceIdentifier.append(label);
}

QString TextBox::getDataSourceIdentifier(int index) {
    if(dSourceIdentifier.count() > index)
        return dSourceIdentifier.at(index);
    return "";
}

int TextBox::getDataSourceIdentifierCount() {
    return dSourceIdentifier.count();
}

bool TextBox::setDataSource(DataStructure * structure) {

    if (structure != 0) {
        dSource.append(structure);
        return true;
    }

    return false;
}

void TextBox::resetDataSource() {

    dSource.clear();

}

/************************ Initializer Methods used by XML parser. ************************/

void TextBox::setLabelName(const QString &value)
{
    ui->valueLabel->setText(value);
}

void TextBox::isBoxVisible(const QString &value)
{
    if(value == "N" || value == "n") {
        ui->valueLineEdit->hide();
    }
}

// Set the indentification name of TextBox
void TextBox::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of TextBox
QString TextBox::getNameIdentifier() {
    return nameIdentifier;
}

bool TextBox::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

DataStructure * TextBox::getStructure() {
    return dStructure;
}

void TextBox::setIndex(int index) {
    this->index = index;
}

int TextBox::getIndex() {
    return index;
}

/************************ Methods used in connected state with other elements. ************************/

/************************ Slots are special methods which execute in response to the connected signal. ************************/
void TextBox::updateEvent() {

    if (dStructure != 0) {

        int value = dStructure->value;
        int reverseValue = 0;
        byteReverse(&value, &reverseValue, 4);

        QByteArray bA = (QString().setNum(reverseValue,16)).toLatin1();
        std::string str = bA.toStdString();

        ui->valueLineEdit->setText(hexToString(str));

    } else if(dSource.count()) {

        int value;
        int reverseValue;
        QString result;

        for(int i = 0; i < dSource.count(); i++) {
            value = dSource.at(i)->value;
            reverseValue = 0;

            byteReverse(&value, &reverseValue, 4);

            QByteArray bA = (QString().setNum(reverseValue,16)).toLatin1();
            std::string str = bA.toStdString();
                result.append(hexToString(str));
        }

        ui->valueLineEdit->setText(result);
    }
}

/*********************************************************************************************************************************************************/
