/*!
 * \file     uipainter.h
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

#ifndef UIPAINTER_H
#define UIPAINTER_H

// Essential Headers
#include <QWidget>
#include <QTimerEvent>

/*!
 * \brief The UiPainter class
 *
 * It provides the clock & necessary signal to indicate update event of HMI elements.
 *
 * This code is simple & sefl-explanatory.
 *
 */
class UiPainter : public QWidget
{
    Q_OBJECT

public:
    UiPainter(QWidget * parent = 0);
    ~UiPainter();

    void start();
    void stop();

protected:
    void timerEvent(QTimerEvent *);

signals:
    void autoUpdate();
    void clickEvent();

private:
    int timerId;

    static const int DELAY = 100;           //!< Delay Time (msec) to update interface.
};

#endif // UIPAINTER_H
