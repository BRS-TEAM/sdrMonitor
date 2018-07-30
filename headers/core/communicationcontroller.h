/*!
 * \file     communicationcontroller.h
 * \brief    Communincation controller.
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

// CommunincationController
#ifndef COMMUNICATIONCONTROLLER_H
#define COMMUNICATIONCONTROLLER_H

// Essential Headers
#include <QWidget>
#include <QtUiTools/QtUiTools>
#include <QTcpSocket>
#include <QNetworkSession>

// Network Setting Window
#include "headers/core/connectionwindow.h"

// Ethernet Communication Protocol
#include "headers/communication/ethernetcomprocess_copy.h"
// Data Manager
#include "headers/core/datamanager.h"

class ReadWriteWindow;


/*!
 * \brief The Communication controller class.
 */
class CommunicationController : public QWidget
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
    CommunicationController(QWidget * parent = 0);

    //! Destructor
    ~CommunicationController();

    /************************ General Methods specific to this class. ************************/

    //! Show Network Setting Window
    void showConnectionWindow();

    //! Initialize the communication controller for selected protocol
    /*!
     * It is used to create link between #CommunicationController class and class of seleted communication protocol.
     *
     * It is usually called by connect(QString, int) SLOT of #CommunicationController
     *
     * \sa connect(QString, int)
     */
    void initConnections();

    //! Request to Stop all the Communication Thread and Disconnect the target.
    void shutDown();

    //! Set the #DataManger to be used by #CommunincationController.
    /*!
     * SETTER :: \a dManager
     *
     * It also calls the loadDataSource() to load necessary #DataStructure (provided by #DataManager) into Selected Communincation Protocol Class.
     *
     * \sa loadDataSource()
     */
    bool setDataManager(DataManager *);

    //! Load necessary #DataStructure (provided by \a dManager) into Selected Communincation Protocol Class
    /*!
     *
     * It is usually called by setDataManager(DataManager *) .
     *
     * \sa setDataManager(DataManager *)
     */
    bool loadDataSource();

    //! Inititate the interruption request to Selected Communincation Protocol Class
    void interruptProcess();

    //! Load temporary #DataStructure into Selected Communincation Protocol Class
    /*!
     *
     * It is used to perform external (initiated from HMI) Read/Write operation.
     *
     */
    void loadDataSourceTemp(DataStructure*);

    //! Inform #CommunicationController about the selected tab.
    /*!
     *
     * Load #DataStructure of the selected tab module.
     *
     * This function uses loadDataSource() to reload #DataStructure into Selected Communincation Protocol Class.
     *
     */
    void setSelectedModule(QString);

    void setCommonModule(QList<QString> moduleList);

    //! Init external (initiated from HMI) Read Request
    void initExtReadRequest(QList<DataStructure*>);

    //! Init external (initiated from HMI) Write Request
    void initExtWriteRequest(QList<DataStructure*>, QList<QString> );

    //! Show Coefficient Read/Write Window.
    void showCoeffReadWriteWindow();

    //! Init external (initiated from Coefficient Read/Write Window.) Continuous Read Request
    void initExtContinuousReadRequest(DataStructure *);

    //! Init external (initiated from Coefficient Read/Write Window.) Continuous Write Request
    void stopExtContinuousReadRequest();

    //! Init Memory Dump : Write to Target
    bool requestWriteAllCoeffs(QList<DataStructure *> dStructures);

    //! Init Memory Dump : Read From Target
    bool requestReadAllCoeffs(QList<DataStructure *> dStructures);

    // *********************************************************************** //
    //                                                                         //
    //                              Signals                                    //
    //                                                                         //
    // *********************************************************************** //
signals:
    /************************ Signals ************************/

    //! Signal : connect;  Receiver: Selected Communincation Protocol Class.
    void requestConnect(QString, int, int);

    //! Signal : disconnect;  Receiver: Selected Communincation Protocol Class.
    void requestDisconnect();

    //! Signal : Init Sampling Process;  Receiver: Selected Communincation Protocol Class.
    void requestInitProcess();

    //! Signal : Stop Sampling Process;  Receiver: Selected Communincation Protocol Class.
    void requestStopProcess();

    //! Signal : Interrupt Sampling Process;            NOT USED ANYMORE.
    void requestInterruptProcess();
    // void terminate();

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to 'connect' signal from #ConnectionWindow
    /*!
     * \param address : Host Address
     * \param port : Port number
     */
    void connect(QString, int);

    //! Response to 'disconnect' signal from #ConnectionWindow
    void disConnect();

    //! It is used to emit requestInitProcess() signal
    /*!
     *
     * It is used by external read/write opertation related methods.
     *
     * It is also used as a response to readButtonSignal() from #ConnectionWindow
     *
     */
    void initProcess();

    //! Response to a signal to interrupt Sampling Process
    /*!
     *
     * It is also used as a response to disconnectButtonSignal() from #ConnectionWindow
     *
     */
    void interruptCommunication();

    //! Response to a signal to perform necessary operation on change of connection status (Connect/Disconnect)
    void changeConnectionStatus(bool);

    //! Response to 'terminate' signal from Coefficient Read/Write Window.
    void closeCoeffReadWriteWindow();


    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:

    /************************ Class Attributes ************************/
    ConnectionWindow * connectionWindow;
    EthernetComProcess * eComProcess;
    DataManager * dManager;
    ReadWriteWindow * rwWin;
    int protocol;
    QString selectedModule;
    QList<QString> commonModuleList;
    QList<DataStructure *> oldDataStructure;
    int checkStatus;

};

#endif // COMMUNICATIONCONTROLLER_H
