/*!
 * \file     radiobutton.h
 * \brief    Radio Button Gropu Element Class
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

// RadioButton
#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

// Essential Headers
#include <QWidget>

// Data Structure
#include "headers/core/datastructure.h"

class QRadioButton;
class QButtonGroup;
class TimeValuePlot;
class SlideBar;

// Parent
class TabController;

namespace Ui {
class RadioButton;
}

/*!
 * \brief The RadioButton class
 */
class RadioButton : public QWidget
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
    explicit RadioButton(TabController *parent = 0);

    //! Destructor
    ~RadioButton();

    /************************ General Methods specific to this class. ************************/


    /************************ Initializer Methods used by XML parser. ************************/

    void setLabelName(const QString &value);

    void addNewButton(QString label);

    void setButtonAttributes(QString value, QString isEnable, QString isSelected);

    //! Set the indentification name of RadioButton to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString name);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of RadioButton
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

    //! Connect radioButton to TimeValuePlot element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - disable   :   disable TimeValuePlot
     */
    void initConnection(TimeValuePlot *, QString action);


    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/
    void radioButtonClicked(QString label);
    void clicked();


    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/
    //! Response to 'buttonClicked(int)' action signal.
    void onRadioButtonClicked(int d);
    void initConnection(SlideBar*, QString action);

public slots:
    //! Response to update signal
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
    Ui::RadioButton *ui;

    /************************ Element Attributes ************************/
    QButtonGroup * buttonGroup;
    QList <QRadioButton*> rButton;
    QList <int> rButtonValue;
    bool updateFlag;
    int index;                              //!< NOT IMPLEMENTED YET

    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */

    /************************ Parent ************************/
    TabController * tabControllerParent;    //!< Pointer to parent class.

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;
    SlideBar* connector;
};

#endif // RADIOBUTTON_H
