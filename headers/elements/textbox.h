/*!
 * \file     textBox.h
 * \brief    Text Box Element Class
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

// TextBox
#ifndef TEXTBOX_H
#define TEXTBOX_H

// Essential Headers
#include <QWidget>

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class TextBox;
}

/*!
 * \brief The TextBox  class
 */
class TextBox : public QWidget
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
    explicit TextBox(QWidget *parent = 0);

    //! Destructor
    ~TextBox();

    /************************ General Methods specific to this class. ************************/

    void setDataSourceIdentifier(QString);

    QString getDataSourceIdentifier(int);

    int getDataSourceIdentifierCount();

    bool setDataSource(DataStructure *);

    void resetDataSource();


    /************************ Initializer Methods used by XML parser. ************************/

    void setLabelName(const QString &value);

    void isBoxVisible(const QString &value);

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
    Ui::TextBox *ui;

    /************************ Element Attributes ************************/
    int index;                              //!< NOT IMPLEMENTED YET

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

#endif // TEXTBOX_H
