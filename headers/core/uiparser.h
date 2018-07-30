/*!
 * \file     uiparser.h
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

// UiParser
#ifndef UIPARSER_H
#define UIPARSER_H

// Essential Headers
#include <QtXml/QtXml>
#include <QTextStream>
// #include <QUiLoader>

// Include Object Elements
class BarGraphH;
class ProgressBarGraph;
class SlideBar;
class XYPlot;
class InputField;
class Button;
class CheckBox;
class TimeValuePlot;
class Indicator;
class RadioButton;
class TextBox;
class SpinBox;
class AGCControl;
class AGCRF;
class Frames;
class AudioFilters;

// Temporary
#include "headers/elements/testelement.h"

class TabController;

/*!
 * \brief The XML file parser class
 */
class UiParser : public QWidget{
public:
    //! Constructor
    UiParser(TabController * parent = 0);
    //! Destructor
    ~UiParser();

    //! Parse the source xml file
    /*!
    * This method loads the input xml file and calls necessary methods to parse file.
    *
    * \param fileName : Input file name
    * \param level : Integer value to describe level type of input XML file.
    *
    */
    void initUiParser(QString, int level = 0);

    //! Test method to read any type of xml file. (Used only for debugging)
    void widgetDomDisplay(QDomNode);

    //! Level 1 xml file handler
    void level1XmlParser(QDomNode);

    //! Level 2 xml file handler
    void level2XmlParser(QDomNode);

    //! Level 1 xml file validator
    bool level1XmlValidator(QDomNode);

    // All methods below are : Data Collector Methods

    //! Tab page layout reader
    void tabWidgetGeometry(QDomNode);
    //! Tab page property reader
    void pageWidgetProperty(QDomNode);
    //! Frame reader
    void pageWidgetFrame(QDomNode);
    //! Layout reader
    void pageWidgetLayout(QDomNode);
    //! Multiple widget element reader
    void widgetParser(QDomNode);
    //! Widget connection reader. It uses connectionParser() method to read connections
    /*!
      \sa connectionParser()
    */
    void pageWidgetConnections(QDomNode);
    //! Widget connection Validator
    bool connectionValidator(QDomNode);
    //! Widget connection reader
    void connectionParser(QDomNode);

    // Tab Property Parser
    //! Tab page property (configuration) source reader
    void propertySourceParser(QDomNode);

    // Specific Widget Parsers
    //! LineEdit widget reader
    void widgetLineEdit(QDomNode, int);
    //! SpinBox widget reader
    void widgetSpinBox(QDomNode, int);
    //! CheckBox widget reader
    void widgetCheckBox(QDomNode, int);
    //! PushButton widget reader
    void widgetPushButton(QDomNode, int);
    //! SlideBar widget reader
    void widgetSlideBar(QDomNode, int);
    //! BarGraph widget reader
    void widgetBarGraph(QDomNode, int);
    //! ProgressBarGraph widget reader
    void widgetProgressBarGraph(QDomNode, int);
    //! XyPlot widget reader
    void widgetXyPlot(QDomNode, int);
    //! TimeValuePlot widget reader
    void widgetTimeValuePlot(QDomNode, int);
    //! Indicator widget reader
    void widgetIndicator(QDomNode, int);
    //! RadioButton widget reader
    void widgetRadioButton(QDomNode, int);
    //! TextBox widget reader
    void widgetTextBox(QDomNode, int);
    //! AGC widget reader
    void widgetAGC(QDomNode, int);
    //! AGCRF widget reader
    void widgetAGCRF(QDomNode, int);
    //! AudioFilters widget reader
    void widgetAudiofilters(QDomNode, int);


    // Connections Builders

    // CheckBox
    //! Connect Checkbox to other Elements
    void checkBoxConnections(QDomNode, int);
    //! Connect Checkbox to LineEdit Elements
    void connectCheckBoxLineEdit(QDomNode, int);
    //! Connect Checkbox to PushButton Elements
    void connectCheckBoxPushButton(QDomNode, int);
    //! Connect Checkbox to SlideBar Elements
    void connectCheckBoxSlideBar(QDomNode, int);
    //! Connect Checkbox to BarGraph Elements
    void connectCheckBoxBarGraph(QDomNode, int);
    //! Connect Checkbox to ProgressBarGraph Elements
    void connectCheckBoxProgressBarGraph(QDomNode, int);
    //! Connect Checkbox to XyPlot Elements
    void connectCheckBoxXyPlot(QDomNode, int);
    //! Connect Checkbox to TimeValuePlot Elements
    void connectCheckBoxTimeValuePlot(QDomNode, int);
    // void connectCheckBoxFrame(QDomNode, int);

    // PushButton
    //! Connect PushButton to other Elements
    void pushButtonConnections(QDomNode, int);
    //! Connect PushButton to LineEdit Elements
    void connectPushButtonLineEdit(QDomNode, int);
    //! Connect PushButton to SpinBox Elements
    void connectPushButtonSpinBox(QDomNode, int);
    //! Connect PushButton to SlideBar Elements
    void connectPushButtonSlideBar(QDomNode, int);
    //! Connect PushButton to BarGraph Elements
    void connectPushButtonBarGraph(QDomNode, int);
    //! Connect PushButton to ProgressBarGraph Elements
    void connectPushButtonProgressBarGraph(QDomNode, int);
    //! Connect PushButton to XyPlot Elements
    void connectPushButtonXyPlot(QDomNode, int);

    // SlideBar
    //! Connect SlideBar to other Elements
    void slideBarConnections(QDomNode, int);
    //! Connect SlideBar to ProgressBarGraph Elements
    void connectSlideBarProgressBarGraph(QDomNode, int);   // Not levelBar


    // BarGraph
    //! Connect BarGraph to other Elements
    void barGraphConnections(QDomNode, int);

    // ProgressBarGraph
    //! Connect ProgessBarGraph to other Elements
    void progressBarGraphConnections(QDomNode, int);

    // XyPlot
    //! Connect XyPlot to other Elements
    void xyPlotConnections(QDomNode, int);

    //TimeValuePlot
    //! Connect TimeValuePlot to other Elements
    void timeValuePlotConnections(QDomNode, int);

    //Indicator
    //! Connect Indicator to other Elements
    void indicatorConnections(QDomNode, int);

    //RadioButton
    //! Connect RadioButton to other Elements
    void radioButtonConnections(QDomNode, int);
    //! Connect RadioButton to TimeValuePlot Elements
    void connectRadioButtonTimeValuePlot(QDomNode, int);



    // Getters & Setters

    //! Reset all UiParser class parameters
    void resetAllParameters();

    //! Getter : tabsCount
    int getTabsCount();

    //! Getter : tabSource
    QString getTabSource(int);

    //! Getter : tabGeometry
    int getTabGeometry(int);

    //! Getter : tabPageName
    QString getTabPageName();

    //! Getter : is Tab common ?
    QString getTabCommon();

    //! Getter : Tab has continuous read mode ?
    QString getTabReadMode();

    //! Getter : configSource
    QString getConfigSource();

    // Element Load Status Descriptors

    //! Check if path to .cfg is provided
    /*!
     *
     *  OBSOLETE :: NOT USED ANYMORE
     *
      \return The .cfg file status
    */
    bool isConfigSourceFound();

private:

    bool level1ErrorFlag;
    bool level2ErrorFlag;
    bool parsingSuccess;                            //!< NOT IMPLEMENTED YET
    int tabsCount;                                  //!< Number of Tabs to be created
    int tempTabsCount;
    int tempTotalTabsCount;
    QList<QString> tabSource;                       //!< Array of : path to individual Tab Page source XML file (Level 2 XML); Used by #CentralWidgetController ;
    QList<QString> tabGeometry;                     //!< Width & Height of tab widget
    QString tabPageName;                            //!< Title of tab widget
    QString tabCommon;                              //!< Flag for Common Tab
    QString tabReadMode;                            //!< Flag for Tab Read Mode
    QList<QString> configSource;                    //!< Array of : path to .cfg files
    QString error;                                  //!< Error information collector (NOT used yet)
    QString inputFileName;

    TabController * tabControllerParent;
    QTextStream layoutStream;

    // Below are the elements that can be loaded by this parser.

    //    LineEdit
private:
    QList<InputField*> inputField;                  //!< Array of : pointer to #InputField element
public:
    //! Getter : inputField
    InputField * getInputField(int);
    //! Getter : No of inputField
    int getInputFieldCount();

    //    SpinBox
private:
    QList<SpinBox*> spinBox;                  //!< Array of : pointer to #SpinBox element
public:
    //! Getter : spinBox
    SpinBox * getSpinBox(int);
    //! Getter : No of spinBox
    int getSpinBoxCount();

    //    CheckBox
private:
    QList<CheckBox *> checkBox;                     //!< Array of : pointer to #CheckBox element
public:
    //! Getter : checkBox
    CheckBox * getCheckBox(int);
    //! Getter : No of checkBox
    int getCheckBoxCount();

    //    PushButton
private:
    QList<Button *> pushButton;                     //!< Array of : pointer to #Button element
public:
    //! Getter : PushButton
    Button * getPushButton(int);
    //! Getter : No of PushButton
    int getPushButtonCount();

    //    SlideBar
private:
    QList<SlideBar *> slideBar;                     //!< Array of : pointer to #SlideBar element
public:
    //! Getter : SlideBar
    SlideBar * getSlideBar(int);
    //! Getter : No of SlideBar
    int getSlideBarCount();

    //    BarGraph
private:
    //QList<BarGraph *> barGraph;                     //!< Array of : pointer to #BarGraph element
    QList<BarGraphH *> barGraph;                     //!< Array of : pointer to #BarGraphH element
public:
    //! Getter : BarGraph
    //BarGraph * getBarGraph(int);
    BarGraphH * getBarGraph(int);
    //! Getter : No of BarGraph
    int getBarGraphCount();

    //    ProgressBarGraph
private:
    QList<ProgressBarGraph *> pbarGraph;                     //!< Array of : pointer to #ProgressBarGraph element
public:
    //! Getter : BarGraph
    ProgressBarGraph * getProgressBarGraph(int);
    //! Getter : No of ProgressBarGraph
    int getProgressBarGraphCount();

    //    XYPlot
private:
    QList<XYPlot *> xyPlot;                         //!< Array of : pointer to #XYPlot element
public:
    //! Getter : XYPlot
    XYPlot * getXyPlot(int);
    //! Getter : No of XYPlot
    int getXyPlotCount();

    //    TimeValuePlot
private:
    QList<TimeValuePlot *> tVPlot;                         //!< Array of : pointer to #TimeValuePlot element
public:
    //! Getter : TimeValuePlot
    TimeValuePlot * getTimeValuePlot(int);
    //! Getter : No of TimeValuePlot
    int getTimeValuePlotCount();

    //    Indicator
private:
    QList<Indicator *> indicator;                         //!< Array of : pointer to #Indicator element
public:
    //! Getter : Indicator
    Indicator * getIndicator(int);
    //! Getter : No of Indicator
    int getIndicatorCount();

    //    RadioButton
private:
    QList<RadioButton *> rButton;                         //!< Array of : pointer to #RadioButton element
public:
    //! Getter : RadioButton
    RadioButton * getRadioButton(int);
    //! Getter : No of RadioButton
    int getRadioButtonCount();

    //    TextBox
private:
    QList<TextBox *> textBox;                         //!< Array of : pointer to #TextBox element
public:
    //! Getter : TextBox
    TextBox * getTextBox(int);
    //! Getter : No of TextBox
    int getTextBoxCount();

    //    AGCControl
private:
    QList<AGCControl *> agc;                         //!< Array of : pointer to #AGCControl element
public:
    //! Getter : AGC
    AGCControl * getAGC(int);
    //! Getter : No of AGCControl
    int getAGCCount();

    //    AGCRF
private:
    QList<AGCRF *> agcRf;                         //!< Array of : pointer to #AGCRF element
public:
    //! Getter : AGCRF
    AGCRF * getAGCRF(int);
    //! Getter : No of AGCRF
    int getAGCRFCount();

    //    Frames
private:
    QList<Frames *> frames;                         //!< Array of : pointer to #Frames element
public:
    //! Getter : Frames
    Frames * getFrames(int i);
    //! Getter : No of Frames
    int getFramesCount();


private:
    QList<AudioFilters *> audioFilters;                         //!< Array of : pointer to #AudioFilters element
public:
    //! Getter : AudioFilters
    AudioFilters * getaudioFilters(int i);
    //! Getter : No of Frames
    int getaudioFiltersCount();


    // Temporary
private:
    TestElement * tElement;                         //!< Pointer to #TestElement element (Used for debugging)

};


#endif // UIPARSER_H
