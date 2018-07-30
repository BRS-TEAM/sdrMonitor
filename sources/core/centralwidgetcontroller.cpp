/*!
 * \file     centralwidgetcontroller.cpp
 * \brief    Highest level controller class to manage central widget
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
#include "headers/core/centralwidgetcontroller.h"

#include "headers/core/tabcontroller.h"
#include "headers/core/configcontroller.h"
#include "headers/core/communicationcontroller.h"
#include "headers/core/datamanager.h"
#include "headers/core/errorlogger.h"
#include <QtUiTools/QtUiTools>

#include <QMainWindow>

// #include <QPushButton>       // Not used yet

#include "headers/core/sharedflags.h"
int radioMode = NONE;

// Constructor
CentralWidgetController::CentralWidgetController(QWidget * parent)
    : QWidget (parent)
{
    setFmTabsCount(0);
    setAmTabsCount(0);
    setDabTabsCount(0);

    configController = 0;

    communicationController = 0;

    radioMode = NONE;

    fmTabWidget = 0;
    fmCommonTab = 0;
    amTabWidget = 0;
    amCommonTab = 0;
    dabTabWidget = 0;
    dabCommonTab = 0;
}

// Destructor
CentralWidgetController::~CentralWidgetController() {

    if(fmTabWidget != 0)
        disconnect(fmTabWidget,SIGNAL(currentChanged(int)),this, SLOT(updateSelectedTab(int)));
    if(amTabWidget != 0)
        disconnect(amTabWidget,SIGNAL(currentChanged(int)),this, SLOT(updateSelectedTab(int)));
    if(dabTabWidget != 0)
        disconnect(dabTabWidget,SIGNAL(currentChanged(int)),this, SLOT(updateSelectedTab(int)));

    for (int i = 0; i < tabPage.count(); i++)
    {
        tabPage.at(i)->close();
        delete tabPage.at(i);
    }

    if (configController != 0)
        delete configController;

    if(fmTabWidget != 0) {
        fmTabWidget->close();
        delete fmTabWidget;
    }

    if(fmCommonTab != 0) {
        fmCommonTab->close();
        delete fmCommonTab;
    }

    if(amTabWidget != 0) {
        amTabWidget->close();
        delete amTabWidget;
    }

    if(amCommonTab != 0) {
        amCommonTab->close();
        delete amCommonTab;
    }

    if(dabTabWidget != 0) {
        dabTabWidget->close();
        delete dabTabWidget;
    }

    if(dabCommonTab != 0) {
        dabCommonTab->close();
        delete dabCommonTab;
    }
}

// Parse the source xml file.
void CentralWidgetController::centralWidgetUiParser() {

    uiParser.resetAllParameters();

    // FM
    if(!fmSource.isEmpty()) {
        // Parse the input XML file.
        uiParser.initUiParser(fmSource, 1);

        // Save the data collected by uiParser
        setFmTabsCount(uiParser.getTabsCount());          // Save tabsCount

        for (int i = 0; i< fmTabsCount; i++ )
            setFmPageSource(uiParser.getTabSource(i));    // Save tabSource
    }

    uiParser.resetAllParameters();

    // AM
    if(!amSource.isEmpty()) {
        // Parse the input XML file.
        uiParser.initUiParser(amSource, 1);

        // Save the data collected by uiParser
        setAmTabsCount(uiParser.getTabsCount());          // Save tabsCount

        for (int i = 0; i< amTabsCount; i++ )
            setAmPageSource(uiParser.getTabSource(i));    // Save tabSource
    }

    uiParser.resetAllParameters();

    // DAB
    if(!dabSource.isEmpty()) {
        // Parse the input XML file.
        uiParser.initUiParser(dabSource, 1);

        // Save the data collected by uiParser
        setDabTabsCount(uiParser.getTabsCount());          // Save tabsCount

        for (int i = 0; i< dabTabsCount; i++ )
            setDabPageSource(uiParser.getTabSource(i));    // Save tabSource
    }

    uiParser.resetAllParameters();

}

// Create central widget (TabWidget)
void CentralWidgetController::centralWidgetUiPainter() {

    qDebug()<< "Build FM";

    if(!fmPageSource.empty()) {

        // Allocate memory.
        fmTabWidget = new QTabWidget;
        fmCommonTab = new QTabWidget;

        // Save/Load the data collected by uiParser
        fmTabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);              // tabWidget->setMinimumWidth(uiParser.getTabGeometry(0)); //    tabWidget->setMinimumHeight(uiParser.getTabGeometry(1));

        // Create widget (tab page) for each tab
        for (int i = 0; i < fmTabsCount; i++) {
            createTabPage(fmPageSource.at(i));                                                   // QString tabName = "Tab &" + QString().setNum(i); // To be changed // (set according to data fetched by ui parser.)
            if(tabController.at(i)->isCommon()){
                printDebugString("Create Common  FM Window here" + QString().setNum(i));
                fmCommonTab->addTab(tabPage.at(i), tabController.at(i)->getPageName());          // commonTab->widget(commonTab->count()-1)->setMaximumWidth(420); // commonTab->widget(i)->setMaximumHeight(100); // commonTab->widget(commonTab->count()-1)->setMaximumWidth(tabPage.at(i)->width());
            } else {
                QWidget * contentHolder = new QWidget(fmTabWidget);
                QVBoxLayout * controlLayout= new QVBoxLayout();
                QScrollArea * scrollArea = new QScrollArea(fmTabWidget);

                controlLayout->addWidget(scrollArea);     // Add widget to layout

                scrollArea->setWidget(tabPage.at(i));
                scrollArea->setWidgetResizable(true);

                contentHolder->setLayout(controlLayout);

                fmTabWidget->addTab(contentHolder, tabController.at(i)->getPageName());
//                fmTabWidget->addTab(tabPage.at(i), tabController.at(i)->getPageName());          // Add tab page to TabWidget       // tabWidget->widget(tabWidget->count()-1)->setMaximumWidth(tabPage.at(i)->width());
            }
        }

        connect(fmTabWidget,SIGNAL(currentChanged(int)),this, SLOT(updateSelectedTab(int)));
    }

    qDebug()<< "Build AM";

    if(!amPageSource.empty()) {

        // Allocate memory.
        amTabWidget = new QTabWidget;
        amCommonTab = new QTabWidget;

        // Save/Load the data collected by uiParser
        amTabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Create widget (tab page) for each tab
        for (int i = fmTabsCount, k = 0; i < amTabsCount + fmTabsCount; i++, k++) {
            createTabPage(amPageSource.at(k));
            if(tabController.at(i)->isCommon()){
                printDebugString("Create Common AM Window here" + QString().setNum(i));
                amCommonTab->addTab(tabPage.at(i), tabController.at(i)->getPageName());
            } else {
                QWidget * contentHolder = new QWidget(amTabWidget);
                QVBoxLayout * controlLayout= new QVBoxLayout();
                QScrollArea * scrollArea = new QScrollArea(amTabWidget);

                controlLayout->addWidget(scrollArea);     // Add widget to layout

                scrollArea->setWidget(tabPage.at(i));
                scrollArea->setWidgetResizable(true);

                contentHolder->setLayout(controlLayout);

                amTabWidget->addTab(contentHolder, tabController.at(i)->getPageName());          // Add tab page to TabWidget
            }
        }

        connect(amTabWidget,SIGNAL(currentChanged(int)),this, SLOT(updateSelectedTab(int)));
    }

    qDebug()<< "Build DAB";

    if(!dabPageSource.empty()) {

        // Allocate memory.
        dabTabWidget = new QTabWidget;
        dabCommonTab = new QTabWidget;

        // Save/Load the data collected by uiParser
        dabTabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Create widget (tab page) for each tab
        for (int i = fmTabsCount + amTabsCount, k = 0; i < dabTabsCount + amTabsCount + fmTabsCount; i++, k++) {
            createTabPage(dabPageSource.at(k));
            if(tabController.at(i)->isCommon()){
                printDebugString("Create Common DAB Window here" + QString().setNum(i));
                dabCommonTab->addTab(tabPage.at(i), tabController.at(i)->getPageName());
            } else {
                QWidget * contentHolder = new QWidget(dabTabWidget);
                QVBoxLayout * controlLayout= new QVBoxLayout();
                QScrollArea * scrollArea = new QScrollArea(dabTabWidget);

                controlLayout->addWidget(scrollArea);     // Add widget to layout

                scrollArea->setWidget(tabPage.at(i));
                scrollArea->setWidgetResizable(true);

                contentHolder->setLayout(controlLayout);

                dabTabWidget->addTab(contentHolder, tabController.at(i)->getPageName());          // Add tab page to TabWidget
            }
        }

        connect(dabTabWidget,SIGNAL(currentChanged(int)),this, SLOT(updateSelectedTab(int)));
    }

}

// Create widget (tab page) for each tab in central widget.
void CentralWidgetController::createTabPage(QString pageSource) {

    // Instantiate pointer to Tab Page Widget
    QMainWindow * tabPage = new QMainWindow;

    // Instantiate TabController
    tabController.append(new TabController(tabPage));
    tabController.last()->setPageSource(pageSource);     // Set path of tab page source XML file
    tabController.last()->pageWidgetUiParser();                // Read tab page source XML file
    tabController.last()->generateUi();                        // Build UI

    tabPage->setCentralWidget(tabController.last()->getPageWidget());

    this->tabPage.append(tabPage);  // Save the pointer to Tab Page Widget

}

void CentralWidgetController::loadConfig(QString fileName) {

    // Interrupt communication process (if Any)
    if(communicationController != 0) {
        communicationController->shutDown();
        // communicationController->requestDisconnect();
    }

    haltAllTabs();

    if (configController != 0) {
        delete configController;
        configController = 0;
    }

    if (configController == 0)
        configController = new ConfigController;

    configController->configFileReader(fileName);

    // for (int i = 0; i < fmTabsCount + amTabsCount; i++) {
    for (int i = 0; i < tabController.count(); i++) {
        tabController.at(i)->setDataManager(configController->getDataManager());                        //
        tabController.at(i)->resetConfig();
        tabController.at(i)->loadConfig();
    }

    if(communicationController != 0) {
        communicationController->shutDown();
        delete communicationController;
        communicationController = 0;
    }
}

void CentralWidgetController::saveConfig(QString fileName) {
    if (configController != 0)
        configController->configFileBuilder(fileName);
}

void CentralWidgetController::displayConfig() {
    if (configController != 0)
        configController->buildConfigEditorWindow();
}

void CentralWidgetController::openConnectionSettings () {

    if(communicationController == 0) {
        communicationController = new CommunicationController();
        communicationController->showConnectionWindow();

        // for (int i = 0; i < fmTabsCount + amTabsCount; i++) {
        for (int i = 0; i < tabController.count(); i++) {
            tabController.at(i)->setCommunicationController(communicationController);
        }

    } else {
        communicationController->showConnectionWindow();
    }

}

void CentralWidgetController::closeNetworkConnection() {

    if(communicationController != 0) {
        communicationController->shutDown();
        delete communicationController;
        communicationController = 0;
    }
}


bool CentralWidgetController::startCommunication() {

    if(communicationController != 0) {
        if(communicationController->setDataManager(getDataManager())) {
            return true;
        } else {
            QMessageBox message;
            message.setText("Please Load .cfg file and connect to target before starting the sampling process.");
            message.exec();
            return false;
        }
    } else {
        QMessageBox message;
        message.setText("Please Load .xml and .cfg files before starting the sampling process.");
        message.exec();
        return false;
    }

}

void CentralWidgetController::stopCommunication() {

    if(communicationController != 0) {
        communicationController->interruptProcess();
    }

}

void CentralWidgetController::openCoeffReadWriteWindow () {

    if(communicationController != 0) {
        communicationController->showCoeffReadWriteWindow();
    }

}

bool CentralWidgetController::initWriteAllCoeffs() {

    QTextStream out(stdout);

    if(communicationController != 0) {
        if (configController != 0) {

            QString srcName = QFileDialog::getOpenFileName(this, tr("Select Coeff Config File"), QDir::currentPath(), tr("Config files (*.cfg)"));

            if(!srcName.isEmpty()) {

                DataManager * dManager = configController->coeffFileReader(srcName);

                if(dManager != 0) {
                    if(communicationController->requestWriteAllCoeffs(dManager->getAllWritableDataStructures())) {

                        delete dManager;
                        return true;
                    }
                    else {
                        delete dManager;
                        out << "communicationController :: requestWriteAllCoeffs not Finished" << endl;
                        return false;
                    }
                } else {
                    out << "communicationController :: Unable create data structure for selected file" << endl;
                }
            }
        } else {
            out << "configController :: not initialized" << endl;
            return false;
        }

    } else {
        out << "communicationController :: not initialized" << endl;
        return false;
    }

    return false;

}

bool CentralWidgetController::initReadAllCoeffs() {

    QTextStream out(stdout);

    if(communicationController != 0) {
        if (configController != 0) {

            QString srcName = QFileDialog::getOpenFileName(this, tr("Select Coeff Config File"), QDir::currentPath(), tr("Config files (*.cfg)"));

            if(!srcName.isEmpty()) {

                DataManager * dManager = configController->coeffFileReader(srcName);

                if(dManager != 0) {
                    if(communicationController->requestReadAllCoeffs(dManager->getAllWritableDataStructures())) {

                        QString dstName = QFileDialog::getSaveFileName(this, tr("Save Coeff Config File"), QDir::currentPath(), tr("Config files (*.cfg)"));

                        if(!dstName.isEmpty()) {
                            configController->coeffFileBuilder(dstName, dManager);
                        }

                        delete dManager;
                        return true;
                    }
                    else {
                        delete dManager;
                        out << "communicationController :: requestReadAllCoeffs not Finished" << endl;
                        return false;
                    }
                } else {
                    out << "communicationController :: Unable create data structure for selected file" << endl;
                }

            }

        } else {
            out << "configController :: not initialized" << endl;
            return false;
        }
    } else {
        out << "communicationController :: not initialized" << endl;
        return false;
    }

    return false;

}

void CentralWidgetController::selectRadioMode(int mode) { radioMode = mode; }

void CentralWidgetController::haltAllTabs() {

    if(communicationController != 0) {

        for(int i = 0; i<tabController.count(); i++) {
            tabController.at(i)->initTab(false);
        }
    }
}

void CentralWidgetController::updateTab() {

    switch (radioMode) {
    case FM_MODE:
        updateSelectedTab(fmTabWidget->currentIndex());
        break;
    case AM_MODE:
        updateSelectedTab(amTabWidget->currentIndex());
        break;
    case DAB_MODE:
        updateSelectedTab(dabTabWidget->currentIndex());
        break;
    default:
        break;
    }

}

/*********************************************** Getters & Setters *************************************************/

DataManager * CentralWidgetController::getDataManager() {
    if (configController != 0)
        return configController->getDataManager();
    return 0;
}

// FM
void CentralWidgetController::setFmSource(QString source) { this->fmSource = source; }
QString CentralWidgetController::getFmSource() { return fmSource; }

void CentralWidgetController::setFmTabsCount(int tabsCount) { this->fmTabsCount = tabsCount; }
int CentralWidgetController::getFmTabsCount() { return fmTabsCount; }

void CentralWidgetController::setFmPageSource(QString pageSource) { this->fmPageSource.append(pageSource); }
// QString CentralWidgetController::getFmPageSource() { return fmPageSource.at(0); }

QTabWidget * CentralWidgetController::getFmTabWidget () { return fmTabWidget; }
QTabWidget * CentralWidgetController::getFmCommonTab () { if(fmCommonTab->count()) return fmCommonTab; else return 0; }

// AM
void CentralWidgetController::setAmSource(QString source) { this->amSource = source; }
QString CentralWidgetController::getAmSource() { return amSource; }

void CentralWidgetController::setAmTabsCount(int tabsCount) { this->amTabsCount = tabsCount; }
int CentralWidgetController::getAmTabsCount() { return amTabsCount; }

void CentralWidgetController::setAmPageSource(QString pageSource) { this->amPageSource.append(pageSource); }
// QString CentralWidgetController::getAmPageSource() { return amPageSource.at(0); }

QTabWidget * CentralWidgetController::getAmTabWidget () { return amTabWidget; }
QTabWidget * CentralWidgetController::getAmCommonTab () { if(amCommonTab->count()) return amCommonTab; else return 0; }

// DAB
void CentralWidgetController::setDabSource(QString source) { this->dabSource = source; }
QString CentralWidgetController::getDabSource() { return dabSource; }

void CentralWidgetController::setDabTabsCount(int tabsCount) { this->dabTabsCount = tabsCount; }
int CentralWidgetController::getDabTabsCount() { return dabTabsCount; }

void CentralWidgetController::setDabPageSource(QString pageSource) { this->dabPageSource.append(pageSource); }
// QString CentralWidgetController::getDabPageSource() { return dabPageSource.at(0); }

QTabWidget * CentralWidgetController::getDabTabWidget () { return dabTabWidget; }
QTabWidget * CentralWidgetController::getDabCommonTab () { if(dabCommonTab->count()) return dabCommonTab; else return 0; }


/************************ Slots are special methods which execute in response to the connected signal. ************************/


void CentralWidgetController::updateSelectedTab(int index) {

    if(index >= 0 && communicationController != 0) {

        communicationController->interruptProcess();
        QThread::currentThread()->msleep(300);

        printDebugString("Index " + QString().setNum(index));
        // for(int i = 0; i<tabController.count(); i++) {
        QList<QString> commonTabList;

        switch (radioMode) {
        case FM_MODE:

            for(int i = fmTabsCount; (i < dabTabsCount + amTabsCount + fmTabsCount) && (i < tabController.count()); i++) {
                tabController.at(i)->initTab(false);
            }

            for(int i = 0; (i < fmTabsCount) && (i < tabController.count()); i++) {
                if(tabController.at(i)->isCommon()) {
                    if(index >= i)
                        index++;
                    tabController.at(i)->initTab(true);
                    commonTabList.append(tabController.at(i)->getPageName());
                }
                else
                    tabController.at(i)->initTab(false);
            }
            break;

        case AM_MODE:

            for(int i = 0; (i < fmTabsCount) && (i < tabController.count()); i++) {
                tabController.at(i)->initTab(false);
            }

            for(int i = amTabsCount; (i < dabTabsCount + amTabsCount) && (i < tabController.count()); i++) {
                tabController.at(i)->initTab(false);
            }

            index = index + fmTabsCount;
            for(int i = fmTabsCount; (i < amTabsCount + fmTabsCount) && (i < tabController.count()); i++) {
                if(tabController.at(i)->isCommon()) {
                    if(index >= i)
                        index++;
                    tabController.at(i)->initTab(true);
                    commonTabList.append(tabController.at(i)->getPageName());
                }
                else
                    tabController.at(i)->initTab(false);
            }
            break;

        case DAB_MODE:

            for(int i = 0; (i < amTabsCount + fmTabsCount) && (i < tabController.count()); i++) {
                tabController.at(i)->initTab(false);
            }

            // if(fmTabsCount == 0 || amTabsCount == 0)
            //  index = index + fmTabsCount + amTabsCount;
            // else
            //  index = index + fmTabsCount + amTabsCount - 1;

            index = index + fmTabsCount + amTabsCount;

            for(int i = fmTabsCount + amTabsCount; (i < dabTabsCount + amTabsCount + fmTabsCount) && (i < tabController.count()); i++) {

                if(tabController.at(i)->isCommon()) {

                    if(index >= i)
                        index++;
                    tabController.at(i)->initTab(true);
                    commonTabList.append(tabController.at(i)->getPageName());
                }
                else
                    tabController.at(i)->initTab(false);

            }
            break;

        default:
            index = -1;
        }

        printDebugString( "Activate Index " + QString().setNum(index));

        if(index != -1 && index < tabController.count()) {
            // qDebug()<< "True Tab Name at Index " << tabController.at(index)->getPageName();
            //        qDebug()<< "Tab Name at Current Index " << tabController.at(tabWidget->currentIndex())->getPageName();

            //        tabWidget->widget(tabWidget->currentIndex())->setMinimumWidth(tabController.at(index)->getContainerFrameSize().width());
            //        tabWidget->widget(tabWidget->currentIndex())->setMaximumWidth(tabController.at(index)->minimumWidth()+10);
            //        tabWidget->widget(tabWidget->currentIndex())->adjustSize();

            // Active
            //        fmTabWidget->setMinimumWidth(tabController.at(index)->getContainerFrameSize().width());
            //        fmTabWidget->adjustSize();


            tabController.at(index)->initTab(true);
            QString selectedTab = tabController.at(index)->getPageName();
            qDebug()<< "Tab Selected : " << selectedTab;

            communicationController->setCommonModule(commonTabList);

            if(tabController.at(index)->hasContinuousRead())
                communicationController->setSelectedModule(selectedTab);
            else
                communicationController->setSelectedModule("NOTSELECTED");

        }

    }
}
