/*!
 * \file     uiparser.cpp
 * \brief    XML file parser class
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
#include <headers/core/uiparser.h>
#include <QFile>

// #include "headers/bargraph.h"
#include "headers/elements/bargraphh.h"
#include "headers/elements/progressbargraph.h"
#include "headers/elements/slidebar.h"
#include "headers/elements/xyplot.h"
#include "headers/elements/inputfield.h"
#include "headers/elements/button.h"
#include "headers/elements/checkbox.h"
#include "headers/elements/timevalueplot.h"
#include "headers/elements/indicator.h"
#include "headers/elements/radiobutton.h"
#include "headers/elements/textbox.h"
#include "headers/elements/agccontrol.h"
#include "headers/elements/agcrf.h"
#include "headers/elements/spinbox.h"
#include "headers/elements/frames.h"
#include "headers/core/errorlogger.h"
#include "headers/elements/audiofilters.h"

// Text Handler
#include <QStringList>

UiParser::UiParser(TabController * parent)
    : QWidget((QWidget*)parent)
{
    tabControllerParent = parent;
    level1ErrorFlag = false;
    level2ErrorFlag = false;

    resetAllParameters();
}


// Destructor
UiParser::~UiParser() {

    tElement = 0;           // TEMP

    printDebugString("Parser Deleted");
}

// Parse the source xml file.
/*
* fileName : Input file name
* level : Integer value to describe level type of input XML file.
* level = 0 : Used for debugging only
* level = 1 : Highest level XML
* level = 2 : Level 2 XML
*
*/
void UiParser::initUiParser(QString fileName, int level) {

    QDomDocument doc(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

    file.close();

    inputFileName = fileName;

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();  // Container Node

    switch (level) {
    case 0:
        widgetDomDisplay(n);
        break;
    case 1:
        if(level1XmlValidator(n) && !level1ErrorFlag) level1XmlParser(n);
        else { reportError("Invalid level1 Xml :: " + fileName);
            reportError("Error Reason :: XML file (.xml) specified in Project file (.proj) is not Level 1 XML file. << or >> Error in XML file");
            reportError("Please verify that the XML resources selected while creating a project are Level 1 XML files. ");
            reportError("Solution : ");
            reportError("Verify .proj file :: For Line Containing : " + fileName);
            reportError("Refer :: User Manual : How to create a project. ");
            reportError("Refer :: User Manual : How to create Level 1 XML file.");
            criticalLevelErrorFlag = true;
        }
        break;
    case 2:
        level2XmlParser(n);
        break;

    }

    inputFileName.clear();


}

// Test method to read any type of xml file. (Used only for debugging)
void UiParser::widgetDomDisplay(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString(qPrintable(e1.tagName()));

            if(n1.firstChild().isNull()) {
                printDebugString(" : ", true);
                printDebugString(e1.text(), true);
            }
            printDebugString("");

            if(e1.hasAttributes()) {
                QDomNamedNodeMap a1 = e1.attributes();

                printDebugString("Attribute : ");
                printDebugString(QString().setNum(a1.count()), true);
            }
        }

        widgetDomDisplay(n1);   // move to if

        n = n.nextSibling();

    }
}

// Level 1 xml file validator.
bool UiParser::level1XmlValidator(QDomNode n) { // Container Node

    // static int tabCount = 0;
    // static int totalTabs = 0;

    while(!n.isNull()) {        

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

                printDebugString(e1.tagName());
                printDebugString("Validating!");

                if (e1.tagName() == "geometry") {

                    n1 = e1.firstChild();

                    while(!n1.isNull()) {                    // Inside Geometry
                        QDomElement e1 = n1.toElement();

                        printDebugString(e1.tagName());
                        // ... Do stuff here

                        n1 = n1.nextSibling();
                    }

                    if(true) {                              // Error condition
                        // ... Do stuff here
                        // return false;
                    }
                } else if (e1.tagName() == "totalTabs") {

                    tempTotalTabsCount = e1.text().simplified().toInt();

                }
                /*
                 * Add more parameters collector here.
                 */

                if (e1.tagName() == "tab") {
                    // Count number of tabs
                    while(!n.isNull()) {                    // At Tab

                        tempTabsCount++;

                        n = n.nextSibling();
                    }

                    printDebugString("tab Count : ");
                    printDebugString(QString().setNum(tempTabsCount), true);
                    printDebugString("totalTabs : ");
                    printDebugString(QString().setNum(tempTotalTabsCount), true);

                    if(tempTabsCount < tempTotalTabsCount) {              // Error condition ( Change to != )
                        return false;
                    }
                }
                /*
                 * Add more verification parameters here.
                 */

            if(!level1XmlValidator(n1)) return false;       // Recurrsion
        }
        n = n.nextSibling();

    }

    if(tempTabsCount == 0 || tempTotalTabsCount == 0)
        level1ErrorFlag = true;
    else
        level1ErrorFlag = false;

//    printDebugString("-----------------------------------------------------------------------------------------------------");
//    printDebugStringInLine("tab Count : ");
//    printDebugString(QString().setNum(tempTabsCount));
//    printDebugStringInLine("totalTabs : ");
//    printDebugString(QString().setNum(tempTotalTabsCount));
//    printDebugString("-----------------------------------------------------------------------------------------------------");

    return true;
}

// Level 1 xml file handler.
void UiParser::level1XmlParser(QDomNode n) {


    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            QString str = e1.tagName();

            if(str == "geometry") {
                tabWidgetGeometry(n1);
            }

            if(str == "totalTabs") {
                tabsCount = e1.text().toInt();
            }

            if(str == "source") {
                tabSource.append(e1.text().simplified());
            }

            level1XmlParser(n1);

        }

        n = n.nextSibling();

    }
}

// Tab page layout reader
void UiParser::tabWidgetGeometry(QDomNode n) {


    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "width") { tabGeometry.append(e1.text().simplified()); }
            else if(e1.tagName() == "height") { tabGeometry.append(e1.text().simplified()); }
            /*
             * Read tabwidget Geometry here.
             */

            tabWidgetGeometry(n1);
        }

        n = n.nextSibling();

    }
}

// Level 2 xml file handler
void UiParser::level2XmlParser(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();  //pageWidget // property

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            /* ****************************************************************** Redundant Section */
            if(e1.hasAttributes()) {
                QDomNamedNodeMap a1 = e1.attributes();
                printDebugString("Attribute : " + QString().setNum(a1.count()), false);    // out << "Attribute Name : " << e1.attribute("name") << endl;
                printDebugString(e1.attribute("type"), false);
                printDebugString(e1.attribute("name"), false);
            }
            /* ************************************************************************************ */


            if(e1.tagName() == "property"){ pageWidgetProperty(n1); }
            else if(e1.tagName() == "frame"){
                printDebugString("FRAME SIBLING :: " + e1.tagName() + " _________________________________ " );
                frames.append(new Frames());
                //
                if(e1.tagName() == "frame" && e1.hasAttributes()) {

                    if (e1.hasAttribute("name")) { frames.last()->setFrameTitle(e1.attribute("name").simplified()); }
                    if (e1.hasAttribute("index")) { bool ok =false; frames.last()->setIndex(e1.attribute("index").simplified().toInt(&ok)); }
                    /*
                     * Add more widget config parameters here.
                     */
                }
                //
                pageWidgetFrame(n1);
            }
            else if(e1.tagName() == "connections"){ pageWidgetConnections(n1); }
            else if(e1.tagName() == "layout"){ // Output .cfg file

                QString outputDir = "layout";

                QDir dir(outputDir);
                if (!dir.exists()) {
                    dir.mkpath(".");
                }

                QString fileName = tabPageName + ".lay";
                QFile outputFile(outputDir+"/"+fileName);

                if (!outputFile.open(QIODevice::WriteOnly|QFile::WriteOnly))
                {
                    // QMessageBox::warning(0,"Could not create Project File",
                    // QObject::tr( "\n Could not create Project File on disk"));
                    reportError("Could not create Layout File: " + outputDir + "/" + fileName);

                }
                else {
                    // Output File Handler
                    QIODevice *outDevice = &outputFile;
                    //                             QTextStream outFileStream(outDevice);
                    layoutStream.setDevice(outDevice);

                    // outFileStream << "#	module	name	address	size	rw	value	dataType" << endl;    // Config file header
                    pageWidgetLayout(n1);
                    // out << "Config Line Written : " << count << " lines" << endl;

                    // for(int i = 0; i < dataManager->getCount(); i++){
                    // {
                    // outFileStream << dataManager->getStructure(i) << endl;    // Insert data into output file
                    // count ++;
                    // }

                    // }
                    // out << "Read ALL :: " << layoutStream.readAll();

                    outputFile.close();

                }
                //
            }
            /*
             * Add more page widget parameters here.
             */
            else
                level2XmlParser(n1);

        }

        n = n.nextSibling();

    }

}

/* ***************************************  Property  ********************************************** */
/*
 * The section below is used to set the properties of given tab page.
 *
 * *************************************************************************************************** */

// Tab page property reader
void UiParser::pageWidgetProperty(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "source"){ propertySourceParser(n); }    // Caution here its n, not n1
            else if(e1.tagName() == "title"){ tabPageName = e1.text().simplified(); }
            else if(e1.tagName() == "isCommon"){ tabCommon = e1.text().simplified(); }
            else if(e1.tagName() == "continuousRead"){ tabReadMode = e1.text().simplified(); }
            /*
             * Read more page widget properties here.
             */


            else
                pageWidgetProperty(n1);

        }

        n = n.nextSibling();

    }

}

// Tab page property (configuration) source reader
void UiParser::propertySourceParser(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString( " : " + e1.text(), true); }
            printDebugString("");

            if(e1.tagName() == "config" && !e1.text().isEmpty() && e1.text().simplified() != "0") {
                configSource.append(e1.text().simplified());
            }
            /*
             * Add more source parameters here.
             */

            else
                propertySourceParser(n1);
        }

        n = n.nextSibling();
    }

}

/* ***************************************  Layout  ********************************************** */
/*
 * The section below is used to collect layout information
 *
 * *************************************************************************************************** */

// Tab page property reader
void UiParser::pageWidgetLayout(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

//            out << qPrintable(e1.tagName());
//            if(n1.firstChild().isNull()) { out << " : " << e1.text(); }
//            out << endl;

            if(e1.tagName() == "frame"){ printDebugString( e1.tagName().simplified() + "\t" + e1.attribute("index", "-1") + "\t" + e1.attribute("row", "-1") + "\t" + e1.attribute("col", "-1") + "\t" + e1.attribute("rs", "1") + "\t" + e1.attribute("cs", "1") + "\t" + e1.attribute("none", "BREAK"));
            layoutStream << e1.tagName().simplified() << "\t" << e1.attribute("index", "-1") << "\t" << e1.attribute("row", "-1") << "\t" << e1.attribute("col", "-1") << "\t" << e1.attribute("rs", "1") << "\t" << e1.attribute("cs", "1") << "\t" << e1.attribute("none", "BREAK") << endl;}
            else { printDebugString( e1.tagName().simplified() + "\t" + e1.attribute("index", "-1") + "\t" + e1.attribute("r", "-1") + "\t" + e1.attribute("c", "-1") + "\t" + e1.attribute("rs", "-1") + "\t" + e1.attribute("cs", "-1") );
            layoutStream << e1.tagName().simplified() << "\t" << e1.attribute("index", "-1") << "\t" << e1.attribute("r", "-1") << "\t" << e1.attribute("c", "-1") << "\t" << e1.attribute("rs", "-1") << "\t" << e1.attribute("cs", "-1") << endl;}
//            else if(e1.tagName() == "checkBox"){ out << e1.tagName().simplified() << e1.attribute("index", "-1") << e1.attribute("r", "-1") << e1.attribute("rs", "-1") << e1.attribute("c", "-1") << e1.attribute("cs", "-1") << endl; }




            pageWidgetLayout(n1);
        }

        n = n.nextSibling();
//        if (n.toElement().tagName() == "layout")
//            break;

    }
}

/* ***************************************  Widgets  ********************************************** */
/*
 * The section below is used to set the parameters for defined elements
 *
 * *************************************************************************************************** */

// Frame reader
void UiParser::pageWidgetFrame(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString( " : " + e1.text(), true); }
            printDebugString("");

            if(e1.tagName() == "widget"){ widgetParser(n); }    // Caution here its n, not n1
            /*
             * Add more frame parameters here.
             */
            else
                pageWidgetFrame(n1);
        }

        n = n.nextSibling();
    }
}

// Multiple widget element reader
void UiParser::widgetParser(QDomNode n) {

    if(!n.isNull()) {

        QDomElement e1 = n.toElement();

        if(!e1.isNull()) {
            if(e1.hasAttributes()) {
                printDebugString( e1.attribute("type") );

                // Instantiate widget element and load the fetched data

                if (e1.attribute("type").simplified() == "lineEdit") {
                    inputField.append(new InputField(tabControllerParent));
                    widgetLineEdit(n, (inputField.count() - 1) ); }
                else if (e1.attribute("type").simplified() == "spinBox") {
                    spinBox.append(new SpinBox(tabControllerParent));
                    widgetSpinBox(n, (spinBox.count() - 1) ); }
                else if (e1.attribute("type") == "checkBox") {
                    checkBox.append(new CheckBox(tabControllerParent));
                    widgetCheckBox(n, (checkBox.count() - 1) ); }
                else if (e1.attribute("type") == "pushButton") {
                    pushButton.append(new Button(tabControllerParent));
                    widgetPushButton(n, (pushButton.count() - 1) ); }
                else if (e1.attribute("type") == "slideBar") {
                    slideBar.append(new SlideBar("sBar", tabControllerParent));
                    widgetSlideBar(n, (slideBar.count() - 1) ); }
                else if (e1.attribute("type") == "levelBar") {
                    barGraph.append(new BarGraphH()); //barGraph.append(new BarGraph("bGraph"));
                    widgetBarGraph(n, (barGraph.count() - 1) ); }
                else if (e1.attribute("type") == "progressBar") {
                    pbarGraph.append(new ProgressBarGraph("pGraph"));
                    widgetProgressBarGraph(n, (pbarGraph.count() - 1) );
                }
                else if (e1.attribute("type") == "xyPlot") {
                    xyPlot.append(new XYPlot(tabControllerParent));
                    widgetXyPlot(n, (xyPlot.count() - 1) ); }
                else if (e1.attribute("type") == "timeValuePlot") {
                    tVPlot.append(new TimeValuePlot());
                    widgetTimeValuePlot(n, (tVPlot.count() - 1) ); }
                else if (e1.attribute("type") == "indicator") {
                    indicator.append(new Indicator());
                    widgetIndicator(n, (indicator.count() - 1) ); }
                else if (e1.attribute("type") == "radioButton") {
                    rButton.append(new RadioButton(tabControllerParent));
                    widgetRadioButton(n, (rButton.count() - 1) ); }
                else if (e1.attribute("type") == "textBox") {
                    textBox.append(new TextBox());
                    widgetTextBox(n, (textBox.count() - 1) ); }
                else if (e1.attribute("type") == "agc") {
                    agc.append(new AGCControl(tabControllerParent));
                    widgetAGC(n, (agc.count() - 1) ); }
                else if (e1.attribute("type") == "agcRf") {
                    agcRf.append(new AGCRF(tabControllerParent));
                    widgetAGCRF(n, (agcRf.count() - 1) ); }
                else if (e1.attribute("type") == "audioFilters") {
                    audioFilters.append(new AudioFilters(tabControllerParent));
                    widgetAudiofilters(n, (audioFilters.count() - 1) ); }
                /*
                 * Add more elements here.
                 */
                else {
                    reportError("ERROR :: In File #" + inputFileName + " At line(" + QString().setNum(n.lineNumber()) + ") : Widget Type " + e1.attribute("type") + " not found");

                }
            }
        }
    }

}

// LineEdit widget reader
void UiParser::widgetLineEdit(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            // Setting Widget Properties in LineEdit
            if(e1.tagName() == "labelName") { inputField.at(widgetNumber)->setValueLabel(e1.text().simplified()); }
            else if(e1.tagName() == "mask") { inputField.at(widgetNumber)->setMask(e1.text().simplified()); }
            else if(e1.tagName() == "defaultValue") { inputField.at(widgetNumber)->setDefaultValue(e1.text().simplified()); }
            else if(e1.tagName() == "placeHolder") { inputField.at(widgetNumber)->setPlaceHolder(e1.text().simplified()); }
            else if(e1.tagName() == "mode") { inputField.at(widgetNumber)->setMode(e1.text().simplified()); }
            else if(e1.tagName() == "output") { inputField.at(widgetNumber)->setOutputType(e1.text().simplified()); }
            else if(e1.tagName() == "halfFormatInterpretation") { inputField.at(widgetNumber)->setHalfFormatInterpretation(e1.text().simplified()); }
            else if(e1.tagName() == "disabledValue") { inputField.at(widgetNumber)->setDisabledValue(e1.text().simplified()); }
            /*
             * Add more widget parameters here.
             */

            // Setting Widget Config Properties in LineEdit
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        inputField.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());

                    }
                    if (e1.hasAttribute("index")) { bool ok; inputField.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }

            widgetLineEdit(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// SpinBox widget reader
void UiParser::widgetSpinBox(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            // Setting Widget Properties in SpinBox
            if(e1.tagName() == "labelName") { spinBox.at(widgetNumber)->setLabelName(e1.text().simplified()); }
            else if(e1.tagName() == "halfFormatInterpretation") { spinBox.at(widgetNumber)->setHalfFormatInterpretation(e1.text().simplified()); }
            else if(e1.tagName() == "disabledValue") { spinBox.at(widgetNumber)->setDisabledValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "lowerRange") { spinBox.at(widgetNumber)->setSpinBoxLowerRange(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "upperRange") { spinBox.at(widgetNumber)->setSpinBoxUpperRange(e1.text().simplified().toDouble()); }
            /*
             * Add more widget parameters here.
             */

            // Setting Widget Config Properties in SpinBox
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        spinBox.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());
                    }
                    if (e1.hasAttribute("index")) { bool ok; spinBox.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }

            widgetSpinBox(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// CheckBox widget reader
void UiParser::widgetCheckBox(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { checkBox.at(widgetNumber)->setCheckBoxTitle(e1.text().simplified()); }
            else if(e1.tagName() == "state") { checkBox.at(widgetNumber)->setCheckBoxState(e1.text().simplified()); }
            else if(e1.tagName() == "checkedValue") { bool ok; checkBox.at(widgetNumber)->setCheckedValue(e1.text().simplified().toInt(&ok, 16)); }
            else if(e1.tagName() == "uncheckedValue") { bool ok; checkBox.at(widgetNumber)->setUncheckedValue(e1.text().simplified().toInt(&ok, 16)); }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in CheckBox
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        checkBox.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());


                    }
                    if (e1.hasAttribute("index")) { bool ok; checkBox.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }

            widgetCheckBox(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// PushButton widget reader
void UiParser::widgetPushButton(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { pushButton.at(widgetNumber)->setButtonTitle(e1.text().simplified()); }
            else if(e1.tagName() == "function") { pushButton.at(widgetNumber)->setButtonFunction(e1.text().simplified()); }
            else if(e1.tagName() == "value") { bool ok; pushButton.at(widgetNumber)->setResetValue(e1.text().simplified().toInt(&ok, 16)); }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in PushButton
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        pushButton.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());


                    }
                    if (e1.hasAttribute("index")) { bool ok; pushButton.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetPushButton(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// SlideBar widget reader
void UiParser::widgetSlideBar(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { slideBar.at(widgetNumber)->loadAttribute(e1.text().simplified()); }
            else if(e1.tagName() == "state") { slideBar.at(widgetNumber)->setEnable(e1.text().simplified()); }
            else if(e1.tagName() == "value") { slideBar.at(widgetNumber)->setValue(e1.text().simplified().toInt()); }
            else if(e1.tagName() == "maxValue") { slideBar.at(widgetNumber)->setMaxValue(e1.text().simplified().toInt()); }
            else if(e1.tagName() == "minValue") { slideBar.at(widgetNumber)->setMinValue(e1.text().simplified().toInt()); }
            else if(e1.tagName() == "stepValue") { slideBar.at(widgetNumber)->setStepValue(e1.text().simplified().toInt()); }
            else if(e1.tagName() == "output") { slideBar.at(widgetNumber)->setOutputType(e1.text().simplified()); }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in SlideBar
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        slideBar.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());

                    }
                    if (e1.hasAttribute("index")) { bool ok; slideBar.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetSlideBar(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// BarGraph widget reader
void UiParser::widgetBarGraph(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { barGraph.at(widgetNumber)->loadAttribute(e1.text().simplified()); }
            else if(e1.tagName() == "value") { barGraph.at(widgetNumber)->setValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "showText") { barGraph.at(widgetNumber)->setTextVisibility(e1.text().simplified().at(0).toLatin1()); }
            else if(e1.tagName() == "maxValue") { barGraph.at(widgetNumber)->setMaxValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "minValue") { barGraph.at(widgetNumber)->setMinValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "minBarValue") { barGraph.at(widgetNumber)->setMinBarValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "maxBarValue") { barGraph.at(widgetNumber)->setMaxBarValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "multiplicatorValue") { barGraph.at(widgetNumber)->setMultiplicatorValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "peakValue") { barGraph.at(widgetNumber)->setPeakValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "orientation") { barGraph.at(widgetNumber)->setBarOrientation(e1.text().simplified()); }
            else if(e1.tagName() == "unit") { barGraph.at(widgetNumber)->setUnit(e1.text().simplified()); }
            else if(e1.tagName() == "showBar") { barGraph.at(widgetNumber)->setBarVisiblity(e1.text().simplified().at(0).toLatin1()); }  // barVisiblity
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in BarGraph
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        barGraph.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());


                    }
                    if (e1.hasAttribute("index")) { bool ok; barGraph.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetBarGraph(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// ProgressBarGraph widget reader
void UiParser::widgetProgressBarGraph(QDomNode n, int widgetNumber) {


    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { pbarGraph.at(widgetNumber)->loadAttribute(e1.text().simplified()); }
            else if(e1.tagName() == "value") { pbarGraph.at(widgetNumber)->setValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "showText") { pbarGraph.at(widgetNumber)->setTextVisibility(e1.text().simplified().at(0).toLatin1()); }
            else if(e1.tagName() == "maxBarValue") { pbarGraph.at(widgetNumber)->setMaxBarValue(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "unit") { pbarGraph.at(widgetNumber)->setUnit(e1.text().simplified()); }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in ProgressBarGraph
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        pbarGraph.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());


                    }
                    if (e1.hasAttribute("index")) { bool ok; pbarGraph.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetProgressBarGraph(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// XyPlot widget reader
void UiParser::widgetXyPlot(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { xyPlot.at(widgetNumber)->setGraphLabel(e1.text().simplified()); }
//            else if(e1.tagName() == "xMin") { xyPlot.at(widgetNumber)->setXRangeMin(e1.text().simplified().toDouble()); }
//            else if(e1.tagName() == "xMax") { xyPlot.at(widgetNumber)->setXRangeMax(e1.text().simplified().toDouble()); }
//            else if(e1.tagName() == "yMin") { xyPlot.at(widgetNumber)->setYRangeMin(e1.text().simplified().toDouble()); }
//            else if(e1.tagName() == "yMax") { xyPlot.at(widgetNumber)->setYRangeMax(e1.text().simplified().toDouble()); }
            else if(e1.tagName() == "type") { xyPlot.at(widgetNumber)->setType(e1.text().simplified()); }
            else if(e1.tagName() == "linearPlotType") { xyPlot.at(widgetNumber)->setLinearPlotType(e1.text().simplified()); }
            else if(e1.tagName() == "xLabel") { xyPlot.at(widgetNumber)->setXAxisLabel(e1.text().simplified()); }
            else if(e1.tagName() == "yLabel") { xyPlot.at(widgetNumber)->setYAxisLabel(e1.text().simplified()); }
            else if(e1.tagName() == "output") {
                xyPlot.at(widgetNumber)->addOutputField(e1.text().simplified());

                QString name = e1.attribute("name", "DEFAULTVALUE").simplified();
                QString output = e1.attribute("output", "DEFAULTVALUE").simplified();
                QString disabledValue = e1.attribute("disabledValue", "DEFAULTVALUE").simplified();
                QString resetValue = e1.attribute("resetValue", "RESETVALUE").simplified();
                QString mode = e1.attribute("mode", "DEFAULTVALUE").simplified();
                QString role = e1.attribute("role", "DEFAULTVALUE").simplified();

                xyPlot.at(widgetNumber)->setOutputFieldParameters(name, output, disabledValue, mode, resetValue, role);
            }
            else if(e1.tagName() == "input") {
                xyPlot.at(widgetNumber)->addInputField(e1.text().simplified());

                QString valueType = e1.attribute("valueType", "DEFAULTVALUE").simplified();
                QString slider = e1.attribute("slider", "DEFAULTVALUE").simplified();
                QString role = e1.attribute("role", "DEFAULTVALUE").simplified();

                xyPlot.at(widgetNumber)->setInputFieldParameters(valueType, slider, role);
            }
            else if(e1.tagName() == "antennaDataSource") {

                QString W0_R = e1.attribute("W0_R", "DEFAULTVALUE").simplified();
                QString W0_I = e1.attribute("W0_I", "DEFAULTVALUE").simplified();
                QString W1_R = e1.attribute("W1_R", "DEFAULTVALUE").simplified();
                QString W1_I = e1.attribute("W1_I", "DEFAULTVALUE").simplified();

                xyPlot.at(widgetNumber)->setAntennaDataSourceIdentifier(W0_R);
                xyPlot.at(widgetNumber)->setAntennaDataSourceIdentifier(W0_I);
                xyPlot.at(widgetNumber)->setAntennaDataSourceIdentifier(W1_R);
                xyPlot.at(widgetNumber)->setAntennaDataSourceIdentifier(W1_I);
            }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in xyPlot
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) { xyPlot.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified()); }
                    if (e1.hasAttribute("name2")) { xyPlot.at(widgetNumber)->setNameIdentifier2(e1.attribute("name2").simplified()); }
                    if (e1.hasAttribute("index")) { bool ok; xyPlot.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
            }


            widgetXyPlot(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// TimeValuePlot widget reader
void UiParser::widgetTimeValuePlot(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { tVPlot.at(widgetNumber)->setGraphLabel(e1.text().simplified()); }
            else if(e1.tagName() == "xLabel") { tVPlot.at(widgetNumber)->setXLabel(e1.text().simplified()); }
            else if(e1.tagName() == "yLabel") { tVPlot.at(widgetNumber)->setYLabel(e1.text().simplified()); }
            else if(e1.tagName() == "height") { bool ok; tVPlot.at(widgetNumber)->setGraphHeight(e1.text().simplified().toInt(&ok)); }
            else if(e1.tagName() == "width") { bool ok; tVPlot.at(widgetNumber)->setGraphWidth(e1.text().simplified().toInt(&ok)); }
            else if(e1.tagName() == "yMax") { bool ok; tVPlot.at(widgetNumber)->setYMax(e1.text().simplified().toInt(&ok)); }
            else if(e1.tagName() == "yMin") { bool ok; tVPlot.at(widgetNumber)->setYMin(e1.text().simplified().toInt(&ok)); }
            else if(e1.tagName() == "minValue") { tVPlot.at(widgetNumber)->setMinValue(e1.text().simplified()); }
            else if(e1.tagName() == "scaleFactor") { bool ok; tVPlot.at(widgetNumber)->setScaleFactor(e1.text().simplified().toInt(&ok)); }
            else if(e1.tagName() == "halfFormatInterpretation") { tVPlot.at(widgetNumber)->setHalfFormatInterpretation(e1.text().simplified()); }
            else if(e1.tagName() == "showWidget") { tVPlot.at(widgetNumber)->setVisiblity(e1.text().simplified()); }
            else if(e1.tagName() == "output") { tVPlot.at(widgetNumber)->setOutputType(e1.text().simplified()); }
            else if(e1.tagName() == "dataSource") {
                tVPlot.at(widgetNumber)->setDataSourceIdentifier(e1.attribute("name", "DEFAULT").simplified());
                tVPlot.at(widgetNumber)->setDataSourceOutputType(e1.attribute("output", "DEFAULT").simplified());
                tVPlot.at(widgetNumber)->setDataSourceHalfFormatInterpretation(e1.attribute("halfFormatInterpretation", "DEFAULT").simplified());
                tVPlot.at(widgetNumber)->setDataSourceMinValue(e1.attribute("minValue", "DEFAULT").simplified());
                tVPlot.at(widgetNumber)->setDataSourceModes(e1.attribute("modes", "DEFAULT").simplified());
            }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in tVPlot
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        tVPlot.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());


                    }
                    if (e1.hasAttribute("index")) { bool ok; tVPlot.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetTimeValuePlot(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// Indicator widget reader
void UiParser::widgetIndicator(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { indicator.at(widgetNumber)->setLabelName(e1.text().simplified()); }
            else if(e1.tagName() == "activeLabel") { indicator.at(widgetNumber)->setActiveStateLabel(e1.text().simplified()); }
            else if(e1.tagName() == "progressiveLabel") { indicator.at(widgetNumber)->setProgressiveStateLabel(e1.text().simplified()); }
            else if(e1.tagName() == "inactiveLabel") { indicator.at(widgetNumber)->setInactiveStateLabel(e1.text().simplified()); }
            else if(e1.tagName() == "defaultColor") { indicator.at(widgetNumber)->setNoneStateColour(e1.text().simplified()); }
            else if(e1.tagName() == "activeColor") { indicator.at(widgetNumber)->setActiveStateColour(e1.text().simplified()); }
            else if(e1.tagName() == "progressiveColor") { indicator.at(widgetNumber)->setProgressiveStateColour(e1.text().simplified()); }
            else if(e1.tagName() == "inactiveColor") { indicator.at(widgetNumber)->setInactiveStateColour(e1.text().simplified()); }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in indicator
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        indicator.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());

                    }
                    if (e1.hasAttribute("index")) { bool ok; indicator.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetIndicator(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// RadioButton widget reader
void UiParser::widgetRadioButton(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { rButton.at(widgetNumber)->setLabelName(e1.text().simplified()); }
            else if(e1.tagName() == "button") {
                rButton.at(widgetNumber)->addNewButton(e1.text().simplified());
                QString value = e1.attribute("value", "DEFAULT").simplified();
                QString isEnable = e1.attribute("enable", "Y").simplified();
                QString isSelected = e1.attribute("select", "N").simplified();
                rButton.at(widgetNumber)->setButtonAttributes(value, isEnable, isSelected);
            }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in RadioButton
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        rButton.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());
                    }
                    if (e1.hasAttribute("index")) { bool ok; rButton.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetRadioButton(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// TextBox widget reader
void UiParser::widgetTextBox(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { textBox.at(widgetNumber)->setLabelName(e1.text().simplified()); }
//            else if(e1.tagName() == "output") { textBox.at(widgetNumber)->setOutputType(e1.text().simplified()); }
            else if(e1.tagName() == "boxVisible") { textBox.at(widgetNumber)->isBoxVisible(e1.text().simplified()); }
            else if(e1.tagName() == "dataSource") {
                textBox.at(widgetNumber)->setDataSourceIdentifier(e1.attribute("name", "DEFAULT").simplified());
            }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in textBox
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                    if (e1.hasAttribute("name")) {
                        textBox.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());


                    }
                    if (e1.hasAttribute("index")) { bool ok; textBox.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                    /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetTextBox(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// AGCControl widget reader
void UiParser::widgetAGC(QDomNode n, int widgetNumber) {


    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { agc.at(widgetNumber)->setLabelName(e1.text().simplified()); }
            else if(e1.tagName() == "mode") {
                agc.at(widgetNumber)->selectBand(e1.text().simplified());
                agc.at(widgetNumber)->initParameterRange(e1.text().simplified());
            }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in agc
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                if (e1.hasAttribute("name")) {
                    agc.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());

                }
                if (e1.hasAttribute("index")) { bool ok; agc.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetAGC(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}

// AGCRF widget reader
void UiParser::widgetAGCRF(QDomNode n, int widgetNumber) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "labelName") { agcRf.at(widgetNumber)->setLabelName(e1.text().simplified()); }
            else if(e1.tagName() == "mode") {
                agcRf.at(widgetNumber)->selectBand(e1.text().simplified());
                agcRf.at(widgetNumber)->initParameterRange(e1.text().simplified());
            }
            /*
             * Add more widget parameters here.
             */
            // Setting Widget Config Properties in agcRf
            else if(e1.tagName() == "widget" && e1.hasAttributes()) {
                // if(e1.hasAttributes()) {
                if (e1.hasAttribute("name")) {
                    agcRf.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified());

                }
                if (e1.hasAttribute("index")) { bool ok; agcRf.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
                /*
                     * Add more widget config parameters here.
                     */
                // }
            }


            widgetAGCRF(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;

    }
}
void UiParser::widgetAudiofilters(QDomNode n, int widgetNumber)
{
    while(!n.isNull())
    {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull())
        {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "widget" && e1.hasAttributes())
            {
                if (e1.hasAttribute("name")) { audioFilters.at(widgetNumber)->setNameIdentifier(e1.attribute("name").simplified()); }
                if (e1.hasAttribute("index")) { bool ok; audioFilters.at(widgetNumber)->setIndex(e1.attribute("index").simplified().toInt(&ok, 10)); }
            }

            widgetAudiofilters(n1, widgetNumber);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
        break;

    }


}

// Getters & Setters

int UiParser::getInputFieldCount() {
    return inputField.count();
}

InputField * UiParser::getInputField(int i) {
    return inputField.at(i);
}

int UiParser::getSpinBoxCount() {
    return spinBox.count();
}

SpinBox * UiParser::getSpinBox(int i) {
    return spinBox.at(i);
}

int UiParser::getCheckBoxCount() {
    return checkBox.count();
}

CheckBox * UiParser::getCheckBox(int i) {
    return checkBox.at(i);
}

int UiParser::getPushButtonCount() {
    return pushButton.count();
}

Button * UiParser::getPushButton(int i) {
    return pushButton.at(i);
}

int UiParser::getSlideBarCount() {
    return slideBar.count();
}

SlideBar * UiParser::getSlideBar(int i) {
    return slideBar.at(i);
}

int UiParser::getBarGraphCount() {
    return barGraph.count();
}

//BarGraph * UiParser::getBarGraph(int i) {
BarGraphH * UiParser::getBarGraph(int i) {
    return barGraph.at(i);
}

int UiParser::getProgressBarGraphCount() {
    return pbarGraph.count();
}

ProgressBarGraph * UiParser::getProgressBarGraph(int i) {
    return pbarGraph.at(i);
}

int UiParser::getXyPlotCount() {
    return xyPlot.count();
}

XYPlot * UiParser::getXyPlot(int i) {
    return xyPlot.at(i);
}

int UiParser::getTimeValuePlotCount() {
    return tVPlot.count();
}

TimeValuePlot * UiParser::getTimeValuePlot(int i) {
    return tVPlot.at(i);
}

int UiParser::getIndicatorCount() {
    return indicator.count();
}

Indicator * UiParser::getIndicator(int i) {
    return indicator.at(i);
}


int UiParser::getRadioButtonCount() {
    return rButton.count();
}

RadioButton * UiParser::getRadioButton(int i) {
    return rButton.at(i);
}

int UiParser::getTextBoxCount() {
    return textBox.count();
}

TextBox * UiParser::getTextBox(int i) {
    return textBox.at(i);
}

int UiParser::getAGCCount() {
    return agc.count();
}

AGCControl * UiParser::getAGC(int i) {
    return agc.at(i);
}

int UiParser::getAGCRFCount() {
    return agcRf.count();
}

AGCRF * UiParser::getAGCRF(int i) {
    return agcRf.at(i);
}

int UiParser::getFramesCount() {
    return frames.count();
}
Frames *UiParser::getFrames(int i) {
    return frames.at(i);
}

int UiParser::getaudioFiltersCount() {
    return audioFilters.count();
}
AudioFilters *UiParser::getaudioFilters(int i) {
    return audioFilters.at(i);
}

/* ***************************************  Connections  ********************************************* */
/*
 * The section below is used to define connections of signal and their respective actions
 * for each element.
 *
 * *************************************************************************************************** */

// Widget connection reader
void UiParser::pageWidgetConnections(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "widget"){
                if(connectionValidator(n)) { connectionParser(n); }
                else { printDebugString( "Invalid connection!" ); break;}
            }    // Caution here its n, not n1
            else
                pageWidgetConnections(n1);
        }

        n = n.nextSibling();

    }

}

// Widget connection Validator
bool UiParser::connectionValidator(QDomNode n) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {
            if(e1.hasAttributes()) {
                printDebugString( e1.attribute("type") );
                printDebugString( "Validating!");

                if (e1.attribute("type").simplified() == "xyPlot") {
                    if(e1.attribute("index").simplified().toInt() >= xyPlot.count())    // check index
                        return false;
                } /*else if (e1.attribute("type").simplified() == "timeValuePlot") {
                    if(e1.attribute("index").simplified().toInt() >= tVPlot.count())    // check index
                        return false;
                } */else if (e1.attribute("type").simplified() == "indicator") {
                    if(e1.attribute("index").simplified().toInt() >= indicator.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "radioButton") {
                    if(e1.attribute("index").simplified().toInt() >= rButton.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "textBox") {
                    if(e1.attribute("index").simplified().toInt() >= textBox.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "agc") {
                    if(e1.attribute("index").simplified().toInt() >= agc.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "agcRf") {
                    if(e1.attribute("index").simplified().toInt() >= agcRf.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "pushButton") {
                    if(e1.attribute("index").simplified().toInt() >= pushButton.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "lineEdit") {
                    if(e1.attribute("index").simplified().toInt() >= inputField.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "spinBox") {
                    if(e1.attribute("index").simplified().toInt() >= spinBox.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "checkBox") {
                    if(e1.attribute("index").simplified().toInt() >= checkBox.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "slideBar") {
                    if(e1.attribute("index").simplified().toInt() >= slideBar.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "levelBar") {
                    if(e1.attribute("index").simplified().toInt() >= barGraph.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "progressBar") {
                    if(e1.attribute("index").simplified().toInt() >= pbarGraph.count())    // check index
                        return false;
                } else if (e1.attribute("type").simplified() == "audioFilters") {
                    if(e1.attribute("index").simplified().toInt() >= audioFilters.count())    // check index
                        return false;
                }
                /*
                 * Add more connector type here.
                 */

                if (e1.tagName() == "lineEdit") {
                    if(e1.attribute("index0").simplified().toInt() >= inputField.count())    // check index
                        return false;
                    else if(e1.attribute("index1").simplified().toInt() >= inputField.count())    // check index
                        return false;
                } else if (e1.tagName() == "spinBox") {
                    if(e1.attribute("index").simplified().toInt() >= spinBox.count())    // check index
                        return false;
                } else if (e1.tagName() == "xyPlot") {
                    if(e1.attribute("index").simplified().toInt() >= xyPlot.count())    // check index
                        return false;
                } /*else if (e1.tagName() == "timeValuePlot") {
                    if(e1.attribute("index").simplified().toInt() >= tVPlot.count())    // check index
                        return false;
                }*/ else if (e1.tagName() == "indicator") {
                    if(e1.attribute("index").simplified().toInt() >= indicator.count())    // check index
                        return false;
                } else if (e1.tagName() == "radioButton") {
                    if(e1.attribute("index").simplified().toInt() >= rButton.count())    // check index
                        return false;
                } else if (e1.tagName() == "textBox") {
                    if(e1.attribute("index").simplified().toInt() >= textBox.count())    // check index
                        return false;
                } else if (e1.tagName() == "agc") {
                    if(e1.attribute("index").simplified().toInt() >= agc.count())    // check index
                        return false;
                } else if (e1.tagName() == "agcRf") {
                    if(e1.attribute("index").simplified().toInt() >= agcRf.count())    // check index
                        return false;
                } else if (e1.tagName() == "pushButton") {
                    if(e1.attribute("index").simplified().toInt() >= pushButton.count())    // check index
                        return false;
                } else if (e1.tagName() == "checkBox") {
                    if(e1.attribute("index").simplified().toInt() >= checkBox.count())    // check index
                        return false;
                } else if (e1.tagName() == "slideBar") {
                    if(e1.attribute("index").simplified().toInt() >= slideBar.count())    // check index
                        return false;
                } else if (e1.tagName() == "levelBar") {
                    if(e1.attribute("index").simplified().toInt() >= barGraph.count())    // check index
                        return false;
                } else if (e1.tagName() == "progressBar") {
                    if(e1.attribute("index").simplified().toInt() >= pbarGraph.count())    // check index
                        return false;
                }else if (e1.tagName() == "audioFilters") {
                    if(e1.attribute("index").simplified().toInt() >= audioFilters.count())    // check index
                        return false;
                }
                /*
                 * Add more elements here.
                 */
            }
            if(!connectionValidator(n1)) return false;
        }
        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }

    return true;
}

// Widget connection reader
void UiParser::connectionParser(QDomNode n) {

    if(!n.isNull()) {

        QDomElement e1 = n.toElement();

        if(!e1.isNull()) {
            if(e1.hasAttributes()) {
                printDebugString( e1.attribute("type"));

                // if (e1.attribute("type").simplified() == "lineEdit") { /* No connections Exist */ }
                // else if (e1.attribute("type").simplified() == "spinBox") { /* No connections Exist */ }
                if (e1.attribute("type").simplified() == "checkBox") {
                    checkBoxConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type").simplified() == "pushButton") {
                    pushButtonConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type") == "slideBar") {
                    slideBarConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type") == "levelBar") {
                    barGraphConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type") == "progressBar") {
                    progressBarGraphConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type").simplified() == "xyPlot") {
                    xyPlotConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type").simplified() == "timeValuePlot") {
                    timeValuePlotConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type").simplified() == "indicator") {
                    indicatorConnections(n, e1.attribute("index").simplified().toInt());
                }
                else if (e1.attribute("type").simplified() == "radioButton") {
                    radioButtonConnections(n, e1.attribute("index").simplified().toInt());
                }
                /*
                 * Add more widget connectors here.
                 */
            }
        }
    }
}

// CheckBox Connections

// Connect Checkbox to other Elements
void UiParser::checkBoxConnections(QDomNode n, int index) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "lineEdit") {                // Connect to LineEdit
                connectCheckBoxLineEdit(n, index);
            }
            else if(e1.tagName() == "pushButton") {         // Connect to PushButton
                connectCheckBoxPushButton(n, index);
            }
            else if(e1.tagName() == "slideBar") {           // Connect to slideBar
                connectCheckBoxSlideBar(n, index);
            }
            else if(e1.tagName() == "progressBar") {           // Connect to progressBarGraph
                connectCheckBoxProgressBarGraph(n, index);
            }
            else if( (e1.tagName() == "levelBar") ||
                     (e1.tagName() == "barGraph")) {           // Connect to barGraph
                connectCheckBoxBarGraph(n, index);
            }
            else if(e1.tagName() == "xyPlot") {             // Connect to xyPlot
                connectCheckBoxXyPlot(n, index);
            }
            else if(e1.tagName() == "timeValuePlot") {      // Connect to timeValuePlot
                connectCheckBoxTimeValuePlot(n, index);
            }
            else if(e1.tagName() == "frame") {              // Connect to frame
                // connectCheckBoxFrame(n, index);
            }
            /*
             * Add more elements here.
             */

            checkBoxConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }

}

// Connect Checkbox to LineEdit Elements
void UiParser::connectCheckBoxLineEdit(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            checkBox.at(i)->initConnection(inputField.at(index), action);
            printDebugString("Loading checkBox - lineEdit " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect Checkbox to PushButton Elements
void UiParser::connectCheckBoxPushButton(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            checkBox.at(i)->initConnection(pushButton.at(index), action);
            printDebugString( "Loading checkBox - pushButton " + QString().setNum(index) + " Connections");
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect Checkbox to SlideBar Elements
void UiParser::connectCheckBoxSlideBar(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            checkBox.at(i)->initConnection(slideBar.at(index), action);
            printDebugString( "Loading checkBox - slideBar " + QString().setNum( index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect Checkbox to ProgressBarGraph Elements
void UiParser::connectCheckBoxProgressBarGraph(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            checkBox.at(i)->initConnection(pbarGraph.at(index), action);
            printDebugString("Loading checkBox - pbarGraph " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect Checkbox to BarGraph Elements
void UiParser::connectCheckBoxBarGraph(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            checkBox.at(i)->initConnection(barGraph.at(index), action);
            printDebugString( "Loading checkBox - barGraph " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect Checkbox to XyPlot Elements
void UiParser::connectCheckBoxXyPlot(QDomNode n, int i) {


    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            checkBox.at(i)->initConnection(xyPlot.at(index), action);
            printDebugString( "Loading checkBox - xyPlot " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect Checkbox to TimeValuePlot Elements
void UiParser::connectCheckBoxTimeValuePlot(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            checkBox.at(i)->initConnection(tVPlot.at(index), action);
            printDebugString("Loading checkBox - TimeValuePlot " + QString().setNum(index) + " Connections");
        }
        /*
         * Add more connection parameters here.
         */
    }
}


// PushButton Connections

// Connect PushButton to other Elements
void UiParser::pushButtonConnections(QDomNode n, int index) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "lineEdit") {        // Connect to LineEdit
                connectPushButtonLineEdit(n, index);
            }
            else if(e1.tagName() == "spinBox") {        // Connect to SpinBox
                connectPushButtonSpinBox(n, index);
            }
            else if(e1.tagName() == "slideBar") {           // Connect to slideBar
                connectPushButtonSlideBar(n, index);
            }
            else if(e1.tagName() == "progressBar") {         // Connect to pbarGraph
                connectPushButtonProgressBarGraph(n, index);
            }
            else if( (e1.tagName() == "levelBar") ||
                     (e1.tagName() == "barGraph")) {           // Connect to barGraph
                connectPushButtonBarGraph(n, index);
            }
            else if(e1.tagName() == "xyPlot") {     // Connect to XYPlot
                connectPushButtonXyPlot(n, index);
            }
            /*
             * Add more elements here.
             */

            pushButtonConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }

}

// Connect PushButton to LineEdit Elements
void UiParser::connectPushButtonLineEdit(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            pushButton.at(i)->initConnection(inputField.at(index), action);
            printDebugString( "Loading pushButton - lineEdit " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect PushButton to SpinBox Elements
void UiParser::connectPushButtonSpinBox(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            pushButton.at(i)->initConnection(spinBox.at(index), action);
            printDebugString("Loading pushButton - spinBox " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect PushButton to SlideBar Elements
void UiParser::connectPushButtonSlideBar(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            pushButton.at(i)->initConnection(slideBar.at(index), action);
            printDebugString( "Loading pushButton - slideBar " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect PushButton to ProgressBarGraph Elements
void UiParser::connectPushButtonProgressBarGraph(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            pushButton.at(i)->initConnection(pbarGraph.at(index), action);
            printDebugString("Loading pushButton - pbarGraph " + QString().setNum(index) + " Connections");
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect PushButton to BarGraph Elements
void UiParser::connectPushButtonBarGraph(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            pushButton.at(i)->initConnection(barGraph.at(index), action);
            printDebugString("Loading pushButton - barGraph " + QString().setNum(index) + " Connections");
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Connect PushButton to XyPlot Elements
void UiParser::connectPushButtonXyPlot(QDomNode n, int i) {

      QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            pushButton.at(i)->initConnection(xyPlot.at(index), action);
            printDebugString("Loading pushButton - xyPlot " + QString().setNum(index) + " Connections");
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// SlideBar Connections

// Connect SlideBar to other Elements
void UiParser::slideBarConnections(QDomNode n, int index) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if( (e1.tagName() == "levelBar") ||
                    (e1.tagName() == "barGraph")) {           // Connect to barGraph
                // connectSlideBarBarGraph(n, index);
            } else if( e1.tagName() == "progressBar") {           // Connect to progressBarGraph
                connectSlideBarProgressBarGraph(n, index);
            }

            /*
             * Add more elements here.
             */

            slideBarConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }

}

// Connect SlideBar to ProgressBarGraph Elements
void UiParser::connectSlideBarProgressBarGraph(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            slideBar.at(i)->initConnection(pbarGraph.at(index), action);
            printDebugString( "Loading slideBar - pbarGraph " + QString().setNum(index) + " Connections");
        }
        /*
             * Add more connection parameters here.
             */
    }
}

// BarGraph Connections

// Connect BarGraph to other Elements
void UiParser::barGraphConnections(QDomNode n, int index) {


    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            if(e1.tagName() == "testElement") {      // Connect to testElement
                tElement = new TestElement(this);
                connect(tElement, SIGNAL(mySignal(float)),barGraph.at(index), SLOT(updateValue(float)));
            }
            /*
             * Add more elements here.
             */

            barGraphConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }

}

// ProgressBarGraph Connections

// Connect ProgressBarGraph to other Elements
void UiParser::progressBarGraphConnections(QDomNode n, int index) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");


            // To be modified for burner
            if(e1.tagName() == "lineEdit") {        // Connect to LineEdit

            } else if(e1.tagName() == "testElement") {
                tElement = new TestElement(this);
                connect(tElement, SIGNAL(mySignal(float)),pbarGraph.at(index), SLOT(updateValue(float)));
            }
            /*
             * Add more elements here.
             */

            progressBarGraphConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }

}

// XYPlot Connections

// Connect XyPlot to other Elements
void UiParser::xyPlotConnections(QDomNode n, int index) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "") {        // Connect to ""
            }
            /*
             * Add more elements here.
             */

            xyPlotConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }
}

// TimeValuePlot Connections

// Connect TimeValuePlot to other Elements
void UiParser::timeValuePlotConnections(QDomNode n, int index) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "lineEdit") {        // Connect to LineEdit
            }
            /*
             * Add more elements here.
             */

            timeValuePlotConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }
}

// Indicator Connections

// Connect Indicator to other Elements
void UiParser::indicatorConnections(QDomNode n, int index) {

    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "") {        // Connect to ""
            }
            /*
             * Add more elements here.
             */

            indicatorConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }
}

// RadioButton Connections

// Connect RadioButton to other Elements
void UiParser::radioButtonConnections(QDomNode n, int index) {


    while(!n.isNull()) {

        QDomElement e1 = n.toElement();

        QDomNode n1 = e1.firstChild();

        if(!e1.isNull()) {

            printDebugString( qPrintable(e1.tagName()));
            if(n1.firstChild().isNull()) { printDebugString(" : " + e1.text(), true); }
            printDebugString( "");

            if(e1.tagName() == "timeValuePlot") {      // Connect to timeValuePlot
                connectRadioButtonTimeValuePlot(n, index);
            }
            /*
             * Add more elements here.
             */

            radioButtonConnections(n1, index);
        }

        n = n.nextSibling();
        if (n.toElement().tagName() == "widget")
            break;
    }
}

// Connect RadioButton to TimeValuePlot Elements
void UiParser::connectRadioButtonTimeValuePlot(QDomNode n, int i) {

    QDomElement e1 = n.toElement();

    if(!e1.isNull()) {

        int index = e1.attribute("index").simplified().toInt();
        QString action = e1.attribute("action").simplified();

        if(!action.isEmpty()) {
            rButton.at(i)->initConnection(tVPlot.at(index), action);
            printDebugString( "Loading radioButton - TimeValuePlot " + QString().setNum(index) + " Connections" );
        }
        /*
         * Add more connection parameters here.
         */
    }
}

// Getters & Setters

int UiParser::getTabsCount() { return tabsCount; }

QString UiParser::getTabSource(int i) { return tabSource.at(i); }

int UiParser::getTabGeometry(int i) { return tabGeometry.at(i).toInt(); }

QString UiParser::getTabPageName() { return tabPageName; }

QString UiParser::getTabCommon() { return tabCommon; }

QString UiParser::getTabReadMode() { return tabReadMode; }

QString UiParser::getConfigSource() { if (configSource.count()) return configSource.at(0); else return 0; }

bool UiParser::isConfigSourceFound() { if (configSource.count()) return true; else return false; }

void UiParser::resetAllParameters() {
    tabsCount = 0;
    tempTabsCount = 0;
    tempTotalTabsCount = 0;
    tabSource.clear();
    tabGeometry.clear();
    tabPageName.clear();
    tabCommon.clear();
    tabReadMode.clear();
}
