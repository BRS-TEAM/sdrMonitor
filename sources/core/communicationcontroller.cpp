#include "headers/core/communicationcontroller.h"
#include "headers/core/readwritewindow.h"
#include "headers/core/errorlogger.h"
#include <QProgressBar>

#define NOTSELECTED 0
#define ETHERNET 1

//#define SELECTED_MODULE "FM_Main"

CommunicationController::CommunicationController(QWidget * parent)
    : QWidget(parent)
{

    protocol = NOTSELECTED;
    selectedModule = "NOTSELECTED";

    eComProcess = 0;
    //eComManagerProcess=0;
    dManager = 0;
    oldDataStructure.clear();


    // Init Network Manager Window
    connectionWindow = new ConnectionWindow;

    // Interface Connections
    QObject::connect(connectionWindow, SIGNAL(connectButtonSignal(QString, int)), this, SLOT(connect(QString, int)));
    QObject::connect(connectionWindow, SIGNAL(readButtonSignal()), this, SLOT(initProcess()));
    QObject::connect(connectionWindow, SIGNAL(disconnectButtonSignal()), this, SLOT(interruptCommunication()));
    QObject::connect(connectionWindow, SIGNAL(disconnectButtonSignal()), this, SLOT(disConnect()));

    //Timer

    // Temp
    QTextStream out(stdout);
    out << "CommunicationController :: belongs to thread: " << QThread::currentThreadId() << endl;
}

CommunicationController::~CommunicationController()
{
    // Stop Communication Process
    shutDown();

    // Release Resources
    if(eComProcess != 0) {
        // QThread::currentThread()->msleep(10);
        delete eComProcess;
    }
    delete connectionWindow;

    // LOG
    printDebugString("CommunicationController :: Deleted");
}

void CommunicationController::showConnectionWindow() {
    connectionWindow->show();
    connectionWindow->raise();
    QApplication::setActiveWindow(connectionWindow);
}

// Initialize Controller Connections with Communication Process.
void CommunicationController::initConnections() {

    switch (protocol) {
    case ETHERNET:
        // Ethernet Connections
        QObject::connect(this, SIGNAL(requestConnect(QString,int, int)), eComProcess, SLOT(requestConnect(QString, int, int)));
        QObject::connect(this, SIGNAL(requestInitProcess()), eComProcess, SLOT(process()));
        QObject::connect(this, SIGNAL(requestDisconnect()), eComProcess, SLOT(requestDisconnect()));
        QObject::connect(eComProcess, SIGNAL(connectionStatus(bool)), this, SLOT(changeConnectionStatus(bool)));
        QObject::connect(eComProcess, SIGNAL(connectionStatus(bool)), connectionWindow, SLOT(changeButtonStatus(bool)));

        break;
        /*
         * Add more protocol CASE here
         */
    }



}

void CommunicationController::shutDown() {

    // Stop Communication Process
    switch (protocol) {
    case ETHERNET:
        this->disConnect();
        eComProcess->shutDown();
        break;

        /*
         * Add more protocol CASE here
         */
    }

    protocol = NOTSELECTED;

}

bool CommunicationController::setDataManager(DataManager * dManager) {
    this->dManager = dManager;
    if(loadDataSource()) {
        return true;
    }
    return false;
}

bool CommunicationController::loadDataSource() {

    switch (protocol) {
    case ETHERNET:
        if(eComProcess != 0 && dManager!= 0) {

            QList<DataStructure*> ds = this->dManager->getModuleStructure(selectedModule);

            QList<DataStructure*> cds = this->dManager->getModuleStructure(commonModuleList);

            for(int i = 0; i < cds.count(); i++) {
                ds.append(cds.at(i));
            }

            eComProcess->interruptProcess();

            eComProcess->resetDataStructure();

            for(int i = 0; i < ds.count(); i++)
                if(ds.at(i)->read && ds.at(i)->write)
                    ds.removeAt(i--);


            for(int i = 0; i < ds.count(); i++)
                eComProcess->setDataStructure(ds.at(i));

            // Restart process
            initProcess();
            return true;
        }
        return false;

        break;
        /*
         * Add more protocol CASE here
         */
    }

    return false;
}

void CommunicationController::interruptProcess() {

    if(eComProcess != 0)
        eComProcess->interruptProcess();
    /*
     * Interrupt other protocol processes here.
     */

}

void CommunicationController::loadDataSourceTemp(DataStructure * dS) {

    switch (protocol) {
    case ETHERNET:
        if(eComProcess != 0) {

            eComProcess->resetTempDataStructure();

            eComProcess->setTempDataStructure(dS);
        }
        break;
        /*
         * Add more protocol CASE here
         */
    }
}

void CommunicationController::setSelectedModule(QString module) {
    this->selectedModule = module;
    loadDataSource();
}

void CommunicationController::setCommonModule(QList<QString> moduleList) {
    this->commonModuleList = moduleList;
}

void CommunicationController::initExtReadRequest(QList<DataStructure *> dS) {
    printDebugString( "CommunicationController :: initAsynRequest ");

    switch (protocol) {
    case ETHERNET:
        if(eComProcess != 0) {
            //QProgressBar pBar;
            for(int i = 0; i < dS.count(); i++) {
                if(dS.at(i) != 0) {

                    if(dS.at(i)->size==32)
                        QThread::currentThread()->msleep(50);
                    else
                        QThread::currentThread()->msleep(20);

                    QString address = dS.at(i)->address;
                    int size = dS.at(i)->size;
                    loadDataSourceTemp(dS.at(i));
                    for(int count = 0; count < 5; count++) {
                        if(eComProcess->readFunction(address, size, 'e') == 1) {
                            printDebugString(  "CommunicationController :: ReadFunction Success ");
                            break;
                        }
                        // qDebug() << "CommunicationController :: Semaphore Occupied ";
                    }
                    if(dS.at(i)->size==32)
                        QThread::currentThread()->msleep(20);
                }

            }
        }
        break;
        /*
         * Add more protocol CASE here
         */
    }

}

void CommunicationController::initExtWriteRequest(QList<DataStructure *> dS, QList<QString> values) {
    printDebugString( "CommunicationController :: initWriteAsynRequest ");

    switch (protocol) {
    case ETHERNET:

        if(eComProcess != 0) {

            eComProcess->setExternalWait(true);
            bool ok = false;
            for(int i = 0; i < dS.count(); i++) {

                if(dS.at(i) != 0) {

                    if(dS.at(i)->size == 32)
                        QThread::currentThread()->msleep(50);
                    else
                        QThread::currentThread()->msleep(20);

                    QString address = dS.at(i)->address;
                    int size = dS.at(i)->size;
                    printDebugString("Address : " + address);
                    printDebugString( "Str Values : " + values.at(i));
                    int value = QString(values.at(i)).toInt(&ok,10);
                    printDebugString( "int Value : " + QString().setNum(value));
                    for(int count = 0; count < 5; count++) {
                        if(eComProcess->writeFunction(address, size, value, 'e') == 1) {
                            qDebug() << "CommunicationController :: WriteFunction Success :: Address : " << address;
                            break;
                        }
                        qDebug() << "CommunicationController :: Semaphore Occupied :: Address : " << address;
                    }

                    if(dS.at(i)->size==32)
                        QThread::currentThread()->msleep(20);
                }

            }
            eComProcess->setExternalWait(false);
        }

        break;
        /*
         * Add more protocol CASE here
         */
    }

}

void CommunicationController::showCoeffReadWriteWindow() {

    switch (protocol) {
    case ETHERNET:
        if(eComProcess != 0 && dManager!= 0) {

            oldDataStructure = eComProcess->getDataStructure();

            eComProcess->interruptProcess();

            eComProcess->resetDataStructure();

            rwWin = new ReadWriteWindow(this);
            rwWin->show();
            rwWin->raise();
            QApplication::setActiveWindow(rwWin);
            QObject::connect(rwWin, SIGNAL(terminate()), this, SLOT(closeCoeffReadWriteWindow()));

            //for(int i = 0; i < ds.count(); i++)
            //  eComProcess->setDataStructure(ds.at(i));

        }
        break;

        /*
         * Add more protocol CASE here
         */
    }

}

void CommunicationController::initExtContinuousReadRequest(DataStructure * dStructure) {

    switch (protocol) {
    case ETHERNET:
        if(eComProcess != 0) {

            if(dManager!= 0 && dStructure != 0) {

                eComProcess->interruptProcess();

                eComProcess->resetDataStructure();

                eComProcess->setDataStructure(dStructure);

                // Restart process
                initProcess();
            }
        }
        break;

        /*
         * Add more protocol CASE here
         */
    }
}

void CommunicationController::stopExtContinuousReadRequest() {

    switch (protocol) {
    case ETHERNET:

        if(eComProcess != 0 && dManager!= 0) {
            eComProcess->interruptProcess();
            eComProcess->resetDataStructure();
        }
        break;
        /*
         * Add more protocol CASE here
         */

    }
}

bool CommunicationController::requestWriteAllCoeffs(QList<DataStructure *> dStructures) {

    switch (protocol) {
    case ETHERNET:
        if(eComProcess != 0) {

            if(dStructures.count()) {

                QWidget processWindow;
                QVBoxLayout vBox;
                QLabel label;
                label.setText("Please Wait...");
                QProgressBar pBar;
                pBar.setMinimum(0);
                pBar.setMaximum(100);
                pBar.setOrientation(Qt::Horizontal);
                //        pBar.setGeometry(500,500,500,300);
                pBar.setStyleSheet(QString("::chunk:horizontal { background-color: qlineargradient(x0: 0, x2: 1, stop: 0 #05B8CC, stop: 1 #05B8CC )}"
                                           "::chunk {background-color: #05B8CC; margin-top: 0.5px; margin-bottom: 0.5px; border-radius: 5px; }"
                                           "QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                                           "QProgressBar:horizontal { border: 2px solid gray; border-radius: 3px; background: white; }"
                                           "::chunk:vertical { background-color: qlineargradient(y0: 1, y2: 0, stop: 1 #05B8CC, stop: 0 #05B8CC )}"
                                           "QProgressBar:vertical { border: 2px solid gray; border-radius: 3px; background: white;}"));
                vBox.addWidget(&pBar);
                vBox.addWidget(&label);

                // pBar.show();
                // pBar.move((QApplication::desktop()->width() - pBar.width() ) / 2, (QApplication::desktop()->height() - pBar.height() ) / 2);

                processWindow.setLayout(&vBox);
                processWindow.show();
                processWindow.move((QApplication::desktop()->width() - pBar.width() ) / 2, (QApplication::desktop()->height() - pBar.height() ) / 2);

                oldDataStructure = eComProcess->getDataStructure();

                QApplication::processEvents();

                pBar.setValue(1);

                QApplication::processEvents();

                QThread::currentThread()->msleep(100);

                eComProcess->interruptProcess();

                QList<QString> values;

                for(int i = 0; i < dStructures.count(); i++) {
                    // values.append(QString().setNum((int)dStructures.at(i)->value, 16).right(dStructures.at(i)->size / 4));
                    values.append(QString().setNum((int)dStructures.at(i)->value));
                }

                pBar.setValue(10);

                QApplication::processEvents();

                QThread::currentThread()->msleep(200);

                initExtWriteRequest(dStructures, values);

                pBar.setValue(100);
                label.setText("Done...");

                QApplication::processEvents();

                QThread::currentThread()->msleep(400);

                //if(!oldDataStructure.isEmpty());
                    // initProcess();


                oldDataStructure.clear();

                return true;
            }
            else {
                qDebug() << "In communicationController :: requestWriteAllCoeffs received empty structure" << endl;
                return false;
            }
        } else
            qDebug() << "In communicationController :: Not connected to target" << endl;
        return false;

        break;
        /*
         * Add more protocol CASE here
         */

    }

    qDebug() << "In communicationController :: Not connected to target" << endl;
    return false;

}

bool CommunicationController::requestReadAllCoeffs(QList<DataStructure *> dStructures) {

    switch (protocol) {
    case ETHERNET:
        if(eComProcess != 0) {

            if(dStructures.count()) {

                QWidget processWindow;
                QVBoxLayout vBox;
                QLabel label;
                label.setText("Please Wait...");
                QProgressBar pBar;
                pBar.setMinimum(0);
                pBar.setMaximum(100);
                pBar.setOrientation(Qt::Horizontal);
                //        pBar.setGeometry(500,500,500,300);
                pBar.setStyleSheet(QString("::chunk:horizontal { background-color: qlineargradient(x0: 0, x2: 1, stop: 0 #05B8CC, stop: 1 #05B8CC )}"
                                           "::chunk {background-color: #05B8CC; margin-top: 0.5px; margin-bottom: 0.5px; border-radius: 5px; }"
                                           "QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                                           "QProgressBar:horizontal { border: 2px solid gray; border-radius: 3px; background: white; }"
                                           "::chunk:vertical { background-color: qlineargradient(y0: 1, y2: 0, stop: 1 #05B8CC, stop: 0 #05B8CC )}"
                                           "QProgressBar:vertical { border: 2px solid gray; border-radius: 3px; background: white;}"));
                vBox.addWidget(&pBar);
                vBox.addWidget(&label);

                // pBar.show();
                // pBar.move((QApplication::desktop()->width() - pBar.width() ) / 2, (QApplication::desktop()->height() - pBar.height() ) / 2);

                processWindow.setLayout(&vBox);
                processWindow.show();
                processWindow.move((QApplication::desktop()->width() - pBar.width() ) / 2, (QApplication::desktop()->height() - pBar.height() ) / 2);

                oldDataStructure = eComProcess->getDataStructure();

                QApplication::processEvents();

                pBar.setValue(1);

                QApplication::processEvents();

                eComProcess->interruptProcess();

                QThread::currentThread()->msleep(100);

                pBar.setValue(10);

                QApplication::processEvents();

                initExtReadRequest(dStructures);

                pBar.setValue(100);
                label.setText("Done...");

                QApplication::processEvents();

                QThread::currentThread()->msleep(50);

                if(!oldDataStructure.isEmpty())
                    initProcess();


                oldDataStructure.clear();

                return true;
            }
            else {
                qDebug() << "In communicationController :: requestReadAllCoeffs received empty structure" << endl;
                return false;
            }
        } else
            qDebug() << "In communicationController :: Not connected to target" << endl;
        return false;

        break;
        /*
         * Add more protocol CASE here
         */
    }

    qDebug() << "In communicationController :: Not connected to target" << endl;
    return false;

}

/************************ Slots are special methods which execute in response to the connected signal. ************************/

void CommunicationController::connect(QString address, int port) {

    // Get Selected Protocol
    protocol = connectionWindow->getProtocol();
    checkStatus = 0;
    // Initialize Protocol
    switch (protocol) {
    case ETHERNET:
        if(eComProcess == 0) {
            eComProcess = EthernetComProcess::createInstance();   // Instantiate Communication Process
            initConnections();                      // Initialize Process Connections
            checkStatus = 1;
        }
        break;
        /*
         * Add more protocol CASE here
         */

    default:
        break;
    }

    // Request Connection
    emit requestConnect(address, port, checkStatus);

}

void CommunicationController::disConnect() {

    // Request Disonnection
    emit requestDisconnect();

}

void CommunicationController::initProcess()
{
    emit requestInitProcess();
}

void CommunicationController::interruptCommunication() {
    if(eComProcess != 0)
        eComProcess->setInterrupt(true);
    /*
     * Interrupt other protocol communication here.
     */
}

void CommunicationController::changeConnectionStatus(bool status) {
    if (status) { }                     // Connected
    else {                              // Disconnected
    /*    if(eComProcess != 0) {
            QObject::disconnect(this, SIGNAL(requestConnect(QString,int)), eComProcess, SLOT(requestConnect(QString, int)));
            QObject::disconnect(this, SIGNAL(requestInitProcess()), eComProcess, SLOT(process()));
            QObject::disconnect(this, SIGNAL(requestDisconnect()), eComProcess, SLOT(requestDisconnect()));
            delete eComProcess;
            eComProcess = 0;
        }

          Check other protocols here.


        protocol = NOTSELECTED;*/
         //delete eComProcess;
    }
}

void CommunicationController::closeCoeffReadWriteWindow() {

    switch (protocol) {
    case ETHERNET:

        if(!oldDataStructure.isEmpty() && eComProcess != 0) {

            eComProcess->interruptProcess();
            eComProcess->resetDataStructure();

            for(int i = 0; i < oldDataStructure.count(); i++)
                eComProcess->setDataStructure(oldDataStructure.at(i));

            oldDataStructure.clear();

            // Restart process
            initProcess();
        }
        break;
        /*
         * Add more protocol CASE here
         */
    }
}
