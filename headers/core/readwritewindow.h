/*!
 * \file     readwritewindow.h
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

#ifndef READWRITEWINDOW_H
#define READWRITEWINDOW_H

#include <QDialog>
#include <QPushButton>

class CommunicationController;
class DataStructure;
class QTimerEvent;

namespace Ui {
class ReadWriteWindow;
}


/*!
 * \brief The readWriteWindow class
 *
 * This creates a Coefficient Read Write Window Interface
 *
 * This code is simple & sefl-explanatory.
 *
 */
class ReadWriteWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReadWriteWindow(CommunicationController *parent = 0);
    ~ReadWriteWindow();

    void startReadTimer();
    void stopReadTimer();

protected:
    void timerEvent(QTimerEvent *);

signals:
    void terminate();

private slots:
    void readButton_clicked();

    void writeButton_clicked();

    void continuousReadButton_clicked();

    void stopContinuousReadButton_clicked();
private:
    Ui::ReadWriteWindow *ui;
    QPushButton * readButton;
    QPushButton * writeButton;
    QPushButton * continuousReadButton;
    QPushButton * stopContinuousReadButton;
    QPushButton * quitButton;
    CommunicationController * parentCommunicationController;
    DataStructure * tempDStructure;
    int timerId;
    static const int DELAY = 100;
};

#endif // READWRITEWINDOW_H
