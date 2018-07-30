/*!
 * \file     agccontrol.cpp
 * \brief    AGC CONTROL Element Class
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
#include "headers/elements/agccontrol.h"
#include "ui_agccontrol.h"

#include <QDebug>
//#include "headers/agcparameterlist.h"
//#include <QLineEdit>
//#include <QDoubleSpinBox>

#include "math.h"
#include "headers/core/sharedflags.h"
#include "headers/staticdependencies/datatypeconversion.h"

// Parent
#include "headers/core/tabcontroller.h"

#include "headers/core/errorlogger.h"

// Constructor
AGCControl::AGCControl(TabController *parent) :
    QWidget(parent),
    ui(new Ui::AGCControl)
{
    ui->setupUi(this);

    selectedMode = -1;
    updateFlag = false;
    tabControllerParent = (TabController*) parent;

    ui->FM_BE->setDisabled(true);
    ui->FM_NB->setDisabled(true);
    ui->AM_BE->setDisabled(true);
    ui->AM_NB->setDisabled(true);
    ui->DAB_BE->setDisabled(true);
    ui->DAB_NB->setDisabled(true);

    initParameterConfig();
    initCoeffConfig();

    connect(ui->FM_BE, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    connect(ui->FM_NB, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    connect(ui->AM_BE, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    connect(ui->AM_NB, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    connect(ui->DAB_BE, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    connect(ui->DAB_NB, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
}

// Destructor
AGCControl::~AGCControl()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

void AGCControl::initParameterConfig() {

    QStringList str;

    str << ui->AGC_TH->objectName();
    str << ui->AGC_TARGET_VALUE->objectName();
    str << ui->AGC_DEADBAND_RATIO->objectName();
    str << ui->AGC_FAST_TH_ATT->objectName();
    str << ui->AGC_FAST_TH_REL->objectName();
    str << ui->AGC_FAST_TAU_ATT->objectName();
    str << ui->AGC_FAST_TAU_REL->objectName();
    str << ui->AGC_SLOW_TAU_ATT->objectName();
    str << ui->AGC_SLOW_TAU_REL->objectName();
    str << ui->AGC_GAIN_FAST_SLOPE_ATT->objectName();
    str << ui->AGC_GAIN_FAST_SLOPE_REL->objectName();
    str << ui->AGC_GAIN_SLOW_SLOPE_ATT->objectName();
    str << ui->AGC_GAIN_SLOW_SLOPE_REL->objectName();

    for(int i = 0; i < str.count(); i++) {
        setDataSourceIdentifier(str.at(i));
    }
}

void AGCControl::initCoeffConfig() {

    QStringList str;

    str << ui->_fst_ths->objectName();
    str << ui->_deadBand->objectName();
    str << ui->maxGain0->objectName();
    str << ui->maxGain0_1->objectName();
    str << ui->_b0sSlow->objectName();
    str << ui->_b0dSlow->objectName();
    str << ui->_b0sFast->objectName();
    str << ui->_b0dFast->objectName();
    str << ui->_fastSlopeAttack->objectName();
    str << ui->_fastSlopeRel->objectName();
    str << ui->_slowSlopeAttack->objectName();
    str << ui->_slowSlopeRel->objectName();
    str << ui->_fastThrAttack->objectName();
    str << ui->_fastThrRelease->objectName();
    str << ui->MODESELECT->objectName();

    for(int i = 0; i < str.count(); i++) {
        setDataSourceIdentifier(str.at(i));
    }
}

void AGCControl::selectBand(QString band) {

    if (band == "FM_MODE")
    {
        ui->FM_BE->setEnabled(true);
        ui->FM_NB->setEnabled(true);
        ui->AM_BE->setDisabled(true);
        ui->AM_NB->setDisabled(true);
        ui->DAB_BE->setDisabled(true);
        ui->DAB_NB->setDisabled(true);
    }
    else if (band == "AM_MODE")
    {
        ui->FM_BE->setDisabled(true);
        ui->FM_NB->setDisabled(true);
        ui->AM_BE->setEnabled(true);
        ui->AM_NB->setEnabled(true);
        ui->DAB_BE->setDisabled(true);
        ui->DAB_NB->setDisabled(true);
    }
    else if (band == "DAB_MODE")
    {
        ui->FM_BE->setDisabled(true);
        ui->FM_NB->setDisabled(true);
        ui->AM_BE->setDisabled(true);
        ui->AM_NB->setDisabled(true);
        ui->DAB_BE->setEnabled(true);
        ui->DAB_NB->setEnabled(true);
    }

    printDebugString("band " + band);

}

void AGCControl::initParameterRange(QString band) {

    if (band == "FM_MODE")
    {

    }
    else if (band == "AM_MODE")
    {

    }
    else if (band == "DAB_MODE")
    {

    }

}

void AGCControl::setDataSourceIdentifier(QString label) {
    dSourceIdentifier.append(label);
}

QString AGCControl::getDataSourceIdentifier(int index) {
    if(dSourceIdentifier.count() > index)
        return dSourceIdentifier.at(index);
    return "";
}

int AGCControl::getDataSourceIdentifierCount() {
    return dSourceIdentifier.count();
}

bool AGCControl::setDataSource(DataStructure * structure) {

    if (structure != 0) {
        dSource.append(structure);
        return true;
    }

    return false;
}

QList<DataStructure *> AGCControl::getDataSource() {

    return dSource;

}

void AGCControl::resetDataSource() {

    dSource.clear();

}

void AGCControl::agcNbGainCalculate() {

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


    /*********************************************************************************************************************/

    // Target Value on BB interface
    // =========================================================================
    Temp.Target =  pow(10,(ui->AGC_TARGET_VALUE->value() / 20));
    double _fst_ths = Temp.Target;

    // Deadband
    // =========================================================================
    double _deadBand = Temp.Target* ui->AGC_DEADBAND_RATIO->text().toFloat();

    // Intervention threshold --> Max gain
    // =========================================================================
    Temp.Kfm = pow(10, (double)KBAND/20);
    Temp.Beth = pow(10, (double)(ui->AGC_TH->value()/20));
    Temp.MaxGain = (pow(10,(ui->AGC_TARGET_VALUE->value() / 20)))*(pow(10,(KBAND/20)))/(pow(10,(ui->AGC_TH->value()/20)));
    double maxGain0 = int(floor(Temp.MaxGain * pow(2,(double)8)));
    double maxGain0_1 = int(((Temp.MaxGain * pow(2,(double)8))-floor(Temp.MaxGain * pow(2,(double)8))) * pow(2,(double)24));

    // Slow time constants of the rectifier after the detector
    // =========================================================================
    double _b0sSlow = ((2 / (1 + 2 * ui->AGC_SLOW_TAU_ATT->text().toFloat() * FS )) + (2 / (1 + 2 * ui->AGC_SLOW_TAU_REL->text().toFloat() * FS ))) / 2;
    double _b0dSlow = ((2 / (1 + 2 * ui->AGC_SLOW_TAU_ATT->text().toFloat() * FS )) - (2 / (1 + 2 * ui->AGC_SLOW_TAU_REL->text().toFloat() * FS ))) / 2;

    // Fast time constants of the rectifier after the detector
    // =========================================================================
    double _b0sFast = ((2 / (1 + 2 * ui->AGC_FAST_TAU_ATT->text().toFloat() * FS )) + (2 / (1 + 2 * ui->AGC_FAST_TAU_REL->text().toFloat() * FS ))) / 2;
    double _b0dFast = ((2 / (1 + 2 * ui->AGC_FAST_TAU_ATT->text().toFloat() * FS )) - (2 / (1 + 2 * ui->AGC_FAST_TAU_REL->text().toFloat() * FS ))) / 2;

    // Gain fast attack/release slope
    // =========================================================================
    double _fastSlopeAttack =  1 - pow(10,( ui->AGC_GAIN_FAST_SLOPE_ATT->text().toFloat() / (FSSLOPE*20)));
    double _fastSlopeRel =  1 - pow(10,( ui->AGC_GAIN_FAST_SLOPE_REL->text().toFloat() / (FSSLOPE*20)));

    // Gain slow attack/release slope
    // =========================================================================
    double _slowSlopeAttack =  1 - pow(10,( ui->AGC_GAIN_SLOW_SLOPE_ATT->text().toFloat() / (FSSLOPE*20)));
    double _slowSlopeRel =  1 - pow(10,( ui->AGC_GAIN_SLOW_SLOPE_REL->text().toFloat() / (FSSLOPE*20)));

    // AGC NB fast threshold attack/release in respect to full scale
    // =========================================================================
    double _fastThrAttack = pow(10,(ui->AGC_FAST_TH_ATT->text().toFloat()/20)) - Temp.Target;
    double _fastThrRelease = pow(10,(ui->AGC_FAST_TH_REL->text().toFloat()/20)) - Temp.Target;

    /**/


    ui->_fst_ths->setText(QString().setNum(double2fixed24(_fst_ths), 16));
    ui->_deadBand->setText(QString().setNum(double2fixed24(_deadBand), 16));
    ui->maxGain0->setText(QString().setNum((int)(maxGain0), 16));
    ui->maxGain0_1->setText(QString().setNum((int)(maxGain0_1), 16));
    ui->_b0sSlow->setText(QString().setNum(double2fixed24(_b0sSlow), 16));
    ui->_b0dSlow->setText(QString().setNum(double2fixed24(_b0dSlow), 16));
    ui->_b0sFast->setText(QString().setNum(double2fixed24(_b0sFast), 16));
    ui->_b0dFast->setText(QString().setNum(double2fixed24(_b0dFast), 16));
    ui->_fastSlopeAttack->setText(QString().setNum(double2fixed24(_fastSlopeAttack), 16));
    ui->_fastSlopeRel->setText(QString().setNum(double2fixed24(_fastSlopeRel), 16));
    ui->_slowSlopeAttack->setText(QString().setNum(double2fixed24(_slowSlopeAttack), 16));
    ui->_slowSlopeRel->setText(QString().setNum(double2fixed24(_slowSlopeRel), 16));
    ui->_fastThrAttack->setText(QString().setNum(double2fixed24(_fastThrAttack), 16));
    ui->_fastThrRelease->setText(QString().setNum(double2fixed24(_fastThrRelease), 16));


}

int AGCControl::double2fixed24(double val_double)
{
    // convert from double to 24 bit
    if (val_double >= 0)
    {
        return (int)(val_double*pow(2,(double)23));
    }
    else
    {
        return (int)(pow(2,(double)24) + val_double*pow(2,(double)23));
    }
}

/************************ Initializer Methods used by XML parser. ************************/

void AGCControl::setLabelName(const QString &value)
{
    Q_UNUSED(value)
}

// Set the indentification name of AGCControl
void AGCControl::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of AGCControl
QString AGCControl::getNameIdentifier() {
    return nameIdentifier;
}

bool AGCControl::setStructure(DataStructure * structure) {
    // dStructure = 0;
    // dStructure = structure;
    // if (dStructure != 0)
    // return true;
    // return false;

    Q_UNUSED(structure)
    return false;
}

DataStructure * AGCControl::getStructure() {
    // return dStructure;
    return 0;
}

void AGCControl::setIndex(int index) {
    this->index = index;
}

int AGCControl::getIndex() {
    return index;
}

void AGCControl::setParameterValue(QString param, float cfgValue) {

    if(param == ui->AGC_TH->objectName())
        ui->AGC_TH->setValue(cfgValue);
    else if(param == ui->AGC_TARGET_VALUE->objectName())
        ui->AGC_TARGET_VALUE->setValue(cfgValue);
    else if(param == ui->AGC_DEADBAND_RATIO->objectName())
        ui->AGC_DEADBAND_RATIO->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_FAST_TH_ATT->objectName())
        ui->AGC_FAST_TH_ATT->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_FAST_TH_REL->objectName())
        ui->AGC_FAST_TH_REL->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_FAST_TAU_ATT->objectName())
        ui->AGC_FAST_TAU_ATT->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_FAST_TAU_REL->objectName())
        ui->AGC_FAST_TAU_REL->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_SLOW_TAU_ATT->objectName())
        ui->AGC_SLOW_TAU_ATT->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_SLOW_TAU_REL->objectName())
        ui->AGC_SLOW_TAU_REL->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_GAIN_FAST_SLOPE_ATT->objectName())
        ui->AGC_GAIN_FAST_SLOPE_ATT->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_GAIN_FAST_SLOPE_REL->objectName())
        ui->AGC_GAIN_FAST_SLOPE_REL->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_GAIN_SLOW_SLOPE_ATT->objectName())
        ui->AGC_GAIN_SLOW_SLOPE_ATT->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_GAIN_SLOW_SLOPE_REL->objectName())
        ui->AGC_GAIN_SLOW_SLOPE_REL->setText(QString().setNum(cfgValue));

}

void AGCControl::updateParameterStructureValue() {

    // double val;
    QString param;
    for(int i = 0; i < dSource.count(); i++) {
        if(dSource.at(i)->address == "0") {

            param.clear();
            param = dSource.at(i)->name;

            if(param == ui->AGC_TH->objectName())
                dSource.at(i)->cfgValue = ui->AGC_TH->value();
            else if(param == ui->AGC_TARGET_VALUE->objectName())
                dSource.at(i)->cfgValue = ui->AGC_TARGET_VALUE->value();
            else if(param == ui->AGC_DEADBAND_RATIO->objectName())
                dSource.at(i)->cfgValue = ui->AGC_DEADBAND_RATIO->text().toFloat();
            else if(param == ui->AGC_FAST_TH_ATT->objectName())
                dSource.at(i)->cfgValue = ui->AGC_FAST_TH_ATT->text().toFloat();
            else if(param == ui->AGC_FAST_TH_REL->objectName())
                dSource.at(i)->cfgValue = ui->AGC_FAST_TH_REL->text().toFloat();
            else if(param == ui->AGC_FAST_TAU_ATT->objectName())
                dSource.at(i)->cfgValue = ui->AGC_FAST_TAU_ATT->text().toFloat();
            else if(param == ui->AGC_FAST_TAU_REL->objectName())
                dSource.at(i)->cfgValue = ui->AGC_FAST_TAU_REL->text().toFloat();
            else if(param == ui->AGC_SLOW_TAU_ATT->objectName())
                dSource.at(i)->cfgValue = ui->AGC_SLOW_TAU_ATT->text().toFloat();
            else if(param == ui->AGC_SLOW_TAU_REL->objectName())
                dSource.at(i)->cfgValue = ui->AGC_SLOW_TAU_REL->text().toFloat();
            else if(param == ui->AGC_GAIN_FAST_SLOPE_ATT->objectName())
                dSource.at(i)->cfgValue = ui->AGC_GAIN_FAST_SLOPE_ATT->text().toFloat();
            else if(param == ui->AGC_GAIN_FAST_SLOPE_REL->objectName())
                dSource.at(i)->cfgValue = ui->AGC_GAIN_FAST_SLOPE_REL->text().toFloat();
            else if(param == ui->AGC_GAIN_SLOW_SLOPE_ATT->objectName())
                dSource.at(i)->cfgValue = ui->AGC_GAIN_SLOW_SLOPE_ATT->text().toFloat();
            else if(param == ui->AGC_GAIN_SLOW_SLOPE_REL->objectName())
                dSource.at(i)->cfgValue = ui->AGC_GAIN_SLOW_SLOPE_REL->text().toFloat();
        }
    }

}

void AGCControl::setCoeffValue(QString coeff, int value) {

    if(coeff == ui->_fst_ths->objectName())
        ui->_fst_ths->setText(QString().setNum(value, 16));
    else if(coeff == ui->_deadBand->objectName())
        ui->_deadBand->setText(QString().setNum(value, 16));
    else if(coeff == ui->maxGain0->objectName())
        ui->maxGain0->setText(QString().setNum(value, 16));
    else if(coeff == ui->maxGain0_1->objectName())
        ui->maxGain0_1->setText(QString().setNum(value, 16));
    else if(coeff == ui->_b0sSlow->objectName())
        ui->_b0sSlow->setText(QString().setNum(value, 16));
    else if(coeff == ui->_b0dSlow->objectName())
        ui->_b0dSlow->setText(QString().setNum(value, 16));
    else if(coeff == ui->_b0sFast->objectName())
        ui->_b0sFast->setText(QString().setNum(value, 16));
    else if(coeff == ui->_b0dFast->objectName())
        ui->_b0dFast->setText(QString().setNum(value, 16));
    else if(coeff == ui->_fastSlopeAttack->objectName())
        ui->_fastSlopeAttack->setText(QString().setNum(value, 16));
    else if(coeff == ui->_fastSlopeRel->objectName())
        ui->_fastSlopeRel->setText(QString().setNum(value, 16));
    else if(coeff == ui->_slowSlopeAttack->objectName())
        ui->_slowSlopeAttack->setText(QString().setNum(value, 16));
    else if(coeff == ui->_slowSlopeRel->objectName())
        ui->_slowSlopeRel->setText(QString().setNum(value, 16));
    else if(coeff == ui->_fastThrAttack->objectName())
        ui->_fastThrAttack->setText(QString().setNum(value, 16));
    else if(coeff == ui->_fastThrRelease->objectName())
        ui->_fastThrRelease->setText(QString().setNum(value, 16));

    else if(coeff == "MODESELECT")
        selectedMode = (int)value;

}

QString AGCControl::getCoeffValue(QString coeff) {

    QString str;
    QByteArray bA;

    if(coeff == ui->_fst_ths->objectName())
        bA = ui->_fst_ths->text().toLatin1();
    else if(coeff == ui->_deadBand->objectName())
        bA = ui->_deadBand->text().toLatin1();
    else if(coeff == ui->maxGain0->objectName())
        bA = ui->maxGain0->text().toLatin1();
    else if(coeff == ui->maxGain0_1->objectName())
        bA = ui->maxGain0_1->text().toLatin1();
    else if(coeff == ui->_b0sSlow->objectName())
        bA = ui->_b0sSlow->text().toLatin1();
    else if(coeff == ui->_b0dSlow->objectName())
        bA = ui->_b0dSlow->text().toLatin1();
    else if(coeff == ui->_b0sFast->objectName())
        bA = ui->_b0sFast->text().toLatin1();
    else if(coeff == ui->_b0dFast->objectName())
        bA = ui->_b0dFast->text().toLatin1();
    else if(coeff == ui->_fastSlopeAttack->objectName())
        bA = ui->_fastSlopeAttack->text().toLatin1();
    else if(coeff == ui->_fastSlopeRel->objectName())
        bA = ui->_fastSlopeRel->text().toLatin1();
    else if(coeff == ui->_slowSlopeAttack->objectName())
        bA = ui->_slowSlopeAttack->text().toLatin1();
    else if(coeff == ui->_slowSlopeRel->objectName())
        bA = ui->_slowSlopeRel->text().toLatin1();
    else if(coeff == ui->_fastThrAttack->objectName())
        bA = ui->_fastThrAttack->text().toLatin1();
    else if(coeff == ui->_fastThrRelease->objectName())
        bA = ui->_fastThrRelease->text().toLatin1();

    else if(coeff == "MODESELECT") {
        str.setNum(selectedMode);
        return str;
    }

    if(!bA.isEmpty()) {
        char * cStr = bA.data();
        str = QString().setNum(atohex(cStr));
    }

    return str;

}

/************************ Methods used in connected state with other elements. ************************/


/************************ Slots are special methods which execute in response to the connected signal. ************************/

void AGCControl::selectMode() {

    if(ui->FM_NB->isChecked())
        selectedMode = 0;
    else if(ui->FM_BE->isChecked())
        selectedMode = 1;
    else if(ui->AM_NB->isChecked())
        selectedMode = 2;
    else if(ui->AM_BE->isChecked())
        selectedMode = 3;
    else if(ui->DAB_NB->isChecked())
        selectedMode = 4;
    else if(ui->DAB_BE->isChecked())
        selectedMode = 5;


    printDebugString( "mode " + selectedMode);


    if(!updateFlag && selectedMode != -1) {
        QList<DataStructure *> dS;
        QList<QString> value;
        //        initHMI(selectedMode);
        printDebugString( "HMI agc");

        for(int i = 0; i < dSource.count(); i++) {
            if(dSource.at(i)->address != "0") {
                if(dSource.at(i)->name == "MODESELECT") {
                    dS.append(dSource.at(i));
                    value.append(QString().setNum(selectedMode));
                    break;
                }
            }
        }

        if(!dS.isEmpty()) {
            tabControllerParent->initHMIWriteReq(dS, value);
        }



    }
    else
        updateFlag = false;
}

void AGCControl::updateEvent() {

    // if (dStructure != 0) {
    // } else
    if(dSource.count()) {

        // selectedBand(); //  call by xml parser

        for(int i = 0; i < dSource.count(); i++) {
            if(dSource.at(i)->address != "0") {
                setCoeffValue(dSource.at(i)->name, dSource.at(i)->value);
            }
        }

        //        updateFlag = true;

        if (radioMode == FM_MODE) {
            switch (selectedMode) {
            case 0:
                ui->FM_NB->setChecked(true);
                break;
            case 1:
                ui->FM_BE->setChecked(true);
                break;
            default:
                ui->FM_NB->setChecked(true);
                break;
            }
        }
        else if (radioMode == AM_MODE) {
            switch (selectedMode) {
            case 2:
                ui->AM_NB->setChecked(true);
                break;
            case 3:
                ui->AM_BE->setChecked(true);
                break;
            default:
                ui->AM_NB->setChecked(true);
                break;
            }

        }
        else if (radioMode == DAB_MODE) {
            switch (selectedMode) {
            case 4:
                ui->DAB_NB->setChecked(true);
                break;
            case 5:
                ui->DAB_BE->setChecked(true);
                break;
            default:
                ui->DAB_NB->setChecked(true);
                break;
            }
        }

        selectMode();

        //        updateFlag = false;

        for(int i = 0; i < dSource.count(); i++) {
            if(dSource.at(i)->address == "0") {
                setParameterValue(dSource.at(i)->name, dSource.at(i)->cfgValue);
            }
        }

    }
}


void AGCControl::on_calculatePushButton_clicked()
{
    agcNbGainCalculate();

    updateParameterStructureValue();
}

void AGCControl::on_writePushButton_clicked()
{

    QList<DataStructure *> dS;
    QList<QString> values;
    QString val;

    for(int i = 0; i < dSource.count(); i++) {
        if(dSource.at(i)->address != "0") {
            val.clear();
            val = getCoeffValue(dSource.at(i)->name);
            if(!val.isEmpty()) {
                dS.append(dSource.at(i));
                values.append(val);
            }
        }
    }

    if(!dS.isEmpty()) {
        tabControllerParent->initHMIWriteReq(dS, values);
    }
}


/*********************************************************************************************************************************************************/
/*

void AGCControl::createParameterForm() {

    QStringList str;

    str << STRINGIFY(AGC_TH);
    str << STRINGIFY(AGC_TARGET_VALUE);

    for(int i = 0; i < str.count(); i++) {
        addSpinBox(str.at(i));
        setDataSourceIdentifier(str.at(i));
    }

    str.clear();

    str << STRINGIFY(AGC_DEADBAND_RATIO);
    str << STRINGIFY(AGC_FAST_TH_ATT);
    str << STRINGIFY(AGC_FAST_TH_REL);
    str << STRINGIFY(AGC_FAST_TAU_ATT);
    str << STRINGIFY(AGC_FAST_TAU_REL);
    str << STRINGIFY(AGC_SLOW_TAU_ATT);
    str << STRINGIFY(AGC_SLOW_TAU_REL);
    str << STRINGIFY(AGC_GAIN_FAST_SLOPE_ATT);
    str << STRINGIFY(AGC_GAIN_FAST_SLOPE_REL);
    str << STRINGIFY(AGC_GAIN_SLOW_SLOPE_ATT);
    str << STRINGIFY(AGC_GAIN_SLOW_SLOPE_REL);

    for(int i = 0; i < str.count(); i++) {
        addInputField(str.at(i));
        setDataSourceIdentifier(str.at(i));
    }
}


void AGCControl::addSpinBox(QString labelName) {

    parameterListSpinBox.append(labelName);
    QDoubleSpinBox * sb = new QDoubleSpinBox();
    ui->formLayout->addRow(labelName,sb);
}

void AGCControl::addInputField(QString labelName) {

    parameterListInputField.append(labelName);
    QLineEdit * le = new QLineEdit();
    ui->formLayout->addRow(labelName,le);
}


QList<double> AGCControl::getCoeffParameters(QString coeff) {

    QList<double> parameters;

    if(coeff == "_fst_ths") {

    }
    else if(coeff == "_deadBand") {

    }
    else if(coeff == "maxGain0") {

    }
    else if(coeff == "maxGain0_1") {

    }
    else if(coeff == "_b0sSlow") {

    }
    else if(coeff == "_b0dSlow") {

    }
    else if(coeff == "_b0sFast") {

    }
    else if(coeff == "_b0dFast") {

    }
    else if(coeff == "_fastSlopeAttack") {

    }
    else if(coeff == "_fastSlopeRel") {

    }
    else if(coeff == "_slowSlopeAttack") {

    }
    else if(coeff == "_slowSlopeRel") {

    }
    else if(coeff == "_fastThrAttack") {

    }
    else if(coeff == "_fastThrRelease") {

    }

    return 0;
}

float AGCControl::getParameterValue(QString param) {
    int leIndex = parameterListInputField.indexOf(param);
    int sbIndex = parameterListInputField.indexOf(param);

    if(leIndex != -1) {
        QLayoutItem * item = ui->formLayout->itemAt(index, QFormLayout::FieldRole);
        if(item != 0) {
            QLineEdit * le =  qobject_cast<QLineEdit *> (item->widget());
            if(le != 0) {
                return le->text().toFloat();
            }
        }
    } else if(sbIndex != -1) {
        QLayoutItem * item = ui->formLayout->itemAt(index, QFormLayout::FieldRole);
        if(item != 0) {
            QDoubleSpinBox * sb =  qobject_cast<QDoubleSpinBox *> (item->widget());
            if(sb != 0) {
                return (float)sb->value();
            }
        }
    }

    return 0;
}

void AGCControl::setParameterValue(QString param, float value) {
    int leIndex = parameterListInputField.indexOf(param);
    int sbIndex = parameterListInputField.indexOf(param);

    if(leIndex != -1) {
        QLayoutItem * item = ui->formLayout->itemAt(leIndex, QFormLayout::FieldRole);
        if(item != 0) {
            QLineEdit * le =  qobject_cast<QLineEdit *> (item->widget());
            if(le != 0) {
                le->setText(QString().setNum(value));
            }
        }
    } else if(sbIndex != -1) {
        QLayoutItem * item = ui->formLayout->itemAt(sbIndex, QFormLayout::FieldRole);
        if(item != 0) {
            QDoubleSpinBox * sb =  qobject_cast<QDoubleSpinBox *> (item->widget());
            if(sb != 0) {
                sb->setValue(value);
            }
        }
    }
}

*/
