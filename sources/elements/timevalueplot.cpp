/*!
 * \file     timevalueplot.cpp
 * \brief    Time-Value Plot Element Class
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
#include "headers/elements/timevalueplot.h"
#include "ui_timevalueplot.h"

// Data Processing
#include "headers/staticdependencies/datatypeconversion.h"
#include "headers/staticdependencies/valueconversion.h"

// Constructor
TimeValuePlot::TimeValuePlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeValuePlot)
{
    ui->setupUi(this);

    ui->customPlot->addGraph(); // line
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(QPen(QColor(rand() % 100, rand() % 100, rand() % 100))); // ui->customPlot->graph(ui->customPlot->graphCount()-1)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setAntialiasedFill(false);

    ui->customPlot->addGraph(); // dot
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->customPlot->xAxis->setTickLabels(false);        // ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setAutoTickStep(false);      // ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->xAxis->setTickLengthIn(0);
    ui->customPlot->xAxis->setSubTickLengthIn(0);
    ui->customPlot->xAxis2->setTickLengthIn(0);
    ui->customPlot->xAxis2->setSubTickLengthIn(0);      // ui->customPlot->xAxis->setTicks(false);
    ui->customPlot->xAxis->grid()->setVisible(false);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->yAxis->setTickLabels(false);
    ui->customPlot->yAxis->grid()->setVisible(false);

    yMax = 100;
    yMin = 0;
    maxValue = 32767;
    ui->scaleFactor->setChecked(false);
    ui->scaleFactor->hide();
    ui->scaleFactorSelect->setValue(1);
    ui->scaleFactorSelect->hide();
    autoScale = false;
    scaleFactor = 1;
    singleSourceMode = false;

    ui->customPlot->yAxis->setRange(yMin, yMax);

//    ui->customPlot->setBackground(Qt::lightGray);
//    ui->customPlot->axisRect()->setBackground(Qt::black);

    dStructure = 0;

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    ui->snapshotButton->setIcon(QPixmap("../SDRToolResources/icons/snapshot.png"));
    ui->snapshotButton->setIconSize(ui->snapshotButton->rect().size());

    ui->snapshotButton->hide();

}

// Destructor
TimeValuePlot::~TimeValuePlot()
{
    delete ui;
}


/************************ General Methods specific to this class. ************************/

void TimeValuePlot::realtimeDataPlot() {

    if (dStructure != 0) {

        if (true) {

            // calculate two new data points:
            double key = QDateTime::currentMSecsSinceEpoch()/15.0; // double key = QDateTime::currentMSecsSinceEpoch()/10.0; //   currentDateTime().toMSecsSinceEpoch()/10.0;

            int val = dStructure->value;

            if(!minSourceValue.isEmpty() && val < minSourceValue.toInt())
                val = minSourceValue.toInt();

            double value0 = convertCoeffForSensor_TimePlot(val, dStructure->dataType, maxValue);

            //            if(outputType == "hex") {
            //                setValueLineEdit(QString().setNum(dStructure->value, 16));
            //            } else if(outputType == "float") {
            //                setValueLineEdit(QString().setNum(single2float((int)value)));
            //            }

            if(autoScale)
                if(ui->scaleFactor->isChecked())
                    value0 = value0 * scaleFactor;

            // add data to line:
            ui->customPlot->graph(0)->addData(key, value0);
            // set data of dot:
            ui->customPlot->graph(1)->clearData();
            ui->customPlot->graph(1)->addData(key, value0);
            // remove data of line that's outside visible range:
            ui->customPlot->graph(0)->removeDataBefore(key-2000);
            ui->customPlot->graph(1)->removeDataBefore(key-2000);
            // rescale value (vertical) axis to fit the current data:
            ui->customPlot->graph(0)->rescaleValueAxis();
            ui->customPlot->graph(1)->rescaleValueAxis(true);

            // make key axis range scroll with the data (at a constant range size of 8):
            ui->customPlot->xAxis->setRange(key+2.25, 2000, Qt::AlignRight);
            ui->customPlot->yAxis->setRange(yMin, yMax);
            ui->customPlot->replot();
        }
    }

    if(dSource.count()) {

        if (true) {

            // calculate two new data points:
            double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/15.0; // double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/20.0;
//            int chagneScale = 0;
//            float largestValue = 0;

            for(int i = 0; i < dSource.count() ; i++) {

                if(singleSourceMode) {
                    if(dSourceModes.at(i).first() != "ALL" && dSourceModes.at(i).indexOf(selectedMode) == -1)
                        continue;
                }

                double value0 = 0;

                int val = dSource.at(i)->value;

                if(dSourceMinValue.at(i) != "DEFAULT" && val < dSourceMinValue.at(i).toInt()) {
                    val = dSourceMinValue.at(i).toInt();
                }

                if(dSourceHalfFormatInterpretation.at(i) != "" && dSourceHalfFormatInterpretation.at(i) != "DEFAULT")
                    value0 = convertCoeffForTextField_TimePlot(val, dSourceHalfFormatInterpretation.at(i));
                else
                    value0 = convertCoeffForSensor_TimePlot(val, dSource.at(i)->dataType);

                //            if(outputType == "hex") {
                //                setValueLineEdit(QString().setNum(dStructure->value, 16));

                if(dSourceOutputType.at(i) == "float") {
                    value0 = single2float((int)dSource.at(i)->value);
                    //value0 = (int)(value0 * 32768);  //removed by CL 19/09/2016
                } else if(dSourceOutputType.at(i) == "scaled") {
                    value0 = (int)(value0 * 32768);
                } else if ( dSourceOutputType.at(i) ==  "angle" ) {
                    value0 = single2float((int)dSource.at(i)->value);
                    value0 = fmod(value0 + 0.5,1);
                    if (value0 < 0){
                        value0 += 1;
                    }
                    value0 -= 0.5;
            }

                dSourceLabel.at(dSourceIdentifier.indexOf(dSource.at(i)->name))->setText(dSource.at(i)->name + " = " + QString().setNum(value0));
                if(autoScale && ui->scaleFactor->isChecked()) {
                    ui->scaleFactorSelect->show();
                    value0 = value0 * scaleFactor;
                } else {
                    ui->scaleFactorSelect->hide();
                }

                // add data to lines:
                ui->customPlot->graph(2 + i)->addData(key, value0);
                // remove data of lines that's outside visible range:
                ui->customPlot->graph(2 + i)->removeDataBefore(key-2000);
                // rescale value (vertical) axis to fit the current data:
                ui->customPlot->graph(2 + i)->rescaleValueAxis();

//                if(value0 > largestValue)
//                    largestValue = value0;

//                if(yMax*0.8 > largestValue )
//                    chagneScale = (yMax*0.95) - largestValue;
//                else if (yMax < largestValue )
//                    chagneScale = 0;

//                dSourceLabel.at(dSourceIdentifier.indexOf(dSource.at(i)->name))->setText(dSource.at(i)->name + " = " + QString().setNum(value0));
            }

            // make key axis range scroll with the data (at a constant range size of 8):
            ui->customPlot->xAxis->setRange(key+2.25, 2000, Qt::AlignRight);
            //        ui->customPlot->yAxis->setRange(yMin, yMax);
            if(autoScale && ui->scaleFactor->isChecked()) {
                ui->customPlot->yAxis->setRange(yMin, yMax);                                                        // PROBABILITY OF CLIPPING
            }
            else
                ui->customPlot->yAxis->setRange(yMin, yMax);                                                        // PROBABILITY OF CLIPPING
            ui->customPlot->replot();
        }

    }

}

void TimeValuePlot::savePlot() {
    QString outputDir = "../PERF3";
             QString fileName = "graph.jpg" ;
             QFile file(outputDir+"/"+fileName);

             if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
                  {
                      QMessageBox::warning(0,"Could not create Project File",
                                                 QObject::tr( "\n Could not create Project File on disk"));


                  }

             ui->customPlot->saveJpg( outputDir+"/"+fileName,  0, 0, 1.0, -1  );

}

void TimeValuePlot::setDataSourceIdentifier(QString label) {
    dSourceIdentifier.append(label);
    setDataSourceLabel(label);
}

QString TimeValuePlot::getDataSourceIdentifier(int index) {
    if(dSourceIdentifier.count() > index)
        return dSourceIdentifier.at(index);
    return "";
}

int TimeValuePlot::getDataSourceIdentifierCount() {
    return dSourceIdentifier.count();
}

void TimeValuePlot::setDataSourceLabel(QString label) {
    dSourceLabel.append(new QLabel(label));
    ui->plotNameArea->addWidget(dSourceLabel.last());
}

void TimeValuePlot::setDataSourceOutputType(QString label) {
    dSourceOutputType.append(label);
}

void TimeValuePlot::setDataSourceHalfFormatInterpretation(QString label) {
    dSourceHalfFormatInterpretation.append(label);
}

void TimeValuePlot::setDataSourceMinValue(const QString &value)
{
    dSourceMinValue.append(value);
}

void TimeValuePlot::setDataSourceModes(QString modes) {

    QStringList lists = modes.split("/");       // Break line at each /

    if(lists.first() == "DEFAULT") {
        lists.clear();
        lists << "ALL";
    }

    dSourceModes.append(lists);
}

bool TimeValuePlot::setDataSource(DataStructure * structure) {

    if (structure != 0) {
        dSource.append(structure);

        ui->customPlot->addGraph();
        QColor color = QColor(rand() % 100, rand() % 100, rand() % 100);
        ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(QPen(color));
//        ui->customPlot->graph(ui->customPlot->graphCount()-1)->setBrush(QBrush(QColor(240, 255, 200)));
        ui->customPlot->graph(ui->customPlot->graphCount()-1)->setAntialiasedFill(false);
        dSourceLabel.at(dSourceIdentifier.indexOf(structure->name))->setStyleSheet("QLabel { color : " + color.name() + "; }");

        return true;
    }

    return false;
}

void TimeValuePlot::resetDataSource() {

    for (int i = 0; i < dSource.count(); i++) {
        ui->customPlot->removeGraph(ui->customPlot->graphCount()-1);
    }

    dSource.clear();

    for (int i = 0; i < dSourceLabel.count(); i++) {
        dSourceLabel.at(i)->setStyleSheet("QLabel { color : " + QColor(0, 0, 0).name() + "; }");
    }

}

/************************ Initializer Methods used by XML parser. ************************/

void TimeValuePlot::setXLabel(QString label) {
    ui->customPlot->xAxis->setLabel(label);
}

void TimeValuePlot::setYLabel(QString label) {
    ui->customPlot->yAxis->setLabel(label);
}

void TimeValuePlot::setGraphLabel(QString label) {
    ui->graphLabel->setText(label);
}

void TimeValuePlot::setGraphHeight(int height) {


    this->setMinimumSize(this->width(),height);
    this->setMaximumSize(this->width(), height);
    this->resize(this->width(), height);
    ui->customPlot->setMinimumSize(this->width(), this->height()-50);
    ui->customPlot->setMaximumSize(this->width(), this->height()-50);
    ui->customPlot->resize(ui->customPlot->width(), this->height()-50);

    QTextStream out(stdout);
    out << "tvp Resize : " << this->width() << "x" << this->height()<< endl ;
    out << "tvp Resize graph : " << ui->customPlot->width() << "x" << ui->customPlot->height()<< endl ;
}

void TimeValuePlot::setGraphWidth(int width) {


    this->setMinimumSize(width, this->height());
    this->setMaximumSize(width, this->height());
    this->resize(width, this->height());
    ui->customPlot->setMinimumSize(this->width(), this->height()-50);
    ui->customPlot->setMaximumSize(this->width(), this->height()-50);
    ui->customPlot->resize(this->width(), this->height()-50);

    QTextStream out(stdout);
    out << "tvp Resize : " << this->width() << "x" << this->height()<< endl ;
    out << "tvp Resize graph : " << ui->customPlot->width() << "x" << ui->customPlot->height()<< endl ;
}

void TimeValuePlot::setYMax(int value) {
    yMax = value;
}

void TimeValuePlot::setYMin(int value) {
    yMin = value;
}

void TimeValuePlot::setMaxValue(int value) {
    maxValue = value;
}

void TimeValuePlot::setMinValue(const QString &value) {
    minSourceValue = value;
}

void TimeValuePlot::setScaleFactor(int factor) {
    autoScale = true;
    ui->scaleFactor->show();
    scaleFactor = factor;
    ui->scaleFactorSelect->setValue(factor);
}

void TimeValuePlot::setOutputType(QString value) {
    outputType = value;
}

void TimeValuePlot::setHalfFormatInterpretation(QString value) {
    halfFormatInterpretation = value;
}

void TimeValuePlot::setVisiblity(QString value) {
    if(value == "N" || value == "n")
        hideWidget();
}

// Set the indentification name of BarGraphH
void TimeValuePlot::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of BarGraphH
QString TimeValuePlot::getNameIdentifier() {
    return nameIdentifier;
}

bool TimeValuePlot::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}

DataStructure * TimeValuePlot::getStructure() {
    return dStructure;
}

void TimeValuePlot::setIndex(int index) {
    this->index = index;
}

int TimeValuePlot::getIndex() {
    return index;
}

/************************ Slots are special methods which execute in response to the connected signal. ************************/

void TimeValuePlot::on_scaleFactorSelect_valueChanged(int arg1)
{
    scaleFactor = arg1;
}

void TimeValuePlot::on_snapshotButton_clicked()
{
    savePlot();
}

void TimeValuePlot::disable()
{
    //this->hide();
    this->setDisabled(true);
}

void TimeValuePlot::enable()
{
    //this->hide();
    this->setEnabled(true);
}

void TimeValuePlot::hideWidget()
{
    this->hide();
}

void TimeValuePlot::showWidget()
{
    this->show();
}

void TimeValuePlot::selectSourceMode(QString mode) {

    for(int i = 0; i < dSourceModes.count(); i++) {
        if(dSourceModes.at(i).indexOf(mode) != -1) {
            singleSourceMode = true;
            selectedMode = mode;
            break;
        }
        else singleSourceMode = false;
    }
}

void TimeValuePlot::updateEvent() {
    realtimeDataPlot();
}

/*********************************************************************************************************************************************************/
