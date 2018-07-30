/*!
 * \file     button.h
 * \brief    Push Button Element Class
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

// Button
#ifndef BUTTON_H
#define BUTTON_H

// Essential Headers
#include <QWidget>

// Conections
class InputField;
class SpinBox;
class SlideBar;
class BarGraphH;
class ProgressBarGraph;
class XYPlot;

// Parent
class TabController;

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class Button;
}

/*!
 * \brief The Button class
 */
class Button : public QWidget
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
    explicit Button(TabController *parent = 0);
    //! Destructor
    ~Button();

    /************************ General Methods specific to this class. ************************/

    /*
     *
     * Add more methods here
     *
     */

    /************************ Initializer Methods used by XML parser. ************************/

    //! Set the button title
    void setButtonTitle(QString label);

    //! Assign function to the button
    /*!
     * \param function : name of the function to be assigned.
     *
     * List of possible functions :
     *  - open      :   OpenDialog window
     *  - close     :   Close parent window     (To be implemented)
     *  - save      :   (To be Updated)
     *  - load      :   (To be Updated)
     *  - settings  :   Open Settings window    (To be implemented)
     *
     * IMPORTANT
     *  - For 'save' and 'load' function to work, the button must be connected to atleast one lineEdit.
     *
     * \sa initConnection(InputField *, QString)
     */
    void setButtonFunction(QString);

    //! Setter : \var resetVal
    void setResetValue(int);

    //! Set the indentification name of Button to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString name);


    /************************ Getters & Setters ************************/

    //! Get the indentification name of Button
    /*!
     * It is used by #ConfigController to identify element.
     * \sa setNameIdentifier()
     */
    QString getNameIdentifier();

    //! Fetch the button title
    QString getButtonTitle();

    //! Setter : \var dStructure \sa getStructure()
    bool setStructure(DataStructure *);
    //! Getter : \var dStructure \sa setStructure()
    DataStructure * getStructure();

    //! Setter : \var index (!! NOT IMPLEMENTED !!)
    void setIndex(int);
    //! Getter : \var index (!! NOT IMPLEMENTED !!)
    int getIndex();

    /************************ Methods used in connected state with other elements. ************************/

    // Connection to lineEdit

    //! Connect pushButton to lineEdit element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - clear     :   clear lineEdit field
     *  - reset     :   reset lineEdit to its defaultValue
     *  - load      :   load lineEdit with given config file
     *  - save      :   save lineEdit data to config file
     *
     * IMPORTANT
     *  - For 'save' or 'load' action to work, the button must set to 'save' or 'load' function.
     *
     * \sa setButtonFunction()
     */
    void initConnection(InputField *, QString action);

    // Connection to spinBox

    //! Connect pushButton to spinBox element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - clear     :   clear lineEdit field
     *  - reset     :   reset lineEdit to its defaultValue
     *  - load      :   load lineEdit with given config file
     *  - save      :   save lineEdit data to config file
     *
     * IMPORTANT
     *  - For 'save' or 'load' action to work, the button must set to 'save' or 'load' function.
     *
     * \sa setButtonFunction()
     */
    void initConnection(SpinBox *, QString action);

    // Connection to SlideBar

    //! Connect pushButton to slideBar element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - reset     :   reset the slideBar to minimum value
     */
    void initConnection(SlideBar *, QString action);

    // Connection to BarGraph

    //! Connect pushButton to barGraph element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - refresh   :   refresh the barGraph
     *  - clear     :   clear the barGraph
     */
    void initConnection(BarGraphH *, QString action);

    // Connection to ProgressBarGraph

    //! Connect pushButton to ProgressBarGraph element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - refresh   :   refresh the ProgressBarGraph
     *  - clear     :   clear the ProgressBarGraph
     */
    void initConnection(ProgressBarGraph *, QString action);

    // Connection to XYPlot

    //! Connect pushButton to xyPlot element with given \a action
    /*!
     * \param action : type of action to connect with the connector
     *
     * List of possible actions :
     *  - refresh   :   refresh the xyPlot
     *  - clear     :   clear the xyPlot
     */
    void initConnection(XYPlot *, QString action);

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/

    //! Signal to be transmitted when Button connected to LineEdit in 'load' action is clicked. \sa initConnection(InputField *, QString action)
    void loadSignal();

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to a signal from pushButton.
    /*!
     * It belongs to the signal associated with 'save' function.
     *
     * \sa setButtonFunction()
     */
    void saveValue();

    //! Response to a signal from pushButton.
    /*!
     * It belongs to the signal associated with 'load' function.
     *
     * \sa setButtonFunction()
     */
    void loadValue();

    //! Response to a signal from pushButton.
    /*!
     * It belongs to the signal associated with 'reset' function.
     *
     * \sa setButtonFunction()
     */
    void resetValue();

    //! Response to a signal from checkBox.
    /*!
     * Disable the pushButton
     */
    void disable();

    //! Response to a signal from checkBox.
    /*!
     * Enable the pushButton
     */
    void enable();

    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:
    Ui::Button *ui;                         //!< Private Istance to access '.ui' file elements

    /************************ Element Attributes ************************/
    int index;                              //!< NOT IMPLEMENTED YET
    int resetVal;

    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */

    /************************ Connections ************************/
    QList<InputField *> sifLoadConnector;   //!< Array of : pointer to #InputField element
    QList<InputField *> sifSaveConnector;   //!< Array of : pointer to #InputField element

    QList<SpinBox *> spinBoxLoadConnector;   //!< Array of : pointer to #SpinBox element
    QList<SpinBox *> spinBoxSaveConnector;   //!< Array of : pointer to #SpinBox element

    /************************ Parent ************************/
    TabController * tabControllerParent;    //!< Pointer to parent class.

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;
};

#endif // BUTTON_H
