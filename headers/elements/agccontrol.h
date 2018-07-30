/*!
 * \file     agccontrol.h
 * \brief    AGC CONTROL Element Class
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

// AGCControl
#ifndef AGCCONTROL_H
#define AGCCONTROL_H

// Essential Headers
#include <QWidget>

// Data Structure
#include "headers/core/datastructure.h"

// Parent
class TabController;

namespace Ui {
class AGCControl;
}

/*!
 * \brief AGC Control Element class
 */
class AGCControl : public QWidget
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
    explicit AGCControl(TabController *parent = 0);

    //! Destructor
    ~AGCControl();

    /************************ General Methods specific to this class. ************************/

    void initParameterConfig();

    void initCoeffConfig();

    // void addInputField(QString labelName);

    // void addSpinBox(QString labelName);

    void setDataSourceIdentifier(QString);

    QString getDataSourceIdentifier(int);

    int getDataSourceIdentifierCount();

    bool setDataSource(DataStructure *);

    QList<DataStructure *> getDataSource();

    void resetDataSource();

    /************************ Initializer Methods used by XML parser. ************************/

    void setLabelName(const QString &value);

    void initParameterRange(QString band);

    void selectBand(QString band);

    //! Set the indentification name of TextBox to \a name
    /*!
     * Indentification name is the name used in Configuration (.cfg) file.
     * \sa getNameIdentifier()
     */
    void setNameIdentifier(QString name);

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

    float getParameterValue(QString param);
    void setParameterValue(QString param, float value);
    void setCoeffValue(QString param, int value);
    QString getCoeffValue(QString coeff);

    /************************ Methods used in connected state with other elements. ************************/

    // *********************************************************************** //
    //                                                                         //
    //                        Private Methods                                  //
    //                                                                         //
    // *********************************************************************** //
private:
    int double2fixed24(double val_double);
    void agcNbGainCalculate();
    void updateParameterStructureValue();

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
    void selectMode();

    void on_calculatePushButton_clicked();

    void on_writePushButton_clicked();

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
    Ui::AGCControl *ui;

    /************************ Element Attributes ************************/
    int selectedMode;
    bool updateFlag;
    // QList<QString> parameterListSpinBox;
    // QList<QString> parameterListInputField;
    int index;                              //!< NOT IMPLEMENTED YET

    /************************ Configuration ************************/
    QString nameIdentifier;                 /*!< Element name */

    QList<QString> dSourceIdentifier;

    /************************ Parent ************************/
    TabController * tabControllerParent;    //!< Pointer to parent class.

    /************************ Data Structure ************************/
    //! Data structure of element
    /*!
     * It is used to store configuration info and coefficient value.
     */
    // DataStructure * dStructure;

    QList<DataStructure *> dSource;
};

#endif // AGCCONTROL_H
