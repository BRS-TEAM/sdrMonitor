/*!
 * \file     readwritewindow.cpp
 * \brief    Read/Write Coefficient Window
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

#include "headers/core/readwritewindow.h"
#include "ui_readwritewindow.h"

#include <QDebug>
#include "headers/core/communicationcontroller.h"
#include "headers/core/datastructure.h"
#include <QTimerEvent>
#include "headers/staticdependencies/datatypeconversion.h"

ReadWriteWindow::ReadWriteWindow(CommunicationController * parent) :
    QDialog((QWidget*)parent),
    ui(new Ui::ReadWriteWindow)
{
    ui->setupUi(this);

    parentCommunicationController = (CommunicationController *)parent;

    ui->sizeLineEdit->setValidator(new QIntValidator(1, 65535, this));

    ui->addressLabel->setBuddy(ui->addressSpinBox);
    ui->sizeLabel->setBuddy(ui->sizeLineEdit);

    readButton = new QPushButton(tr("Read"), this);
    readButton->setDefault(true);
    connect(readButton, SIGNAL(clicked()), this, SLOT(readButton_clicked()));

    writeButton = new QPushButton(tr("Write"), this);
    connect(writeButton, SIGNAL(clicked()), this, SLOT(writeButton_clicked()));

    continuousReadButton = new QPushButton(tr("Continuous Read"), this);
    connect(continuousReadButton, SIGNAL(clicked()), this, SLOT(continuousReadButton_clicked()));

    stopContinuousReadButton = new QPushButton(tr("Stop Read"), this);
    connect(stopContinuousReadButton, SIGNAL(clicked()), this, SLOT(stopContinuousReadButton_clicked()));
    stopContinuousReadButton->setDisabled(true);

    quitButton = new QPushButton(tr("Quit"), this);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(rejected()), this, SLOT(deleteLater()));

    ui->buttonBox->addButton(readButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(writeButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(continuousReadButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(stopContinuousReadButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);


    setWindowTitle(tr("Read/Write Coefficient"));
    ui->addressSpinBox->setFocus();

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->progressBar->setTextVisible(false);

    this->setWindowModality(Qt::ApplicationModal);

    tempDStructure = 0;
    timerId = 0;
}

ReadWriteWindow::~ReadWriteWindow()
{
    stopContinuousReadButton_clicked();

    emit terminate();

    delete ui;
    qDebug() << "crw window deleted";
}

void ReadWriteWindow::readButton_clicked()
{
    QString address = QString().setNum(ui->addressSpinBox->value(), 16);

    if(!address.isEmpty()) {
        QList<DataStructure *> dS;
        DataStructure * dStructure = new DataStructure;

        dStructure->address = address;

        if(ui->addressSpinBox->value() < 0x1000) {
            dStructure->size = 16;
            ui->sizeLineEdit->setText("16");
        } else {
            dStructure->size = 32;
            ui->sizeLineEdit->setText("32");
        }

        dStructure->read = true;
        dStructure->write = false;

        dS.append(dStructure);

        parentCommunicationController->initExtReadRequest(dS);

        ui->valueLineEdit->setText(QString().setNum(dStructure->value));

        if(ui->valueTypeComboBox->currentText() == "HEX") {
            ui->valueLineEdit->setText(QString().setNum((int)dStructure->value, 16));
        }


        dS.clear();
        delete dStructure;
    }
}

void ReadWriteWindow::writeButton_clicked()
{
    QString address = QString().setNum(ui->addressSpinBox->value(), 16);
    QString val = ui->valueLineEdit->text();

    if(ui->valueTypeComboBox->currentText() == "HEX") {
        QByteArray bA = ui->valueLineEdit->text().toLatin1();
        char * cStr = bA.data();
        val = QString().setNum(atohex(cStr));
    }

    if(!address.isEmpty() && !val.isEmpty()) {
        QList<DataStructure *> dS;
        DataStructure * dStructure = new DataStructure;
        QList<QString> value;
        value.append(val);

        dStructure->address = address;

        if(ui->addressSpinBox->value() < 0x1000) {
            dStructure->size = 16;
            ui->sizeLineEdit->setText("16");
        } else {
            dStructure->size = 32;
            ui->sizeLineEdit->setText("32");
        }

        dStructure->read = true;
        dStructure->write = true;

        dS.append(dStructure);

        parentCommunicationController->initExtWriteRequest(dS, value);
        QThread::currentThread()->msleep(150);
        ui->valueLineEdit->setText(QString().setNum(dStructure->value));
        if(ui->valueTypeComboBox->currentText() == "HEX") {
            ui->valueLineEdit->setText(QString().setNum((int)dStructure->value, 16));
        }

        dS.clear();
        delete dStructure;
    }
}

void ReadWriteWindow::continuousReadButton_clicked()
{
    QString address = QString().setNum(ui->addressSpinBox->value(), 16);

    if(!address.isEmpty()) {
        if(tempDStructure != 0) {
            delete tempDStructure;
            tempDStructure = 0;
        }

        tempDStructure = new DataStructure;

        tempDStructure->address = address;

        if(ui->addressSpinBox->value() < 0x1000) {
            tempDStructure->size = 16;
            ui->sizeLineEdit->setText("16");
            ui->progressBar->setRange(0,32768);
        } else {
            tempDStructure->size = 32;
            ui->sizeLineEdit->setText("32");
            ui->progressBar->setRange(0,65536);
        }

        tempDStructure->read = true;
        tempDStructure->write = false;

        parentCommunicationController->initExtContinuousReadRequest(tempDStructure);

        readButton->setDisabled(true);
        writeButton->setDisabled(true);
        continuousReadButton->setDisabled(true);
        stopContinuousReadButton->setEnabled(true);
        quitButton->setDisabled(true);

        startReadTimer();
        // timer

        // ui->valueLineEdit->setText(QString().setNum(dStructure->value));

    }
}

void ReadWriteWindow::stopContinuousReadButton_clicked()
{

    stopReadTimer();
    // kill timer

    parentCommunicationController->stopExtContinuousReadRequest();

    readButton->setEnabled(true);
    writeButton->setEnabled(true);
    continuousReadButton->setEnabled(true);
    stopContinuousReadButton->setDisabled(true);
    quitButton->setEnabled(true);


    if(tempDStructure != 0) {
        delete tempDStructure;
        tempDStructure = 0;
    }

}


void ReadWriteWindow::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    if(tempDStructure != 0) {
        int val = tempDStructure->value;
        ui->valueLineEdit->setText(QString().setNum(val));

        if(ui->valueTypeComboBox->currentText() == "HEX") {
            ui->valueLineEdit->setText(QString().setNum(val, 16));
        }

        ui->progressBar->setValue(val);
    }
}

void ReadWriteWindow::startReadTimer() { if(timerId == 0) timerId = startTimer(DELAY); }
void ReadWriteWindow::stopReadTimer() { if(timerId != 0) killTimer(timerId); timerId = 0; }
