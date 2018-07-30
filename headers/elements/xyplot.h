/*!
 * \file     xyplot.h
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

// XyPlot
#ifndef XYPLOT_H
#define XYPLOT_H

// Essential Headers
#include <QWidget>

// Related Elements
#include <QSlider>
#include <QMap>

// Conections
class InputField;

// Parent
class TabController;

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class XYPlot;
}

/*!
 * \brief The XYPlot class
 */
class XYPlot : public QWidget
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
    explicit XYPlot(TabController *parent = 0);
    //! Destructor
    ~XYPlot();

    /************************ General Methods specific to this class. ************************/

    void drawPlot();

    void clearPlot();

    int getOutputSourceIdentifierCount();

    QString getOutputSourceIdentifier(int);

    void resetOutputSource();

    // Linear Function Plot
    void plotCoordinates();

    void calculateCoordinates();

    void setCoordinates(double xValueMax, double yValueMax, double xValueStart, double yValueStart, double xValueStop, double yValueStop, double xValueMin, double yValueMin);

    void getCoordinates(QMap<QString, double> &coordinates);

    QList<int> calculateCoefficient();

    void adjustInputSliders();

    void updateOutputField();

    // Antenna

    void setAntennaDataSourceIdentifier(QString);

    QString getAntennaDataSourceIdentifier(int);

    int getAntennaDataSourceIdentifierCount();

    bool setAntennaDataSource(DataStructure *);

    void resetAntennaDataSource();

    void initAntennaBeam();

    void plotAntennaBeam();

    /************************ Initializer Methods used by XML parser. ************************/

    void setType(QString);

    // Linear Function Plot
    void setLinearPlotType(QString);

    void addInputField(QString);

    void addOutputField(QString);

    void setInputFieldParameters(QString, QString, QString);

    void setOutputFieldParameters(QString, QString, QString, QString, QString, QString);

    //! Set the indentification name of XYPlot to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString);


    //! Set the indentification name of XYPlot to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier2(const QString &value);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of XYPlot
    /*!
     * It is used by #ConfigController to identify element.
     * \sa setNameIdentifier()
     */
    QString getNameIdentifier();

    //! Get the indentification name of XYPlot
    /*!
     * It is used by #ConfigController to identify element.
     * \sa setNameIdentifier()
     */
    QString getNameIdentifier2() const;

    //! Setter : \var dStructure \sa getStructure()
    bool setStructure(DataStructure *);
    //! Getter : \var dStructure \sa setStructure()
    DataStructure * getStructure();

    //! Setter : \var dStructure \sa getStructure()
    bool setStructure2(DataStructure * structure);
    //! Getter : \var dStructure \sa setStructure()
    DataStructure *getStructure2() const;

    bool setOutputSourceStructure(DataStructure *);

    //! Setter : \var index (!! NOT IMPLEMENTED !!)
    void setIndex(int);
    //! Getter : \var index (!! NOT IMPLEMENTED !!)
    int getIndex();

    //! Setter : xAxisLabel
    void setXAxisLabel(QString);

    //! Setter : yAxisLabel
    void setYAxisLabel(QString);

    //! Setter : graphLabel
    void setGraphLabel(QString);
    //! Getter : graphLabel
    QString getGraphLabel();

    QString getGraphType();

    /************************ Methods used in connected state with other elements. ************************/


    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
    void recalculateBeforeSend();
signals:
    /************************ Signals ************************/
    void enableElements();

    void disableElements();


    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to a signal from checkBox.
    /*!
     * Disable the xyPlot
     */
    void disable();

    //! Response to a signal from checkBox.
    /*!
     * Enable the xyPlot
     */
    void enable();

    //! Response to 'refreshPlot' signal from pushButton.
    /*!
     * Redraw plot with updated source values
     */
    void on_refreshButton_clicked();

    //! Response to 'clearPlot' signal from pushButton.
    /*!
     * Clear plot and set initial values
     */
    void on_clearButton_clicked();

    void on_setButton_clicked();

    // Linear Function Plot
    void inputSliderValueChange(int);

    void on_checkBox_toggled(bool checked);

    void plotSensor();


    // *********************************************************************** //
    //                                                                         //
    //                           Public Slots                                  //
    //                                                                         //
    // *********************************************************************** //
public slots:
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
    Ui::XYPlot *ui;                     //!< Private Istance to access '.ui' file elements

    /************************ Element Attributes ************************/
    float xRangeMin;                           //!< Value for test plot
    float xRangeMax;                           //!< Value for test plot
    float yRangeMin;                           //!< Value for test plot
    float yRangeMax;                           //!< Value for test plot
    QString xAxisLabel;                 //!< x-Axis Label text
    QString yAxisLabel;                 //!< y-Axis Label text
    QString graphLabel;                 //!< graph Label text
    int index;                          //!< NOT IMPLEMENTED YET
    QString graphType;
    bool updateFlag;

    // Linear Function Plot
    QString linearPlotType;
    QList<int> coefficeintValues;
    double m_XValues[4];
    double m_YValues[4];
    QList<QSlider*> inputSlider;
    QList<InputField*> inputEditField;
    QList<InputField*> outputEditField;
    QList<QString> outputEditFieldOldValue;
    QMap<QString, int> outputRoleIndex;
    QMap<QString, int> inputRoleIndex;


    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */

    QString nameIdentifier2;                 /*!< Element name */

    QList<QString> antennaDataSourceIdentifier;

    /************************ Parent ************************/
    TabController * tabControllerParent; //!< Pointer to parent class.

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;

    DataStructure * dStructure2;

    QList<DataStructure *> antennaDataSource;


    double mAntennaDistance;
    double mCosTable[50];
    double mSinTable[50];
    double mThetaRe[50];
    double mThetaIm [50];
    double mDataX[50];
    double mDataY[50];
};

#endif // XYPLOT_H

/* ***************************************** The Information below is not updated since 20 Feb.
 * Properties:          By Designer
 *      Alignment
 *      size
 * Attributes
 *      label
 *      axis-labels
 *      value
 * Methods
 *      Load Atttributes
 *      setValue
 *      setLabel
 * Mode
 *      WriteOnly       User
 *      DisplayOnly
 * Temporary
 *
 */


// Getters & Setters

//    //! Setter : xMin
//    void setXRangeMin(float);

//    //! Setter : xMin
//    void setXRangeMax(float);

//    //! Setter : yMin
//    void setYRangeMin(float);

//    //! Setter : yMin
//    void setYRangeMax(float);
