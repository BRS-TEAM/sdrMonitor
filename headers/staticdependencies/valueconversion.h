#ifndef VALUECONVERSION
#define VALUECONVERSION

#include <QString>

double convertCoeffForSensor_TimePlot(const double value, const QString & configDataType, double maxCoeffValue = 32767.0, const QString & dataType2 = "NOT_DEFINED");

double convertCoeffForTextField_TimePlot(const int value, const QString & halfFormatInterpretation, const QString & attribute2 = "NOT_DEFINED");
int convertCoeffFromTextField(const int value, const QString & halfFormatInterpretation, const QString & attribute2 = "NOT_DEFINED");

void byteReverse (int* input_ptr, int* output_ptr, int len);
int hexCharToInt(char a);
QString hexToString(std::string str);

//double agcNbGainCalculate(QString fECoeff, QList<double> parameters);

#endif // VALUECONVERSION

