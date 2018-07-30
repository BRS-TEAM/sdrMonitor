/*!
 * \file     indicator.h
 * \brief    Indicator Element Class
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

// Indicator
#ifndef INDICATOR_H
#define INDICATOR_H

// Essential Headers
#include <QWidget>

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class Indicator;
}

/*!
 * \brief The Indicator class
 */
class Indicator : public QWidget
{
    // Essential object needed by class which have its own private slots
    Q_OBJECT

    // *********************************************************************** //
    //                                                                         //
    //                           Public Methods                                //
    //                                                                         //
    // *********************************************************************** //
public:
    enum IndicatorStates {
        INACTIVE,
        PROGRESSIVE,
        ACTIVE,
        NONE
    };

    //! Constructor
    explicit Indicator(QWidget *parent = 0);

    //! Destructor
    ~Indicator();

    /************************ General Methods specific to this class. ************************/


    /************************ Initializer Methods used by XML parser. ************************/

    void setActiveStateLabel(const QString &value);

    void setProgressiveStateLabel(const QString &value);

    void setInactiveStateLabel(const QString &value);

    void setLabelName(const QString &value);

    void setNoneStateColour(const QString &value);

    void setActiveStateColour(const QString &value);

    void setProgressiveStateColour(const QString &value);

    void setInactiveStateColour(const QString &value);

    //! Set the indentification name of Indicator to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString name);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of Indicator
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
    //                           Private Methods                               //
    //                                                                         //
    // *********************************************************************** //

private:
    void setText(QString stateLabel);

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/
    void stateChanged(IndicatorStates);
    /*
     * Add more Signals here.
     */

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
    /*
     * Add more Private Slots here.
     */

    // *********************************************************************** //
    //                                                                         //
    //                           Public Slots                                  //
    //                                                                         //
    // *********************************************************************** //
public slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/
    //! Response to 'stateChanged(IndicatorStates)' action signal.
    void changeState(IndicatorStates indicatorState);


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
    Ui::Indicator *ui;

    /************************ Element Attributes ************************/
    QString valueLabel;
    QString activeState;
    QString progressiveState;
    QString inactiveState;
    QString noneStateColour;
    QString activeStateColour;
    QString progressiveStateColour;
    QString inactiveStateColour;
    int index;                              //!< NOT IMPLEMENTED YET

    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;

};

#endif // INDICATOR_H
