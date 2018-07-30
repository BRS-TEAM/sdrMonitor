/*!
 * \file     createprojectwindow.cpp
 * \brief    Create Project Window.
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

#include "headers/core/createprojectwindow.h"
#include "ui_createprojectwindow.h"

// Text Handler
#include <QTextStream>
#include <QStringList>

#include <QFile>
#include <QFileDialog>

CreateProjectWindow::CreateProjectWindow(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CreateProjectWindow)
{
    ui->setupUi(this);
}

CreateProjectWindow::~CreateProjectWindow()
{
    delete ui;

}

bool CreateProjectWindow::validateCurrentPage()
{

    if (ui->newProjectLineEdit->text().isEmpty()) {
        QTextStream out(stdout);
        out << "Field marked with (*) are necessary." << endl;
        ui->newProjectLineEdit->setPlaceholderText("Field marked with (*) are necessary.");
        return false;
    }


    if (ui->radioModeResourcesLineEdit->text().isEmpty()) {
        QTextStream out(stdout);
        out << "Field cannot be empty! Select an input XML resource." << endl;
        ui->radioModeResourcesLineEdit->setPlaceholderText("Select resource XML file.");
        return false;
    }

    for(int i = 0; i < resourcesLineEdit.count(); i++)
        if(resourcesLineEdit.at(i)->text().isEmpty()) {
            resourcesLineEdit.at(i)->setPlaceholderText("Select resource XML file.");
            return false;
        }

    QString str;

    str.append(tr("PROJECT_FILE  :  ") + ui->newProjectLineEdit->text() + tr("\n\n"));
    str.append( ui->radioModeComboBox->currentText() + "_SOURCE  :  " + ui->radioModeResourcesLineEdit->text() + tr("\n\n"));
    for(int i = 0; i < resourcesLineEdit.count(); i++)
        str.append( resourcesCBox.at(i)->currentText() + "_SOURCE  :  " + resourcesLineEdit.at(i)->text() + tr("\n\n"));

    str.append(tr("CONFIG_SOURCE  :  ") + ui->configResourceLineEdit->text() + tr("\n"));

    ui->projectDescriptionLabel->setText(str);

    return true;
}

void CreateProjectWindow::on_newProjectButton_clicked()
{
    // Get Highest level XML file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Select Project Destination"), QDir::currentPath(), tr("PROJ files (*.proj)"));

    if(!fileName.isEmpty()) {

        // Auxilliary variable
        QTextStream out(stdout);

        out << fileName << endl;
        if((fileName.right(5) != ".proj") && (fileName.right(5) != ".PROJ"))
            ui->newProjectLineEdit->setText(fileName + ".proj");
        else
            ui->newProjectLineEdit->setText(fileName);
    }
}

void CreateProjectWindow::on_configResourcePushButton_clicked()
{
    // Get Highest level XML file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Configuration source"), QDir::currentPath(), tr("Config files (*.cfg)"));

    if(!fileName.isEmpty()) {

        // Auxilliary variable
        QTextStream out(stdout);

        out << fileName << endl;
        ui->configResourceLineEdit->setText(fileName);
    }
}


void CreateProjectWindow::on_CreateProjectWindow_accepted()
{
    // Output .proj file
    QFile outputFile(ui->newProjectLineEdit->text());

    // Write .proj file
    if (outputFile.open(QIODevice::WriteOnly))
    {
        // Output File Handler
        QIODevice *outDevice = &outputFile;
        QTextStream outFileStream(outDevice);

        outFileStream << "#	This is a project file. The source file name and its keyword must be seperated by a tab space" << endl;

        outFileStream << ui->radioModeComboBox->currentText()  << "_SOURCE" << "\t";
        outFileStream << ui->radioModeResourcesLineEdit->text() << endl;

        for(int i = 0; i < resourcesLineEdit.count(); i++) {
            outFileStream << resourcesCBox.at(i)->currentText()  << "_SOURCE" << "\t";
            outFileStream << resourcesLineEdit.at(i)->text() << endl;
        }

        outFileStream << "CONFIG_SOURCE" << "\t";
        outFileStream << ui->configResourceLineEdit->text() << endl;

        outputFile.close();
    }

}

void CreateProjectWindow::on_radioModeResourcesPushButton_clicked()
{
    // Get Highest level XML file
    QString message = "Select " + ui->radioModeComboBox->currentText() + " source XML";
    QString fileName = QFileDialog::getOpenFileName(this, message, QDir::currentPath(), tr("XML files (*.xml)"));

    if(!fileName.isEmpty()) {

        // Auxilliary variable
        QTextStream out(stdout);

        out << fileName << endl;
        ui->radioModeResourcesLineEdit->setText(fileName);
    }


}

void CreateProjectWindow::on_addNewModeButton_clicked()
{
    QHBoxLayout * cBoxHLayout = new QHBoxLayout();
    resourcesCBox.append(new QComboBox());
    removeModePushButton.append(new QPushButton("Remove"));
    resourcesCBox.last()->addItem("FM");
    resourcesCBox.last()->addItem("AM");
    resourcesCBox.last()->addItem("DAB");
    cBoxHLayout->addWidget(resourcesCBox.last());
    cBoxHLayout->addWidget(removeModePushButton.last());
    cBoxHLayout->addStretch(1);
    ui->verticalLayout->insertLayout(ui->verticalLayout->count() - 2, cBoxHLayout);

    QHBoxLayout * resourcesHLayout = new QHBoxLayout();
    resourcesLineEdit.append(new QLineEdit());
    resourcesPushButton.append(new QPushButton("..."));
    resourcesHLayout->addWidget(resourcesLineEdit.last());
    resourcesHLayout->addWidget(resourcesPushButton.last());
    ui->verticalLayout->insertLayout(ui->verticalLayout->count() - 2, resourcesHLayout);

    QObject::connect(removeModePushButton.last(), SIGNAL(clicked()), this, SLOT(removeRadioModeButtonClicked()));
    QObject::connect(resourcesPushButton.last(), SIGNAL(clicked()), this, SLOT(resourcesPushButtonClicked()));
}

void CreateProjectWindow::resourcesPushButtonClicked()
{

    for(int i = 0; i < resourcesPushButton.count(); i++)
        if(resourcesPushButton.at(i) == qobject_cast<QPushButton*> (sender())) {

            // Get Highest level XML file
            QString message = "Select " + resourcesCBox.at(i)->currentText() + " source XML";
            QString fileName = QFileDialog::getOpenFileName(this, message, QDir::currentPath(), tr("XML files (*.xml)"));

            if(!fileName.isEmpty()) {

                // Auxilliary variable
                QTextStream out(stdout);

                out << fileName << endl;
                resourcesLineEdit.at(i)->setText(fileName);
            }
            break;
        }


}

void CreateProjectWindow::removeRadioModeButtonClicked()
{

    for(int i = 0; i < removeModePushButton.count(); i++)
        if(removeModePushButton.at(i) == qobject_cast<QPushButton*> (sender())) {

            delete resourcesCBox.at(i);
            resourcesCBox.removeAt(i);

            delete resourcesLineEdit.at(i);
            resourcesLineEdit.removeAt(i);

            QObject::disconnect(resourcesPushButton.at(i), SIGNAL(clicked()), this, SLOT(resourcesPushButtonClicked()));
            delete resourcesPushButton.at(i);
            resourcesPushButton.removeAt(i);

            QObject::disconnect(removeModePushButton.at(i), SIGNAL(clicked()), this, SLOT(removeRadioModeButtonClicked()));
            delete removeModePushButton.at(i);
            removeModePushButton.removeAt(i);

            break;
        }


}
