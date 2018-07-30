/*!
 * \file     bargraphh.h
 * \brief    Bar Graph Element Class
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

// BarGraph
#ifndef BARGRAPHH_H
#define BARGRAPHH_H

// Essential Headers
#include <QWidget>

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class BarGraphH;
}

/*!
 * \brief The BarGraphH class
 *
 * The BarGraphH can be used as Horizontal Level Bar.
 *
 * Vertical-align is also a levelBar. See #BarGraphV (To be implemented !)
 *
 */
class BarGraphH : public QWidget
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
    explicit BarGraphH(QWidget *parent = 0);

    //! Destructor
    ~BarGraphH();


    /************************ General Methods specific to this class. ************************/

    //! Initialize name and value
    void loadAttribute(QString title, double val = 0);

    //! Redraw the graph to show changes
    void repaint();

    //! Claculate percent of given \a value out of \var maxValue
    double getPercent(double value);

    //! Set new value to Bar. This function includes call to value conversion functions to calculate appropriate value. \see valueconversion.h
    /*!
     * update the \var value variable to the value given by \a val.
     */
    void setValue(double val);

    /************************ Initializer Methods used by XML parser. ************************/

    //! Show / Hide text on graph
    /*!
     * \param show = Y : Hide text from Bar
     */
    void setTextVisibility(char show = 'N');

    //! Show / Hide Level Bar
    /*!
     * \param show = N : Hide Bar
     */
    void setBarVisiblity(char show = 'Y');

    //! Setter : peakValue
    void setPeakValue(double);

    //! Setter : maxSourceValue
    /*!
     * \var maxSourceValue is the maximum value expected to be received from Targert.
     *
     *
     * <b> ** NOTE ** </b>
     * For most of the sensors in \b PERF3, it's value is \b 32767
     *
     * It is used for calculation purpose. \see convertCoeffForSensor_TimePlot() in valueconversion.h
     *
     */
    void setMaxValue(double value);

    //! Setter : minBarValue
    /*!
     * \var minBarValue is the minimum value that the bar can display.
     */
    void setMinBarValue(double value);

    //! Setter : maxBarValue
    /*!
    * \var maxBarValue is the maximum value that the bar can display.
    */
    void setMaxBarValue(double value);

    //! Setter : unit
    void setUnit(QString);

    //! Set the indentification name of BarGraphH to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString name);

    void setBarOrientation(QString);

    void setMinValue(double value);

    void setMultiplicatorValue(double value);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of BarGraphH
    /*!
     * It is used by #ConfigController to identify element.
     * \sa setNameIdentifier()
     */
    QString getNameIdentifier();

    //! Setter : \var dStructure \sa getStructure()
    bool setStructure(DataStructure *);
    //! Getter : \var dStructure \sa setStructure()
    DataStructure * getStructure();

    //! Setter : \var index (!! NOT IMPLEMENTED !!)
    void setIndex(int);
    //! Getter : \var index (!! NOT IMPLEMENTED !!)
    int getIndex();

    /************************ Methods used in connected state with other elements. ************************/

    /*
     *
     * Add methods here
     *
     */

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //

signals:
    /************************ Signals ************************/

    /*
     *
     * Add Signals here
     *
     */

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    // Slots for response to internal signals.

    //! Response to 'valueChanged(int)' action signal.
    void on_pBar_valueChanged(int);

    // Slots for response to external signals.

    //! Response to a signal from checkBox.
    /*!
     * Disable the BarGraphH
     */
    void disable();

    //! Response to a signal from checkBox.
    /*!
     * Enable the BarGraphH
     */
    void enable();

    //! Response to a signal from pushButton.
    /*!
     * Re-initialize the BarGraphH with values obtained from input sources
     */
    void refresh();

    //! Response to a signal from pushButton.
    /*!
     * Clear the BarGraphH
     */
    void clear();

    //! Response to a signal from slideBar.
    /*!
     * Re-initialize the BarGraphH with values obtained from slideBar
     */
    void updateValue(double);

    //! Response to a signal from slideBar.
    /*!
     * Re-initialize the BarGraphH with paekValue obtained from slideBar
     */
    void updatePeakValue(double);

    //! Response to a signal from uiPainter.
    /*!
     * Updates the element to latest changes.
     */
    void updateEvent();


    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:
    Ui::BarGraphH *ui;                      //!< Private Istance to access '.ui' file elements

    /************************ Element Attributes ************************/
    double value;                            //!< value to be displayed at graph
    double peakValue;                        //!< peakValue to be marked at graph.
    int index;                              //!< NOT IMPLEMENTED YET

    bool barVal0to1;


    /************************ Configuration ************************/
    QString nameIdentifier;                 //!< Element Configuration Name.

    /************************ Calculation ************************/
    double maxSourceValue;                   //!< maxValue expected to be received from Targert. It is used for calculation purpose. \see convertCoeffForSensor_TimePlot() in valueconversion.h
    double minSourceValue;
    double maxBarValue;
    double minBarValue;
    double multiplicatorValue;
    double barValue;

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;
};

//
#endif // BARGRAPHH_H
