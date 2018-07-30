/*!
 * \file     connectionwindow.h
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

#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QWidget>

#include <QPushButton>

namespace Ui {
class ConnectionWindow;
}

/*!
 * \brief Network Settings Window Class.
 *
 * This creates a Network Settings Window Interface
 *
 * This code is simple & sefl-explanatory.
 *
 */
class ConnectionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWindow(QWidget *parent = 0);
    ~ConnectionWindow();

    int getProtocol();


signals:
    void connectButtonSignal(QString, int);
    void readButtonSignal();
    void disconnectButtonSignal();


private slots:
    void changeButtonStatus(bool);
    void connectButton_clicked();
    void readButton_clicked();
    void disconnectButton_clicked();



private:
    Ui::ConnectionWindow *ui;

    QPushButton *connectButton;
    QPushButton *quitButton;
    QPushButton *disconnectButton;
    QPushButton *readButton;

};

#endif // CONNECTIONWINDOW_H
