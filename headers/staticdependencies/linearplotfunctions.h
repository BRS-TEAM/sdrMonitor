#ifndef LINEARPLOTFUNCTIONS_H
#define LINEARPLOTFUNCTIONS_H

#include <QMap>
#include "headers/elements/xyplot.h"

void calculateLinearPlotCoordinates(QMap<QString, int> sourceCoefficients, QString linearFunctionType, QMap<QString, double> &resultCoordinates);
void calculateLinearPlotCoefficients(QMap<QString, double> sourceCoordinates, QString linearFunctionType, QList<int> &resultCoefficients);

void calculateSensorVertices( QString linearFunctionType, double xSourceValue, QString xSourceType, double ySourceValue, QString ySourceType, double &pointX, double &pointY);

/* //////////////////////////////// Softmute ///////////////////////////////// */
QMap<QString, double> calculateFMSoftmuteOnLevelCoordinates(int smLvlSlope, int smLvlOffset, int smLvlMin);
QMap<QString, double> calculateAMSoftmuteOnLevelCoordinates(int smLvlSlope, int smLvlOffset, int smLvlMin);
QMap<QString, double> calculateSoftmuteCoordinates(int smLvlSlope, int smLvlOffset, int smLvlMin);

QList<int> calculateSoftmuteOnLevelCoefficients(QMap<QString, double> coordinates);
QList<int> calculateSoftmuteCoefficients(QMap<QString, double> coordinates);

/* //////////////////////////////// Stereo ///////////////////////////////// */
QMap<QString, double> calculateStereoOnLevelCoordinates(int stLvlSlope, int stLvlOffset);
QMap<QString, double> calculateStereoCoordinates(int stSlope, int stOffset);

QList<int> calculateStereoOnLevelCoefficients(QMap<QString, double> coordinates);
QList<int> calculateStereoCoefficients(QMap<QString, double> coordinates);

/* //////////////////////////////// Highcut ///////////////////////////////// */
QMap<QString, double> calculateHighcutOnLevelCoordinates(int hcLvlSlope, int hcLvlOffset);
QMap<QString, double> calculateHighcutCoordinates(int hcSlope, int hcOffset);

QList<int> calculateHighcutOnLevelCoefficients(QMap<QString, double> coordinates);
QList<int> calculateHighcutCoefficients(QMap<QString, double> coordinates);

/* //////////////////////////////// Audio Filter ///////////////////////////////// */
QMap<QString, double> calculateAudioFilterCoordinates(int hcSlope, int hcOffset);

QList<int> calculateAudioFilterCoefficients(QMap<QString, double> coordinates);

//XYPlot *m_ptr;


#endif // LINEARPLOTFUNCTIONS_H
