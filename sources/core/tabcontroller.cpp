/*!
 * \file     tabcontroller.cpp
 * \brief    Controller class to manage Individual Tab Pages
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
#include "headers/core/tabcontroller.h"

// Dialog Display
#include <QFileDialog>
#include <QMessageBox>

// Dynamic Loader
#include <QtUiTools/QtUiTools>
// #include "/home/uidp9843/Qt/qcustomplot-sharedlib/header/qcustomplot.h"

#include "headers/elements/bargraphh.h"
#include "headers/elements/progressbargraph.h"
#include "headers/elements/slidebar.h"
#include "headers/elements/xyplot.h"
#include "headers/elements/inputfield.h"
#include "headers/elements/spinbox.h"
#include "headers/elements/button.h"
#include "headers/elements/checkbox.h"
#include "headers/elements/timevalueplot.h"
#include "headers/elements/indicator.h"
#include "headers/elements/radiobutton.h"
#include "headers/elements/textbox.h"
#include "headers/elements/agccontrol.h"
#include "headers/elements/agcrf.h"
#include "headers/elements/frames.h"
#include "headers/elements/audiofilters.h"

#include "headers/core/communicationcontroller.h"
#include "headers/core/uiparser.h"
#include "headers/core/uipainter.h"
#include "headers/core/errorlogger.h"

// Constructor
TabController::TabController(QWidget * parent)
    : QWidget(parent)
{
    pageLoaded = false;
    common = false;
    pageWidget = 0;
    dataManager = 0;
    uiPainter = 0;
    cController = 0;
    continuousRead = true;

    containerFrame = 0;

}

// Destructor
TabController::~TabController(){

    if(uiPainter !=0) {
        uiPainter->stop();
        delete uiPainter;
        uiPainter = 0;
    }

    if(pageLoaded) {
        pageWidget->close();
        delete pageWidget;
        delete pageParser;

        // for( int i = 0; i < inputField.count(); i++ );
        //delete simpleInputField.at(i);                    // Deleted by pageWidget

        // if(configFound)
        // delete configController;

        printDebugString("Page Deleted");
    }
}

// Parse the source xml file.
void TabController::pageWidgetUiParser() {

    // Instantiate parser
    pageParser = new UiParser((TabController*)this);
    pageParser->initUiParser(pageSource, 2);   // Parse the input XML file.
    pageName = pageParser->getTabPageName();
    setCommon(pageParser->getTabCommon());
    setReadMode(pageParser->getTabReadMode());

    /*
     * ...
     */


}


// Store data collected by Parser. Build tab page UI.
void TabController::generateUi() {

    // Store data collected by Parser

    for(int i = 0; i < pageParser->getFramesCount(); i++ )
        frames.append(pageParser->getFrames(i));

    for(int i = 0; i < pageParser->getInputFieldCount(); i++ )
        inputField.append(pageParser->getInputField(i));

    for(int i = 0; i < pageParser->getSpinBoxCount(); i++ )
        spinBox.append(pageParser->getSpinBox(i));

    for(int i = 0; i < pageParser->getCheckBoxCount(); i++ )
        checkBox.append(pageParser->getCheckBox(i));

    for(int i = 0; i < pageParser->getPushButtonCount(); i++ )
        pushButton.append(pageParser->getPushButton(i));

    for(int i = 0; i < pageParser->getProgressBarGraphCount(); i++ )
        pbarGraph.append(pageParser->getProgressBarGraph(i));

    for(int i = 0; i < pageParser->getSlideBarCount(); i++ )
        slideBar.append(pageParser->getSlideBar(i));

    for(int i = 0; i < pageParser->getBarGraphCount(); i++ )
        barGraph.append(pageParser->getBarGraph(i));

    for(int i = 0; i < pageParser->getXyPlotCount(); i++ )
        xyPlot.append(pageParser->getXyPlot(i));

    for(int i = 0; i < pageParser->getTimeValuePlotCount(); i++ )
        tVPlot.append(pageParser->getTimeValuePlot(i));

    for(int i = 0; i < pageParser->getIndicatorCount(); i++ )
        indicator.append(pageParser->getIndicator(i));

    for(int i = 0; i < pageParser->getRadioButtonCount(); i++ )
        radioButton.append(pageParser->getRadioButton(i));

    for(int i = 0; i < pageParser->getTextBoxCount(); i++ )
        textBox.append(pageParser->getTextBox(i));

    for(int i = 0; i < pageParser->getAGCCount(); i++ )
        agc.append(pageParser->getAGC(i));

    for(int i = 0; i < pageParser->getAGCRFCount(); i++ )
        agcRf.append(pageParser->getAGCRF(i));

    for(int i = 0; i < pageParser->getaudioFiltersCount(); i++ )
        audioFilters.append(pageParser->getaudioFilters(i));

    // Build tab Page UI.
    pageLoaded = designLoader();

    /*
     */

}

// Build tab page UI.
bool TabController::designLoader() {


    // Generate tab Layout
    /*
     * ********************************** IMPORTANT *************************************************
     *
     * This section can be modified by the algorithm to develop desired layout
     *
     * All the widgets collected in generateUi() can be placed on layout and are easily accessible.
     *
     * Below is the algorithm that place all the widgets in a matrix pattern using directions given in LEVEL 2 XML file.
     *
     * During the XML parsing phase, The layout information given in LEVEL 2 XML file is copied to respective .lay files for each tab.
     * These files are read below to generate the layout.
     * If the .lay file is not found for any tab then all the widgets are placed in single column in undefined order.
     *
     * These .lay files are temporary and are deleted when the application is closed.
     *
     */

    int count = 0;

    // Input layout file
    QString inputDir = "layout";
    QString fileName = pageParser->getTabPageName() + ".lay";
    QFile inputFile(inputDir+"/"+fileName);

    // Read .lay file
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QGridLayout * gBox = new QGridLayout;           // Matrix Grid

        // Input File Handler
        QTextStream in(&inputFile);

        int frameIndex = -1;
        int frameRows = -1;
        int frameCols = -1;
        int frameRowSpan = -1;
        int frameColSpan = -1;

        while (!in.atEnd())
        {
            // Read one line
            QString line = in.readLine();
            if(line.at(0) != '#') {                         // Check if comment (Normally Header)

                QStringList lists = line.split("\t");       // Break line at each tab space


                if(lists.length() == 7) {
                    /*
                     * frame
                     */
                    if(lists.at(0) == "frame") {
                        for(int i = 0; i < lists.length(); i++){    // store data

                            printDebugString( lists.at(i));     // Redundant
                            switch (i) {
                            case 1:
                                for(int j = 0; j < pageParser->getFramesCount(); j++ )
                                    if(frames.at(j)->getIndex() == QString(lists.at(i)).toInt()) { frameIndex = j; break; }
                                    else { frameIndex = -1; }
                                break;
                            case 2:
                                frameRows = QString(lists.at(i)).toInt();
                                break;
                            case 3:
                                frameCols = QString(lists.at(i)).toInt();
                                break;
                            case 4:
                                frameRowSpan = QString(lists.at(i)).toInt();
                                break;
                            case 5:
                                frameColSpan = QString(lists.at(i)).toInt();
                                break;
                            }
                        }
                    }
                    printDebugString("Frame Index : " +
                                     QString().setNum(frameIndex) +
                                     ", Frame Rows : " +
                                     QString().setNum(frameRows) +
                                     ", Frame Cols : " +
                                     QString().setNum(frameCols) +
                                     ", Frame RowSpan : " +
                                     QString().setNum(frameRowSpan ) +
                                     ", Frame ColSpan : " +
                                     QString().setNum(frameColSpan));

                    if((frameIndex != -1) && (frameRows != -1) && (frameCols != -1)) {
                        gBox->addWidget(frames.at(frameIndex), frameRows, frameCols, frameRowSpan, frameColSpan);
                    }
                }

                if(lists.length() == 6) {
                    /*
                     * widgets
                     */
                    if(frameIndex != -1) {
                        int widgetIndex = -1;
                        int row = QString(lists.at(2)).toInt();
                        int col = QString(lists.at(3)).toInt();
                        int rowSpan = QString(lists.at(4)).toInt();
                        int colSpan = QString(lists.at(5)).toInt();

                        if(lists.at(0) == "lineEdit") {
                            for(int i = 0; i < pageParser->getInputFieldCount(); i++ )
                                if(inputField.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(inputField.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'lineEdit' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "spinBox") {
                            for(int i = 0; i < pageParser->getSpinBoxCount(); i++ )
                                if(spinBox.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(spinBox.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'spinBox' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "checkBox") {
                            for(int i = 0; i < pageParser->getCheckBoxCount(); i++ )
                                if(checkBox.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(checkBox.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'checkBox' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "pushButton") {
                            for(int i = 0; i < pageParser->getPushButtonCount(); i++ )
                                if(pushButton.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(pushButton.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'pushButton' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "slideBar") {
                            for(int i = 0; i < pageParser->getSlideBarCount(); i++ )
                                if(slideBar.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(slideBar.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'slideBar' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "levelBar") {
                            for(int i = 0; i < pageParser->getBarGraphCount(); i++ )
                                if(barGraph.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(barGraph.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'levelBar' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "progressBar") {
                            for(int i = 0; i < pageParser->getProgressBarGraphCount(); i++ )
                                if(pbarGraph.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(pbarGraph.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'progressBar' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "xyPlot") {
                            for(int i = 0; i < pageParser->getXyPlotCount(); i++ )
                                if(xyPlot.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(xyPlot.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'xyPlot' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "timeValuePlot") {
                            for(int i = 0; i < pageParser->getTimeValuePlotCount(); i++ )
                                if(tVPlot.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(tVPlot.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'timeValuePlot' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "indicator") {
                            for(int i = 0; i < pageParser->getIndicatorCount(); i++ )
                                if(indicator.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(indicator.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'indicator' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "radioButton") {
                            for(int i = 0; i < pageParser->getRadioButtonCount(); i++ )
                                if(radioButton.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(radioButton.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'radioButton' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "textBox") {
                            for(int i = 0; i < pageParser->getTextBoxCount(); i++ )
                                if(textBox.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(textBox.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'textBox' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "agc") {
                            for(int i = 0; i < pageParser->getAGCCount(); i++ )
                                if(agc.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(agc.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'agc' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "agcRf") {
                            for(int i = 0; i < pageParser->getAGCRFCount(); i++ )
                                if(agcRf.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(agcRf.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'agcRf' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else if(lists.at(0) == "audioFilters") {
                            for(int i = 0; i < pageParser->getaudioFiltersCount(); i++ )
                                if(audioFilters.at(i)->getIndex() == QString(lists.at(1)).toInt()) { widgetIndex = i; break; }

                            if(widgetIndex != -1)
                                frames.at(frameIndex)->insertWidget(audioFilters.at(widgetIndex), row, col, rowSpan, colSpan);
                            else
                                reportError("Layout ERROR : Invalid 'AudioFilters' widget index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                        }
                        else
                            reportError("Layout ERROR : Unkown widget '" + lists.at(0) + "'. Check XML File for :: " + pageParser->getTabPageName());
                    } else
                        reportError("Layout ERROR : Invalid 'frame' index '" + lists.at(1) + "'. Check XML File for :: " + pageParser->getTabPageName());
                }

            } else continue;
            count ++;
        }
        inputFile.close();
        printDebugString("Layout Line count : " + QString().setNum(count));

        for(int i = 0; i < pageParser->getFramesCount(); i++ )
            frames.at(i)->addSpace();


        /*
         *
         * Add to tabWidget
         *
         */

        containerFrame = new QFrame;
        QHBoxLayout * container = new QHBoxLayout;
        QVBoxLayout *vAdjust = new QVBoxLayout;

        QVBoxLayout *vBox = new QVBoxLayout;

        //        for(int i = 0; i < pageParser->getFramesCount(); i++ )
        //            vBox->addWidget(frames.at(i));

        vBox->addLayout(gBox);

        vBox->addStretch(1);

        QHBoxLayout *hBox = new QHBoxLayout;
        hBox->addLayout(vBox);
        hBox->addStretch(1);

        containerFrame->setAutoFillBackground(true);
        containerFrame->setLayout(hBox);
        containerFrame->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        container->addWidget(containerFrame);
        container->addStretch(1);

        vAdjust->addLayout(container);
        vAdjust->addStretch(1);

        QWidget *formWidget = new QWidget(this);
        //        formWidget->setLayout(hBox);
        formWidget->setLayout(vAdjust);


        if (formWidget != 0) {
            /*
             * If everything went well. Return True
             */
            pageWidget = formWidget;
            return true;
        }
    }
    else {

        /*
     *
     * Strict Layout
     *
     * This section is executed when .lay file is not found for the given tab.
     *
     */

        QVBoxLayout *vBox = new QVBoxLayout;

        QWidget *formWidget = new QWidget(this);

        for(int i = 0; i < pageParser->getFramesCount(); i++ )
            vBox->addWidget(frames.at(i));

        for(int i = 0; i < pageParser->getXyPlotCount(); i++ )
            vBox->addWidget(xyPlot.at(i));

        for(int i = 0; i < pageParser->getTimeValuePlotCount(); i++ )
            vBox->addWidget(tVPlot.at(i));

        for(int i = 0; i < pageParser->getIndicatorCount(); i++ )
            vBox->addWidget(indicator.at(i));

        for(int i = 0; i < pageParser->getRadioButtonCount(); i++ )
            vBox->addWidget(radioButton.at(i));

        for(int i = 0; i < pageParser->getTextBoxCount(); i++ )
            vBox->addWidget(textBox.at(i));

        for(int i = 0; i < pageParser->getAGCCount(); i++ )
            vBox->addWidget(agc.at(i));

        for(int i = 0; i < pageParser->getAGCRFCount(); i++ )
            vBox->addWidget(agcRf.at(i));

        for(int i = 0; i < pageParser->getInputFieldCount(); i++ )
            vBox->addWidget(inputField.at(i));

        for(int i = 0; i < pageParser->getSpinBoxCount(); i++ )
            vBox->addWidget(spinBox.at(i));

        for(int i = 0; i < pageParser->getCheckBoxCount(); i++ ) {
            vBox->addWidget(checkBox.at(i));
            // if(checkBox.at(i)->isCheckBoxChecked())
            // checkBox.at(i)->checked();
        }

        for(int i = 0; i < pageParser->getBarGraphCount(); i++ )
            vBox->addWidget(barGraph.at(i));

        for(int i = 0; i < pageParser->getProgressBarGraphCount(); i++ )
            vBox->addWidget(pbarGraph.at(i));

        for(int i = 0; i < pageParser->getSlideBarCount(); i++ )
            vBox->addWidget(slideBar.at(i));

        for(int i = 0; i < pageParser->getPushButtonCount(); i++ ){
            vBox->addWidget(pushButton.at(i));
        }

        for(int i = 0; i < pageParser->getaudioFiltersCount(); i++ ){
            vBox->addWidget(audioFilters.at(i));
        }

        vBox->addStretch(1);
        formWidget->setLayout(vBox);


        // formWidget = simpleInputField.at(0);
        if (formWidget != 0) {

            /*
         * If everything went well. Return True
         */

            pageWidget = formWidget;

            return true;

        }

    }

    return false;
}

bool TabController::isCommon() { return common; }

void TabController::setCommon(QString status) { if(status == "y" || status == "Y") common = true; }

void TabController::loadConfig() {

    int loadCount = 0;

    for(int i = 0; i < inputField.count(); i++ )
        if(inputField.at(i)->setStructure(dataManager->getDataStructure(pageName, inputField.at(i)->getNameIdentifier()))) {
            loadCount++;
            connect(uiPainter, SIGNAL(clickEvent()), inputField.at(i), SLOT(updateEvent()));
        }

    for(int i = 0; i < spinBox.count(); i++ )
        if(spinBox.at(i)->setStructure(dataManager->getDataStructure(pageName, spinBox.at(i)->getNameIdentifier()))) {
            loadCount++;
            connect(uiPainter, SIGNAL(clickEvent()), spinBox.at(i), SLOT(updateEvent()));
        }

    for(int i = 0; i < checkBox.count(); i++ )
        if(checkBox.at(i)->setStructure(dataManager->getDataStructure(pageName, checkBox.at(i)->getNameIdentifier()))) {
            loadCount++;
            //              connect(&uiPainter, SIGNAL(mySignal()), checkBox.at(i), SLOT(updateEvent()));
        }

    for(int i = 0; i < pushButton.count(); i++ )
        if(pushButton.at(i)->setStructure(dataManager->getDataStructure(pageName, pushButton.at(i)->getNameIdentifier()))) {
            loadCount++;
            //              connect(&uiPainter, SIGNAL(mySignal()), pushButton.at(i), SLOT(updateEvent()));
        }


    for(int i = 0; i < slideBar.count(); i++ )
        if(slideBar.at(i)->setStructure(dataManager->getDataStructure(pageName, slideBar.at(i)->getNameIdentifier()))) {
            loadCount++;
            //                connect(&uiPainter, SIGNAL(mySignal()), slideBar.at(i), SLOT(updateEvent()));
        }

    for(int i = 0; i < barGraph.count(); i++ )
        if(barGraph.at(i)->setStructure(dataManager->getDataStructure(pageName, barGraph.at(i)->getNameIdentifier()))) {
            loadCount++;
            connect(uiPainter, SIGNAL(autoUpdate()), barGraph.at(i), SLOT(updateEvent()));
        }

    for(int i = 0; i < pbarGraph.count(); i++ )
        if(pbarGraph.at(i)->setStructure(dataManager->getDataStructure(pageName, pbarGraph.at(i)->getNameIdentifier()))) {
            loadCount++;
            connect(uiPainter, SIGNAL(autoUpdate()), pbarGraph.at(i), SLOT(updateEvent()));
        }

    for(int i = 0; i < xyPlot.count(); i++ ) {
        if(xyPlot.at(i)->setStructure(dataManager->getDataStructure(pageName, xyPlot.at(i)->getNameIdentifier()))) {
            loadCount++;
            if(xyPlot.at(i)->getGraphType() == "Linear")
                connect(uiPainter, SIGNAL(autoUpdate()), xyPlot.at(i), SLOT(plotSensor()));
            //            else if(xyPlot.at(i)->getGraphType() == "Non-Linear")
            //                connect(uiPainter, SIGNAL(autoUpdate()), xyPlot.at(i), SLOT(updateEvent()));
        }

        if(xyPlot.at(i)->setStructure2(dataManager->getDataStructure(pageName, xyPlot.at(i)->getNameIdentifier2()))) {
            loadCount++;
        }

        //        bool done = false;
        xyPlot.at(i)->resetOutputSource();

        for(int j = 0; j < xyPlot.at(i)->getOutputSourceIdentifierCount(); j++ ) {
            if(xyPlot.at(i)->setOutputSourceStructure(dataManager->getDataStructure(pageName, xyPlot.at(i)->getOutputSourceIdentifier(j)))) {
                loadCount++;
                //                done = true;
            }
        }


        bool done = false;
        xyPlot.at(i)->resetAntennaDataSource();

        for(int j = 0; j < xyPlot.at(i)->getAntennaDataSourceIdentifierCount(); j++ ) {
            if(xyPlot.at(i)->setAntennaDataSource(dataManager->getDataStructure(pageName, xyPlot.at(i)->getAntennaDataSourceIdentifier(j)))) {
                loadCount++;
                done = true;
            }
        }
        if(done)
            connect(uiPainter, SIGNAL(autoUpdate()), xyPlot.at(i), SLOT(updateEvent()));

    }

    for(int i = 0; i < tVPlot.count(); i++ ) {
        if(tVPlot.at(i)->setStructure(dataManager->getDataStructure(pageName, tVPlot.at(i)->getNameIdentifier()))) {
            loadCount++;
            connect(uiPainter, SIGNAL(autoUpdate()), tVPlot.at(i), SLOT(updateEvent()));
        }

        bool done = false;
        tVPlot.at(i)->resetDataSource();

        for(int j = 0; j < tVPlot.at(i)->getDataSourceIdentifierCount(); j++ ) {
            if(tVPlot.at(i)->setDataSource(dataManager->getDataStructure(pageName, tVPlot.at(i)->getDataSourceIdentifier(j)))) {
                loadCount++;
                done = true;
            }
        }
        if(done)
            connect(uiPainter, SIGNAL(autoUpdate()), tVPlot.at(i), SLOT(updateEvent()));
    }

    for(int i = 0; i < indicator.count(); i++ ) {
        if(indicator.at(i)->setStructure(dataManager->getDataStructure(pageName, indicator.at(i)->getNameIdentifier()))) {
            loadCount++;
            connect(uiPainter, SIGNAL(autoUpdate()), indicator.at(i), SLOT(updateEvent()));
        }
    }

    for(int i = 0; i < radioButton.count(); i++ ) {
        if(radioButton.at(i)->setStructure(dataManager->getDataStructure(pageName, radioButton.at(i)->getNameIdentifier()))) {
            loadCount++;
            //            connect(uiPainter, SIGNAL(autoUpdate()), radioButton.at(i), SLOT(updateEvent()));
        }
    }

    for(int i = 0; i < textBox.count(); i++ ) {
        if(textBox.at(i)->setStructure(dataManager->getDataStructure(pageName, textBox.at(i)->getNameIdentifier()))) {
            loadCount++;
            connect(uiPainter, SIGNAL(autoUpdate()), textBox.at(i), SLOT(updateEvent()));
        }

        bool done = false;
        textBox.at(i)->resetDataSource();

        for(int j = 0; j < textBox.at(i)->getDataSourceIdentifierCount(); j++ ) {
            if(textBox.at(i)->setDataSource(dataManager->getDataStructure(pageName, textBox.at(i)->getDataSourceIdentifier(j)))) {
                loadCount++;
                done = true;
            }
        }
        if(done)
            connect(uiPainter, SIGNAL(autoUpdate()), textBox.at(i), SLOT(updateEvent()));
    }

    for(int i = 0; i < agc.count(); i++ ) {
        if(agc.at(i)->setStructure(dataManager->getDataStructure(pageName, agc.at(i)->getNameIdentifier()))) {
            loadCount++;
            //            connect(uiPainter, SIGNAL(autoUpdate()), agc.at(i), SLOT(updateEvent()));
        }

        //        bool done = false;
        agc.at(i)->resetDataSource();

        for(int j = 0; j < agc.at(i)->getDataSourceIdentifierCount(); j++ ) {
            if(agc.at(i)->setDataSource(dataManager->getDataStructure(pageName, agc.at(i)->getDataSourceIdentifier(j)))) {
                loadCount++;
                //                done = true;
            }
        }
        //        if(done)
        //            connect(uiPainter, SIGNAL(autoUpdate()), agc.at(i), SLOT(updateEvent()));
    }


    for(int i = 0; i < agcRf.count(); i++ ) {
        if(agcRf.at(i)->setStructure(dataManager->getDataStructure(pageName, agcRf.at(i)->getNameIdentifier()))) {
            loadCount++;
        }

        agcRf.at(i)->resetDataSource();

        for(int j = 0; j < agcRf.at(i)->getDataSourceIdentifierCount(); j++ ) {
            if(agcRf.at(i)->setDataSource(dataManager->getDataStructure(pageName, agcRf.at(i)->getDataSourceIdentifier(j)))) {
                loadCount++;
            }
        }
    }

    for(int i = 0; i < audioFilters.count(); i++ ) {
        if(audioFilters.at(i)->setStructure(dataManager->getDataStructure(pageName, audioFilters.at(i)->getNameIdentifier()))) {
            loadCount++;
            //            connect(uiPainter, SIGNAL(autoUpdate()), agc.at(i), SLOT(updateEvent()));
        }

        //        bool done = false;
        audioFilters.at(i)->resetDataSource();

        for(int j = 0; j < audioFilters.at(i)->getDataSourceIdentifierCount(); j++ ) {
            if(audioFilters.at(i)->setDataSource(dataManager->getDataStructure(pageName, audioFilters.at(i)->getDataSourceIdentifier(j)))) {
                loadCount++;
                //                done = true;
            }
        }
    }


    printDebugString("Element Config Load : " + QString().setNum(loadCount) + " successful for tab : " + pageName );

    //    uiPainter->start();

}

void TabController::resetConfig() {
    if(uiPainter !=0) {
        uiPainter->stop();
        delete uiPainter;
        uiPainter = 0;
    }
    uiPainter = new UiPainter(this);
}

void TabController::initHMIReadReq(QList<DataStructure *> dS) { printDebugString("initHMIReq "); if(cController != 0) cController->initExtReadRequest(dS);/*communicationController->initExtReadRequest("604");*/}

void TabController::initHMIWriteReq(QList<DataStructure *> dS, QList<QString> values) { printDebugString("initHMIWriteReq "); if(cController != 0) cController->initExtWriteRequest(dS, values);/*communicationController->initExtReadRequest("604");*/}

void TabController::setReadMode(QString continuousReadStatus) { if(continuousReadStatus == "n" || continuousReadStatus == "N") continuousRead = false; }

bool TabController::hasContinuousRead() { return continuousRead; }

void TabController::initTab(bool active) {

    if(active) {

        QList<DataStructure *> dS;

        // Fetch Element Data from target

        // CheckBox
        for(int i = 0; i < checkBox.count(); i++ )
            dS.append(checkBox.at(i)->getStructure());

        initHMIReadReq(dS);
        dS.clear();

        // inputField
        for(int i = 0; i < inputField.count(); i++ )
            dS.append(inputField.at(i)->getStructure());

        initHMIReadReq(dS);
        dS.clear();

        // spinBox
        for(int i = 0; i < spinBox.count(); i++ )
            dS.append(spinBox.at(i)->getStructure());

        initHMIReadReq(dS);
        dS.clear();

        // slideBar
        for(int i = 0; i < slideBar.count(); i++ )
            dS.append(slideBar.at(i)->getStructure());

        initHMIReadReq(dS);
        dS.clear();

        // RadioButton
        for(int i = 0; i < radioButton.count(); i++ )
            dS.append(radioButton.at(i)->getStructure());

        initHMIReadReq(dS);
        dS.clear();

        // AGCControl
        for(int i = 0; i < agc.count(); i++ )
            dS = agc.at(i)->getDataSource();    // Exception

        initHMIReadReq(dS);
        dS.clear();

        // AGCRF
        for(int i = 0; i < agcRf.count(); i++ )
            dS = agcRf.at(i)->getDataSource();    // Exception

        initHMIReadReq(dS);
        dS.clear();

        // AudioFilters
        for(int i = 0; i < audioFilters.count(); i++ )
            dS.append(audioFilters.at(i)->getStructure());

        initHMIReadReq(dS);
        dS.clear();

        /*
         * Fetch more element data here
         */

        QThread::currentThread()->msleep(200);

        // Update each element with new data

        // CheckBox
        for(int i = 0; i < checkBox.count(); i++ )
            checkBox.at(i)->updateEvent();

        // inputField
        for(int i = 0; i < inputField.count(); i++ )
            inputField.at(i)->updateEvent();

        // spinBox
        for(int i = 0; i < spinBox.count(); i++ )
            spinBox.at(i)->updateEvent();

        // slideBar
        for(int i = 0; i < slideBar.count(); i++ )
            slideBar.at(i)->updateEvent();

        // XYPlot
        for(int i = 0; i < xyPlot.count(); i++ )
            xyPlot.at(i)->updateEvent();

        //        // TimeValuePlot
        //        for(int i = 0; i < tVPlot.count(); i++ )
        //            tVPlot.at(i)->startPlot();

        // RadioButton
        for(int i = 0; i < radioButton.count(); i++ )
            radioButton.at(i)->updateEvent();

        // AGCControl
        for(int i = 0; i < agc.count(); i++ )
            agc.at(i)->updateEvent();

        // AGCRF
        for(int i = 0; i < agcRf.count(); i++ )
            agcRf.at(i)->updateEvent();

        // Audiofilter
        for(int i = 0; i < audioFilters.count(); i++ )
            audioFilters.at(i)->updateEvent();

        /*
         * Update more elements here
         */



        //        uiPainter->updateAll();

        uiPainter->start();

    } else {

        //        // TimeValuePlot
        //        for(int i = 0; i < tVPlot.count(); i++ )
        //            tVPlot.at(i)->stopPlot();

        if(uiPainter != 0)
            uiPainter->stop();
    }
}

/************************ Getters & Setters ************************/

void TabController::setPageSource(QString pageSource) { this->pageSource = pageSource; }

QWidget * TabController::getPageWidget() { return pageWidget; }

QString TabController::getPageName() { return pageName; }

void TabController::setDataManager(DataManager * dataManager) { this->dataManager = dataManager; }

void TabController::setCommunicationController(CommunicationController * cc) { printDebugString("communicationController set "); cController = cc;}

// void TabController::showPageWidget() { pageWidget->show(); }                 // Not used anymore.


// ***********************************************************************************************
