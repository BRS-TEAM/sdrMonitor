/*!
 * \file     agcrf.cpp
 * \brief    AGC RF Element Class
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
#include "headers/elements/agcrf.h"
#include "ui_agcrf.h"

#include <QDebug>

#include "math.h"
#include "headers/core/sharedflags.h"
#include "headers/staticdependencies/datatypeconversion.h"

// Parent
#include "headers/core/tabcontroller.h"

#include "headers/core/errorlogger.h"

// Constructor
AGCRF::AGCRF(TabController *parent) :
    QWidget(parent),
    ui(new Ui::AGCRF)
{
    ui->setupUi(this);


    selectedMode = -1;
    updateFlag = false;
    tabControllerParent = (TabController*) parent;

    ui->FM->setDisabled(true);
    ui->AM->setDisabled(true);
    ui->DAB->setDisabled(true);

    initParameterConfig();
    initCoeffConfig();

    connect(ui->FM, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    connect(ui->AM, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    connect(ui->DAB, SIGNAL(clicked(bool)), this, SLOT(selectMode()));


    // init the thr table
    // c.f. STA710_OM_v2.1.6.equ provided by ST
    //=========================================================================
    // FM AGC-RF High Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcFmThrMaxMap[84] = 0xAB000;
    m_RfAgcFmThrMaxMap[83] = 0xB3000;
    m_RfAgcFmThrMaxMap[82] = 0xBB000;
    m_RfAgcFmThrMaxMap[81] = 0xC2000;
    m_RfAgcFmThrMaxMap[80] = 0xC8000;
    m_RfAgcFmThrMaxMap[79] = 0xCD000;
    m_RfAgcFmThrMaxMap[78] = 0xD2000;
    m_RfAgcFmThrMaxMap[77] = 0xD7000;
    m_RfAgcFmThrMaxMap[76] = 0xDA000;
    m_RfAgcFmThrMaxMap[75] = 0xDE000;
    m_RfAgcFmThrMaxMap[74] = 0xE1000;
    m_RfAgcFmThrMaxMap[73] = 0xE4000;

    // FM AGC-RF LOW Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcFmThrMinMap[83] = 0xC2000;
    m_RfAgcFmThrMinMap[82] = 0xC8000;
    m_RfAgcFmThrMinMap[81] = 0xCD000;
    m_RfAgcFmThrMinMap[80] = 0xD2000;
    m_RfAgcFmThrMinMap[79] = 0xD7000;
    m_RfAgcFmThrMinMap[78] = 0xDA000;
    m_RfAgcFmThrMinMap[77] = 0xDE000;
    m_RfAgcFmThrMinMap[76] = 0xE1000;
    m_RfAgcFmThrMinMap[75] = 0xE4000;
    m_RfAgcFmThrMinMap[74] = 0xE6000;
    m_RfAgcFmThrMinMap[73] = 0xE8000;
    m_RfAgcFmThrMinMap[72] = 0xEA000;

    // FM AGC-RF High Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcFmThrMaxMap_inv[0xAB000] = 84;
    m_RfAgcFmThrMaxMap_inv[0xB3000] = 83;
    m_RfAgcFmThrMaxMap_inv[0xBB000] = 82;
    m_RfAgcFmThrMaxMap_inv[0xC2000] = 81;
    m_RfAgcFmThrMaxMap_inv[0xC8000] = 80;
    m_RfAgcFmThrMaxMap_inv[0xCD000] = 79;
    m_RfAgcFmThrMaxMap_inv[0xD2000] = 78;
    m_RfAgcFmThrMaxMap_inv[0xD7000] = 77;
    m_RfAgcFmThrMaxMap_inv[0xDA000] = 76;
    m_RfAgcFmThrMaxMap_inv[0xDE000] = 75;
    m_RfAgcFmThrMaxMap_inv[0xE1000] = 74;
    m_RfAgcFmThrMaxMap_inv[0xE4000] = 73;

    // FM AGC-RF LOW Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcFmThrMinMap_inv[0xC2000] = 83;
    m_RfAgcFmThrMinMap_inv[0xC8000] = 82;
    m_RfAgcFmThrMinMap_inv[0xCD000] = 81;
    m_RfAgcFmThrMinMap_inv[0xD2000] = 80;
    m_RfAgcFmThrMinMap_inv[0xD7000] = 79;
    m_RfAgcFmThrMinMap_inv[0xDA000] = 78;
    m_RfAgcFmThrMinMap_inv[0xDE000] = 77;
    m_RfAgcFmThrMinMap_inv[0xE1000] = 76;
    m_RfAgcFmThrMinMap_inv[0xE4000] = 75;
    m_RfAgcFmThrMinMap_inv[0xE6000] = 74;
    m_RfAgcFmThrMinMap_inv[0xE8000] = 73;
    m_RfAgcFmThrMinMap_inv[0xEA000] = 72;


    // AM AGC-RF High Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcAmThrMaxMap[73] = 0xD000;
    m_RfAgcAmThrMaxMap[72] = 0x3F000;
    m_RfAgcAmThrMaxMap[71] = 0x6B000;
    m_RfAgcAmThrMaxMap[70] = 0x93000;
    m_RfAgcAmThrMaxMap[69] = 0xB6000;
    m_RfAgcAmThrMaxMap[68] = 0xD5000;
    m_RfAgcAmThrMaxMap[67] = 0xF1000;
    m_RfAgcAmThrMaxMap[66] = 0x10A000;
    m_RfAgcAmThrMaxMap[65] = 0x120000;
    m_RfAgcAmThrMaxMap[64] = 0x134000;

    // AM AGC-RF LOW Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcAmThrMinMap[72] = 0x10A000;
    m_RfAgcAmThrMinMap[71] = 0x120000;
    m_RfAgcAmThrMinMap[70] = 0x134000;
    m_RfAgcAmThrMinMap[69] = 0x146000;
    m_RfAgcAmThrMinMap[68] = 0x155000;
    m_RfAgcAmThrMinMap[67] = 0x163000;
    m_RfAgcAmThrMinMap[66] = 0x170000;
    m_RfAgcAmThrMinMap[65] = 0x17B000;
    m_RfAgcAmThrMinMap[64] = 0x185000;
    m_RfAgcAmThrMinMap[63] = 0x18E000;

    // AM AGC-RF High Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcAmThrMaxMap_inv[0xD000] = 73;
    m_RfAgcAmThrMaxMap_inv[0x3F000] = 72;
    m_RfAgcAmThrMaxMap_inv[0x6B000] = 71;
    m_RfAgcAmThrMaxMap_inv[0x93000] = 70;
    m_RfAgcAmThrMaxMap_inv[0xB6000] = 69;
    m_RfAgcAmThrMaxMap_inv[0xD5000] = 68;
    m_RfAgcAmThrMaxMap_inv[0xF1000] = 67;
    m_RfAgcAmThrMaxMap_inv[0x10A000] = 66;
    m_RfAgcAmThrMaxMap_inv[0x120000] = 65;
    m_RfAgcAmThrMaxMap_inv[0x134000] = 64;

    // AM AGC-RF LOW Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcAmThrMinMap_inv[0x10A000] = 72;
    m_RfAgcAmThrMinMap_inv[0x120000] = 71;
    m_RfAgcAmThrMinMap_inv[0x134000] = 70;
    m_RfAgcAmThrMinMap_inv[0x146000] = 69;
    m_RfAgcAmThrMinMap_inv[0x155000] = 68;
    m_RfAgcAmThrMinMap_inv[0x163000] = 67;
    m_RfAgcAmThrMinMap_inv[0x170000] = 66;
    m_RfAgcAmThrMinMap_inv[0x17B000] = 65;
    m_RfAgcAmThrMinMap_inv[0x185000] = 64;
    m_RfAgcAmThrMinMap_inv[0x18E000] = 63;

    // DAB AGC-RF High Threshold fmb (SDM0) CHB
    //=========================================================================
    m_RfAgcDabThrMaxMap[-28] = 0x081000;
    m_RfAgcDabThrMaxMap[-29] = 0x091000;
    m_RfAgcDabThrMaxMap[-30] = 0x09E000;
    m_RfAgcDabThrMaxMap[-31] = 0xAA000;
    m_RfAgcDabThrMaxMap[-32] = 0x0B5000;
    m_RfAgcDabThrMaxMap[-33] = 0x0BE000;
    m_RfAgcDabThrMaxMap[-34] = 0x0C7000;
    m_RfAgcDabThrMaxMap[-35] = 0x0CE000;
    m_RfAgcDabThrMaxMap[-36] = 0x0D5000;

    // DAB AGC-RF LOW Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcDabThrMinMap[-29] = 0xB5000;
    m_RfAgcDabThrMinMap[-30] = 0xBE000;
    m_RfAgcDabThrMinMap[-31] = 0xC7000;
    m_RfAgcDabThrMinMap[-32] = 0xCE000;
    m_RfAgcDabThrMinMap[-33] = 0xD5000;
    m_RfAgcDabThrMinMap[-34] = 0xDB000;
    m_RfAgcDabThrMinMap[-35] = 0xE0000;
    m_RfAgcDabThrMinMap[-36] = 0xE5000;
    m_RfAgcDabThrMinMap[-37] = 0xE9000;

    //DAB AGC-RF High Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcDabThrMaxMap_inv[0x081000] = -28;
    m_RfAgcDabThrMaxMap_inv[0x091000] = -39;
    m_RfAgcDabThrMaxMap_inv[0x09E000] = -30;
    m_RfAgcDabThrMaxMap_inv[0xAA000] = -31;
    m_RfAgcDabThrMaxMap_inv[0x0B5000] = -32;
    m_RfAgcDabThrMaxMap_inv[0x0BE000] = -33;
    m_RfAgcDabThrMaxMap_inv[0x0C7000] = -34;
    m_RfAgcDabThrMaxMap_inv[0x0CE000] = -35;
    m_RfAgcDabThrMaxMap_inv[0x0D5000] = -36;

    // DAB AGC-RF LOW Threshold fmb (SDM0)
    //=========================================================================
    m_RfAgcDabThrMinMap_inv[0xB5000] = -29;
    m_RfAgcDabThrMinMap_inv[0xBE000] = -30;
    m_RfAgcDabThrMinMap_inv[0xC7000] = -31;
    m_RfAgcDabThrMinMap_inv[0xCE000] = -32;
    m_RfAgcDabThrMinMap_inv[0xD5000] = -33;
    m_RfAgcDabThrMinMap_inv[0xDB000] = -34;
    m_RfAgcDabThrMinMap_inv[0xE0000] = -35;
    m_RfAgcDabThrMinMap_inv[0xE5000] = -36;
    m_RfAgcDabThrMinMap_inv[0xE9000] = -37;

}

// Destructor
AGCRF::~AGCRF()
{
    delete ui;
}

/************************ General Methods specific to this class. ************************/

void AGCRF::initParameterConfig() {

    QStringList str;

    str << ui->AGC_RF_MAX_TH->objectName();
    str << ui->AGC_RF_MIN_TH->objectName();
    str << ui->AGC_RF_BL_TAU->objectName();
    str << ui->AGC_RF_TAU_ATT->objectName();
    str << ui->AGC_RF_TAU_REL->objectName();

    for(int i = 0; i < str.count(); i++) {
        setDataSourceIdentifier(str.at(i));
    }
}

void AGCRF::initCoeffConfig() {

    QStringList str;

    str << ui->_Sdm0thsHigh->objectName();
    str << ui->_Sdm0thsLow->objectName();
    str << ui->_Sdm1thsHigh->objectName();
    str << ui->_Sdm1thsLow->objectName();
    str << ui->_b0s->objectName();
    str << ui->_b0d->objectName();
    str << ui->MODESELECT->objectName();


    for(int i = 0; i < str.count(); i++) {
        setDataSourceIdentifier(str.at(i));
    }
}

void AGCRF::selectBand(QString band) {

    if (band == "FM_MODE")
    {
        ui->FM->setEnabled(true);
        ui->FM->setChecked(true);
        ui->AM->setDisabled(true);
        ui->DAB->setDisabled(true);
    }
    else if (band == "AM_MODE")
    {
        ui->FM->setDisabled(true);
        ui->AM->setEnabled(true);
        ui->AM->setChecked(true);
        ui->DAB->setDisabled(true);
    }
    else if (band == "DAB_MODE")
    {
        ui->FM->setDisabled(true);
        ui->AM->setDisabled(true);
        ui->DAB->setEnabled(true);
        ui->DAB->setChecked(true);
    }

    printDebugString( "band " + band);

}

void AGCRF::initParameterRange(QString band) {

    if (band == "FM_MODE")
    {
        ui->AGC_RF_MAX_TH->setRange(73, 84);
        ui->unit_AGC_RF_MAX_TH->setText("dBuv         [73, 84]");
        ui->AGC_RF_MIN_TH->setRange(72, 83);
        ui->unit_AGC_RF_MIN_TH->setText("dBuv         [72, 83]");
    }
    else if (band == "AM_MODE")
    {
        ui->AGC_RF_MAX_TH->setRange(64, 73);
        ui->unit_AGC_RF_MAX_TH->setText("dBuv         [64, 73]");
        ui->AGC_RF_MIN_TH->setRange(63, 72);
        ui->unit_AGC_RF_MIN_TH->setText("dBuv         [63, 72]");
    }
    else if (band == "DAB_MODE")
    {
        ui->AGC_RF_MAX_TH->setRange(-36, -28);
        ui->unit_AGC_RF_MAX_TH->setText("dBuv         [-36, -28]");
        ui->AGC_RF_MIN_TH->setRange(-37, -29);
        ui->unit_AGC_RF_MIN_TH->setText("dBuv         [-37, -29]");
    }

}

void AGCRF::setDataSourceIdentifier(QString label) {
    dSourceIdentifier.append(label);
}

QString AGCRF::getDataSourceIdentifier(int index) {
    if(dSourceIdentifier.count() > index)
        return dSourceIdentifier.at(index);
    return "";
}

int AGCRF::getDataSourceIdentifierCount() {
    return dSourceIdentifier.count();
}

bool AGCRF::setDataSource(DataStructure * structure) {

    if (structure != 0) {
        dSource.append(structure);
        return true;
    }

    return false;
}

QList<DataStructure *> AGCRF::getDataSource() {

    return dSource;

}

void AGCRF::resetDataSource() {

    dSource.clear();

}

void AGCRF::rfAgcGainCalculate() {

    //=========================================================================
    // FM AGC RF Processing Equation
    // c.f. STA710_OM_v2.1.6.equ provided by ST
    //=========================================================================
    // Constants - do NOT change
    //=========================================================================
    if (radioMode == FM_MODE)
    {
        if(m_RfAgcFmThrMaxMap.find(ui->AGC_RF_MAX_TH->value()) == m_RfAgcFmThrMaxMap.end())
        {
            ui->_Sdm0thsHigh->setText("Invalid threshold value! ");
            ui->_Sdm1thsHigh->setText("Invalid threshold value! ");
            return;
        }
        else
        {
            ui->_Sdm0thsHigh->setText( QString().setNum( m_RfAgcFmThrMaxMap[ui->AGC_RF_MAX_TH->value()], 16));
            ui->_Sdm1thsHigh->setText( QString().setNum( m_RfAgcFmThrMaxMap[ui->AGC_RF_MAX_TH->value()], 16));
        }

        if(m_RfAgcFmThrMinMap.find(ui->AGC_RF_MIN_TH->value()) == m_RfAgcFmThrMinMap.end())
        {
            ui->_Sdm0thsLow->setText("Invalid threshold value! ");
            ui->_Sdm1thsLow->setText("Invalid threshold value! ");
            return;
        }
        else
        {
            ui->_Sdm0thsLow->setText( QString().setNum( m_RfAgcFmThrMinMap[ui->AGC_RF_MIN_TH->value()], 16));
            ui->_Sdm1thsLow->setText( QString().setNum( m_RfAgcFmThrMinMap[ui->AGC_RF_MIN_TH->value()], 16));
        }
    }
    else if (radioMode == AM_MODE)
    {
        if(m_RfAgcAmThrMaxMap.find(ui->AGC_RF_MAX_TH->value()) == m_RfAgcAmThrMaxMap.end())
        {
            ui->_Sdm0thsHigh->setText("Invalid threshold value! ");
            ui->_Sdm1thsHigh->setText("Invalid threshold value! ");
            return;
        }
        else
        {
            ui->_Sdm0thsHigh->setText( QString().setNum( m_RfAgcAmThrMaxMap[ui->AGC_RF_MAX_TH->value()], 16));
            ui->_Sdm1thsHigh->setText( QString().setNum( m_RfAgcAmThrMaxMap[ui->AGC_RF_MAX_TH->value()], 16));
        }

        if(m_RfAgcAmThrMinMap.find(ui->AGC_RF_MIN_TH->value()) == m_RfAgcAmThrMinMap.end())
        {
            ui->_Sdm0thsLow->setText("Invalid threshold value! ");
            ui->_Sdm1thsLow->setText("Invalid threshold value! ");
            return;
        }
        else
        {
            ui->_Sdm0thsLow->setText( QString().setNum( m_RfAgcAmThrMinMap[ui->AGC_RF_MIN_TH->value()], 16));
            ui->_Sdm1thsLow->setText( QString().setNum( m_RfAgcAmThrMinMap[ui->AGC_RF_MIN_TH->value()], 16));
        }
    }
    else if (radioMode == DAB_MODE)
    {
        if(m_RfAgcDabThrMaxMap.find(ui->AGC_RF_MAX_TH->value()) == m_RfAgcDabThrMaxMap.end())
        {
            ui->_Sdm0thsHigh->setText("Invalid threshold value! ");
            ui->_Sdm1thsHigh->setText("Invalid threshold value! ");
            return;
        }
        else
        {
            ui->_Sdm0thsHigh->setText( QString().setNum( m_RfAgcDabThrMaxMap[ui->AGC_RF_MAX_TH->value()], 16));
            ui->_Sdm1thsHigh->setText( QString().setNum( m_RfAgcDabThrMaxMap[ui->AGC_RF_MAX_TH->value()], 16));
        }

        if(m_RfAgcDabThrMinMap.find(ui->AGC_RF_MIN_TH->value()) == m_RfAgcDabThrMinMap.end())
        {
            ui->_Sdm0thsLow->setText("Invalid threshold value! ");
            ui->_Sdm1thsLow->setText("Invalid threshold value! ");
            return;
        }
        else
        {
            ui->_Sdm0thsLow->setText( QString().setNum( m_RfAgcDabThrMinMap[ui->AGC_RF_MIN_TH->value()], 16));
            ui->_Sdm1thsLow->setText( QString().setNum( m_RfAgcDabThrMinMap[ui->AGC_RF_MIN_TH->value()], 16));
        }
    }

    float FS = 45600;                    // Sample frequency
    double b0s = ((2 / (1 + 2 * ui->AGC_RF_TAU_ATT->text().toFloat()  * FS / 5)) + (2 / (1 + 2 * ui->AGC_RF_TAU_REL->text().toFloat() * FS / 5))) / 2;
    double b0d = ((2 / (1 + 2 * ui->AGC_RF_TAU_ATT->text().toFloat()  * FS / 5)) - (2 / (1 + 2 * ui->AGC_RF_TAU_REL->text().toFloat() * FS / 5))) / 2;

    ui->_b0s->setText(QString().setNum(double2fixed24(b0s), 16));
    ui->_b0d->setText(QString().setNum(double2fixed24(b0d), 16));

}

int AGCRF::double2fixed24(double val_double)
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

void AGCRF::setLabelName(const QString &value)
{
    Q_UNUSED(value)
}

// Set the indentification name of AGCControl
void AGCRF::setNameIdentifier(QString name) {
    nameIdentifier = name;
}

/************************ Getters & Setters ************************/

// Get the indentification name of AGCControl
QString AGCRF::getNameIdentifier() {
    return nameIdentifier;
}

bool AGCRF::setStructure(DataStructure * structure) {
    // dStructure = 0;
    // dStructure = structure;
    // if (dStructure != 0)
    // return true;
    // return false;

    Q_UNUSED(structure)
    return false;
}

DataStructure * AGCRF::getStructure() {
    // return dStructure;
    return 0;
}

void AGCRF::setIndex(int index) {
    this->index = index;
}

int AGCRF::getIndex() {
    return index;
}

void AGCRF::setParameterValue(QString param, float cfgValue) {

    bool ok = false;


    if(param == ui->AGC_RF_MAX_TH->objectName()) {
        if (radioMode == FM_MODE)
        {
            if(m_RfAgcFmThrMaxMap_inv.find(ui->_Sdm0thsHigh->text().toInt(&ok, 16)) == m_RfAgcFmThrMaxMap_inv.end())
                ui->AGC_RF_MAX_TH->setValue(-1);
            else
                ui->AGC_RF_MAX_TH->setValue(m_RfAgcFmThrMaxMap_inv[ui->_Sdm0thsHigh->text().toInt(&ok, 16)]);
        }
        else if (radioMode == AM_MODE)
        {
            if(m_RfAgcAmThrMaxMap_inv.find(ui->_Sdm0thsHigh->text().toInt(&ok, 16)) == m_RfAgcAmThrMaxMap_inv.end())
                ui->AGC_RF_MAX_TH->setValue(-1);
            else
                ui->AGC_RF_MAX_TH->setValue(m_RfAgcAmThrMaxMap_inv[ui->_Sdm0thsHigh->text().toInt(&ok, 16)]);
        }
        else if (radioMode == DAB_MODE)
        {
            if(m_RfAgcDabThrMaxMap_inv.find(ui->_Sdm0thsHigh->text().toInt(&ok, 16)) == m_RfAgcDabThrMaxMap_inv.end())
                ui->AGC_RF_MAX_TH->setValue(-1);
            else
                ui->AGC_RF_MAX_TH->setValue(m_RfAgcDabThrMaxMap_inv[ui->_Sdm0thsHigh->text().toInt(&ok, 16)]);
        }
    }

    else if(param == ui->AGC_RF_MIN_TH->objectName()) {
        if (radioMode == FM_MODE)
        {
            if(m_RfAgcFmThrMinMap_inv.find(ui->_Sdm0thsLow->text().toInt(&ok, 16)) == m_RfAgcFmThrMinMap_inv.end())
                ui->AGC_RF_MIN_TH->setValue(-1);
            else
                ui->AGC_RF_MIN_TH->setValue(m_RfAgcFmThrMinMap_inv[ui->_Sdm0thsLow->text().toInt(&ok, 16)]);
        }
        else if (radioMode == AM_MODE)
        {
            if(m_RfAgcAmThrMinMap_inv.find(ui->_Sdm0thsLow->text().toInt(&ok, 16)) == m_RfAgcAmThrMinMap_inv.end())
                ui->AGC_RF_MIN_TH->setValue(-1);
            else
                ui->AGC_RF_MIN_TH->setValue(m_RfAgcAmThrMinMap_inv[ui->_Sdm0thsLow->text().toInt(&ok, 16)]);
        }
        else if (radioMode == DAB_MODE)
        {
            if(m_RfAgcDabThrMinMap_inv.find(ui->_Sdm0thsLow->text().toInt(&ok, 16)) == m_RfAgcDabThrMinMap_inv.end())
                ui->AGC_RF_MIN_TH->setValue(-1);
            else
                ui->AGC_RF_MIN_TH->setValue(m_RfAgcDabThrMinMap_inv[ui->_Sdm0thsLow->text().toInt(&ok, 16)]);
        }
    }


    else if(param == ui->AGC_RF_BL_TAU->objectName())
        ui->AGC_RF_BL_TAU->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_RF_TAU_ATT->objectName())
        ui->AGC_RF_TAU_ATT->setText(QString().setNum(cfgValue));
    else if(param == ui->AGC_RF_TAU_REL->objectName())
        ui->AGC_RF_TAU_REL->setText(QString().setNum(cfgValue));

}

void AGCRF::updateParameterStructureValue() {

    // double val;
    QString param;
    for(int i = 0; i < dSource.count(); i++) {
        if(dSource.at(i)->address == "0") {

            param.clear();
            param = dSource.at(i)->name;

            if(param == ui->AGC_RF_MAX_TH->objectName())
                dSource.at(i)->cfgValue = ui->AGC_RF_MAX_TH->value();
            else if(param == ui->AGC_RF_MIN_TH->objectName())
                dSource.at(i)->cfgValue = ui->AGC_RF_MIN_TH->value();
            else if(param == ui->AGC_RF_BL_TAU->objectName())
                dSource.at(i)->cfgValue = ui->AGC_RF_BL_TAU->text().toFloat();
            else if(param == ui->AGC_RF_TAU_ATT->objectName())
                dSource.at(i)->cfgValue = ui->AGC_RF_TAU_ATT->text().toFloat();
            else if(param == ui->AGC_RF_TAU_REL->objectName())
                dSource.at(i)->cfgValue = ui->AGC_RF_TAU_REL->text().toFloat();
        }
    }
}

void AGCRF::setCoeffValue(QString coeff, int value) {

    if(coeff == ui->_Sdm0thsHigh->objectName())
        ui->_Sdm0thsHigh->setText(QString().setNum(value, 16));
    else if(coeff == ui->_Sdm0thsLow->objectName())
        ui->_Sdm0thsLow->setText(QString().setNum(value, 16));
    else if(coeff == ui->_Sdm1thsHigh->objectName())
        ui->_Sdm1thsHigh->setText(QString().setNum(value, 16));
    else if(coeff == ui->_Sdm1thsLow->objectName())
        ui->_Sdm1thsLow->setText(QString().setNum(value, 16));
    else if(coeff == ui->_b0s->objectName())
        ui->_b0s->setText(QString().setNum(value, 16));
    else if(coeff == ui->_b0d->objectName())
        ui->_b0d->setText(QString().setNum(value, 16));

    else if(coeff == "MODESELECT")
        selectedMode = (int)value;

}

QString AGCRF::getCoeffValue(QString coeff) {

    QString str;
    QByteArray bA;

    if(coeff == ui->_Sdm0thsHigh->objectName())
        bA = ui->_Sdm0thsHigh->text().toLatin1();
    else if(coeff == ui->_Sdm0thsLow->objectName())
        bA = ui->_Sdm0thsLow->text().toLatin1();
    else if(coeff == ui->_Sdm1thsHigh->objectName())
        bA = ui->_Sdm1thsHigh->text().toLatin1();
    else if(coeff == ui->_Sdm1thsLow->objectName())
        bA = ui->_Sdm1thsLow->text().toLatin1();
    else if(coeff == ui->_b0s->objectName())
        bA = ui->_b0s->text().toLatin1();
    else if(coeff == ui->_b0d->objectName())
        bA = ui->_b0d->text().toLatin1();

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

void AGCRF::selectMode() {

    if(ui->FM->isChecked())
        selectedMode = 0;
    else if(ui->AM->isChecked())
        selectedMode = 1;
    else if(ui->DAB->isChecked())
        selectedMode = 2;

    qDebug() << "mode " << selectedMode;


    if(selectedMode != -1 && !updateFlag) {
        QList<DataStructure *> dS;
        QList<QString> value;
//        initHMI(selectedMode);
        qDebug() << "HMI agc";

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
    } else updateFlag = false;
}

void AGCRF::updateEvent() {

    if (radioMode == FM_MODE)
        ui->FM->setChecked(true);
    else if (radioMode == AM_MODE)
        ui->AM->setChecked(true);
    else if (radioMode == DAB_MODE)
        ui->DAB->setChecked(true);

    selectMode();

    // if (dStructure != 0) {
    // } else
    if(dSource.count()) {

        // selectedBand(); //  call by xml parser

        for(int i = 0; i < dSource.count(); i++) {
            if(dSource.at(i)->address != "0") {
                setCoeffValue(dSource.at(i)->name, dSource.at(i)->value);
            }
        }

        updateFlag = true;

//        switch (selectedMode) {
//        case 0:
//            ui->FM->setChecked(true); break;
//        case 1:
//            ui->AM->setChecked(true); break;
//        case 2:
//            ui->DAB->setChecked(true); break;
//        }

        updateFlag = false;

        for(int i = 0; i < dSource.count(); i++) {
            if(dSource.at(i)->address == "0") {
                setParameterValue(dSource.at(i)->name, dSource.at(i)->cfgValue);
            }
        }

    }
}


void AGCRF::on_calculatePushButton_clicked()
{
     rfAgcGainCalculate();

    updateParameterStructureValue();
}

void AGCRF::on_writePushButton_clicked()
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
