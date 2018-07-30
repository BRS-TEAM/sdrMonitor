/*!
 * \file     createprojectwindow.h
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

#ifndef CREATEPROJECTWINDOW_H
#define CREATEPROJECTWINDOW_H

#include <QWizard>

class QLineEdit;
class QComboBox;

namespace Ui {
class CreateProjectWindow;
}

/*!
 * \brief The Create Project Window class
 *
 * This creates a Create New Project Window Interface
 *
 * This code is simple & sefl-explanatory.
 *
 */
class CreateProjectWindow : public QWizard
{
    Q_OBJECT

public:
    explicit CreateProjectWindow(QWidget *parent = 0);
    ~CreateProjectWindow();

private slots:
    bool validateCurrentPage();

    void on_newProjectButton_clicked();

    void on_CreateProjectWindow_accepted();

    void on_configResourcePushButton_clicked();

    void on_radioModeResourcesPushButton_clicked();

    void on_addNewModeButton_clicked();

    void resourcesPushButtonClicked();

    void removeRadioModeButtonClicked();
private:
    Ui::CreateProjectWindow *ui;

    QList<QComboBox *> resourcesCBox;
    QList<QPushButton *> removeModePushButton;
    QList<QLineEdit *> resourcesLineEdit;
    QList<QPushButton *> resourcesPushButton;


};

#endif // CREATEPROJECTWINDOW_H
