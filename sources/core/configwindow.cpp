/*!
 * \file     configwindow.cpp
 * \brief    Configuration Editor Window.
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

#include "headers/core/configwindow.h"
#include "ui_configwindow.h"

#include <QColumnView>


ConfigWindow::ConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
}

ConfigWindow::~ConfigWindow()
{
    delete ui;

    for(int i = 0; i < table.count(); i++)
        delete table.at(i);

    qDebug() << "ConfigWindow Deleted" << endl;
}

// Standard Item Model is used to store config file information in a text type table.
bool ConfigWindow::addConfigTab(QList<DataStructure*> ds) {

    this->ds.append(ds);

    if(ds.count()) {

        table.append(new QTableView);

        QStandardItemModel * model = new QStandardItemModel(ds.count(),5,this); //ds.count() Rows and 5 Columns
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));        
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Address")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Size")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("rw")));
        model->setHorizontalHeaderItem(4, new QStandardItem(QString("dataType")));

        for(int i = 0; i < ds.count(); i++ ) {
            model->setItem(i, 0, new QStandardItem(QString(ds.at(i)->name)));
            model->item(i, 0)->setFlags(model->item(i, 0)->flags() & ~Qt::ItemIsEditable);
            model->setItem(i, 1, new QStandardItem(ds.at(i)->address));       // To be changed
            model->setItem(i, 2, new QStandardItem(QString().setNum(ds.at(i)->size)));
            QString rw;
            if(ds.at(i)->read && !ds.at(i)->write) {
                rw = "r";
            } else if(!ds.at(i)->read && ds.at(i)->write) {
                rw = "w";
            } else if(ds.at(i)->read && ds.at(i)->write) {
                rw = "x";
            } else {
                rw = "";
            }
            model->setItem(i, 3, new QStandardItem(QString(rw)));
            model->setItem(i, 4, new QStandardItem(QString(ds.at(i)->dataType)));
        }

        table.last()->setModel(model);


        QVBoxLayout *vBox = new QVBoxLayout;

        QWidget *formWidget = new QWidget(this);

        /*
        * add inputFields to vBox here
        */
        // vBox->addWidget(inputField.at(i));
        vBox->addWidget(table.last());

        // vBox->addStretch(1);
        formWidget->setLayout(vBox);

        if (formWidget != 0) {

            /*
            * If everything went well. Return True
            */

            pageWidget = formWidget;

            tableName.append(ds.first()->module);
            ui->tabWidget->addTab(pageWidget, ds.first()->module);

            return true;

        }

    }
    return false;

}

// If changes are made to the config and accepted then they will be updated into their respective #DataStructure
void ConfigWindow::accept() {

    for (int i = 0; i < table.count(); i ++)
        for (int j = 0; j < table.at(i)->model()->rowCount(); j++)
            if(ds.at(i).at(j)->module == tableName.at(i)) {

                bool ok = false;
                ds.at(i).at(j)->name = table.at(i)->model()->data(table.at(i)->model()->index(j,0)).toString();
                ds.at(i).at(j)->address = table.at(i)->model()->data(table.at(i)->model()->index(j,1)).toString();  // To be changed
                ds.at(i).at(j)->size = QString(table.at(i)->model()->data(table.at(i)->model()->index(j,2)).toString()).toInt(&ok, 10);
                QString rw = table.at(i)->model()->data(table.at(i)->model()->index(j,3)).toString();
                if(rw == "r") {
                    ds.at(i).at(j)->read = true;
                    ds.at(i).at(j)->write = false;
                } else if (rw == "w") {
                    ds.at(i).at(j)->read = false;
                    ds.at(i).at(j)->write = true;
                } else if (rw == "x") {
                    ds.at(i).at(j)->read = true;
                    ds.at(i).at(j)->write = true;
                } else {
                    ds.at(i).at(j)->read = false;
                    ds.at(i).at(j)->write = false;
                }
                ds.at(i).at(j)->dataType = table.at(i)->model()->data(table.at(i)->model()->index(j,4)).toString();
            }
    emit finished(1);
    this->deleteLater();
}

void ConfigWindow::reject() {
    emit finished(1);
    this->deleteLater();
}
