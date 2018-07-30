/*!
 * \file     spinBox.h
 * \brief    Spin Box Element Class
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

// SpinBox
#ifndef SPINBOX_H
#define SPINBOX_H

// Essential Headers
#include <QWidget>

// Parent
class TabController;

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class SpinBox;
}

/*!
 * \brief The SpinBox  class
 */
class SpinBox : public QWidget
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
    explicit SpinBox(TabController *parent = 0);

    //! Destructor
    ~SpinBox();

    /************************ General Methods specific to this class. ************************/

    //! Get the value of lineEdit field
    float getValueSpinBox();

    //! Set the value of lineEdit field
    void setValueSpinBox(float);

    int preprocessSpinBoxValue(float);

    void setStepSizeSpinBox(int);

    /************************ Initializer Methods used by XML parser. ************************/

    void setLabelName(const QString &value);

    //! Set the indentification name of TextBox to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString name);

    //! Setter : defaultValue
    void setDefaultValue(float);

    //! Setter : disabledValue
    void setDisabledValue(float);

    //! Setter : halfFormatInterpretation
    /*!
     * \param format : UPPER | LOWER | COMPLEX
     */
    void setHalfFormatInterpretation(QString format);

    void setSpinBoxLowerRange(float);

    void setSpinBoxUpperRange(float);

    /************************ Getters & Setters ************************/

    //! Get the indentification name of TextBox
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

    //! Setter : defaultValue
    void setDefaultValue(QString);



    /************************ Methods used in connected state with other elements. ************************/

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to a signal from pushButton
    /*!
     * Empty the spinBox
     */
    void clear();

    //! Response to a signal from pushButton
    /*!
     * Set the value to defaultValue
     */
    void reset();

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
    Ui::SpinBox *ui;

    /************************ Element Attributes ************************/
    float disabledValue;              //!< Value of element when disabled
    float defaultValue;               //!< Default value of element
    QString halfFormatInterpretation;   //!< halfFormatInterpretation of Coefficient. \see setHalfFormatInterpretation()s

    int index;                              //!< NOT IMPLEMENTED YET

    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */

    /************************ Parent ************************/
    TabController * tabControllerParent; //!< Pointer to parent class.

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    DataStructure * dStructure;

};

#endif // SPINBOX_H
