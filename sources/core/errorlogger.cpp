/*!
 * \file     errorlogger.cpp
 * \brief    Error Log & Debugging
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


#include <QString>

#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>

QTextStream out(stdout);

void printDebugString(QString str, bool printInLine, int priority) {
    switch (priority) {
    case 0:
        if(printInLine)
            out << str;
        else
            out << endl << str;
        break;

    case 1:
        if(printInLine)
            out << str;
        else
            out << endl << str;
        break;

    case 2:
        if(printInLine)
            out << str;
        else
            out << endl << str;
        break;
    }
}

void printDebugInt(int num, bool printInLine, int priority) {
    switch (priority) {
    case 0:
        if(printInLine)
            out << num;
        else
            out << endl << num;
        break;
    }
}

void reportError(QString str) {
    qDebug() << str;
}

// See Developper guide B.a.iii
void myErrorLog(QtMsgType type, const QMessageLogContext &context, const QString &msg) {

    Q_UNUSED(context)

    QString outputDir = "errorLog";

    QDir dir(outputDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(outputDir + "/" + QDate::currentDate().toString("SDRLOG_dd_MM_yyyy.log"));

    if(file.open(QIODevice::Append | QIODevice::Text)) {

        QTextStream out(&file);

        out << QTime::currentTime().toString("[hh:mm:ss.zzz ");

        switch (type)

        {

        case QtDebugMsg:    out << "DBG]  "; break;

        case QtWarningMsg:  out << "WRN]  "; break;

        case QtCriticalMsg: out << "CRT]  "; break;

        case QtFatalMsg:    out << "FTL]  "; break;

        case QtInfoMsg:    break;

        }

        out << msg << '\n';

        out.flush();
        file.close();
    }
}

