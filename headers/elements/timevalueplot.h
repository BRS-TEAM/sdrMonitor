/*!
 * \file     timevalueplot.h
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


// TimeValuePlot
#ifndef TIMEVALUEPLOT_H
#define TIMEVALUEPLOT_H

// Essential Headers
#include <QWidget>
#include <QLabel>

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class TimeValuePlot;
}

/*!
 * \brief The TimeValuePlot class
 */
class TimeValuePlot : public QWidget
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
    explicit TimeValuePlot(QWidget *parent = 0);
    //! Destructor
    ~TimeValuePlot();

    /************************ General Methods specific to this class. ************************/

    void realtimeDataPlot();

    void savePlot();

    void setDataSourceIdentifier(QString);

    QString getDataSourceIdentifier(int);

    int getDataSourceIdentifierCount();

    void setDataSourceLabel(QString);

    void setDataSourceOutputType(QString);

    void setDataSourceHalfFormatInterpretation(QString);

    void setDataSourceMinValue(const QString &value);

    void setDataSourceModes(QString modes);

    bool setDataSource(DataStructure *);

    void resetDataSource();

    /************************ Initializer Methods used by XML parser. ************************/

    void setType(QString);

    void setXLabel(QString);

    void setYLabel(QString);

    void setGraphLabel(QString);

    void setGraphHeight(int);

    void setGraphWidth(int);

    void setYMax(int);

    void setYMin(int);

    void setMaxValue(int);

    void setMinValue(const QString &value);

    void setScaleFactor(int);

    void setOutputType(QString);

    void setHalfFormatInterpretation(QString);

    void setVisiblity(QString);

    //! Set the indentification name of TimeValuePlot to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString name);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of TimeValuePlot
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
     * Add more methods here.
     */

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/
    /*
     * Add more Signals here.
     */

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to 'valueChanged()' action signal.
    void on_scaleFactorSelect_valueChanged(int arg1);

    //! Response to 'clicked()' action signal.
    void on_snapshotButton_clicked();

    //! Response to a signal from checkBox
    /*!
     * Disable the TimeValuePlot
     */
    void disable();

    //! Response to a signal from checkBox
    /*!
     * Enable the TimeValuePlot
     */
    void enable();

    //! Response to a signal from checkBox
    /*!
     * Hide the TimeValuePlot
     */
    void hideWidget();

    //! Response to a signal from checkBox
    /*!
     * Show the TimeValuePlot
     */
    void showWidget();

    //! Response to a signal from radioButton
    /*!
     * Select dataSource to be Shown
     */
    void selectSourceMode(QString mode);

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
    Ui::TimeValuePlot *ui;

    /************************ Element Attributes ************************/
    float maxValue;
    QString minSourceValue;
    float yMin;
    float yMax;
    bool autoScale;
    float scaleFactor;
    QString outputType;
    QString halfFormatInterpretation;
    int index;                              //!< NOT IMPLEMENTED YET

    QList<QLabel *> dSourceLabel;
    QList<QString> dSourceOutputType;
    QList<QString> dSourceHalfFormatInterpretation;
    QList<QStringList> dSourceModes;
    QList<QString> dSourceMinValue;

    bool singleSourceMode;
    QString selectedMode;

    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */

    QList<QString> dSourceIdentifier;

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;

    QList<DataStructure *> dSource;

};

#endif // TIMEVALUEPLOT_H
