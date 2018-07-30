/*!
 * \file     testelement.cpp
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

#include "headers/elements/testelement.h"
#include <QtGlobal>

#include <QTextStream>

TestElement::TestElement(QWidget * parent)
    : QWidget(parent)
{
    int number = 100;

    randomValue = qrand() % number;

    timerId = startTimer(DELAY);
}

TestElement::~TestElement()
{
    killTimer(timerId);

    QTextStream out(stdout);
    out << "Test Element Deleted" << endl;
}

void TestElement::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    randomValue = qrand() % 100;

     // QTextStream out(stdout);
     // out << "Timer Event" << randomValue << endl;

    emitMySignal();

}

// Private Function
void TestElement::emitMySignal() {
     mySignal(randomValue);
}

