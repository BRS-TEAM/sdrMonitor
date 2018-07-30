/*!
 * \file     checkbox.h
 * \brief    CheckBox Element Class
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

// CheckBox
#ifndef CHECKBOX_H
#define CHECKBOX_H

// Essential Headers
#include <QWidget>

// Conections
class InputField;
class Button;
class SlideBar;
class BarGraphH;
class ProgressBarGraph;
class XYPlot;
class TimeValuePlot;

// Parent
class TabController;

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class CheckBox;
}

/*!
 * \brief The CheckBox class
 */
class CheckBox : public QWidget
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
    explicit CheckBox(TabController *parent = 0);
    //! Destructor
    ~CheckBox();

    /************************ General Methods specific to this class. ************************/

    //! Check the checkBox state
    /*!
     * \return true : if state = checked; Otherwise, false;
     */
    bool isCheckBoxChecked();


    /************************ Initializer Methods used by XML parser. ************************/

    //! Set the checkBox title
    void setCheckBoxTitle(QString);

    //! Set the checkBox state
    /*!
     * \param state = checked : CheckBox is ticked;  Otherwise, not ticked.
     */
    void setCheckBoxState(QString state);

    //! Set the indentification name of checkBox
    /*!
     * It is used by #ConfigController
     */
    void setNameIdentifier(QString);

    //! Set the default value that should be transmitted to the target when the CheckBox is Checked
    void setCheckedValue(int);

    //! Set the default value that should be transmitted to the target when the CheckBox is Unchecked
    void setUncheckedValue(int);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of checkBox
    /*!
     * It is used by #ConfigController to identify element.
     */
    QString getNameIdentifier();

    //! Setter : dStructure
    bool setStructure(DataStructure *);
    //! Getter : dStructure
    DataStructure * getStructure();

    //! Setter : \var index (!! NOT IMPLEMENTED !!)
    void setIndex(int);
    //! Getter : \var index (!! NOT IMPLEMENTED !!)
    int getIndex();

    /************************ Methods used in connected state with other elements. ************************/

    // Connection to lineEdit
    //! Connect checkBox to lineEdit element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - disable   :   disable lineEdit
     *  - select    :   NOT IMPLEMENTED YET
     */
    void initConnection(InputField *, QString action);

    //! Connect checkBox to pushButton element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - disable   :   disable pushButton
     */
    void initConnection(Button *, QString action);

    //! Connect checkBox to SlideBar element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - disable   :   disable SlideBar
     */
    void initConnection(SlideBar *, QString action);

    //! Connect checkBox to BarGraph element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - disable   :   disable BarGraph
     */
    void initConnection(BarGraphH *, QString action);

    //! Connect checkBox to ProgressBarGraph element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - disable   :   disable ProgressBarGraph
     */
    void initConnection(ProgressBarGraph *, QString action);

    //! Connect checkBox to XYPlot element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - disable   :   disable XYPlot
     */
    void initConnection(XYPlot *, QString action);

    //! Connect checkBox to TimeValuePlot element with given \a action
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

    //! Signal to be transmitted when CheckBox is checked
    void checked();     // void checked(int);

    //! Signal to be transmitted when CheckBox is unchecked
    void unchecked();   // void unchecked(int);

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

   //! Response to 'stateChanged()' action signal.
    void on_checkBox_stateChanged(int state);

    //! Response to 'checked()' action signal.
    void checkedEvent();

    //! Response to 'unchecked()' action signal.
    void uncheckedEvent();


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


    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:
    Ui::CheckBox *ui;                   //!< Private Istance to access '.ui' file elements

    /************************ Element Attributes ************************/
    int index;                              //!< NOT IMPLEMENTED YET
    int checkedValue;                       //!< Value to be transmitted to the Targert when checkBox is checked. It is used only when the CheckBox has a valid #DataStructure.
    int uncheckedValue;                     //!< Value to be transmitted to the Targert when checkBox is unchecked. It is used only when the CheckBox has a valid #DataStructure.
    bool updateFlag;                        //!< Flag used in internal operations.

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
};

#endif // CHECKBOX_H
