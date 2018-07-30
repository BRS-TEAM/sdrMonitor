/*!
 * \file     configcontroller.cpp
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

// Essential Headers
#include "headers/core/configcontroller.h"
#include "headers/staticdependencies/datatypeconversion.h"
#include <QApplication>
#include "headers/core/errorlogger.h"

// Constructor
ConfigController::ConfigController() {
    dataManager = 0;
    configWindow = 0;
}

// Destructor
ConfigController::~ConfigController() {

    if(dataManager != 0)
        delete dataManager;              //              To be modified

    QTextStream out(stdout);
    out << "ConfigController Deleted" << endl;
}

/************************ General Methods specific to this class. ************************/

void ConfigController::configFileReader(QString fileName) {

    int count = 0;

    if(dataManager == 0)
        dataManager = new DataManager;

    // Reset data.
    resetConfigControllerParameters();

    // Auxilliary variable

    printDebugString("ConfigReaderNew : Started" );

    // Input .cfg file
    QFile inputFile(fileName);

    // Read .cfg file
    if (inputFile.open(QIODevice::ReadOnly))
    {
        // Input File Handler
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            // Read one line
            QString line = in.readLine();
            if(line.at(0) != '#') {                         // Check if comment (Normally Header)
                dataManager->addStructure();

                int index = 0;
                QString module;
                QString name;
                QString address;
                int size = 0;
                QString rw;
                int value = 0;
                float cfgValue = 0;
                QString dataType;

                // LeStructure * leStruct = new LeStructure;   // Instantiate Configuration data structure
                // leStructure.append(leStruct);

                QStringList lists = line.split("\t");       // Break line at each tab space
                for(int i = 0; i < lists.length(); i++){    // store data
                    if(lists.at(i) == "")
                        printDebugString( "Blank" );
                    else if (lists.at(i).contains("#"))     // Skip comment
                        continue;
                    else {
                        printDebugString( lists.at(i));     // Redundant
                        switch (i) {
                        case 0:
                            index = QString(lists.at(i)).toInt();
                            break;
                        case 1:
                            module = lists.at(i);
                            break;
                        case 2:
                            name = lists.at(i);
                            break;
                        case 3:
                            address = lists.at(i);
                            break;
                        case 4:
                            size = QString(lists.at(i)).toInt();
                            break;
                        case 5:
                            rw = lists.at(i);
                            break;
                        case 6:
                            value = QString(lists.at(i)).toInt();
                            if(rw == "w" || rw == "W")
                                cfgValue = QString(lists.at(i)).toFloat();
                            break;
                        case 7:
                            dataType = lists.at(i);
                            break;
                        }
                    }
                }
                dataManager->setStructureContent(index, module, name, address, size, rw, value, cfgValue, dataType);
            } else continue;
            count ++;
        }
        inputFile.close();
    }
    printDebugString("Config Line count : " + QString().setNum(count));

}

void ConfigController::configFileBuilder(QString fileName) {

    int count = 0;

    // Output .cfg file
    QFile outputFile(fileName);

    // Write .cfg file
    if (outputFile.open(QIODevice::WriteOnly))
    {
        // Output File Handler
        QIODevice *outDevice = &outputFile;
        QTextStream outFileStream(outDevice);

        outFileStream << "#	module	name	address	size	rw	value	dataType" << endl;    // Config file header

        for(int i = 0; i < dataManager->getCount(); i++){
            {
                outFileStream << dataManager->getStructureContent(i) << endl;    // Insert data into output file
                count ++;
            }

        }

        outputFile.close();
    }
    printDebugString("Config Line Written : " + QString().setNum(count) + " lines" );
}

DataManager * ConfigController::coeffFileReader(QString fileName) {

    int count = 0;

    // Auxilliary variable

    printDebugString("CoeffFileReaderNew : Started");

    // Input .cfg file
    QFile inputFile(fileName);

    // Read .cfg file
    if (inputFile.open(QIODevice::ReadOnly))
    {

        DataManager * dManager = new DataManager;

        // Input File Handler
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            // Read one line
            QString line = in.readLine();
            if(line.at(0) != '#') {                         // Check if comment (Normally Header)

                int index = 0;
                QString module;
                QString name;
                QString address;
                int size = 0;
                QString rw;
                int value = 0;
                QString dataType;

                // LeStructure * leStruct = new LeStructure;   // Instantiate Configuration data structure
                // leStructure.append(leStruct);

                QStringList lists = line.split("\t");       // Break line at each tab space
                for(int i = 0; i < lists.length(); i++){    // store data
                    if(lists.at(i) == "")
                        printDebugString( "Blank" );
                    else if (lists.at(i).contains("#"))     // Skip comment
                        continue;
                    else {
                        printDebugString(lists.at(i));     // Redundant

                        QByteArray bA;
                        char * cStr;
                        QString str;

                        switch (i) {
                        case 0:
                            index = QString(lists.at(i)).toInt();
                            break;
                        case 1:
                            module = lists.at(i);
                            break;
                        case 2:
                            name = lists.at(i);
                            break;
                        case 3:
                            address = lists.at(i);
                            break;
                        case 4:
                            size = QString(lists.at(i)).toInt();
                            break;
                        case 5:
                            rw = lists.at(i);
                            break;
                        case 6:
                            bA = QString(lists.at(i)).toLatin1();
                            cStr = bA.data();
                            str = QString().setNum(atohex(cStr));
                            value = str.toInt();
                            break;
                        case 7:
                            dataType = lists.at(i);
                            break;
                        }
                    }
                }
                if(rw == "x" or rw == "X") {
                    dManager->addStructure();
                    dManager->setStructureContent(index, module, name, address, size, rw, value, 0,dataType);
                }
            } else continue;
            count ++;
        }
        inputFile.close();

        return dManager;
    }
    printDebugString("Config Line count : " + QString().setNum(count));

    return 0;

}

void ConfigController::coeffFileBuilder(QString fileName, DataManager* dManager) {

    int count = 0;


    // Output .cfg file
    QFile outputFile(fileName);

    // Write .cfg file
    if (outputFile.open(QIODevice::WriteOnly))
    {
        // Output File Handler
        QIODevice *outDevice = &outputFile;
        QTextStream outFileStream(outDevice);

        outFileStream << "#	module	name	address	size	rw	value	dataType" << endl;    // Config file header

        for(int i = 0; i < dManager->getCount(); i++){
            {
                outFileStream << dManager->getStructureContent(i) << endl;    // Insert data into output file
                count ++;
            }

        }

        outputFile.close();
    } else {
        printDebugString("unable to open seleted Coeff Save File" );
    }
    printDebugString("Config Line Written : " + QString().setNum(count) + " lines");
}

void ConfigController::buildConfigEditorWindow() {

    if(configWindow == 0) {
        configWindow = new ConfigWindow;

        int count = dataManager->getModuleCount();
        for(int i= 0; i < count; i++) {
            configWindow->addConfigTab(dataManager->getModuleStructure(i));
        }
        QObject::connect(configWindow, SIGNAL(finished(int)), this, SLOT(changeWindowStatus(int)));
        // QObject::connect(configWindow, SIGNAL(rejected()), configWindow, SLOT(deleteLater()));
        configWindow->show();
    } else {
        configWindow->show();
        configWindow->raise();
        QApplication::setActiveWindow(configWindow);
    }

}

void ConfigController::resetConfigControllerParameters() {

}

/************************ Getters & Setters ************************/

DataManager * ConfigController::getDataManager() {
    return dataManager;
}

/************************ Slots are special methods which execute in response to the connected signal. ************************/

void ConfigController::changeWindowStatus(int val) {

    Q_UNUSED(val)
    if(configWindow != 0) {
        configWindow = 0;
    }

}


/********************************************************************************************************************************************/
