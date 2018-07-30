/*!
 * \file     uipainter.cpp
 * \brief    Update Event Manager to repaint the HMI
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

#include "headers/core/uipainter.h"

#include <QTextStream>

UiPainter::UiPainter(QWidget * parent)
    : QWidget(parent)
{
    timerId = 0;
}

UiPainter::~UiPainter()
{
    if(timerId != 0)
        killTimer(timerId);

    QTextStream out(stdout);
    out << "UiPainter Deleted" << endl;

}

void UiPainter::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    emit autoUpdate();

}

void UiPainter::start() { if(timerId == 0) timerId = startTimer(DELAY); }
void UiPainter::stop() { killTimer(timerId); timerId = 0; }
