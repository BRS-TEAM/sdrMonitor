/*!
 * \file     datastructure.h
 * \brief    Data Structure
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


#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QString>

struct DataStructure {
    // Search       // Build ConfigSetupScreen
    int index;
    QString module;         /*!< Element module name */
    QString name;           /*!< Element name */

    // Communication
    QString address;            /*!< Element address (HEX) */
    int size;               /*!< Element size (bits) */

    // Access
    bool read;              /*!< Element mode; ReadOnly */
    bool write;             /*!< Element mode; WriteOnly */

    // Update
    int value;              /*!< Element value */
    float cfgValue;              /*!< Element value */

    // Display
    QString dataType;       /*!< Element dataType */
};


#endif // DATASTRUCTURE_H
