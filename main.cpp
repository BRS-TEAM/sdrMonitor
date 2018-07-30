/*!
 * \file     main.cpp
 * \brief    Main file of application.
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


/*! \mainpage SDRTool 2.0
 *
 * \section intro_sec Introduction
 *
 * This tool can be used to :
 *  - Build SDR monitor interface
 *  - Connect / monitor the target
 *
 *
 * \section behaviour_sec Application Behaviour
 *
 * For Application Behavioral & Usage information:
 *  - Please refer  SDRTOOL_GUIDE.docx
 *
 *
 */

#include "headers/core/mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QStyle>
#include <QDesktopWidget>


//! main functions
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap("../SDRToolResources/icons/SDRsplash_new.bmp");
    QSplashScreen splash(pixmap);

    MainWindow w;
    w.setWindowIcon(QPixmap("../SDRToolResources/icons/logoBRS.bmp"));
    w.setWindowTitle("Continental Software Defined Radio Tool 2.0");
    w.show();

    w.setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    w.size(),
                    qApp->desktop()->availableGeometry()
                    )
                );


    splash.show();
    splash.setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    splash.size(),
                    qApp->desktop()->availableGeometry()
                    )
                );
    QTimer::singleShot(7500, &splash, SLOT(close()));

    a.processEvents();

    QThread::currentThread()->setObjectName("MainThread");
    // qDebug() << "From main thread: " << QThread::currentThreadId() << " Name : " << QThread::currentThread()->objectName();;

    // Execute Application
    return a.exec();
}
