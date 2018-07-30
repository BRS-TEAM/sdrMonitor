#include "headers/staticdependencies/valueconversion.h"
#include "headers/staticdependencies/datatypeconversion.h"
#include "headers/core/sharedflags.h"

#include <math.h>
#include <QList>

#define REGULAR "REGULAR"
#define REVERSE "REVERSE"
#define TO_DB "TO_DB"
#define TO_40DB "TO_40DB"
#define IN_DB "IN_DB"
#define MODULATION "MODULATION"
#define HALF_SCALE "HALF_SCALE"
#define DIRECT "DIRECT"

#define UPPER "UPPER"
#define LOWER "LOWER"
#define COMPLEX "COMPLEX"

double convertCoeffForSensor_TimePlot(const double value, const QString & configDataType, double maxCoeffValue, const QString & dataType2) {

    Q_UNUSED(dataType2)

    if ( configDataType == REGULAR ) {


        double source;

        source = value / maxCoeffValue;
        source = source * 100;

        return source;

    }
    else if ( configDataType ==  REVERSE ) {

        return -value;

    }
    else if ( configDataType ==  TO_DB ) {


        double source;

        source = value / maxCoeffValue;

        if (!source) source = .000000001;           // Change to minimum    .000000001      maybe

        source = log10 (source) * 20;

        return source;

    } else if ( configDataType ==  TO_40DB ) {


        double source;

        source = value / maxCoeffValue;

        if (!source) source =  .01;
        if (source == 1) source = .9999999999999999999;

        source = (source+1)/(1-source);
        source = log10 (source) * 20 ;

        if (source > 40) source = 40;   // Conversion en dB Full effect = 40dB
        if (source<1) source = 0;


        return source;

    } else if ( configDataType ==  IN_DB ) {

        double source;

        source = value / 318.;

        return source;

    } else if ( configDataType ==  MODULATION ) {

        double source;

        source = value / 200.0 / 1.7;

        return source;

    } else if ( configDataType ==  HALF_SCALE ) {

        double source;

        source = value / maxCoeffValue;
        source = source * 200;

        return source;

    } else if ( configDataType ==  DIRECT ) {

        return value;

    } else {



        double source;

        source = value / maxCoeffValue;

        if (source < 0.001) source = 0;                // Change to minimum         if (source < 0.001)     MayBe

        return source;

    }

}


double convertCoeffForTextField_TimePlot(const int value, const QString & halfFormatInterpretation, const QString & attribute2)
{
    Q_UNUSED(attribute2)

    if( halfFormatInterpretation ==  UPPER ) {

        double result = half2double((short)(value >> 16));
        return result;

    }
    else if( halfFormatInterpretation ==  LOWER ) {

        double result = half2double((short)(value & 0x0000ffff));
        return result;

    }
    else if( halfFormatInterpretation ==  COMPLEX ) {        // Used only by Time Plot

        double iValue = half2double((short)(value & 0x0000ffff));
        double qValue = half2double((short)(value >> 16));
        double result = sqrt(iValue * iValue + qValue * qValue);
        return result;

    }
    else {

        return (int)value;

    }

}


int convertCoeffFromTextField(const int value, const QString & halfFormatInterpretation, const QString & attribute2)
{
    Q_UNUSED(attribute2)

    if( halfFormatInterpretation == UPPER ) {

        int result;
        u_int16_t convertOut = 0;
        convertOut = doubles2half(value);
        result = (convertOut & 0xffff) << 16; // convert to 16 bit half
        return result;

    }
    else if( halfFormatInterpretation == LOWER ) {

        int result;
        u_int16_t convertOut = 0;
        convertOut = doubles2half(value);
        result = (convertOut & 0xffff); // convert to 16 bit half
        return result;

    }
    else {

        return value;

    }

}

void byteReverse (int* input_ptr, int* output_ptr, int len)
{
    char * input_ptr_char = (char*) input_ptr;
    char * output_ptr_char = (char*) output_ptr;
    for (int i = 0; i< len; i++)
    {
        output_ptr_char[i] = input_ptr_char[len-1-i];
    }
}

int hexCharToInt(char a){
    if(a>='0' && a<='9')
        return(a-48);
    else if(a>='A' && a<='Z')
        return(a-55);
    else
        return(a-87);
}

QString hexToString(std::string str)
{
    QString hexString;
    for(unsigned int i=0 ; i < str.length(); i++)
    {
        if ((i+2)<= str.length())
        {
            char a = str.at(i++);
            char b = str.at(i);

            int x = hexCharToInt(a);
            int y = hexCharToInt(b);
            hexString.append((char)((16*x)+y));
        }
    }

    return hexString;
}
/*
double agcNbGainCalculate(QString fECoeff, QList<double> parameters) {

    struct sTemp
    {
        double Target;
        double Kfm;
        double Beth;
        double MaxGain;
    };

    sTemp Temp;
    const double FS = 45600;                    // Sample frequency
    const double FSSLOPE = 45600;               // Slope Sample frequency
    double KBAND=0;

    if (radioMode == FM_MODE)
    {
        //=========================================================================
        // FM AGC BE/NB Processing Equation
        // c.f. STA710_OM_v1.1.7.equ provided by ST
        //=========================================================================
        // Constants - do NOT change
        //=========================================================================
        KBAND = 78;                      // Constant for FM band [dB]

    }
    else if (radioMode == DAB_MODE)
    {
        //=========================================================================
        // DAB3 AGC BE/NB Processing Equation
        // c.f. STA710_OM_v1.1.7.equ provided by ST
        //=========================================================================
        // Constants - do NOT change
        //=========================================================================
        KBAND = 76.8;                      // Constant for dab3 band [dB]
    }
    else if (radioMode == AM_MODE)
    {
        //=========================================================================
        // DAB3 AGC BE/NB Processing Equation
        // c.f. STA710_OM_v1.1.7.equ provided by ST
        //=========================================================================
        // Constants - do NOT change
        //=========================================================================
        KBAND = 67.5;                      // Constant for AM band [dB]
    }




    // Target Value on BB interface
    // =========================================================================

    if(fECoeff == "_fst_ths" && parameters.size() >= 1) {
        double FmAgcNbTargetValue = parameters.at(0);
        Temp.Target =  pow(10,(FmAgcNbTargetValue / 20));
        return Temp.Target;
    }

    // Deadband
    // =========================================================================

    else if(fECoeff == "_deadBand" && parameters.size() >= 1) {
        double FmAgcNbDeadBandRatio = parameters.at(0);
        return Temp.Target* FmAgcNbDeadBandRatio;
    }

    // Intervention threshold --> Max gain
    // =========================================================================

    else if(fECoeff == "maxGain0" && parameters.size() >= 2) {

        double FmAgcNbTh = parameters.at(0);
        double FmAgcNbTargetValue = parameters.at(1);

        Temp.Kfm = pow(10, (double)KBAND/20);
        Temp.Beth = pow(10, (double)(FmAgcNbTh/20));
        Temp.MaxGain = (pow(10,(FmAgcNbTargetValue / 20)))*(pow(10,(KBAND/20)))/(pow(10,(FmAgcNbTh/20)));
        return int(floor(Temp.MaxGain * pow(2,(double)8)));
    }
    else if(fECoeff == "maxGain0_1" && parameters.size() >= 2) {

        double FmAgcNbTh = parameters.at(0);
        double FmAgcNbTargetValue = parameters.at(1);

        Temp.Kfm = pow(10, (double)KBAND/20);
        Temp.Beth = pow(10, (double)(FmAgcNbTh/20));
        Temp.MaxGain = (pow(10,(FmAgcNbTargetValue / 20)))*(pow(10,(KBAND/20)))/(pow(10,(FmAgcNbTh/20)));

        return int(((Temp.MaxGain * pow(2,(double)8))-floor(Temp.MaxGain * pow(2,(double)8))) * pow(2,(double)24));
    }

    // Slow time constants of the rectifier after the detector
    // =========================================================================

    else if(fECoeff == "_b0sSlow" && parameters.size() >= 2) {

        double FmAgcNbSlowTauAtt = parameters.at(0);
        double FmAgcNbSlowTauRel = parameters.at(1);

        return ((2 / (1 + 2 * FmAgcNbSlowTauAtt * FS )) + (2 / (1 + 2 * FmAgcNbSlowTauRel * FS ))) / 2;
    }
    else if(fECoeff == "_b0dSlow" && parameters.size() >= 2) {

        double FmAgcNbSlowTauAtt = parameters.at(0);
        double FmAgcNbSlowTauRel = parameters.at(1);

        return ((2 / (1 + 2 * FmAgcNbSlowTauAtt * FS )) - (2 / (1 + 2 * FmAgcNbSlowTauRel * FS ))) / 2;
    }

    // Fast time constants of the rectifier after the detector
    // =========================================================================

    else if(fECoeff == "_b0sFast"  && parameters.size() >= 2) {

        double FmAgcNbFastTauAtt = parameters.at(0);
        double FmAgcNbFastTauRel = parameters.at(1);

        return ((2 / (1 + 2 * FmAgcNbFastTauAtt * FS )) + (2 / (1 + 2 * FmAgcNbFastTauRel * FS ))) / 2;
    }
    else if(fECoeff == "_b0dFast" && parameters.size() >= 2) {

        double FmAgcNbFastTauAtt = parameters.at(0);
        double FmAgcNbFastTauRel = parameters.at(1);

        return ((2 / (1 + 2 * FmAgcNbFastTauAtt * FS )) - (2 / (1 + 2 * FmAgcNbFastTauRel * FS ))) / 2;
    }

    // Gain fast attack/release slope
    // =========================================================================

    else if(fECoeff == "_fastSlopeAttack" && parameters.size() >= 1) {

        double FmAgcNbGainFastSlopeAtt = parameters.at(0);

        return 1 - pow(10,( FmAgcNbGainFastSlopeAtt / (FSSLOPE*20)));
    }
    else if(fECoeff == "_fastSlopeRel" && parameters.size() >= 1) {

        double FmAgcNbGainFastSlopeRel = parameters.at(0);

        return 1 - pow(10,( FmAgcNbGainFastSlopeRel / (FSSLOPE*20)));
    }

    // Gain slow attack/release slope
    // =========================================================================

    else if(fECoeff == "_slowSlopeAttack" && parameters.size() >= 1) {

        double FmAgcNbGainSlowSlopeAtt = parameters.at(0);

        return  1 - pow(10,( FmAgcNbGainSlowSlopeAtt / (FSSLOPE*20)));
    }
    else if(fECoeff == "_slowSlopeRel" && parameters.size() >= 1) {

        double FmAgcNbGainSlowSlopeRel = parameters.at(0);

        return  1 - pow(10,( FmAgcNbGainSlowSlopeRel / (FSSLOPE*20)));
    }

    // AGC NB fast threshold attack/release in respect to full scale
    // =========================================================================

    else if(fECoeff == "_fastThrAttack" && parameters.size() >= 1) {

        double FmAgcNbFastThAtt = parameters.at(0);

        return pow(10,(FmAgcNbFastThAtt/20)) - Temp.Target;
    }
    else if(fECoeff == "_fastThrRelease" && parameters.size() >= 1) {

        double FmAgcNbFastThRel = parameters.at(0);

        return pow(10,(FmAgcNbFastThRel/20)) - Temp.Target;

    }


}
*/
