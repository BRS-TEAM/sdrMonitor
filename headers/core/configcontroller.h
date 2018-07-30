/*!
 * \file     configcontroller.h
 * \brief    Configuration file controller.
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

// ConfigController
#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H

// Essential Headers
#include <QTextStream>
#include <QFile>

// Data Manager
#include "headers/core/datamanager.h"

// Config Editor Window
#include "headers/core/configwindow.h"

/*!
 * \brief The Configuration file controller class.
 */
class ConfigController : public QObject
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
    ConfigController();

    //! Destructor
    ~ConfigController();

    /************************ General Methods specific to this class. ************************/

    //! Parse the source configuration file.
    /*!
    * This method read the .cfg file and load data from it for HMI & Communication use.
    */
    void configFileReader(QString);

    //! Save new configuration file.
    /*!
    * This method read the element data and save the configuration into new .cfg file from HMI.
    *
    * It saves ALL THE COEFFICIENTS which have "rw = x", "rw = r" or "rw = w".
    *
    *  IMPORTANT :: This function DOES NOT update all coefficients. It will save 0 value for the coefficients which are not read manually.
    * This function can be used to generate a CFG file having LIST OF ALL THE COEFFICIENTS.
    *
    *  IMPORTANT :: Use coeffFileReader(QString fileName) instead of configFileBuilder(QString fileName) function if you want to save VALUES of all the coefficients.
    *
    * \param [in] fileName : output file name
    * \param [in] sourceStream : List of strings to be stored in output file.
    *
    * \sa coeffFileReader(QString fileName)
    */
    void configFileBuilder(QString);

    //! Parse the source Coeff file.
    /*!
    * This method read the .cfg file and returns the temp DataManager loaded with DataStructure of input cfg file.
    *
    * The temp DataManager returned by this function, contains ONLY THOSE COEFFICIENTS which have "rw = x", and can be used to write values into the target.
    *
    */
    DataManager *coeffFileReader(QString fileName);

    //! Save new Coeff file.
    /*!
    * This method read the element data and save the configuration into new .cfg file.
    *
    * It saves ONLY THOSE COEFFICIENTS which have "rw = x",
    *
    * \param [in] fileName : output file name
    * \param [in] dManager : Pointer to DataManager retured by coeffFileReader().
    */
    void coeffFileBuilder(QString fileName, DataManager *dManager);

    //! Show Configuration Editor.
    void buildConfigEditorWindow();

    //! Reset the #ConfigController class data variables.
    void resetConfigControllerParameters();

    /************************ Getters & Setters ************************/

    //! Returns the DataManager of loaded configFile by configFileReader();.
    DataManager * getDataManager();

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/
    //! Response to 'finished' signal from #ConfigWindow.
    /*!
    * Performs necessary action on CLOSE action of #ConfigWindow
    */
    void changeWindowStatus(int);

    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:

    /************************ Class Attributes ************************/
    ConfigWindow * configWindow;                         //!< Private Istance of #ConfigWindow
    DataManager * dataManager;                         //!< Private Istance of #DataManager


};

#endif // CONFIGCONTROLLER_H
