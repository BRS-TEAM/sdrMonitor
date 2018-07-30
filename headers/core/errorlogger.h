/*!
 * \file     errorlogger.h
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

#ifndef ERRORLOGGER
#define ERRORLOGGER

class QString;
class QTextStream;

void printDebugString(QString str, bool printInLine = false, int priority = 0);
void printDebugInt(int num, bool printInLine = false, int priority = 0);
void reportError(QString str);
void myErrorLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

extern bool criticalLevelErrorFlag;
extern bool highLevelErrorFlag;
extern bool averageLevelErrorFlag;
extern bool lowLevelErrorFlag;

#endif // ERRORLOGGER

