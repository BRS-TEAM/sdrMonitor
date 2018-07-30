/*!
 * \file     datamanager.h
 * \brief    Data Manager
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

// DataManager
#ifndef DATAMANAGER_H
#define DATAMANAGER_H

// Essential Headers
#include <QList>

// Data Structure
#include "headers/core/datastructure.h"

/*!
 * \brief The Data Manager class.
 *
 * Object of #DataManager class is mainlt instantiated by #ConfigController
 * The information collected by #ConfigController is stored by #DataManager in #DataStructure
 *
 */
class DataManager
{

    // *********************************************************************** //
    //                                                                         //
    //                           Public Methods                                //
    //                                                                         //
    // *********************************************************************** //
public:
    //! Destructor
    ~DataManager();

    //! Locate the index number of #DataStructure that belongs to \a moduleName module & has \a elementName name in \a dStructure array.
    int locateStructure(QString moduleName, QString elementName);

    //! Add new #DataStructure to \a dStructure array.
    void addStructure();

    //! Set parameters of last #DataStructure entered into \a dStructure array.
    /*!
     * addStructure() must be called before calling this method.
     *
     * \sa addStructure()
     */
    void setStructureContent(int index, QString module, QString name, QString address, int size, QString rw, int value, float cfgValue, QString dataType);

    //! Get String (Tab Formatted) of #DataStructure at \a index index from \a dStructure array.
    QString getStructureContent(int index);

    //! Get #DataStructure that belongs to \a moduleName module & has \a elementName name in \a dStructure array.
    DataStructure * getDataStructure(QString moduleName, QString elementName);

    //! Get total number of #DataStructure present in \a dStructure array.
    int getCount();

    //! Get total number of different module present in \a dStructure array.
    /*!
     * module is an attribute of #DataStructure
     * See #DataStructure for more info.
     *
     * \sa #DataStructure
     */
    int getModuleCount();

    //! Get list of #DataStructure of \a moduleName module present in \a dStructure array.
    /*!
     * module is an attribute of #DataStructure
     * See #DataStructure for more info.
     *
     * \sa #DataStructure
     */
    QList<DataStructure*> getModuleStructure(QString moduleName);                      // :D

    //! Get list of #DataStructure of the module present at \a moduleNumber location in \a dStructure array.
    /*!
     * module is an attribute of #DataStructure
     * See #DataStructure for more info.
     *
     * \sa #DataStructure
     */
    QList<DataStructure*> getModuleStructure(int moduleNumber);

    //! Get list of #DataStructure of all modules given in array \a commonModuleNameList and are present in \a dStructure array.
    /*!
     * module is an attribute of #DataStructure
     * See #DataStructure for more info.
     *
     * \sa #DataStructure
     */
    QList<DataStructure*> getModuleStructure(QList<QString> commonModuleNameList);

    //! Get all the #DataStructure from \a dStructure array which have "rw = x".
    QList<DataStructure*> getAllWritableDataStructures();


private:

    //! Data structure of elements
    /*!
     * It is used to store configuration info
     */
    QList<DataStructure *> dStructure;

    int moduleCount;
};

#endif // DATAMANAGER_H
