/*!
 * \file     mainwindow.cpp
 * \brief    Highest Level class for handling application structure & services
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
#include "headers/core/mainwindow.h"
#include "ui_mainwindow.h"

// IO
#include <QDir>

// Text Handler
#include <QTextStream>
#include <QStringList>

// Dialog Display
#include <QFileDialog>
#include <QMessageBox>
#include <QWizard>
#include <QLabel>

// Scroll Area
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPixmap>

#include <QPlainTextEdit>
#include <QDockWidget>
#include <QScrollBar>
#include "headers/core/sharedflags.h"


// ERROR REPORTING FLAGS
bool criticalLevelErrorFlag = false;
bool highLevelErrorFlag = false;
bool averageLevelErrorFlag = false;
bool lowLevelErrorFlag = false;

// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup the layout and widgets defined in .ui file
    ui->setupUi(this);

    // Initialize data members
    centralWidgetOpen = false;

    ui->toolBar->setMovable(false);
    ui->actionOpen->setIcon(QPixmap("../SDRToolResources/icons/openXML.png"));      // QPixmap openXml("../SDRToolResources/icons/pen-icon.png");

    ui->actionNetSettings->setIcon(QPixmap("../SDRToolResources/icons/netSetting.png"));
    ui->actionNetSettings->setIconVisibleInMenu(false);
    ui->actionNetSettings->setDisabled(true);

    ui->actionLoadConfig->setIcon(QPixmap("../SDRToolResources/icons/loadConfig.png"));
    ui->actionSaveConfig->setIcon(QPixmap("../SDRToolResources/icons/saveConfig.png"));
    ui->actionSetupConfig->setIcon(QPixmap("../SDRToolResources/icons/setupConfig.png"));
    ui->actionLoadConfig->setDisabled(true);
    ui->actionSaveConfig->setDisabled(true);
    ui->actionSetupConfig->setDisabled(true);

    ui->actionStart->setIcon(QPixmap("../SDRToolResources/icons/startSampling.bmp"));
    ui->actionStop->setIcon(QPixmap("../SDRToolResources/icons/stopSampling.bmp"));
    ui->actionStart->setDisabled(true);
    ui->actionStop->setDisabled(true);

    ui->actionReadCoeffs->setIcon(QPixmap("../SDRToolResources/icons/readCoeff.png"));
    ui->actionWriteCoeffs->setIcon(QPixmap("../SDRToolResources/icons/writeCoeff.png"));
    ui->actionReadCoeffs->setDisabled(true);
    ui->actionWriteCoeffs->setDisabled(true);

    QLabel * logo_Conti = new QLabel(this);
    logo_Conti->setPixmap(QPixmap("../SDRToolResources/icons/logoConti.bmp"));
    logo_Conti->setAlignment(Qt::AlignCenter);
    logo_Conti->setMinimumWidth(200);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(logo_Conti);

    QLabel * logo = new QLabel(this);
    logo->setPixmap(QPixmap("../SDRToolResources/icons/logoBRSfull.bmp"));
    logo->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    logo->setMinimumWidth(200);
    ui->toolBar->addWidget(logo);


    FM_MAIN_INDEX = -1;
    AM_MAIN_INDEX = -1;
    DAB_MAIN_INDEX = -1;
    FM_COMM_INDEX = -1;
    AM_COMM_INDEX = -1;
    DAB_COMM_INDEX = -1;

    //    mainDock = 0;
    //    commonDock = 0;
    logDock = 0;
    logViewer = 0;
    setCentralWidget(0);

    qInstallMessageHandler(myErrorLog);
    reportError( "Starting SDR TOOL");

    ui->menuRecentProjects->setDisabled(true);
    loadRecentProjectList();

}

// Deconstructor
MainWindow::~MainWindow()
{
    on_actionClose_triggered();

    if(logDock != 0) {
        delete logDock;
        logDock = 0;

        QString path = "errorLog";
        QDir dir(path);
        if (dir.exists()) {

            dir.setNameFilters(QStringList() << "*.log");
            dir.setFilter(QDir::Files);
            foreach(QString dirFile, dir.entryList())
            {
                dir.remove(dirFile);

                printDebugString("Remove file : " + dirFile );
            }
        }
    }

    delete ui;

    printDebugString("MainWindow Deleted.");
    qDebug() << "Closing SDR TOOL";
}

void MainWindow::openProject(QString fileName)
{

    if(!fileName.isEmpty()) {

        reportError("Opening Project : " + fileName);

        QFile selectedFile(fileName);

        // Read .proj file
        if (selectedFile.open(QIODevice::ReadOnly)) {

            QString fmSource;
            QString amSource;
            QString dabSource;
            QString configSource;


            // Input File Handler
            QTextStream in(&selectedFile);
            while (!in.atEnd())
            {
                // Read one line
                QString line = in.readLine();
                if(line.at(0) != '#') {                         // Check if comment (Normally Header)

                    QString keyword;
                    QString sourceName;

                    QStringList lists = line.split("\t");       // Break line at each tab space
                    if(lists.length() == 2) {
                        keyword = lists.at(0);
                        sourceName = lists.at(1);


                        if(keyword == "FM_SOURCE") {
                            if(!fmSource.isEmpty()) {                                   // Avoid Repeated entry for same mode in PROJ file.
                                printDebugString("Multiple FM_Source... Ignored!");
                                continue;
                            }
                            QFile checkFile(sourceName);
                            if (checkFile.open(QIODevice::ReadOnly)) {
                                checkFile.close();
                                fmSource = sourceName;
                            } else
                                reportError("FM_Source : " + sourceName + " Not Found!");

                        }
                        else if(keyword == "AM_SOURCE") {                                   // Avoid Repeated entry for same mode in PROJ file.
                            if(!amSource.isEmpty()) {
                                printDebugString("Multiple AM_Source... Ignored!");
                                continue;
                            }
                            QFile checkFile(sourceName);
                            if (checkFile.open(QIODevice::ReadOnly)) {
                                checkFile.close();
                                amSource = sourceName;
                            } else
                                reportError("AM_Source : " + sourceName + " Not Found!");

                        }
                        else if(keyword == "DAB_SOURCE") {                                   // Avoid Repeated entry for same mode in PROJ file.
                            if(!dabSource.isEmpty()) {
                                printDebugString("Multiple DAB_Source... Ignored!");
                                continue;
                            }
                            QFile checkFile(sourceName);
                            if (checkFile.open(QIODevice::ReadOnly)) {
                                checkFile.close();
                                dabSource = sourceName;
                            } else
                                reportError("DAB_Source : " + sourceName + " Not Found!");

                        }
                        else if(keyword == "CONFIG_SOURCE") {
                            QFile checkFile(sourceName);
                            if (checkFile.open(QIODevice::ReadOnly)) {
                                checkFile.close();
                                configSource = sourceName;
                            } else
                                reportError("CONFIG_SOURCE : " + sourceName + " Not Found!");

                        }

                    }

                } else continue;

            }
            selectedFile.close();


            if(!fmSource.isEmpty() || !amSource.isEmpty() || !dabSource.isEmpty()) {
                // Close Previous Widget (if already Open)
                on_actionClose_triggered();
                // Instantiate CentralWidgetController
                centralWidgetOpen = true;
                cWController = new CentralWidgetController;

                // FM
                if(!fmSource.isEmpty()) {
                    printDebugString(fmSource);

                    // Define source to CentralWidgetController
                    cWController->setFmSource(fmSource);
                    printDebugString("FM Source: " + cWController->getFmSource());
                }

                // AM
                if(!amSource.isEmpty()) {
                    printDebugString(amSource);

                    // Define source to CentralWidgetController
                    cWController->setAmSource(amSource);
                    printDebugString("AM Source: " + cWController->getAmSource());
                }

                // DAB
                if(!dabSource.isEmpty()) {
                    printDebugString(dabSource);

                    // Define source to CentralWidgetController
                    cWController->setDabSource(dabSource);
                    printDebugString("DAB Source: " + cWController->getDabSource());
                }

                // Parse Level 1 XML files
                cWController->centralWidgetUiParser();

                // Terminate Process if Error Occured during XML file Parsing
                if(criticalLevelErrorFlag) {
                    on_actionClose_triggered();
                    return;
                }


                printDebugString("FM TabsCount: " + cWController->getFmTabsCount() );
                printDebugString("AM TabsCount: " + cWController->getAmTabsCount() );
                printDebugString("DAB TabsCount: " + cWController->getDabTabsCount() );

                // Parse Level 2 XML Files & Generate Central Widget UI
                cWController->centralWidgetUiPainter();

                // FM
                if(!fmSource.isEmpty()) {
                    FM_COMM_INDEX = addCommonWindow(cWController->getFmCommonTab(), "FM");                // create Common Window;
                    FM_MAIN_INDEX = addMainWindow(cWController->getFmTabWidget(), "FM");                  // create Main Window;

                }

                // AM
                if(!amSource.isEmpty()) {
                    AM_COMM_INDEX = addCommonWindow(cWController->getAmCommonTab(), "AM");              // create Common Window;
                    AM_MAIN_INDEX = addMainWindow(cWController->getAmTabWidget(), "AM");                // create Main Window;
                }

                // DAB
                if(!dabSource.isEmpty()) {
                    DAB_COMM_INDEX = addCommonWindow(cWController->getDabCommonTab(), "DAB");            // create Common Window;
                    DAB_MAIN_INDEX = addMainWindow(cWController->getDabTabWidget(), "DAB");             // create MAIN Window;
                    //                    mainDock.last()->setDisabled(true);
                }

                // Display one of the Loaded Radio Mode on top
                if(FM_MAIN_INDEX != -1 || FM_COMM_INDEX != -1 ) {
                    on_actionFM_triggered();
                } else if(AM_MAIN_INDEX != -1 || AM_COMM_INDEX != -1 ) {
                    on_actionAM_triggered();
                } else if(DAB_MAIN_INDEX != -1 || DAB_COMM_INDEX != -1 ) {
                    on_actionDAB_triggered();
                }

                this->layout()->setContentsMargins(0,0,0,0);

                // Manage Recent Project List
                addRecentProject(fileName);
                loadRecentProjectList();

                // Load Config File (if any)
                if(!configSource.isEmpty()) {
                    cWController->loadConfig(configSource);
                    ui->actionStart->setDisabled(true);
                    ui->actionStop->setDisabled(true);
                }
            }

        }

    }

    if(centralWidgetOpen) {
        ui->actionNetSettings->setEnabled(true);

        ui->actionLoadConfig->setEnabled(true);
        ui->actionSaveConfig->setEnabled(true);
        ui->actionSetupConfig->setEnabled(true);

        ui->actionWriteCoeffs->setEnabled(true);
        ui->actionReadCoeffs->setEnabled(true);
    }

}

int MainWindow::addCommonWindow(QWidget * widget, QString name) {

    if(centralWidgetOpen) {

        if(widget != 0) {

            commonDock.append(new QDockWidget);
            QWidget * commonDockContent = new QWidget(commonDock.last());

            commonDock.last()->setWidget(commonDockContent);
            commonDock.last()->setWindowTitle(name);
            commonDock.last()->layout()->setContentsMargins(0,0,0,0);
            commonDock.last()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            this->addDockWidget(Qt::TopDockWidgetArea, commonDock.last());

            if(commonDock.count() > 1)
                this->tabifyDockWidget(commonDock.at(commonDock.count()-2),commonDock.last());

            QVBoxLayout * controlLayout= new QVBoxLayout();
            QScrollArea * scrollArea = new QScrollArea(commonDock.last());

            controlLayout->addWidget(scrollArea);     // Add widget to layout

            scrollArea->setWidget(widget);

            scrollArea->setWidgetResizable(true);

            commonDockContent->setLayout(controlLayout);
            commonDockContent->setMinimumWidth(widget->width()+38);
            commonDockContent->setMaximumWidth(widget->width()+50);
            commonDockContent->show();
            commonDock.last()->show();
            ui->actionCommonWindow->setChecked(true);

            return (commonDock.count()-1);
        }
    }
    if(commonDock.empty())
        ui->actionCommonWindow->setChecked(false);

    return -1;
}

int MainWindow::addMainWindow(QWidget * widget, QString name) {
    if(centralWidgetOpen) {

        if(widget != 0) {

            mainDock.append(new QDockWidget);
            QWidget * mainDockContent = new QWidget(mainDock.last());

            mainDock.last()->setWidget(mainDockContent);
            mainDock.last()->layout()->setContentsMargins(0,0,0,0);
            mainDock.last()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            this->addDockWidget(Qt::TopDockWidgetArea, mainDock.last());

            if(mainDock.count() > 1)
                this->tabifyDockWidget(mainDock.at(mainDock.count()-2),mainDock.last());

            QVBoxLayout * controlLayout= new QVBoxLayout();
            QScrollArea * scrollArea = new QScrollArea(mainDock.last());

            controlLayout->addWidget(scrollArea);     // Add widget to layout

            scrollArea->setWidget(widget);
            mainDock.last()->setWindowTitle(name);

            scrollArea->setWidgetResizable(true);

            mainDockContent->setLayout(controlLayout);
            mainDockContent->resize(800,600);
            // mainDockContent->setMaximumWidth(cWController->getTabWidget()->width()+38);
            // mainDockContent->setMinimumHeight(600);
            mainDockContent->show();
            mainDock.last()->show();
            ui->actionMainWindow->setChecked(true);

            return (mainDock.count()-1);
        }
    }
    if (mainDock.empty()) {
        ui->actionMainWindow->setChecked(false);
    }
    return -1;
}

void MainWindow::createLogWindow() {
    if(logDock == 0) {
        logDock = new QDockWidget;
        QWidget * logDockContent = new QWidget(logDock);

        logDock->setWidget(logDockContent);
        logDock->setWindowTitle("Error Log");
        logDock->layout()->setContentsMargins(0,0,0,0);
        logDock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        logDock->setMaximumHeight(130);
        this->addDockWidget(Qt::BottomDockWidgetArea, logDock);

        QVBoxLayout * controlLayout= new QVBoxLayout();
        QScrollArea * scrollArea = new QScrollArea(logDock);

        controlLayout->addWidget(scrollArea);     // Add widget to layout

        logViewer = new QPlainTextEdit(logDock);
        scrollArea->setWidget(logViewer);

        scrollArea->setWidgetResizable(true);

        watcher.addPath("errorLog/" + QDate::currentDate().toString("SDRLOG_dd_MM_yyyy.log"));
        connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateErrorLog(QString)));

        logDockContent->setLayout(controlLayout);
        logDockContent->show();
        logDock->show();
        updateErrorLog("");

        ui->actionLog_Window->setChecked(true);
    } else {
        ui->actionLog_Window->setChecked(false);
    }
}

void MainWindow::addRecentProject(QString fileName) {

    QString outputDir = "tmp";

    QDir dir(outputDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile outputFile(outputDir+"/recentProjList");

    if (!outputFile.open(QIODevice::ReadWrite|QFile::ReadWrite)) {
        printDebugString("Could not create File: " + outputDir + "/recentProjList");
    }
    else {
        // Output File Handler
        QIODevice *outDevice = &outputFile;
        QTextStream outFileStream(outDevice);

        QStringList line;
        while( !outFileStream.atEnd())
        {
            line << outFileStream.readLine();
        }

        if(line.count() == 0) {
            line << "#	This is a list of recent project files.";
            line << fileName;
        } else {

            for(int i = 0; i < line.count(); i++){
                if(line.at(i) == fileName) {
                    line.removeAt(i);
                    break;
                }
            }

            if(line.count() >= 6)
                line.removeAt(1);

            line << fileName;

        }

        outputFile.close();

        if (outputFile.open(QIODevice::WriteOnly|QFile::WriteOnly)) {

            for(int i = 0; i < line.count(); i++)
                outFileStream << line.at(i) << endl;

            outputFile.close();
        }

    }
}

void MainWindow::loadRecentProjectList() {

    QString inputDir = "tmp";
    QString fileName = "recentProjList";
    QFile inputFile(inputDir+"/"+fileName);

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QList<QAction *> actions = ui->menuRecentProjects->actions();
        for(int i = 0; i < actions.count(); i++)
            disconnect(actions.at(i), SIGNAL(triggered()));

        ui->menuRecentProjects->clear();

        // Input File Handler
        QTextStream in(&inputFile);

        // Read one line
        // QString line = in.readLine();

        QStringList lines;
        while( !in.atEnd())
        {
            lines << in.readLine();
        }

        QString line;
        for(int i = lines.count() - 1; i > 0; i--) {
            line = lines.at(i);
            if(line.at(0) != '#') {                         // Check if comment (Normally Header)

                QStringList lists = line.split("/");       // Break line at each tab space
                if(lists.length() >= 2) {

                    QAction * recent = new QAction(tr("...") + tr("/") + lists.at(lists.length()-2) + tr("/") + lists.at(lists.length()-1), this);
                    ui->menuRecentProjects->addAction(recent);
                    ui->menuRecentProjects->setEnabled(true);
                    recentProjects.append(line);

                    connect(recent, SIGNAL(triggered()), this, SLOT(actionRecentProject()));

                }

            } else continue;
        }
        inputFile.close();
    } else {
        printDebugString("Could not open recent File: " + inputDir + "/" + fileName);

    }
}

void MainWindow::resetErrorFlags() {
    criticalLevelErrorFlag = false;
    highLevelErrorFlag = false;
    averageLevelErrorFlag = false;
    lowLevelErrorFlag = false;

}

void MainWindow::clearTemporaryFiles() {

    QString path = "layout";
    QDir dir(path);
    if (dir.exists()) {

        dir.setNameFilters(QStringList() << "*.lay");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);

            printDebugString("Remove file : " + dirFile );
        }
    }

}

// Response to 'open' action signal.
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Project"), QDir::currentPath(), tr("PROJ files (*.proj)"));
    openProject(fileName);
}

// Response to 'close' action signal.
void MainWindow::on_actionClose_triggered()
{
    if(centralWidgetOpen) {

        reportError("Closing Project");

        printDebugString("closing");
        cWController->closeNetworkConnection();
        printDebugString("netclose");
        setCentralWidget(0);                   // takeCentralWidget();
        printDebugString("cw 0");
        centralWidgetOpen = false;
        printDebugString("del cw");
        delete cWController;
        printDebugString("cw delted");

        for(int i = 0; i < mainDock.count(); i++) {
            delete mainDock.at(i);
        }
        mainDock.clear();

        for(int i = 0; i < commonDock.count(); i++) {
            delete commonDock.at(i);
        }
        commonDock.clear();


        FM_MAIN_INDEX = -1;
        AM_MAIN_INDEX = -1;
        DAB_MAIN_INDEX = -1;
        FM_COMM_INDEX = -1;
        AM_COMM_INDEX = -1;
        DAB_COMM_INDEX = -1;

        printDebugString("Controller Deleted.");
    }

    if(!centralWidgetOpen) {
        ui->actionNetSettings->setDisabled(true);
        ui->actionStart->setDisabled(true);
        ui->actionStop->setDisabled(true);
        ui->actionLoadConfig->setDisabled(true);
        ui->actionSaveConfig->setDisabled(true);
        ui->actionSetupConfig->setDisabled(true);
    }

    clearTemporaryFiles();

    resetErrorFlags();

}

// Response to 'quit' action signal.
void MainWindow::on_actionQuit_triggered()
{
    on_actionClose_triggered();
    qApp->quit();
}

void MainWindow::on_actionNetSettings_triggered()
{
    if(centralWidgetOpen) {
        cWController->openConnectionSettings();
        ui->actionStart->setEnabled(true);
    }

}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_actionLoadConfig_triggered()
{
    if(centralWidgetOpen) {

        QString srcName = QFileDialog::getOpenFileName(this, tr("Select Config File"), QDir::currentPath(), tr("Config files (*.cfg)"));

        if(!srcName.isEmpty()) {

            printDebugString(srcName);

            QMessageBox message;
            message.setText("You have selected :" + srcName);
            message.exec();
            QThread::currentThread()->msleep(50);

            cWController->loadConfig(srcName);

            ui->actionStart->setDisabled(true);
            ui->actionStop->setDisabled(true);
        }
    }
}

void MainWindow::on_actionSaveConfig_triggered()
{
    if(centralWidgetOpen) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Config File"), QDir::currentPath(), tr("Config File (*.cfg)"));

        if(!fileName.isEmpty()) {

            printDebugString(fileName);

            QMessageBox message;
            message.setText("Saving File : " + fileName);
            message.exec();
            // wait();

            cWController->saveConfig(fileName);
        }
    }
}

void MainWindow::on_actionSetupConfig_triggered()
{
    if(centralWidgetOpen) {
        cWController->displayConfig();
    }
}

void MainWindow::on_actionStart_triggered()
{
    if(centralWidgetOpen) {
        if(cWController->startCommunication()) {
            cWController->updateTab();
            ui->actionStop->setEnabled(true);
            ui->actionStop->setEnabled(true);

            printDebugString("Start Action.");
        }
    }

}

void MainWindow::on_actionStop_triggered()
{
    if(centralWidgetOpen) {
        cWController->stopCommunication();
        cWController->haltAllTabs();
    }
}

void MainWindow::on_actionRead_Coefficient_triggered()
{
    if(centralWidgetOpen) {
        cWController->openCoeffReadWriteWindow();
    }
}

void MainWindow::on_actionMainWindow_triggered()
{
    for(int i = 0; i < mainDock.count(); i++){
        if(mainDock.at(i)->isHidden()) {
            mainDock.at(i)->show();
            ui->actionMainWindow->setChecked(true);
        } else {
            mainDock.at(i)->hide();
            ui->actionMainWindow->setChecked(false);
        }
    }

    if(mainDock.empty())
        ui->actionMainWindow->setChecked(false);
}

void MainWindow::on_actionCommonWindow_triggered()
{
    for(int i = 0; i < commonDock.count(); i++){
        if(commonDock.at(i)->isHidden()) {
            commonDock.at(i)->show();
            ui->actionCommonWindow->setChecked(true);
        } else {
            commonDock.at(i)->hide();
            ui->actionCommonWindow->setChecked(false);
        }
    }

    if(commonDock.empty())
        ui->actionCommonWindow->setChecked(false);
}

void MainWindow::on_actionLog_Window_triggered()
{
    if(logDock != 0) {
        if(logDock->isHidden()) {
            logDock->show();
            ui->actionLog_Window->setChecked(true);
        } else {
            logDock->hide();
            ui->actionLog_Window->setChecked(false);
        }
    } else {
        createLogWindow();
    }
}

void MainWindow::on_actionNew_triggered()
{
    createProjectWindow = new CreateProjectWindow(this);
    createProjectWindow->show();
}

void MainWindow::actionRecentProject() {

    QAction * recent = qobject_cast<QAction*>(sender());

    QString str;
    QStringList lists;

    for(int i = 0; i < recentProjects.count(); i++) {
        lists = recentProjects.at(i).split("/");

        if(lists.length() >= 2) {
            str = QString(tr("/") + lists.at(lists.length()-2) + tr("/") + lists.at(lists.length()-1));

            if(recent->iconText() == str) {
                openProject(recentProjects.at(i));
                break;
            }
        }
    }
}

void MainWindow::on_actionFM_triggered()
{
    if(centralWidgetOpen) {

        if((FM_COMM_INDEX != -1) || (FM_MAIN_INDEX != -1))
            for(int i = 0; i < commonDock.count(); i++)
                commonDock.at(i)->setDisabled(true);

        if(FM_COMM_INDEX != -1) {

            commonDock.at(FM_COMM_INDEX)->setEnabled(true);
            ui->actionFM->setChecked(true);
            ui->actionAM->setChecked(false);
            ui->actionDAB->setChecked(false);

            cWController->selectRadioMode(FM_MODE);

            commonDock.at(FM_COMM_INDEX)->raise();
        }

        if((FM_COMM_INDEX != -1) || (FM_MAIN_INDEX != -1))
            for(int i = 0; i < mainDock.count(); i++)
                mainDock.at(i)->setDisabled(true);

        if(FM_MAIN_INDEX != -1) {

            mainDock.at(FM_MAIN_INDEX)->setEnabled(true);
            ui->actionFM->setChecked(true);
            ui->actionAM->setChecked(false);
            ui->actionDAB->setChecked(false);

            cWController->selectRadioMode(FM_MODE);

            mainDock.at(FM_MAIN_INDEX)->raise();
        }

        if((FM_COMM_INDEX != -1) || (FM_MAIN_INDEX != -1)) {
            cWController->updateTab();
        } else
            ui->actionFM->setChecked(false);

    } else
        ui->actionFM->setChecked(false);
}

void MainWindow::on_actionAM_triggered()
{
    if(centralWidgetOpen) {

        if((AM_COMM_INDEX != -1) || (AM_MAIN_INDEX != -1))
            for(int i = 0; i < commonDock.count(); i++)
                commonDock.at(i)->setDisabled(true);

        if(AM_COMM_INDEX != -1) {

            commonDock.at(AM_COMM_INDEX)->setEnabled(true);
            ui->actionFM->setChecked(false);
            ui->actionAM->setChecked(true);
            ui->actionDAB->setChecked(false);

            cWController->selectRadioMode(AM_MODE);

            commonDock.at(AM_COMM_INDEX)->raise();
        }

        if((AM_COMM_INDEX != -1) || (AM_MAIN_INDEX != -1))
            for(int i = 0; i < mainDock.count(); i++)
                mainDock.at(i)->setDisabled(true);

        if(AM_MAIN_INDEX != -1) {

            mainDock.at(AM_MAIN_INDEX)->setEnabled(true);
            ui->actionFM->setChecked(false);
            ui->actionAM->setChecked(true);
            ui->actionDAB->setChecked(false);

            cWController->selectRadioMode(AM_MODE);

            mainDock.at(AM_MAIN_INDEX)->raise();
        }

        if((AM_COMM_INDEX != -1) || (AM_MAIN_INDEX != -1)) {
            cWController->updateTab();
        } else
            ui->actionAM->setChecked(false);

    } else
        ui->actionAM->setChecked(false);

}

void MainWindow::on_actionDAB_triggered()
{
    if(centralWidgetOpen) {

        if((DAB_COMM_INDEX != -1) || (DAB_MAIN_INDEX != -1))
            for(int i = 0; i < commonDock.count(); i++)
                commonDock.at(i)->setDisabled(true);

        if(DAB_COMM_INDEX != -1) {

            commonDock.at(DAB_COMM_INDEX)->setEnabled(true);
            ui->actionFM->setChecked(false);
            ui->actionAM->setChecked(false);
            ui->actionDAB->setChecked(true);

            cWController->selectRadioMode(DAB_MODE);

            commonDock.at(DAB_COMM_INDEX)->raise();
        }

        if((DAB_COMM_INDEX != -1) || (DAB_MAIN_INDEX != -1))
            for(int i = 0; i < mainDock.count(); i++)
                mainDock.at(i)->setDisabled(true);

        if(DAB_MAIN_INDEX != -1) {

            mainDock.at(DAB_MAIN_INDEX)->setEnabled(true);
            ui->actionFM->setChecked(false);
            ui->actionAM->setChecked(false);
            ui->actionDAB->setChecked(true);

            cWController->selectRadioMode(DAB_MODE);

            mainDock.at(DAB_MAIN_INDEX)->raise();
        }

        if((DAB_COMM_INDEX != -1) || (DAB_MAIN_INDEX != -1)) {
            cWController->updateTab();
        } else
            ui->actionDAB->setChecked(false);

    } else
        ui->actionDAB->setChecked(false);
}

void MainWindow::on_actionWriteCoeffs_triggered()
{
    if(centralWidgetOpen) {
        if(cWController->initWriteAllCoeffs()) {
            qDebug() << "Write all coeffs finished sucessfully." ;
        } else {
            qDebug() << "Write all coeffs ERROR." ;
        }
    }

}

void MainWindow::on_actionReadCoeffs_triggered()
{
    if(centralWidgetOpen) {
        if(cWController->initReadAllCoeffs()) {
            qDebug() << "Read all coeffs finished sucessfully." ;
        } else {
            qDebug() << "Read all coeffs ERROR." ;
        }
    }
}


void MainWindow::updateErrorLog(QString str)
{
    Q_UNUSED(str)

    if(logViewer != 0) {

        QFile file("errorLog/" + QDate::currentDate().toString("SDRLOG_dd_MM_yyyy.log"));

        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream readFile(&file);
            logViewer->setPlainText(readFile.readAll());
            logViewer->verticalScrollBar()->setValue((logViewer->verticalScrollBar())->maximum());

            file.close();
        }

    }

}


// ***********************************************************************

/*
QMdiSubWindow * MainWindow::createSubWindow(QWidget * newWidget) {
    if(newWidget != 0) {

        // Creates the layout to contain central widget.
        QScrollArea * scrollArea = new QScrollArea(this);

        QVBoxLayout * controlLayout= new QVBoxLayout;
        controlLayout->addWidget(newWidget);     // Add widget to layout

        QHBoxLayout * hLayout3 = new QHBoxLayout;
        hLayout3->addLayout(controlLayout);

        QWidget *baseArea = new QWidget();
        baseArea->setLayout(hLayout3);
        scrollArea->setWidget(baseArea);

        scrollArea->widget()->setLayout(hLayout3);

        //        QHBoxLayout *hLayoutScrollArea = new QHBoxLayout;
        //        hLayoutScrollArea->addWidget(scrollArea);

        //        QVBoxLayout * vLayout=new QVBoxLayout;
        //        vLayout->addLayout(hLayoutScrollArea);

        //Create QMdiSubWindow
        QMdiSubWindow * subWindow = new QMdiSubWindow(this);
        subWindow->resize(600,400);
        subWindow->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//        subWindow->showMaximized();

        subWindow->layout()->addWidget(scrollArea);
        return subWindow;
    }
    return 0;
}
*/
