/*!
 * \file     slidebar.h
 * \brief    SlideBar Element Class
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


// SlideBar
#ifndef SLIDEBAR_H
#define SLIDEBAR_H

// Essential Headers
#include <QWidget>
#include <QSlider>

// Conections
class BarGraphH;
class ProgressBarGraph;

// Parent
class TabController;

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class SlideBar;
}

/*!
 * \brief The SlideBar class
 */
class SlideBar : public QWidget
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
    explicit SlideBar(QString title = "sBar", TabController *parent = 0);
    //! Destructor
    ~SlideBar();

    /************************ General Methods specific to this class. ************************/

    //! Initialize name and value
    void loadAttribute(QString, int val = 0);

    //! Setter : value
    void setValue(int);

    /************************ Initializer Methods used by XML parser. ************************/

    //! Set the minimum value
    void setMinValue(int);

    //! enable slide bar
    void setEnable(QString);

    //! Set the steps between consecutive slide values
    void setStepValue(int);

    //! Setter : maxValue
    void setMaxValue(int);

    //! Set the indentification name of SlideBar to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString);

    //! Setter : outputType
    /*!
     * \param type : hex | float
     */
    void setOutputType(QString type);
    /************************ Getters & Setters ************************/

    //! Get the indentification name of SlideBar
    /*!
     * It is used by #ConfigController to identify element.
     * \sa setNameIdentifier()
     */
    QString getNameIdentifier();

    //! Getter : value
    int getValue();

    //! Setter : \var dStructure \sa getStructure()
    bool setStructure(DataStructure *);
    //! Getter : \var dStructure \sa setStructure()
    DataStructure * getStructure();

    //! Setter : \var index (!! NOT IMPLEMENTED !!)
    void setIndex(int);
    //! Getter : \var index (!! NOT IMPLEMENTED !!)
    int getIndex();

    /************************ Methods used in connected state with other elements. ************************/

    // Connection to barGraph
    //! Connect slideBar to barGraph element with given action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - setValue      :   set the value of BarGraph
     *  - setPeakValue  :   set the peakValue of BarGraph
     */
    void initConnection(BarGraphH *, QString);

    // Connection to progressBarGraph
    //! Connect slideBar to progressBarGraph element with given action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - setValue      :   set the value of ProgressBarGraph
     *  - setPeakValue  :   set the peakValue of ProgressBarGraph
     */
    void initConnection(ProgressBarGraph *, QString);

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/

    //! Signal to be transmitted when SlideBar value is changed.
    void mySignal(float);

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to 'valueChanged()' action signal.
    void on_slideBar_valueChanged(int value);

    //! Response to a signal from checkBox.
    /*!
     * Disable the slideBar
     */
    void disable();

    //! Response to a signal from checkBox.
    /*!
     * Enable the slideBar
     */
    void enable();

    //! Response to a signal from pushButton.
    /*!
     * Reset the slideBar to minimum value position
     */
    void reset();

    // *********************************************************************** //
    //                                                                         //
    //                           Public Slots                                  //
    //                                                                         //
    // *********************************************************************** //
public slots:
    //! Public Slot
    /*!
     * Updates the element to latest changes.
     */
    void updateEvent();
    void RD45();
    void Wave();
    void Default();



    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:
    Ui::SlideBar *ui;                   //!< Private Istance to access '.ui' file elements

    /************************ Element Attributes ************************/
    int value;                          //!< Value of element
    int MAX_VALUE;                      //!< Maximum value of element
    int MIN_VALUE;                      //!< Maximum value of element
    int index;                          //!< NOT IMPLEMENTED YET

    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */
    QString outputType;                 //!< Type Output to be displayed. \see setOutputType()

    /************************ Parent ************************/
    TabController * tabControllerParent;    //!< Pointer to parent class.

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;


};

#endif // SLIDEBAR_H
