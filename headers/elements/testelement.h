/*!
 * \file     testelement.h
 * \brief    Test Element Class for Debugging use only
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

// TestElement
#ifndef TESTELEMENT_H
#define TESTELEMENT_H

// Essential Headers
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QTimerEvent>

/*!
 * \brief The TestElement Class for Debugging use only
 *
 * It generates random numbers in every 100 mSec
 *
 */
class TestElement : public QWidget
{

    Q_OBJECT

public:
    TestElement(QWidget * parent = 0);
    ~TestElement();

    int * getTestValue();

protected:
    void timerEvent(QTimerEvent *);

signals:
    void mySignal(float);

private:
    void emitMySignal();

private:
    int timerId;
    int randomValue;

    static const int DELAY = 1000;
};

#endif // TESTELEMENT_H
