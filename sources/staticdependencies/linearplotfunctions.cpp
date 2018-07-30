#include "headers/staticdependencies/linearplotfunctions.h"
#include "headers/staticdependencies/valueconversion.h"

#include <QTextStream>
#include <math.h>
#include <QDebug>
void calculateLinearPlotCoordinates(QMap<QString, int> sourceCoefficients, QString linearFunctionType, QMap<QString, double> &resultCoordinates) {

    if( ( (sourceCoefficients.find("slope") != sourceCoefficients.end())
          && (sourceCoefficients.find("offset") != sourceCoefficients.end())
          && (sourceCoefficients.find("min") != sourceCoefficients.end())
          )
            ||
            ( (sourceCoefficients.find("slope") != sourceCoefficients.end())
              && (sourceCoefficients.find("offset") != sourceCoefficients.end())
              )
            )
    {

        // Softmute

        if(linearFunctionType == "Softmute Level") {

            if(sourceCoefficients.count() >= 3)
                resultCoordinates = calculateFMSoftmuteOnLevelCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"], sourceCoefficients["min"]);

        }
        else if(linearFunctionType == "Softmute AM Level") {

            if(sourceCoefficients.count() >= 3)
                resultCoordinates = calculateAMSoftmuteOnLevelCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"], sourceCoefficients["min"]);

        }
        else if(linearFunctionType == "Softmute General" || linearFunctionType == "Softmute") {

            if(sourceCoefficients.count() >= 3)
                resultCoordinates = calculateSoftmuteCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"], sourceCoefficients["min"]);

        }
        /*
     * Add More Softmute Plot Equation Here
     */


        // Stereo

        else if(linearFunctionType == "Stereo Level") {

            if(sourceCoefficients.count() >= 2)
                resultCoordinates = calculateStereoOnLevelCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"]);

        }
        else if(linearFunctionType == "Stereo General" || linearFunctionType == "Stereo") {

            if(sourceCoefficients.count() >= 2)
                resultCoordinates = calculateStereoCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"]);

        }
        /*
     * Add more Stereo Plot Equations Here
     */

        else if(linearFunctionType == "Highcut Level") {

            if(sourceCoefficients.count() >= 2)
                resultCoordinates = calculateHighcutOnLevelCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"]);

        }
        else if(linearFunctionType == "Highcut General" || linearFunctionType == "Highcut") {

            if(sourceCoefficients.count() >= 2)
                resultCoordinates = calculateHighcutCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"]);

        }
        /*
     * Add more Highcut Plot Equations Here
     */

        // AudioFilter

        else if(linearFunctionType == "AF") {

            if(sourceCoefficients.count() >= 2)
                resultCoordinates = calculateAudioFilterCoordinates(sourceCoefficients["slope"], sourceCoefficients["offset"]);

        }
        /*
     * Add more AF Plot Equations Here
     */

    }
}

void calculateLinearPlotCoefficients(QMap<QString, double> sourceCoordinates, QString linearFunctionType, QList<int> &resultCoefficients) {

    // Softmute

    if(linearFunctionType == "Softmute Level" || linearFunctionType == "Softmute AM Level") {

        resultCoefficients = calculateSoftmuteOnLevelCoefficients(sourceCoordinates);

    } else if(linearFunctionType == "Softmute General" || linearFunctionType == "Softmute") {

        resultCoefficients = calculateSoftmuteCoefficients(sourceCoordinates);

    }
    /*
     * Add more Softmute Coefficent Equations Here
     */

    // Stereo

    else if(linearFunctionType == "Stereo Level") {

        resultCoefficients = calculateStereoOnLevelCoefficients(sourceCoordinates);

    } else if(linearFunctionType == "Stereo General" || linearFunctionType == "Stereo") {
        resultCoefficients = calculateStereoCoefficients(sourceCoordinates);
    }
    /*
     * Add more Stereo Coefficent Equations Here
     */

    // Highcut

    else if(linearFunctionType == "Highcut Level") {

        resultCoefficients = calculateHighcutOnLevelCoefficients(sourceCoordinates);

    } else if(linearFunctionType == "Highcut General" || linearFunctionType == "Highcut") {

        resultCoefficients = calculateHighcutCoefficients(sourceCoordinates);

    }
    /*
     * Add more Highcut Coefficent Equations Here
     */
    else if(linearFunctionType == "AF") {

        resultCoefficients = calculateAudioFilterCoefficients(sourceCoordinates);

       }
       /*
        * Add more AF Coefficent Equations Here
        */

}

void calculateSensorVertices( QString linearFunctionType, double xSourceValue, QString xSourceType, double ySourceValue, QString ySourceType, double &pointX, double &pointY) {

    int max = 32767;

    // Calculate point X
    int x = xSourceValue;

    if( (linearFunctionType == "Softmute AM Level" || linearFunctionType == "Highcut Level")) {
        if(x < -3277)
            x = -3277;
    }

//    pointX  = (float)x / max;
//    pointX  = pointX * 100;
    pointX = convertCoeffForSensor_TimePlot(x, xSourceType, max);


    // Calculate point Y
    int y = ySourceValue;

//    if (linearFunctionType == "Highcut") {

//        pointY = (float)y / max;

//        pointY = pointY * 100;

//    } else if (linearFunctionType == "Highcut Level") {

//        pointY = (float)y / max;

//        if (!pointY) pointY = (float) .000000001;

//    } else {

//        pointY = (float)y / max;

//        if (!pointY) pointY = (float) .000000001;

//        pointY = log10 (pointY) * 20;
//    }
    pointY = convertCoeffForSensor_TimePlot(y, ySourceType, max);
}



/* //////////////////////////////// Softmute ///////////////////////////////// */

QMap<QString, double> calculateFMSoftmuteOnLevelCoordinates(int smLvlSlope, int smLvlOffset, int smLvlMin) {

    float tempFloat;

    // Calculate a
    float a;
    if (smLvlSlope >= 0x8000)
        smLvlSlope = (-(0xFFFF - smLvlSlope));

    a = smLvlSlope / pow ((float)2, 16 - 1) / (float).0156;     //size

    // Calculate b
    float b;
    if (smLvlOffset >= 0x8000)
        smLvlOffset = (-(0xFFFF - smLvlOffset));
    b = smLvlOffset / pow ((float)2, 16 - 1) / (float).0156;    //size =16

    // Calculate xStart
    double xValueStart;
    tempFloat = (float) -3 / (float)20.;
    tempFloat = pow ((float)10, tempFloat);
    xValueStart = (float)100*((tempFloat-b)/a);

    // Calculate xStop
    double xValueStop;
    tempFloat = (float) -20/ (float)20.;
    tempFloat = pow ((float)10, tempFloat);
    xValueStop = (float)100*((tempFloat-b)/a);

    // Calculate yStart
    double yValueStart = 0;

    // Calculate yStop
    double yValueStop;
    tempFloat = smLvlMin / pow ((float)2, 16 - 1);    // Size
    yValueStop = (int)20*log10(tempFloat);

    // Calculate xMin
    double xValueMin = -20;
    // Calculate xMax
    double xValueMax = 80;

    // Calculate yMin
    double yValueMin;
    tempFloat = smLvlMin / pow ((float)2, 16 - 1);
    yValueMin = (int)20*log10(tempFloat);

    // Calculate yMax
    double yValueMax = 0;

    // setCoordinates(xValueMax, yValueMax, xValueStart, yValueStart, xValueStop, yValueStop, xValueMin, yValueMin);
    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    return coordinates;

}

QMap<QString, double> calculateAMSoftmuteOnLevelCoordinates(int smLvlSlope, int smLvlOffset, int smLvlMin) {

    float tempFloat;

    // Calculate a
    float a;
    if (smLvlSlope >= 0x8000)
        smLvlSlope = (-(0xFFFF - smLvlSlope));

    a = smLvlSlope / pow ((float)2, 16 - 1) / (float).0156;     //size

    // Calculate b
    float b;
    if (smLvlOffset >= 0x8000)
        smLvlOffset = (-(0xFFFF - smLvlOffset));
    b = smLvlOffset / pow ((float)2, 16 - 1) / (float).0156;    //size =16

    // Calculate xStart
    double xValueStart;
    tempFloat = (float) -10 / (float)20.;
    tempFloat = pow ((float)10, tempFloat);
    xValueStart = (float)100*((tempFloat-b)/a);

    // Calculate xStop
    double xValueStop;
    tempFloat = (float) -22/ (float)20.;
    tempFloat = pow ((float)10, tempFloat);
    xValueStop = (float)100*((tempFloat-b)/a);

    // Calculate yStart
    double yValueStart = 0;

    // Calculate yStop
    double yValueStop;
    tempFloat = smLvlMin / pow ((float)2, 16 - 1);    // Size
    yValueStop = (int)20*log10(tempFloat);

    // Calculate xMin
    double xValueMin = -30;
    // Calculate xMax
    double xValueMax = 80;

    // Calculate yMin
    double yValueMin;
    tempFloat = smLvlMin / pow ((float)2, 16 - 1);
    yValueMin = (int)20*log10(tempFloat);

    // Calculate yMax
    double yValueMax = 0;

    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    return coordinates;
}

QMap<QString, double> calculateSoftmuteCoordinates(int smSlope, int smOffset, int smMin) {

    float tempFloat;

    // Calculate a
    float a;
    if (smSlope >= 0x8000)
        smSlope = (-(0xFFFF - smSlope));

    a = smSlope / pow ((float)2, 16 - 1) / (float).0156;     //size

    // Calculate b
    float b;
    if (smOffset >= 0x8000)
        smOffset = (-(0xFFFF - smOffset));
    b = smOffset / pow ((float)2, 16 - 1) / (float).0156;    //size =16

    // Calculate xStart
    double xValueStart;
    xValueStart = (float)100*((1-b)/a);

    // Calculate xStop
    double xValueStop;
    tempFloat = smMin / pow ((float)2, 16 - 1);
    xValueStop = (float)100*((tempFloat-b)/a);

    // Calculate yStart
    double yValueStart = 0;

    // Calculate yStop
    double yValueStop;
    tempFloat = smMin / pow ((float)2, 16 - 1);
    yValueStop = (int)20*log10(tempFloat);

    // Calculate xMin
    double xValueMin = 0;
    // Calculate xMax
    double xValueMax = 100;

    // Calculate yMin
    double yValueMin = 0;

    // Calculate yMax
    double yValueMax;
    tempFloat = smMin / pow ((float)2, 16 - 1);
    yValueMax = (int)20*log10(tempFloat);

    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    return coordinates;
}


QList<int> calculateSoftmuteOnLevelCoefficients(QMap<QString, double> coordinates) {

    // Collect output values
    float alpha3, neg20dB;
    int limiter;
    alpha3 = coordinates["xValueStart"];
    neg20dB = coordinates["xValueStop"];
    limiter = coordinates["yValueMin"];

    // Calculate new coordinates
    int att1, att2;
    att1 = -3;
    att2 = -20;
/*
    float a = (att1-att2)/(alpha3-neg20dB);
    float b = att1-(alpha3*(att1-att2)/(alpha3-neg20dB));

    double xValueStart = -b/a;
    double yValueStart = 0;
    double xValueStop = (limiter - b)/a;
    double yValueStop = limiter;
    double yValueMin = limiter; // m_YValue_min

    // update coordinates
    setCoordinates(this->m_XValues_SmtLvl[0], this->m_YValues_SmtLvl[0], xValueStart, yValueStart, xValueStop, yValueStop, this->m_XValues_SmtLvl[3], yValueMin);
*/

    // coefficients computation DSP J2
    alpha3 = alpha3/100;
    neg20dB = neg20dB/100;

    // convert from dB to linear
    float fp2, fq2  , fatt1, fatt2, fintnse;

    fatt1 = (float) att1 / (float)20.;
    fatt1 = pow ((float)10, fatt1);

    fatt2 = (float) att2 / (float)20. ;
    fatt2 = pow ((float)10, fatt2);

    fintnse = (float) limiter / (float)20.;
    fintnse = pow ((float)10, fintnse);

    fp2 = ((fatt1-fatt2)/(alpha3-neg20dB));  // scaling 64 times
    fq2 = (fatt1-(alpha3*fp2));
    fp2 = fp2 * (float).0156;
    fq2 = fq2 * (float).0156;

    // convert to Q15 format
    fp2 = fp2 * pow ((float)2,15);
    fq2 = fq2 * pow ((float)2,15);
    fintnse = fintnse * pow ((float)2,15);

    int slope, offset, minSm;
    slope = (int) fp2;
    offset = (int) fq2;
    minSm = (int) fintnse;

    if (slope < 0)
        slope &= (int) pow ((float)2,16)-1;
    if (offset < 0)
        offset &= (int) pow ((float)2,16)-1;
    if (minSm < 0)
        minSm &= (int) pow ((float)2,16)-1;

    // set the value to DSP
    QTextStream out(stdout);
    out << "slp : " <<  QString().setNum(slope,16) << ", off : " << QString().setNum(offset,16) << " min : " << QString().setNum(minSm,16) << endl;

    QList<int> outVal;
    outVal.append(slope);
    outVal.append(offset);
    outVal.append(minSm);
    return outVal;

}

QList<int> calculateSoftmuteCoefficients(QMap<QString, double> coordinates) {       // Caustion setCordinates Deactivated

    // Collect output values
    float noEffectPoint, fullEffectPoint;
    int limiter;

    noEffectPoint = coordinates["xValueStart"];

    fullEffectPoint = coordinates["xValueStop"];

    limiter = coordinates["yValueMax"];

    // Calculate new coordinates
//    double xValueStart = noEffectPoint;
//    double yValueStart = 0;
//    double xValueStop = fullEffectPoint;
//    double yValueStop = limiter;
//    double yValueMax = limiter;

    // update coordinates
//    setCoordinates(this->m_XValues_SmtLvl[0], yValueMax, xValueStart, yValueStart, xValueStop, yValueStop, this->m_XValues_SmtLvl[3], this->m_YValues_SmtLvl[3]);

    // update plot
    // plotCoordinates();

    // coefficients computation DSP J2
    float fintnse, fp, fq;

    fintnse = (float) limiter / (float)20.;
    fintnse = pow ((float)10, fintnse);
    noEffectPoint = noEffectPoint/(float)100;
    fullEffectPoint = fullEffectPoint/(float)100;
    fp = noEffectPoint-fullEffectPoint;
    fp = (1-fintnse)/fp;
    fq = 1-noEffectPoint*fp;

    fp = fp * (float).0156;
    fq = fq * (float).0156;

    // convert to Q format
    fp = fp * pow ((float)2, 15);
    fq = fq * pow ((float)2, 15);
    fintnse = fintnse * pow ((float)2, 15);

    int minSm,slope,offset;
    slope = (int) fp;
    offset = (int) fq;
    minSm = (int) fintnse;

    if (slope < 0)
        slope &= (int) pow ((float)2,16)-1;
    if (offset < 0)
        offset &= (int) pow ((float)2,16)-1;
    if (minSm < 0)
        minSm &= (int) pow ((float)2,16)-1;

    // set the value to DSP
    QTextStream out(stdout);
    out << "slp : " <<  QString().setNum(slope,16) << ", off : " << QString().setNum(offset,16) << " min : " << QString().setNum(minSm,16) << endl;

    QList<int> outVal;
    outVal.append(slope);
    outVal.append(offset);
    outVal.append(minSm);
    return outVal;

}

/* //////////////////////////////// Stereo ///////////////////////////////// */

QMap<QString, double> calculateStereoOnLevelCoordinates(int stLvlSlope, int stLvlOffset) {

    float tempFloat;

    // Calculate a
    float a;
    if (stLvlSlope >= 0x8000)
        stLvlSlope = (-(0xFFFF - stLvlSlope));

    a = stLvlSlope / pow ((float)2, 16 - 1) / (float).0156;     // size =16

    // Calculate b
    float b;
    if (stLvlOffset >= 0x8000)
        stLvlOffset = (-(0xFFFF - stLvlOffset));
    b = stLvlOffset / pow ((float)2, 16 - 1) / (float).0156;    // size =16

    // Calculate xStart
    double xValueStart;
    tempFloat = (float) 0 / (float)20.;
    tempFloat = pow ((float)10, tempFloat);
    tempFloat = -((float)1. - tempFloat)/((float)1. + tempFloat);
    xValueStart = (float)100*((tempFloat-b)/a);

    // Calculate xStop
    double xValueStop;
    tempFloat = (float) 40/ (float)20.;
    tempFloat = pow ((float)10, tempFloat);
    tempFloat = -((float)1. - tempFloat)/((float)1. + tempFloat);
    xValueStop = (float)100*((tempFloat-b)/a);

    // Calculate yStart
    double yValueStart = 0;

    // Calculate yStop
    double yValueStop;
    yValueStop = 40;

    // Calculate xMin
    double xValueMin = -20;
    // Calculate xMax
    double xValueMax = 80;

    // Calculate yMin
    double yValueMin;
    yValueMin = 0;

    // Calculate yMax
    double yValueMax = 40;

    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    qDebug() << "xValueMax : " <<  xValueMax
           << ", yValueMax : " <<  yValueMax
              << ", xValueStart : " <<  xValueStart
                 << ", yValueStart : " <<  yValueStart
                    << ", xValueStop : " <<  xValueStop
                       << ", yValueStop : " <<  yValueStop
                          << ", xValueMin : " <<  xValueMin
                             << ", yValueMin : " <<  yValueMin
                               << endl;
    return coordinates;

}

QMap<QString, double> calculateStereoCoordinates(int stSlope, int stOffset) {

    float tempFloat;

    // Calculate a
    float a;
    if (stSlope >= 0x8000)
        stSlope = (-(0xFFFF - stSlope));

    a = stSlope / pow ((float)2, 16 - 1) / (float).0156;     //size

    // Calculate b
    float b;
    if (stOffset >= 0x8000)
        stOffset = (-(0xFFFF - stOffset));
    b = stOffset / pow ((float)2, 16 - 1) / (float).0156;    //size =16

    // Calculate xStart
    double xValueStart;
    xValueStart = (float)100*((1-b)/a);

    // Calculate xStop
    double xValueStop;
    tempFloat = 40 / pow ((float)2, 16 - 1);
    xValueStop = (float)100*((tempFloat-b)/a);

    // Calculate yStart
    double yValueStart = 40;

    // Calculate yStop
    double yValueStop;
    yValueStop = 0;

    // Calculate xMin
    double xValueMin = 0;
    // Calculate xMax
    double xValueMax = 100;

    // Calculate yMin
    double yValueMin = 40;

    // Calculate yMax
    double yValueMax;
    yValueMax = 0;

    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    return coordinates;
}


QList<int> calculateStereoOnLevelCoefficients(QMap<QString, double> coordinates) {       // Caustion setCordinates Deactivated

    // Collect output values
    float noStereoPoint, fullStereoPoint;
    noStereoPoint = coordinates["xValueStart"];
    fullStereoPoint = coordinates["xValueStop"];

    // Calculate new coordinates
    int att1, att2;
    att1 = 0;
    att2 = 40;

//    double xValueStart = noStereoPoint;
//    double yValueStart = 0;
//    double xValueStop = fullStereoPoint;
//    double yValueStop = 40;
//    double yValueMin = 0; // m_YValue_min

    // update coordinates
//     setCoordinates(this->m_XValues_SmtLvl[0], this->m_YValues_SmtLvl[0], xValueStart, yValueStart, xValueStop, yValueStop, this->m_XValues_SmtLvl[3], yValueMin);

    // update plot
//        plotCoordinates();

    // coefficients computation DSP J2
    noStereoPoint = noStereoPoint/100;
    fullStereoPoint = fullStereoPoint/100;

    // convert from dB to linear
    float fp, fq  , fatt1, fatt2;

    fatt1 = (float) att1 / (float)20.;
    fatt1 = pow ((float)10, fatt1);

    fatt2 = (float) att2 / (float)20. ;
    fatt2 = pow ((float)10, fatt2);

    fatt1 = -((float)1. - fatt1)/((float)1. + fatt1);
    fatt2 = -((float)1. - fatt2)/((float)1. + fatt2);

    fp = ((fatt1-fatt2)/(noStereoPoint-fullStereoPoint));  // scaling 64 times
    fq = (fatt1-(noStereoPoint*fp));
    fp = fp * (float).0156;
    fq = fq * (float).0156;

    // convert to Q15 format
    fp = fp * pow ((float)2,15);
    fq = fq * pow ((float)2,15);

    int slope, offset;
    slope = (int) fp;
    offset = (int) fq;

    if (slope < 0)
        slope &= (int) pow ((float)2,16)-1;
    if (offset < 0)
        offset &= (int) pow ((float)2,16)-1;

    // set the value to DSP
    QTextStream out(stdout);
    out << "slp : " <<  QString().setNum(slope,16) << ", off : " << QString().setNum(offset,16) << endl;
    QList<int> outVal;
    outVal.append(slope);
    outVal.append(offset);
    return outVal;
}

QList<int> calculateStereoCoefficients(QMap<QString, double> coordinates) {       // Caustion setCordinates Deactivated

    // Collect output values
    float noStereoPoint, fullStereoPoint;
    noStereoPoint = coordinates["xValueStart"];
    fullStereoPoint = coordinates["xValueStop"];

    // Calculate new coordinates
//    double xValueStart = noStereoPoint;
//    double yValueStart = 40;
//    double xValueStop = fullStereoPoint;
//    double yValueStop = 0;
//    double yValueMax = 0;

    // update coordinates
//    setCoordinates(this->m_XValues_SmtLvl[0], yValueMax, xValueStart, yValueStart, xValueStop, yValueStop, this->m_XValues_SmtLvl[3], this->m_YValues_SmtLvl[3]);

    // update plot
//        plotCoordinates();

    // coefficients computation DSP J2
    float fp, fq;

    noStereoPoint = noStereoPoint/(float)100;
    fullStereoPoint = fullStereoPoint/(float)100;
    fp = noStereoPoint-fullStereoPoint;
    fp = 1 / fp;
    fq = 1-noStereoPoint*fp;

    fp = fp * (float).0156;
    fq = fq * (float).0156;

    // convert to Q format
    fp = fp * pow ((float)2, 15);
    fq = fq * pow ((float)2, 15);

    int slope,offset;
    slope = (int) fp;
    offset = (int) fq;

    if (slope < 0)
        slope &= (int) pow ((float)2,16)-1;
    if (offset < 0)
        offset &= (int) pow ((float)2,16)-1;

    // set the value to DSP
    QTextStream out(stdout);
    out << "slp : " <<  QString().setNum(slope,16) << ", off : " << QString().setNum(offset,16) << endl;
    QList<int> outVal;
    outVal.append(slope);
    outVal.append(offset);
    return outVal;
}


/* //////////////////////////////// Highcut ///////////////////////////////// */

QMap<QString, double> calculateHighcutOnLevelCoordinates(int hcLvlSlope, int hcLvlOffset) {

    // Calculate a
    float a;
    if (hcLvlSlope >= 0x8000)
        hcLvlSlope = (-(0xFFFF - hcLvlSlope));

    a = hcLvlSlope / pow ((float)2, 16 - 1) / (float).0156;     // size =16

    // Calculate b
    float b;
    if (hcLvlOffset >= 0x8000)
        hcLvlOffset = (-(0xFFFF - hcLvlOffset));
    b = hcLvlOffset / pow ((float)2, 16 - 1) / (float).0156;    // size =16

    // Calculate xStart
    double xValueStart;
    xValueStart = (float)100*((0-b)/a);

    // Calculate xStop
    double xValueStop;
    xValueStop = (float)100*((1-b)/a);

    // Calculate yStart
    double yValueStart = 0;

    // Calculate yStop
    double yValueStop;
    yValueStop = 1;

    // Calculate xMin
    double xValueMin = -20;
    // Calculate xMax
    double xValueMax = 80;

    // Calculate yMin
    double yValueMin;
    yValueMin = 0;

    // Calculate yMax
    double yValueMax = 1;

    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    return coordinates;

}

QMap<QString, double> calculateHighcutCoordinates(int hcSlope, int hcOffset) {

    // Calculate a
    float a;
    if (hcSlope >= 0x8000)
        hcSlope = (-(0xFFFF - hcSlope));

    a = hcSlope / pow ((float)2, 16 - 1) / (float).0156;     //size

    // Calculate b
    float b;
    if (hcOffset >= 0x8000)
        hcOffset = (-(0xFFFF - hcOffset));
    b = hcOffset / pow ((float)2, 16 - 1) / (float).0156;    //size =16

    // Calculate xStart
    double xValueStart;
    xValueStart = (float)100*((1-b)/a);

    // Calculate xStop
    double xValueStop;
    xValueStop = (float)100*((0-b)/a);

    // Calculate yStart
    double yValueStart = 1;

    // Calculate yStop
    double yValueStop;
    yValueStop = 0;

    // Calculate xMin
    double xValueMin = 0;
    // Calculate xMax
    double xValueMax = 100;

    // Calculate yMin
    double yValueMin = 1;

    // Calculate yMax
    double yValueMax;
    yValueMax = 0;


    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    return coordinates;
}

QList<int> calculateHighcutOnLevelCoefficients(QMap<QString, double> coordinates) {

    // Collect output values
    float noHighcutPoint, fullHighcutPoint;
    // m_set_edit1.GetLine(0,buffer,20); // change made 19/02/2015, should be tested, CL
    noHighcutPoint = coordinates["xValueStart"];               // Replace by output 'InputField element'
    // m_set_edit2.GetLine(0,buffer,20);
    fullHighcutPoint = coordinates["xValueStop"];              // Replace by output 'InputField element'

    // Calculate new coordinates

//    double xValueStart = noHighcutPoint;
//    double yValueStart = 0;
//    double xValueStop = fullHighcutPoint;
//    double yValueStop = 1;
//    double yValueMin = 0; // m_YValue_min

    // update coordinates
//    setCoordinates(this->m_XValues_SmtLvl[0], this->m_YValues_SmtLvl[0], xValueStart, yValueStart, xValueStop, yValueStop, this->m_XValues_SmtLvl[3], yValueMin);

    // update plot
//        plotCoordinates();

    // coefficients computation DSP J2
    noHighcutPoint = noHighcutPoint/100;
    fullHighcutPoint = fullHighcutPoint/100;

    // convert from dB to linear
    float fp, fq;

    fp = 1/(fullHighcutPoint-noHighcutPoint);  // scaling 64 times
    fq = -noHighcutPoint*fp;
    fp = fp * (float).0156;
    fq = fq * (float).0156;

    // convert to Q15 format
    fp = fp * pow ((float)2,15);
    fq = fq * pow ((float)2,15);

    int slope, offset;
    slope = (int) fp;
    offset = (int) fq;

    if (slope < 0)
        slope &= (int) pow ((float)2,16)-1;
    if (offset < 0)
        offset &= (int) pow ((float)2,16)-1;

    // set the value to DSP
    QTextStream out(stdout);
    out << "slp : " <<  QString().setNum(slope,16) << ", off : " << QString().setNum(offset,16) << endl;
    QList<int> outVal;
    outVal.append(slope);
    outVal.append(offset);
    return outVal;

}

QList<int> calculateHighcutCoefficients(QMap<QString, double> coordinates) {

    // Collect output values
    float noHighcutPoint, fullHighcutPoint;
                                                            // m_set_edit1.GetLine(0,buffer,20); // change made 19/02/2015, should be tested, CL
    noHighcutPoint = coordinates["xValueStart"];               // Replace by output 'InputField element'
                                                            // m_set_edit2.GetLine(0,buffer,20);
    fullHighcutPoint = coordinates["xValueStop"];              // Replace by output 'InputField element'

    // Calculate new coordinates
//    double xValueStart = noHighcutPoint;
//    double yValueStart = 1;
//    double xValueStop = fullHighcutPoint;
//    double yValueStop = 0;
//    double yValueMax = 0;

    // update coordinates
//    setCoordinates(this->m_XValues_SmtLvl[0], yValueMax, xValueStart, yValueStart, xValueStop, yValueStop, this->m_XValues_SmtLvl[3], this->m_YValues_SmtLvl[3]);

    // update plot
//        plotCoordinates();

    // coefficients computation DSP J2
    float fp, fq;

    noHighcutPoint = noHighcutPoint/(float)100;
    fullHighcutPoint = fullHighcutPoint/(float)100;
    fp = noHighcutPoint-fullHighcutPoint;
    fp = 1 / fp;
    fq = 1-noHighcutPoint*fp;

    fp = fp * (float).0156;
    fq = fq * (float).0156;

    // convert to Q format
    fp = fp * pow ((float)2, 15);
    fq = fq * pow ((float)2, 15);

    int slope,offset;
    slope = (int) fp;
    offset = (int) fq;

    if (slope < 0)
        slope &= (int) pow ((float)2,16)-1;
    if (offset < 0)
        offset &= (int) pow ((float)2,16)-1;

    // set the value to DSP
    QTextStream out(stdout);
    out << "slp : " <<  QString().setNum(slope,16) << ", off : " << QString().setNum(offset,16) << endl;
    QList<int> outVal;
    outVal.append(slope);
    outVal.append(offset);
    return outVal;
}


/* //////////////////////////////// Audio Filter ///////////////////////////////// */

QMap<QString, double> calculateAudioFilterCoordinates(int hcSlope, int hcOffset) {


    // Calculate a
    float a;
    if (hcSlope >= 0x8000)
        hcSlope = (-(0xFFFF - hcSlope));

    a = hcSlope / pow ((float)2, 16 - 1) / (float).0156;     //size

    // Calculate b
    float b;
    if (hcOffset >= 0x8000)
        hcOffset = (-(0xFFFF - hcOffset));
    b = hcOffset / pow ((float)2, 16 - 1) / (float).0156;    //size =16

    // Calculate xStart
    double xValueStart;
    xValueStart = (float)100*((0-b)/a);

    // Calculate xStop
    double xValueStop;
    xValueStop = (float)100*((1-b)/a);

    // Calculate yStart
    double yValueStart = 0;

    // Calculate yStop
    double yValueStop;
    yValueStop = 1;

    // Calculate xMin
    double xValueMin = 0;
    // Calculate xMax
    double xValueMax = 80;

    // Calculate yMin
    double yValueMin = 0;

    // Calculate yMax
    double yValueMax;
    yValueMax = 1;

    QMap<QString, double> coordinates;
    coordinates["xValueMax"] = xValueMax;
    coordinates["yValueMax"] = yValueMax;
    coordinates["xValueStart"] = xValueStart;
    coordinates["yValueStart"] = yValueStart;
    coordinates["xValueStop"] = xValueStop;
    coordinates["yValueStop"] = yValueStop;
    coordinates["xValueMin"] = xValueMin;
    coordinates["yValueMin"] = yValueMin;

    return coordinates;
}

QList<int> calculateAudioFilterCoefficients(QMap<QString, double> coordinates) {

    // Collect output values
    float fullOpenPoint, fullClosePoint;
                                                            // m_set_edit1.GetLine(0,buffer,20); // change made 19/02/2015, should be tested, CL
    fullOpenPoint = coordinates["xValueStart"];               // Replace by output 'InputField element'
                                                            // m_set_edit2.GetLine(0,buffer,20);
    fullClosePoint = coordinates["xValueStop"];              // Replace by output 'InputField element'

    // Calculate new coordinates
//    double xValueStart = fullOpenPoint;
//    double yValueStart = 0;
//    double xValueStop = fullClosePoint;
//    double yValueStop = 1;
//    double yValueMax = 1;

    // update coordinates
//    setCoordinates(this->m_XValues_SmtLvl[0], yValueMax, xValueStart, yValueStart, xValueStop, yValueStop, this->m_XValues_SmtLvl[3], this->m_YValues_SmtLvl[3]);

    // update plot
//           plotCoordinates();

    // coefficients computation DSP J2
    float fp, fq;

    fullOpenPoint = fullOpenPoint/(float)100;
    fullClosePoint = fullClosePoint/(float)100;
    fp = fullClosePoint-fullOpenPoint;
    fp = 1 / fp;
    fq = -fullOpenPoint*fp;

    fp = fp * (float).0156;
    fq = fq * (float).0156;

    // convert to Q format
    fp = fp * pow ((float)2, 15);
    fq = fq * pow ((float)2, 15);

    int slope,offset;
    slope = (int) fp;
    offset = (int) fq;

    if (slope < 0)
        slope &= (int) pow ((float)2,16)-1;
    if (offset < 0)
        offset &= (int) pow ((float)2,16)-1;

    // set the value to DSP
    QTextStream out(stdout);
    out << "slp : " <<  QString().setNum(slope,16) << ", off : " << QString().setNum(offset,16) << endl;
    QList<int> outVal;
    outVal.append(slope);
    outVal.append(offset);
    return outVal;
}
