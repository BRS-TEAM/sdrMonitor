/*!
 * \file     datamanager.cpp
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

#include "headers/core/datamanager.h"

#include <QTextStream>

#include "headers/core/sharedflags.h"

DataManager::~DataManager()
{
    for(int i = 0; i < dStructure.count(); i++)
        delete dStructure.at(i);

    QTextStream out(stdout);
    out << "DataManager Deleted" << endl;

}

int DataManager::locateStructure(QString moduleName, QString elementName) {

    int index = -1;

    for(int i = 0; i < dStructure.count(); i++) {
        if(dStructure.at(i)->module == moduleName) {
            while(dStructure.at(i)->module == moduleName) {
                if(dStructure.at(i)->name == elementName) {
                    index = i;
                    break;
                }
                i++;
                if(i == dStructure.count()) break;
            }
            //break;                      // Comment this to remove (OR Uncomment this to add) restriction on configFile 'module' sorting
        }                               // &
        if(index != -1)  break;      // Uncomment this to remove (OR Comment this to add) restriction on configFile 'module' sorting
    }

    return index;

}

void DataManager::addStructure()
{
    DataStructure * dStruct = new DataStructure;
    dStructure.append(dStruct);
}

void DataManager::setStructureContent(int index, QString module, QString name, QString address, int size, QString rw, int value, float cfgValue, QString dataType)
{
    if(!dStructure.isEmpty()) {

        dStructure.last()->index = index;
        dStructure.last()->module = module;
        dStructure.last()->name = name;
        dStructure.last()->address = address;
        dStructure.last()->size = size;
        if(rw == "r") {
            dStructure.last()->read = true;
            dStructure.last()->write = false;
        } else if (rw == "w") {
            dStructure.last()->read = false;
            dStructure.last()->write = true;
        } else if (rw == "x") {
            dStructure.last()->read = true;
            dStructure.last()->write = true;
        } else {
            dStructure.last()->read = false;
            dStructure.last()->write = false;
        }

        dStructure.last()->value = value;
        dStructure.last()->cfgValue = cfgValue;
        dStructure.last()->dataType = dataType;

    }
}

QString DataManager::getStructureContent(int i)
{
    if(i < dStructure.count()) {

        QString rw;
        if(dStructure.at(i)->read && !dStructure.at(i)->write) {
            rw = "r";
        } else if(!dStructure.at(i)->read && dStructure.at(i)->write) {
            rw = "w";
        } else if(dStructure.at(i)->read && dStructure.at(i)->write) {
            rw = "x";
        } else {
            rw = "";
        }

        QString str;
        str.append(QString().setNum(dStructure.at(i)->index));
        str.append("\t");
        str.append(dStructure.at(i)->module);
        str.append("\t");
        str.append(dStructure.at(i)->name);
        str.append("\t");
        str.append(dStructure.at(i)->address);
        str.append("\t");
        str.append(QString().setNum(dStructure.at(i)->size));
        str.append("\t");
        str.append(rw);
        str.append("\t");

        if(rw == "x")
            str.append(QString().setNum((int)dStructure.at(i)->value, 16).right(dStructure.at(i)->size/4));
        else if(rw == "w")
            str.append(QString().setNum(dStructure.at(i)->cfgValue));
        else
            str.append("0");

        str.append("\t");
        str.append(dStructure.at(i)->dataType);

        return str;
    }

    return "#";
}

DataStructure * DataManager::getDataStructure(QString moduleName, QString elementName) {

    int index = locateStructure(moduleName, elementName);
    DataStructure * elementStructure;

    if(index != -1) {
        elementStructure = dStructure.at(index);
        return elementStructure;
    }
    return 0;

}

int DataManager::getCount() { return dStructure.count(); }

int DataManager::getModuleCount() {

    int count = 0;

    if(dStructure.count()) {

        QString moduleName = dStructure.first()->module;

        for(int i = 0; i < dStructure.count(); i++) {

            if(dStructure.at(i)->module != moduleName) {
                count++;
                moduleName = dStructure.at(i)->module;
            }
        }
        return count + 1;
    }

    return count;
}

QList<DataStructure*> DataManager::getModuleStructure(QString moduleName) {         // :D

    QList<DataStructure*> ds;

    for(int i = 0; i < dStructure.count(); i++) {
        if(dStructure.at(i)->module == moduleName) {
            while(dStructure.at(i)->module == moduleName) {
                ds.append(dStructure.at(i));
                i++;
                if(i == dStructure.count()) break;
            }
            //break;
        }

    }

    return ds;

}

QList<DataStructure*> DataManager::getModuleStructure(int moduleNumber) {         // :D

    int count = 0;

    QList<DataStructure*> ds;

    if(dStructure.count()) {

        QString moduleName = dStructure.first()->module;

        for(int i = 0; i < dStructure.count(); i++) {
            if(count == moduleNumber) break;

            if(dStructure.at(i)->module != moduleName) {

                count ++;
                moduleName = dStructure.at(i)->module;
            }
        }

        ds = getModuleStructure(moduleName);

        return ds;
    }

    return ds;
}

QList<DataStructure*> DataManager::getModuleStructure(QList<QString> commonModuleNameList) {         // :D


    QString moduleName;

//    if(radioMode == FM_MODE)
//        moduleName = "FM_Common";
//    else if (radioMode == AM_MODE)
//        moduleName = "AM_Common";
//    else if (radioMode == DAB_MODE)
//        moduleName = "DAB_Common";

    QList<DataStructure*> ds;

    for(int i = 0; i < commonModuleNameList.count(); i++) {
        moduleName = commonModuleNameList.at(i);
        for(int i = 0; i < dStructure.count(); i++) {
            if(dStructure.at(i)->module == moduleName) {
                while(dStructure.at(i)->module == moduleName) {
                    ds.append(dStructure.at(i));
                    i++;
                    if(i == dStructure.count()) break;
                }
                //break;
            }

        }
    }

    return ds;

}

QList<DataStructure *> DataManager::getAllWritableDataStructures()
{
    QList<DataStructure *> dS;

    for(int i =0; i < dStructure.count(); i++) {
        if(dStructure.at(i)->read && dStructure.at(i)->write)
            dS.append(dStructure.at(i));
    }
    return dS;
}
