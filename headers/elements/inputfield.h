/*!
 * \file     inputfield.h
 * \brief    LineEdit Element Class
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

// InputField
#ifndef INPUTFIELD_H
#define INPUTFIELD_H

// Essential Headers
#include <QWidget>

// Parent
class TabController;

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class InputField;
}

/*!
 * \brief The InputField class
 */
class InputField : public QWidget
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
    explicit InputField(TabController *parent = 0);
    //! Destructor
    ~InputField();

    /************************ General Methods specific to this class. ************************/

    //! Get the value of lineEdit field
    QString getValueLineEdit();

    void saveValueLineEdit(QString value);

    //! Set the value of lineEdit field
    void setValueLineEdit(QString);

    float preprocessFieldText(float);

    /************************ Initializer Methods used by XML parser. ************************/

    //! Set Mask
    /*!
     * \param mask : type of value that can be inserted in lineEdit
     *
     * List of possible masks :
     *  - Numeric     :   Any Digit and ',' for decimal point
     */
    void setMask(QString mask);

    //! Set place holder text
    void setPlaceHolder(QString);

    //! Set Mode
    /*!
     * \param mode : behaviour of lineEdit
     *
     * List of possible mode :
     *  - readOnly     :   value of lineEdit cannot be edited by user
     */
    void setMode(QString mode);

    //! Set the indentification name of lineEdit
    /*!
     * It is used by #ConfigController
     */
    void setNameIdentifier(QString);

    //! Setter : valueLabel
    void setValueLabel(QString);

    //! Setter : defaultValue
    void setDefaultValue(QString);

    //! Setter : disabledValue
    void setDisabledValue(QString);

    //! Setter : halfFormatInterpretation
    /*!
     * \param format : UPPER | LOWER | COMPLEX
     */
    void setHalfFormatInterpretation(QString format);

    //! Setter : outputType
    /*!
     * \param type : hex | float
     */
    void setOutputType(QString type);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of lineEdit
    /*!
     * It is used by #ConfigController to identify element.
     */
    QString getNameIdentifier();

    //! Getter : valueLabel
    QString getValueLabel();

    //! Setter : dStructure
    bool setStructure(DataStructure *);
    //! Getter : dStructure
    DataStructure * getStructure();

    //! Setter : resetValue
    void setResetValue(QString);

    //! Setter : disabledValue
    QString getDisabledValue() const;

    //! Setter : \var index (!! NOT IMPLEMENTED !!)
    void setIndex(int);
    //! Getter : \var index (!! NOT IMPLEMENTED !!)
    int getIndex();

    /************************ Methods used in connected state with other elements. ************************/
    /*
     *
     * Add more methods here
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
     * Add signals here
     *
     */

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to a signal from pushButton
    /*!
     * Empty the lineEdit
     */
    void clear();

    //! Response to a signal from pushButton
    /*!
     * Set the value to defaultValue
     */
    void reset();

    //! Response to a signal from checkBox
    /*!
     * Disable the lineEdit
     */
    void disable();

    //! Response to a signal from checkBox
    /*!
     * Enable the lineEdit
     */
    void enable();

    //! Response to a signal from checkBox
    /*!
     * NOT IMPLEMENTED YET
     */
    void select();

    //! Response to a signal from checkBox
    /*!
     * NOT IMPLEMENTED YET
     */
    void deselect();

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
    Ui::InputField *ui;                 //!< Private Istance to access '.ui' file elements

    /************************ Element Attributes ************************/
    QString valueLabel;                 //!< name to be displayed
    QString defaultValue;               //!< Default value of element
    int oldValue;                       //!< Variable used in internal operations.
    QString disabledValue;              //!< Value of element when disabled
    QString resetValue;                 //!< Reset Value of element
    QString halfFormatInterpretation;   //!< halfFormatInterpretation of Coefficient. \see setHalfFormatInterpretation()
    QString outputType;                 //!< Type Output to be displayed. \see setOutputType()
    int index;                          //!< NOT IMPLEMENTED YET

    /************************ Configuration ************************/
    QString nameIdentifier;             /*!< Element name */

    /************************ Parent ************************/
    TabController * tabControllerParent; //!< Pointer to parent class.

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;

};

#endif // INPUTFIELD_H

/* ***************************************** The Information below is not updated since 20 Feb.
 * Properties:          By Designer
 *      Alignment
 *      size
 *      Style
 *      Input Limit
 * Attributes
 *      label
 * Methods
 *      setValueLabel
 *      getValueLineEdit
 * Mode
 *      ReadOnly        User
 * Temporary
 *
 */
