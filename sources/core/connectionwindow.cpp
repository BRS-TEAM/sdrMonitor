/*!
 * \file     connectionwindow.cpp
 * \brief    Network Settings Window.
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

#include "headers/core/connectionwindow.h"
#include "ui_connectionwindow.h"

#include <QTextStream>

ConnectionWindow::ConnectionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWindow)
{
    ui->setupUi(this);


    ui->hostCombo->addItem("192.168.1.112");
    //ui->hostCombo->addItem("127.0.0.1");

    ui->portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    ui->portLineEdit->setText(QString().setNum(811));

    ui->hostLabel->setBuddy(ui->hostCombo);
    ui->portLabel->setBuddy(ui->portLineEdit);

    ui->statusLabel->setText("Enter Server Name and Port then Click on 'Connect'");

    connectButton = new QPushButton(tr("Connect"), this);
    connectButton->setDefault(true);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectButton_clicked()));

    quitButton = new QPushButton(tr("Quit"), this);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    disconnectButton = new QPushButton(tr("Disconnect"), this);
    disconnectButton ->setDisabled(true);
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectButton_clicked()));

    readButton = new QPushButton(tr("Read"), this);
    readButton ->setDisabled(true);
    connect(readButton, SIGNAL(clicked()), this, SLOT(readButton_clicked()));

    ui->buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    ui->buttonBox->addButton(disconnectButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(readButton, QDialogButtonBox::ActionRole);

    setWindowTitle(tr("SDR Monitor Connection"));
    ui->portLineEdit->setFocus();

    this->setWindowModality(Qt::ApplicationModal);

}

ConnectionWindow::~ConnectionWindow()
{
    delete ui;

    QTextStream out(stdout);
    out << "Connection window deleted" << endl;
    connectButton->deleteLater();
    disconnectButton->deleteLater();
    readButton->deleteLater();
}

// SLOTS

void ConnectionWindow::connectButton_clicked()
{

    QTextStream out(stdout);
    out << "Connection clicked" << endl;
    emit connectButtonSignal(ui->hostCombo->currentText(), ui->portLineEdit->text().toInt());

}

void ConnectionWindow::readButton_clicked()
{

    QTextStream out(stdout);
    out << "Read clicked" << endl;
    emit readButtonSignal();

}


void ConnectionWindow::disconnectButton_clicked()
{

    QTextStream out(stdout);
    out << "Disconnect clicked" << endl;
    emit disconnectButtonSignal();

}

void ConnectionWindow::changeButtonStatus(bool connectionStatus) {

    connectButton->setEnabled(!connectionStatus);
    disconnectButton->setEnabled(connectionStatus);
    readButton->setEnabled(connectionStatus);
}

int ConnectionWindow::getProtocol() {
    if(ui->ethernetRadioButton->isChecked())
        return 1;

    return 0;
}
