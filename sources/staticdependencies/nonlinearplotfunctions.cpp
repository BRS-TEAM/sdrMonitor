#include "headers/staticdependencies/nonlinearplotfunctions.h"

#include <math.h>
#include <QDebug>
#include "headers/staticdependencies/datatypeconversion.h"

#define PI 3.1415926535897932384626433832795

double mCosTable[50];
double mSinTable[50];
double mThetaRe[50];
double mThetaIm [50];
double mDataX[50];
double mDataY[50];

void drawAntennaBeam(int w0r, int w0i, int w1r, int w1i, QVector<double> &x, QVector<double> &y) {


    double mAntennaDistance = 0.5;
    int NB_PTS = 50;

        double lambda, theta;

        for (int i = 0; i < NB_PTS; i++)
        {
            theta = 2 * PI * i / (NB_PTS - 1);
            lambda = PI * mAntennaDistance * cos(theta);
            mCosTable[i] = lambda;//cos (lambda);
            mSinTable[i] = sin (lambda);
            mThetaRe[i] = cos (theta);
            mThetaIm [i] = sin (theta);
        }

        int W0_R = w0r;
        int W0_I = w0i;
        int W1_R = w1r;
        int W1_I = w1i;

        int i;
        double gain_re;
        double gain_im;
        double module;
        double amp1, amp2, phase1, phase2;

        float w0_r, w0_i, w1_r, w1_i;

        w0_r = single2float(W0_R);
        w0_i = single2float(W0_I);
        w1_r = single2float(W1_R);
        w1_i = single2float(W1_I);


        amp1 = sqrt (w0_r * w0_r + w0_i * w0_i);
        amp2 = sqrt (w1_r * w1_r + w1_i * w1_i);

        phase1 = atan2 (w0_r, w0_i);
        phase2 = atan2 (w1_r, w1_i);

        for (i = 0; i < NB_PTS; i++)
        {
            gain_re = amp1 * cos (mCosTable[i] + phase1) + amp2 * cos (mCosTable[i] + phase2);
            gain_im = amp1 * sin (mCosTable[i] + phase1) - amp2 * sin (mCosTable[i] + phase2);
            module = sqrt (gain_re * gain_re + gain_im * gain_im);

            mDataX[i] = mThetaRe[i] * module;
            mDataY[i] = mThetaIm[i] * module;

        }

        /***************************************************************************************/
        x.clear();
        y.clear();

        for (i = 0; i < NB_PTS; i++)
        {

            x.append( mDataX[i] );
            y.append( mDataY[i] );

        }

}
