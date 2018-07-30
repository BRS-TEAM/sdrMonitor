/*!
 * \file     configwindow.h
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


#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QDialog>
#include <QtGui>
#include <QTableView>

#include "headers/core/datastructure.h"

namespace Ui {
class ConfigWindow;
}

/*!
 * \brief The ConfigWindow class
 *
 * This creates a Config Editor Window Interface
 *
 * This code is simple & sefl-explanatory.
 *
 */
class ConfigWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();

    bool addConfigTab(QList<DataStructure*>) ;

private slots:
    void accept();
    void reject();

private:
    Ui::ConfigWindow *ui;    

    QWidget * pageWidget;                   //!< Pointer to Tab Page Widget

    QList <QTableView *> table;
    QList <QString> tableName;

    // QList<DataStructure*> ds;
    QList<QList<DataStructure*> > ds;
};

#endif // CONFIGWINDOW_H
