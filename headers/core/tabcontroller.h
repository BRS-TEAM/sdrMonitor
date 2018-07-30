/*!
 * \file     tabcontroller.h
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

// TabController
#ifndef TABCONTROLLER_H
#define TABCONTROLLER_H

// Essential Headers
#include <QWidget>

// Elements
class BarGraphH;
class ProgressBarGraph;
class SlideBar;
class XYPlot;
class InputField;
class SpinBox;
class Button;
class CheckBox;
class TimeValuePlot;
class Indicator;
class RadioButton;
class TextBox;
class AGCControl;
class AGCRF;
class Frames;
class AudioFilters;

// Parser
class UiParser;

// GUI Painter
class UiPainter;

// Data Manager
class DataManager;

// Communication Controller
class CommunicationController;

class QFrame;

// Data Structure
#include "headers/core/datastructure.h"

/*!
 * \brief Controller class to manage Individual Tab Pages
 *
 * It saves all the data collected bu #UiParser and keeps reference of all the widgets created during parsing process.
 */
class TabController: public QWidget
{
    // Essential object needed by class which have its own private slots
     Q_OBJECT


    // *********************************************************************** //
    //                                                                         //
    //                           Public Methods                                //
    //                                                                         //
    // *********************************************************************** //
public:
    //! Constructor
    TabController(QWidget * parent = 0);
    //! Destructor
    ~TabController();

    /*              UI              */

    //! Parse the LEVEL 2 XML file.
    /*!
    * This method initialize #UiParser to to load LEVEL 2 XML files.
    */
    void pageWidgetUiParser();

    //! Store data collected by #UiParser. Build tab page UI by using designLoader() method.
    /*!
      \sa designLoader()
    */
    void generateUi();

    //! Build tab page layout.
    /*!
    * Returns the build success status
    *
    * The MATRIX Layout Design Algorithm is defined here.
    *
    */
    bool designLoader();

    //! Set the page as Common Sensor Dock type or Main Dock Type.
    /*!
    * \param status : if status = y, then the Tab is marked as COMMON SENSOR DOCK type.
    */
    void setCommon(QString status);

    //! Check if the Tab is Common
    /*!
    * Returns true if the page is common.
    */
    bool isCommon();

    /*              Config              */

    //! Load configuration.
    /*!
     * Load Configuration Information (#DataStructure) to their respective elements.
     */
    void loadConfig();

    //! Reset Configuration Setting
    void resetConfig();

    /*              Communication              */

    //! Perform Coefficient Read Operation for given list of #DataStructures \a dS
    /*!
     * This function can be called by Generic Elements to interact with #CommunicationController.
     */
    void initHMIReadReq(QList<DataStructure *> dS);

    //! Perform Coefficient Write Operation for given list of #DataStructures \a dS
    /*!
     * This function can be called by Generic Elements to interact with #CommunicationController.
     */
    void initHMIWriteReq(QList<DataStructure *> dS, QList<QString> values);

    //! Set read mode of Tab
    /*!
     * \param continuousReadStatus : If it is 'N' then the Tab is excluded from Sampling Operation
     *
     * By default a Tab is always included in sampling operation.
     */
    void setReadMode(QString continuousReadStatus);

    //! Returns true if the Tab has Continuous Read Mode, otherwise false.
    bool hasContinuousRead();

    /*              Real Time Operation              */

    //! Initialize Tab
    /*!
     * This method is called each time when a Tab is selected / changed.
     *
     * When a Tab is enabled / active, It is regularly updated bu UiPainter and its coefficients are included in Sampling Process.
     *
     * \param active : If active = true, the Tab is enabled / activated.
     */
    void initTab(bool active);

    /******************************** Getters & Setters **************************************/

    //! Setter : \a pageSource
    void setPageSource(QString pageSource);

    //! Getter : \a pageWidget
    QWidget * getPageWidget();

    //! Getter : \a pageName
    QString getPageName();

    //! Getter : \a dataManager
    void setDataManager(DataManager * dataManager);

    //! Setter : \a cController
    /*!
     * A reference of #CommunicationController is given to each #TabController to allow its elements to make read/write request.
     */
    void setCommunicationController(CommunicationController * cc);

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/
    void startPaint();
    void stopPaint();

    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:

    /************************ Class Attributes ************************/
    QString pageSource;                     //!< Path to Tab Page source XML file (Level 2 XML)
    QString pageName;                       //!< Title of Tab Page
    QWidget * pageWidget;                   //!< Pointer to Tab Page Widget
    bool pageLoaded;                        //!< Page load status
    bool common;
    bool continuousRead;
    // bool configFound;                       //!< Config file search status

    UiParser * pageParser;                  //!< XML reader. (Private Istance of #UiParser)
    DataManager * dataManager;
    UiPainter * uiPainter;

    QFrame * containerFrame;


    // ConfigController * configController;    //!< CFG reader. (Private Istance of #ConfigController)

    // List of All possible elements
    QList<InputField*> inputField;          //!< Array of : pointer to #InputField element
    QList<SpinBox*> spinBox;                //!< Array of : pointer to #SpinBox element
    QList<CheckBox*> checkBox;              //!< Array of : pointer to #CheckBox element
    QList<Button*> pushButton;              //!< Array of : pointer to #Button element
    QList<SlideBar*> slideBar;              //!< Array of : pointer to #SlideBar element
    // QList<BarGraph*> barGraph;              //!< Array of : pointer to #BarGraph element
    QList<BarGraphH*> barGraph;              //!< Array of : pointer to #BarGraph element
    QList<ProgressBarGraph*> pbarGraph;              //!< Array of : pointer to #ProgressBarGraph element
    QList<XYPlot*> xyPlot;                  //!< Array of : pointer to #XYPlot element
    QList<TimeValuePlot*> tVPlot;                  //!< Array of : pointer to #TimeValuePlot element
    QList<Indicator*> indicator;                  //!< Array of : pointer to #Indicator element
    QList<RadioButton*> radioButton;                  //!< Array of : pointer to #RadioButton element
    QList<TextBox*> textBox;                  //!< Array of : pointer to #TextBox element
    QList<AGCControl*> agc;                  //!< Array of : pointer to #AGCControl element
    QList<AGCRF*> agcRf;                  //!< Array of : pointer to #AGCRF element
    QList<Frames*> frames;                  //!< Array of : pointer to #XYPlot element
    QList<AudioFilters *> audioFilters;                         //!< Array of : pointer to #AudioFilters element
    /*
     * Add more elements here.
     */
    CommunicationController * cController;
};


#endif // TABCONTROLLER_H
