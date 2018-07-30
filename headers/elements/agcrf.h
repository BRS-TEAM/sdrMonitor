/*!
 * \file     agcrf.h
 * \brief    AGC RF Element Class
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

// AGCRF
#ifndef AGCRF_H
#define AGCRF_H

// Essential Headers
#include <QWidget>
#include <QMap>

// Data Structure
#include "headers/core/datastructure.h"

// Parent
class TabController;

/*!
 * \brief AGC RF Element class
 */
namespace Ui {
class AGCRF;
}

class AGCRF : public QWidget
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
    explicit AGCRF(TabController *parent = 0);

    //! Destructor
    ~AGCRF();

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


private:
    int double2fixed24(double val_double);
    void rfAgcGainCalculate();
    void updateParameterStructureValue();

public:
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
    Ui::AGCRF *ui;

    /************************ Element Attributes ************************/
    int selectedMode;
    bool updateFlag;
    QMap<int, int> m_RfAgcFmThrMaxMap;
    QMap<int, int> m_RfAgcFmThrMinMap;
    QMap<int, int> m_RfAgcFmThrMaxMap_inv;
    QMap<int, int> m_RfAgcFmThrMinMap_inv;
    QMap<int, int> m_RfAgcAmThrMaxMap;
    QMap<int, int> m_RfAgcAmThrMinMap;
    QMap<int, int> m_RfAgcAmThrMaxMap_inv;
    QMap<int, int> m_RfAgcAmThrMinMap_inv;
    QMap<int, int> m_RfAgcDabThrMaxMap;
    QMap<int, int> m_RfAgcDabThrMinMap;
    QMap<int, int> m_RfAgcDabThrMaxMap_inv;
    QMap<int, int> m_RfAgcDabThrMinMap_inv;
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

#endif // AGCRF_H
