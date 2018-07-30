/*!
 * \file     xyplot.cpp
 * \brief    XyPlot Element Class
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


// Essential Headers
#include "headers/elements/xyplot.h"
#include "ui_xyplot.h"

// Data Processing
#include "headers/staticdependencies/datatypeconversion.h"
#include "headers/staticdependencies/linearplotfunctions.h"
#include "headers/staticdependencies/nonlinearplotfunctions.h"

// Connection
#include "headers/elements/inputfield.h"

#include <QHBoxLayout>
#include <QList>

// Parent
#include "headers/core/tabcontroller.h"

#define SLIDER_PRECISION 1000

#define X_VALUE_MAX 0
#define Y_VALUE_MAX 0
#define X_VALUE_START 1
#define Y_VALUE_START 1
#define X_VALUE_STOP 2
#define Y_VALUE_STOP 2
#define X_VALUE_MIN 3
#define Y_VALUE_MIN 3

// Constructor
XYPlot::XYPlot(TabController *parent) :
    QWidget((QWidget*)parent),
    ui(new Ui::XYPlot)
{
    ui->setupUi(this);

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::red));
    //    ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot->graph(0)->setAntialiasedFill(false);

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
    ui->customPlot->xAxis->setAutoTickStep(true);
    //    ui->customPlot->xAxis->setTickStep(20);
    ui->customPlot->yAxis->setAutoTickStep(true);
    //    ui->customPlot->yAxis->setTickStep(0.2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->addGraph(); // Red dot
    ui->customPlot->graph(1)->setPen(QPen(Qt::blue,2));
    ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssCross);

    graphLabel = "X-Y Plot";
    ui->graphLabel->setText(graphLabel);

//    ui->refreshButton->setIcon(QPixmap("../SDRToolResources/icons/refresh.png"));
//    ui->refreshButton->setIconSize(ui->refreshButton->rect().size());

//    ui->clearButton->setIcon(QPixmap("../SDRToolResources/icons/clear.png"));
//    ui->clearButton->setIconSize(ui->clearButton->rect().size());

    ui->setButton->setIcon(QPixmap("../SDRToolResources/icons/send.png"));
    ui->setButton->setIconSize(ui->setButton->rect().size());

    ui->checkBox->hide();
    //ui->clearButton->hide();

    dStructure = 0;
    dStructure2 = 0;
    tabControllerParent = (TabController*) parent;
    updateFlag = false;

}

// Destructor
XYPlot::~XYPlot()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

void XYPlot::drawPlot() {

    if(graphType == "Linear") {
        clearPlot();
        calculateCoordinates();
        plotCoordinates();
        coefficeintValues = calculateCoefficient();
        updateOutputField();
        adjustInputSliders();
    }
    else if(graphType == "Non-Linear") {
        // clearPlot();
        plotAntennaBeam();
    }
}

void XYPlot::plotAntennaBeam() {

    if( antennaDataSource.size() == 4) {

        QVector<double> x, y;

        drawAntennaBeam(antennaDataSource.at(0)->value, antennaDataSource.at(1)->value, antennaDataSource.at(2)->value, antennaDataSource.at(3)->value, x, y);

        xRangeMin = -8;
        xRangeMax = 8;
        yRangeMin = -8;
        yRangeMax = 8;

        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 4));
        ui->customPlot->graph(0)->setErrorType(QCPGraph::etValue);
        ui->customPlot->graph(0)->setName(graphLabel);

        if(!x.isEmpty() && !y.isEmpty())
            ui->customPlot->graph(0)->setData(x, y);

        // give the axes some labels:
        //ui->customPlot->xAxis->setLabel(xAxisLabel);
        //ui->customPlot->yAxis->setLabel(yAxisLabel);
        // set axes ranges, so we see all data:
        ui->customPlot->yAxis->setRange(yRangeMin, yRangeMax);
        ui->customPlot->xAxis->setRange(xRangeMin, xRangeMax);

        ui->customPlot->replot();

    }

}

void XYPlot::clearPlot() {
    ui->customPlot->graph(0)->clearData();
    //ui->customPlot->graph(1)->clearData();
    ui->customPlot->replot();
}

int XYPlot::getOutputSourceIdentifierCount() {
    int count = 0;
    for(int i = 0; i < outputEditField.count(); i++)
        if(outputEditField.at(i)->getNameIdentifier() != "DEFAULTVALUE")
            count++;

    return count;
}

QString XYPlot::getOutputSourceIdentifier(int index) {
    int offset = 0;
    if(getOutputSourceIdentifierCount() > index)
        for(int i = 0; i <= index; i++)
            if(outputEditField.at(i)->getNameIdentifier() == "DEFAULTVALUE")
                offset++;

    return outputEditField.at(index + offset)->getNameIdentifier();
}

void XYPlot::resetOutputSource() {
    for(int i = 0; i < outputEditField.count(); i++)
        outputEditField.at(i)->setStructure(0);
}

// Linear Plot Functions

void XYPlot::plotCoordinates() {

    QVector<double> x(4), y(4);

    QTextStream out(stdout);

    xRangeMin = 0;
    xRangeMax = 0;
    yRangeMin = 0;
    yRangeMax = 0;

    bool isInvalid = false;
    QString invalidValue = "NaN";

    for (unsigned int i=0; i < 4; i++)
    {
        double xVal = m_XValues[i];
        double yVal = m_YValues[i];

        if (xVal != xVal || yVal != yVal){

            isInvalid = true;
            invalidValue = "NaN";

        } else if (xVal > std::numeric_limits<qreal>::max() || yVal > std::numeric_limits<qreal>::max()){

            isInvalid = true;
            invalidValue = "+Inf";

        } else if (xVal < -std::numeric_limits<qreal>::max() || yVal < -std::numeric_limits<qreal>::max()){

            isInvalid = true;
            invalidValue = "-Inf";

        } else {
            x[i] = m_XValues[i];
            y[i] = m_YValues[i];
            out << "x : " <<  x[i] << ", y : " << y[i] << endl;

            if(x[i] < xRangeMin)
                xRangeMin = x[i];
            if(x[i] > xRangeMax)
                xRangeMax = x[i];
            if(y[i] < yRangeMin)
                yRangeMin = y[i];
            if(y[i] > yRangeMax)
                yRangeMax = y[i];
        }
    }

    ui->customPlot->graph(0)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    ui->customPlot->graph(0)->setErrorType(QCPGraph::etValue);
    ui->customPlot->graph(0)->setName(graphLabel);

    if(isInvalid) {
        x.fill(0);
        y.fill(0);
    }

    ui->customPlot->graph(0)->setData(x, y);


    // give the axes some labels:
    ui->customPlot->xAxis->setLabel(xAxisLabel);
    ui->customPlot->yAxis->setLabel(yAxisLabel);
    // set axes ranges, so we see all data:
    float xMinScale = (xRangeMax - xRangeMin)*0.1, xMaxScale = (xRangeMax - xRangeMin)*0.1, yMinScale = (yRangeMax - yRangeMin)*0.1, yMaxScale = (yRangeMax - yRangeMin)*0.1;

//    if(xMinScale < 0) xMinScale = - xMinScale;
//    if(yMinScale < 0) yMinScale = - yMinScale;
//    if((xRangeMax - xRangeMin) < 0) xMaxScale = - (xRangeMax - xRangeMin);
//    if((yRangeMax - yRangeMin) < 0) { yMaxScale = - (yRangeMax - yRangeMin); }
//    ui->customPlot->yAxis->setRange(yRangeMin - yMinScale,yMaxScale + yMaxScale*0.2, Qt::AlignLeft);
//    ui->customPlot->xAxis->setRange(xRangeMin - xMinScale,xMaxScale + xMaxScale*0.2, Qt::AlignLeft);

    ui->customPlot->yAxis->setRange(yRangeMin - yMinScale,yRangeMax + yMaxScale);
    ui->customPlot->xAxis->setRange(xRangeMin - xMinScale,xRangeMax + xMaxScale);

    ui->customPlot->replot();
}

void XYPlot::calculateCoordinates() {

    QMap<QString, int> outValue;

    // Get Coefficients
    if (outputRoleIndex.find("slope") != outputRoleIndex.end())
        outValue["slope"] = QString(outputEditField.at(outputRoleIndex["slope"])->getValueLineEdit()).toInt();

    if (outputRoleIndex.find("offset") != outputRoleIndex.end())
        outValue["offset"] = QString(outputEditField.at(outputRoleIndex["offset"])->getValueLineEdit()).toInt();

    if (outputRoleIndex.find("min") != outputRoleIndex.end())
        outValue["min"] = QString(outputEditField.at(outputRoleIndex["min"])->getValueLineEdit()).toInt();

    // Calculate Coordinates
    QMap<QString, double> coordinates;
    calculateLinearPlotCoordinates(outValue, linearPlotType, coordinates);


    // Set Coordinates

    if(!coordinates.empty()) {
        setCoordinates(coordinates["xValueMax"], coordinates["yValueMax"]
                , coordinates["xValueStart"], coordinates["yValueStart"]
                , coordinates["xValueStop"], coordinates["yValueStop"]
                , coordinates["xValueMin"], coordinates["yValueMin"]);
    }

}

void XYPlot::setCoordinates(double xValueMax, double yValueMax, double xValueStart, double yValueStart, double xValueStop, double yValueStop, double xValueMin, double yValueMin) {

    this->m_XValues[X_VALUE_MAX] = xValueMax;
    this->m_YValues[Y_VALUE_MAX] = yValueMax;
    this->m_XValues[X_VALUE_START] = xValueStart;
    this->m_YValues[Y_VALUE_START] = yValueStart;
    this->m_XValues[X_VALUE_STOP] = xValueStop;
    this->m_YValues[Y_VALUE_STOP] = yValueStop;
    this->m_XValues[X_VALUE_MIN] = xValueMin;
    this->m_YValues[Y_VALUE_MIN] = yValueMin;

    QTextStream out(stdout);
    out << "xValueMax : " <<  xValueMax
           << ", yValueMax : " <<  yValueMax
              << ", xValueStart : " <<  xValueStart
                 << ", yValueStart : " <<  yValueStart
                    << ", xValueStop : " <<  xValueStop
                       << ", yValueStop : " <<  yValueStop
                          << ", xValueMin : " <<  xValueMin
                             << ", yValueMin : " <<  yValueMin
                               << endl;

}

void XYPlot::getCoordinates(QMap<QString, double> &coordinates) {

    coordinates["xValueMax"] = this->m_XValues[X_VALUE_MAX];
    coordinates["yValueMax"] = this->m_YValues[Y_VALUE_MAX];
    coordinates["xValueStart"] = this->m_XValues[X_VALUE_START];
    coordinates["yValueStart"] = this->m_YValues[Y_VALUE_START];
    coordinates["xValueStop"] = this->m_XValues[X_VALUE_STOP];
    coordinates["yValueStop"] = this->m_YValues[Y_VALUE_STOP];
    coordinates["xValueMin"] = this->m_XValues[X_VALUE_MIN];
    coordinates["yValueMin"] = this->m_YValues[Y_VALUE_MIN];

}

QList<int> XYPlot::calculateCoefficient() {

    QList<int> outVal;
    QMap<QString, double> coordinates;
    getCoordinates(coordinates);

    calculateLinearPlotCoefficients(coordinates, linearPlotType, outVal);

    return outVal;

}

void XYPlot::adjustInputSliders() {
    if(inputSlider.count()) {
        for(int i = 0; i < inputSlider.count(); i++) {
            inputSlider.at(i)->setMinimum(m_XValues[X_VALUE_MIN] * SLIDER_PRECISION);
            inputSlider.at(i)->setMaximum(m_XValues[X_VALUE_MAX] * SLIDER_PRECISION);
            inputSlider.at(i)->setTickInterval(5);
        }


        if (inputRoleIndex.find("start") != inputRoleIndex.end()) {
            inputSlider.at(inputRoleIndex["start"])->setValue((int)m_XValues[X_VALUE_START] * SLIDER_PRECISION);
            inputEditField.at(inputRoleIndex["start"])->setValueLineEdit(QString().number(m_XValues[X_VALUE_START], 'f', 1));
        }

        if (inputRoleIndex.find("stop") != inputRoleIndex.end()) {
            inputSlider.at(inputRoleIndex["stop"])->setValue((int)m_XValues[X_VALUE_STOP] * SLIDER_PRECISION);
            inputEditField.at(inputRoleIndex["stop"])->setValueLineEdit(QString().number(m_XValues[X_VALUE_STOP], 'f', 1));
        }

        if(linearPlotType == "Softmute Level" || linearPlotType == "Softmute AM Level" || linearPlotType == "Stereo Level" || linearPlotType == "Highcut Level") {
            if (inputRoleIndex.find("limit") != inputRoleIndex.end()) {
                inputSlider.at(inputRoleIndex["limit"])->setValue((int)m_YValues[Y_VALUE_MIN] * SLIDER_PRECISION);
                inputEditField.at(inputRoleIndex["limit"])->setValueLineEdit(QString().number(m_YValues[Y_VALUE_MIN], 'f', 1));
            }
        }  else if (linearPlotType == "Softmute" || linearPlotType == "Stereo" || linearPlotType == "Highcut" || linearPlotType == "Softmute General" || linearPlotType == "Stereo General" || linearPlotType == "Highcut General") {
            if (inputRoleIndex.find("limit") != inputRoleIndex.end()) {
                inputSlider.at(inputRoleIndex["limit"])->setValue((int)m_YValues[Y_VALUE_MAX] * SLIDER_PRECISION);
                inputEditField.at(inputRoleIndex["limit"])->setValueLineEdit(QString().number(m_YValues[Y_VALUE_MAX], 'f', 1));
            }
        }
    }
}

void XYPlot::updateOutputField() {

    if(coefficeintValues.count()) {
        for(int i = 0; i < coefficeintValues.count(); i++) {
            if(outputEditField.count() > i) {

                if(outputEditField.at(i)->getStructure() != 0) {
                    outputEditField.at(i)->setValueLineEdit(QString().setNum(coefficeintValues.at(i), 16).right(outputEditField.at(i)->getStructure()->size/4));
                }
                else {
                    outputEditField.at(i)->setValueLineEdit(QString().setNum(coefficeintValues.at(i), 16));
                }

            }
        }
    }

    coefficeintValues.clear();

}

// Antenna

void XYPlot::setAntennaDataSourceIdentifier(QString label) {
    if(label != "DEFAULTVALUE")
        antennaDataSourceIdentifier.append(label);
}

QString XYPlot::getAntennaDataSourceIdentifier(int index) {
    if(antennaDataSourceIdentifier.count() > index)
        return antennaDataSourceIdentifier.at(index);
    return "";
}

int XYPlot::getAntennaDataSourceIdentifierCount() {
    return antennaDataSourceIdentifier.count();
}

bool XYPlot::setAntennaDataSource(DataStructure * structure) {

    if (structure != 0) {
        antennaDataSource.append(structure);

//        ui->customPlot->addGraph();
//        QColor color = QColor(rand() % 100, rand() % 100, rand() % 100);
//        ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(QPen(color));
// //        ui->customPlot->graph(ui->customPlot->graphCount()-1)->setBrush(QBrush(QColor(240, 255, 200)));
//        ui->customPlot->graph(ui->customPlot->graphCount()-1)->setAntialiasedFill(false);
//        dSourceLabel.at(dSourceIdentifier.indexOf(structure->name))->setStyleSheet("QLabel { color : " + color.name() + "; }");

        return true;
    }

    return false;
}

void XYPlot::resetAntennaDataSource() {

    for (int i = 0; i < antennaDataSource.count(); i++) {
//        ui->customPlot->removeGraph(ui->customPlot->graphCount()-1);
    }

    antennaDataSource.clear();

}


/************************ Initializer Methods used by XML parser. ************************/


void XYPlot::setType(QString t) {
    graphType = t;
}

QString XYPlot::getGraphType() { return graphType; }

// Linear Function Plot

void XYPlot::setLinearPlotType(QString t) {
    linearPlotType = t;
}

void XYPlot::addInputField(QString label) {
    inputSlider.append(new QSlider(this));
    inputSlider.last()->setOrientation(Qt::Horizontal);
    inputSlider.last()->setMinimumWidth(ui->customPlot->width());
    inputSlider.last()->setMaximumWidth(400);
    inputSlider.last()->setSingleStep(1);

    connect(inputSlider.last(), SIGNAL(valueChanged(int)), this, SLOT(inputSliderValueChange(int)));

    inputEditField.append(new InputField());
    inputEditField.last()->setValueLabel(label);

    if(label != "Limiter")
    {
        inputEditField.last()->setMode("readOnly");
    }

    connect(this, SIGNAL(enableElements()), inputEditField.last(), SLOT(enable()));
    connect(this, SIGNAL(disableElements()), inputEditField.last(), SLOT(disable()));

    ui->inputFieldLayout->insertWidget(ui->inputFieldLayout->count()-1, inputSlider.last());
    ui->inputFieldLayout2->insertWidget(ui->inputFieldLayout2->count()-1, inputEditField.last());

}

void XYPlot::addOutputField(QString label) {

    outputEditField.append(new InputField(tabControllerParent));
    //        inputEditField.last()->setHorizontalSizeLimit(65);
    outputEditField.last()->setValueLabel(label);
    outputEditField.last()->setMaximumSize(236,35);

    connect(this, SIGNAL(enableElements()), outputEditField.last(), SLOT(enable()));
    connect(this, SIGNAL(disableElements()), outputEditField.last(), SLOT(disable()));

    ui->outputFieldLayout->insertWidget(ui->outputFieldLayout->count()-1, outputEditField.last());

    if(ui->checkBox->isHidden())
        ui->checkBox->show();

    outputEditFieldOldValue.append("xxxx");
}

void XYPlot::setInputFieldParameters(QString valueType, QString slider, QString role) {

    Q_UNUSED(valueType)
    if(slider == "n" || slider == "N")
        inputSlider.last()->hide();

    if(role == "start" || role == "stop" || role == "limit")
        inputRoleIndex.insert(role, inputEditField.count()-1 );

}

void XYPlot::setOutputFieldParameters(QString name, QString output, QString disabledValue, QString mode, QString resetValue, QString role) {

    if(mode != "NA")
    {
        outputEditField.last()->setNameIdentifier(name);
        outputEditField.last()->setOutputType(output);
        outputEditField.last()->setDisabledValue(disabledValue);
        outputEditField.last()->setMode(mode);
        outputEditField.last()->setResetValue(resetValue);
        if(role == "slope" || role == "offset" || role == "min")
            outputRoleIndex.insert(role, outputEditField.count()-1 );
    }
    else
    {
    outputEditField.last()->setNameIdentifier(name);
    outputEditField.last()->setOutputType(output);
    outputEditField.last()->setDisabledValue(disabledValue);
    outputEditField.last()->setMode(mode);
    outputEditField.last()->setResetValue(resetValue);
    if(role == "slope" || role == "offset" || role == "min")
        outputRoleIndex.insert(role, outputEditField.count()-1 );
        outputEditField.last()->hide();
    }
}

// Set the indentification name of BarGraphH
void XYPlot::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

void XYPlot::setNameIdentifier2(const QString &value)
{
    nameIdentifier2 = value;
}

/************************ Getters & Setters ************************/

// Get the indentification name of BarGraphH
QString XYPlot::getNameIdentifier() {
    return nameIdentifier;
}

QString XYPlot::getNameIdentifier2() const
{
    return nameIdentifier2;
}

bool XYPlot::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

DataStructure * XYPlot::getStructure() {
    return dStructure;
}

bool XYPlot::setStructure2(DataStructure *structure)
{
    dStructure2 = 0;
    dStructure2 = structure;
    if (dStructure2 != 0)
        return true;
    return false;
}

DataStructure *XYPlot::getStructure2() const
{
    return dStructure2;
}

bool XYPlot::setOutputSourceStructure(DataStructure * structure) {

    if (structure != 0) {
        for(int i = 0; i < outputEditField.count(); i++)
            if(outputEditField.at(i)->getNameIdentifier() == structure->name) {
                outputEditField.at(i)->setStructure(structure);
                return true;
            }
    }

    return false;
}

void XYPlot::setIndex(int index) {
    this->index = index;
}

int XYPlot::getIndex() {
    return index;
}

void XYPlot::setXAxisLabel(QString label) {
    this->xAxisLabel = label;
}

void XYPlot::setYAxisLabel(QString label) {
    this->yAxisLabel = label;
}

void XYPlot::setGraphLabel(QString label) {
    this->graphLabel = label;
    ui->graphLabel->setText(graphLabel);
}

QString XYPlot::getGraphLabel(){ return graphLabel;}

/************************ Methods used in connected state with other elements. ************************/


/************************ Slots are special methods which execute in response to the connected signal. ************************/

// Response to a signal from checkBox.
void XYPlot::disable()
{
    this->setDisabled(true);
}

// Response to a signal from checkBox.
void XYPlot::enable()
{
    this->setEnabled(true);
}

void XYPlot::on_refreshButton_clicked()
{
//    updateFlag = true;
//    if(!ui->checkBox->isChecked())
//        drawPlot();
//    updateFlag = false;
}

void XYPlot::on_clearButton_clicked()
{
    clearPlot();
}

void XYPlot::on_setButton_clicked()
{
    if(!ui->checkBox->isHidden() && !ui->checkBox->isChecked()) {
            if(outputEditField.count()) {
            //
            recalculateBeforeSend();
            QList<DataStructure *> dS;
            QList<QString> values;
            for(int i = 0; i < outputEditField.count(); i++) {
                if(outputEditField.at(i)->getValueLineEdit() != outputEditFieldOldValue.at(i)) {
                    outputEditFieldOldValue.replace(i,outputEditField.at(i)->getValueLineEdit());
                    dS.append(outputEditField.at(i)->getStructure());
                    values.append(outputEditField.at(i)->getValueLineEdit());
                    outputEditField.at(i)->saveValueLineEdit(QString(outputEditField.at(i)->getValueLineEdit()));
                }
            }
            if(!dS.isEmpty()) {
                tabControllerParent->initHMIWriteReq(dS, values);
                on_refreshButton_clicked();
            }
        }
    }

}

void XYPlot::recalculateBeforeSend() {

    if(linearPlotType == "Softmute Level" || linearPlotType == "Softmute AM Level") {
        if ((inputRoleIndex.find("start") != inputRoleIndex.end()) && (inputRoleIndex.find("stop") != inputRoleIndex.end())) {
            if(inputSlider.at(inputRoleIndex["start"])->value() > inputSlider.at(inputRoleIndex["stop"])->value()) {

                if(inputSlider.at(inputRoleIndex["start"]) == qobject_cast<QSlider*> (sender())){
                    m_XValues[X_VALUE_START] = (double)inputSlider.at(inputRoleIndex["start"])->value()/(double)SLIDER_PRECISION;
                }else if (inputSlider.at(inputRoleIndex["stop"]) == qobject_cast<QSlider*> (sender())){
                    m_XValues[X_VALUE_STOP] = (double)inputSlider.at(inputRoleIndex["stop"])->value()/(double)SLIDER_PRECISION;
                }
                if (inputRoleIndex.find("limit") != inputRoleIndex.end())
                {
                    m_YValues[Y_VALUE_MIN] = inputEditField.at(inputRoleIndex["limit"])->getValueLineEdit().toInt();
                    m_YValues[Y_VALUE_STOP] = inputEditField.at(inputRoleIndex["limit"])->getValueLineEdit().toInt();
                }
                updateFlag = true;
                plotCoordinates();
                coefficeintValues = calculateCoefficient();
                updateOutputField();
                updateFlag = false;

            }
        }
    }
    else if (linearPlotType == "Stereo Level" || linearPlotType == "Highcut Level" || linearPlotType == "AF" ||
             linearPlotType == "Softmute" || linearPlotType == "Stereo" || linearPlotType == "Highcut" ||
             linearPlotType == "Softmute General" || linearPlotType == "Stereo General" || linearPlotType == "Highcut General") {
        if ((inputRoleIndex.find("start") != inputRoleIndex.end()) && (inputRoleIndex.find("stop") != inputRoleIndex.end())) {

            if(inputSlider.at(inputRoleIndex["start"])->value() < inputSlider.at(inputRoleIndex["stop"])->value()) {
                if(inputSlider.at(inputRoleIndex["start"]) == qobject_cast<QSlider*> (sender()))
                    m_XValues[X_VALUE_START] = (double)inputSlider.at(inputRoleIndex["start"])->value()/(double)SLIDER_PRECISION;
                else if (inputSlider.at(inputRoleIndex["stop"]) == qobject_cast<QSlider*> (sender()))
                    m_XValues[X_VALUE_STOP] = (double)inputSlider.at(inputRoleIndex["stop"])->value()/(double)SLIDER_PRECISION;


                if (inputRoleIndex.find("limit") != inputRoleIndex.end())
                    m_YValues[Y_VALUE_MAX] = inputEditField.at(inputRoleIndex["limit"])->getValueLineEdit().toInt();

                updateFlag = true;
                plotCoordinates();
                coefficeintValues = calculateCoefficient();
                updateOutputField();
                updateFlag = false;
            }
        }
    }

}

// Linear Function Plot

void XYPlot::inputSliderValueChange(int value) {

    Q_UNUSED(value)

    if(!ui->checkBox->isChecked())
        if(!updateFlag) {

            for(int i = 0; i< inputSlider.count(); i++) {
                if(!inputSlider.at(i)->isHidden() && inputSlider.at(i) == qobject_cast<QSlider*> (sender()))
                    inputEditField.at(i)->setValueLineEdit(QString().number((double)inputSlider.at(i)->value()/(double)SLIDER_PRECISION, 'f', 1));
            }

            if(linearPlotType == "Softmute Level" || linearPlotType == "Softmute AM Level") {
                if ((inputRoleIndex.find("start") != inputRoleIndex.end()) && (inputRoleIndex.find("stop") != inputRoleIndex.end())) {
                    if(inputSlider.at(inputRoleIndex["start"])->value() > inputSlider.at(inputRoleIndex["stop"])->value()) {

                        if(inputSlider.at(inputRoleIndex["start"]) == qobject_cast<QSlider*> (sender()))
                            m_XValues[X_VALUE_START] = (double)inputSlider.at(inputRoleIndex["start"])->value()/(double)SLIDER_PRECISION;
                        else if (inputSlider.at(inputRoleIndex["stop"]) == qobject_cast<QSlider*> (sender()))
                            m_XValues[X_VALUE_STOP] = (double)inputSlider.at(inputRoleIndex["stop"])->value()/(double)SLIDER_PRECISION;


                        if (inputRoleIndex.find("limit") != inputRoleIndex.end())
                        {
                            m_YValues[Y_VALUE_MIN] = inputEditField.at(inputRoleIndex["limit"])->getValueLineEdit().toInt();
                            //m_YValues[Y_VALUE_STOP] = inputEditField.at(inputRoleIndex["limit"])->getValueLineEdit().toInt();
                        }

// qDebug() << "linearPlotType : " << linearPlotType << endl << " X_VALUE_START :: " << m_XValues[X_VALUE_START] << " X_VALUE_STOP :: " << m_XValues[X_VALUE_STOP] << " Y_VALUE_MIN :: " << m_YValues[Y_VALUE_MIN]  << " inputField :: " << inputEditField.at(inputRoleIndex["limit"])->getValueLineEdit();
                        updateFlag = true;
                        plotCoordinates();
                        coefficeintValues = calculateCoefficient();
                        updateOutputField();
                        updateFlag = false;

                    } else {
                        adjustInputSliders();
                    }
                }
            }
            else if (linearPlotType == "Stereo Level" || linearPlotType == "Highcut Level" || linearPlotType == "AF" ||
                     linearPlotType == "Softmute" || linearPlotType == "Stereo" || linearPlotType == "Highcut" ||
                     linearPlotType == "Softmute General" || linearPlotType == "Stereo General" || linearPlotType == "Highcut General") {
                if ((inputRoleIndex.find("start") != inputRoleIndex.end()) && (inputRoleIndex.find("stop") != inputRoleIndex.end())) {

                    if(inputSlider.at(inputRoleIndex["start"])->value() < inputSlider.at(inputRoleIndex["stop"])->value()) {
                        if(inputSlider.at(inputRoleIndex["start"]) == qobject_cast<QSlider*> (sender()))
                            m_XValues[X_VALUE_START] = (double)inputSlider.at(inputRoleIndex["start"])->value()/(double)SLIDER_PRECISION;
                        else if (inputSlider.at(inputRoleIndex["stop"]) == qobject_cast<QSlider*> (sender()))
                            m_XValues[X_VALUE_STOP] = (double)inputSlider.at(inputRoleIndex["stop"])->value()/(double)SLIDER_PRECISION;


                        if (inputRoleIndex.find("limit") != inputRoleIndex.end())
                            m_YValues[Y_VALUE_MAX] = inputEditField.at(inputRoleIndex["limit"])->getValueLineEdit().toInt();

                        updateFlag = true;
                        plotCoordinates();
                        coefficeintValues = calculateCoefficient();
                        updateOutputField();

                        updateFlag = false;
                    } else {
                        adjustInputSliders();
                    }

                }
            }
        }

}

void XYPlot::on_checkBox_toggled(bool checked)
{
    if(!ui->checkBox->isHidden()) {
        if(checked) {
            if(outputEditField.count()) {

                // for(int i = 0; i < outputEditField.count(); i++ )
                // outputEditField.at(i)->setInputFieldDisable();

                emit disableElements();

                for(int i = 0; i < inputEditField.count(); i++ ) {
                    // inputEditField.at(i)->setInputFieldDisable();
                    inputSlider.at(i)->setDisabled(true);
                }
            }
        } else {
            if(outputEditField.count()) {

                // for(int i = 0; i < outputEditField.count(); i++ )
                // outputEditField.at(i)->setInputFieldEnable();

                emit enableElements();


                for(int i = 0; i < inputEditField.count(); i++ ) {
                    // inputEditField.at(i)->setInputFieldEnable();
                    //outputEditField.at(i)->saveValueLineEdit(outputEditField.at(i)->getValueLineEdit());
                    inputSlider.at(i)->setEnabled(true);
                }
            }
        }
    }
}

void XYPlot::plotSensor() {

    if(!ui->checkBox->isChecked()) {
        float xSourceValue = 0;
        float ySourceValue = 0;

        QString xSourceType;
        QString ySourceType;

        if (dStructure != 0) {
            xSourceValue = dStructure->value;
            xSourceType = dStructure->dataType;
        }

        if (dStructure2 != 0) {
            ySourceValue = dStructure2->value;
            ySourceType = dStructure2->dataType;
        }

        double pointX = 0;
        double pointY = 0;

        calculateSensorVertices(linearPlotType, xSourceValue, xSourceType, ySourceValue, ySourceType, pointX, pointY);

        ui->customPlot->graph(1)->clearData();

        ui->customPlot->graph(1)->addData(pointX,pointY);

        ui->customPlot->replot();
    }

}

void XYPlot::updateEvent() {
    if(outputEditField.count()) {

        QList<DataStructure *> dS;

        // Fetch Element Data from target
        for(int i = 0; i < outputEditField.count(); i++ )
            dS.append(outputEditField.at(i)->getStructure());

        tabControllerParent->initHMIReadReq(dS);

        // Update each element with new data
        for(int i = 0; i < outputEditField.count(); i++ ) {
            outputEditField.at(i)->updateEvent();
            outputEditFieldOldValue.replace(i, outputEditField.at(i)->getValueLineEdit());

        }

        for(int i = 0; i < outputEditField.count(); i++ )
            if(outputEditField.at(i)->getValueLineEdit() == outputEditField.at(i)->getDisabledValue()) {
                ui->checkBox->setChecked(true);
                break;
            }

    }

    updateFlag = true;
    if(!ui->checkBox->isChecked())
        drawPlot();
        //clearPlot();
    updateFlag = false;
}

/*********************************************************************************************************************************************************/
