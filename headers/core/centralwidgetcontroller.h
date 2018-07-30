/*!
 * \file     centralwidgetcontroller.h
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

// Central Widget Controller
#ifndef CENTRALWIDGETCONTROLLER_H
#define CENTRALWIDGETCONTROLLER_H

// Essential Headers
#include <QTabWidget>
#include <QtUiTools/QtUiTools>
#include <QTcpSocket>
#include <QNetworkSession>

// XML Parser
#ifndef _UIPARSER_H
#define _UIPARSER_H
#include <headers/core/uiparser.h>
#endif // _UIPARSER_H

class TabController;
class ConfigController;
class CommunicationController;
class DataManager;

/*!
 * \brief Highest level controller class to manage central widget
 */
class CentralWidgetController : public QWidget
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
    CentralWidgetController(QWidget * parent = 0);

    //! Destructor
    ~CentralWidgetController();

    //! Parse the source xml file.
    /*!
    * This method initialize #UiParser to to load LEVEL 1 XML file.
    */
    void centralWidgetUiParser();           // Extract info

    //! Create central widget (TabWidget)
    /*!
    * This method calls createTabPage(QString pageSource) to load TabWidget for each input source.
    */
    void centralWidgetUiPainter();          // Create Primary interface

    //! Create widget (tab page) for each tab in central widget.
    /*!
    *
    * This method initialize #TabController to create and manage widgets in each tab page.
    *
    * #TabController initialize #UiParser to to load LEVEL 2 XML file.
    *
    */
    void createTabPage(QString pageSource);                // Create Second Layer

    //! Load CFG file.
    void loadConfig(QString);

    //! Save CFG file.
    void saveConfig(QString);

    //! Initialize Config Editor Window.
    void displayConfig();

    //! Initialize Communication Controller
    /*!
    *
    * Initialize #CommunicationController
    *
    * Load Network Setting Window.
    *
    */
    void openConnectionSettings();

    //! Terminate Network Connection
    /*!
    *
    * Shut down Communication Controller
    *
    * Remove #CommunicationController
    *
    */
    void closeNetworkConnection();

    //! Request to START Sampling Process
    bool startCommunication();

    //! Request to STOP Sampling Process
    void stopCommunication();

    //! Open READ/WRITE COEFFICIENT window
    void openCoeffReadWriteWindow();

    //! Initialize MEMORY DUMP : Write to Target Operation.
    bool initWriteAllCoeffs();

    //! Initialize MEMORY DUMP : Read From Target Operation.
    bool initReadAllCoeffs();

    //! Switch Radio Mode FM/AM/DAB.
    /*!
     * \param mode : Check sharedflags.h for possible RADIO MODE
     */
    void selectRadioMode(int mode);

    //! Stop all the operations being performed on each TabWidget managed by #TabController
    void haltAllTabs();

    //! Initialize tabs for the selected RADIO MODE
    /*!
     * This method calls updateSelectedTab(int) with the index number of Currently Selected Tab
     */
    void updateTab();

    /************************ Getters & Setters ************************/

    //! Getter : Get #DataManager created by #ConfigController .
    DataManager * getDataManager();

    // FM
    //! Setter : \a fmSource
    void setFmSource(QString);
    //! Getter : \a fmSource
    QString getFmSource();

    //! Setter : \a fmTabsCount
    void setFmTabsCount(int);
    //! Getter : \a fmTabsCount
    int getFmTabsCount();

    //! Getter : \a fmTabWidget
    QTabWidget * getFmTabWidget();

    //! Getter : \a fmCommonTab
    QTabWidget * getFmCommonTab();

    //! Setter : \a fmPageSource
    void setFmPageSource(QString);

    // //! Getter : pageSource
    // QString getFmPageSource();

    // AM
    //! Setter : \a amSource
    void setAmSource(QString);
    //! Getter : \a amSource
    QString getAmSource();

    //! Setter : \a amTabsCount
    void setAmTabsCount(int);
    //! Getter : \a amTabsCount
    int getAmTabsCount();

    //! Getter : \a amTabWidget
    QTabWidget * getAmTabWidget();

    //! Getter : \a amCommonTab
    QTabWidget * getAmCommonTab();

    //! Setter : \a amPageSource
    void setAmPageSource(QString);
    // //! Getter : pageSource
    // QString getAmPageSource();

    // DAB
    //! Setter : \a dabSource
    void setDabSource(QString);
    //! Getter : \a dabSource
    QString getDabSource();

    //! Setter : \a dabTabsCount
    void setDabTabsCount(int);
    //! Getter : \a dabTabsCount
    int getDabTabsCount();

    //! Getter : \a dabTabWidget
    QTabWidget * getDabTabWidget();

    //! Getter : \a dabCommonTab
    QTabWidget * getDabCommonTab();

    //! Setter : \a dabPageSource
    void setDabPageSource(QString);
    // //! Getter : pageSource
    // QString getDabPageSource();

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/

    //! Response to 'currentChanged' signal from TabWidget
    /*!
     * Activate the selected tab page.
     * Disable all other tab pages.
     * Collect coefficient information of selected tab.
     * Reinitialize #CommunicationController with coefficients information of selected tab.
     *
     */
    void updateSelectedTab(int);

    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:

    /************************ Class Attributes ************************/

    // FM
    QString fmSource;                         //!< Path to Central Widget Source XML file (Highest Level XML)
    QTabWidget * fmTabWidget;                 //!< Pointer to Central Widget (Tab Widget)
    QTabWidget * fmCommonTab;                 //!< Pointer to Central Widget (Tab Widget)
    int fmTabsCount;                          //!< Number of Tabs to be created
    QList <QString> fmPageSource;             //!< Array of : path to individual Tab Page source XML file (Level 2 XML)

    //AM
    QString amSource;                         //!< Path to Central Widget Source XML file (Highest Level XML)
    QTabWidget * amTabWidget;                 //!< Pointer to Central Widget (Tab Widget)
    QTabWidget * amCommonTab;                 //!< Pointer to Central Widget (Tab Widget)
    int amTabsCount;                          //!< Number of Tabs to be created
    QList <QString> amPageSource;             //!< Array of : path to individual Tab Page source XML file (Level 2 XML)

    //DAB
    QString dabSource;                         //!< Path to Central Widget Source XML file (Highest Level XML)
    QTabWidget * dabTabWidget;                 //!< Pointer to Central Widget (Tab Widget)
    QTabWidget * dabCommonTab;                 //!< Pointer to Central Widget (Tab Widget)
    int dabTabsCount;                          //!< Number of Tabs to be created
    QList <QString> dabPageSource;             //!< Array of : path to individual Tab Page source XML file (Level 2 XML)

    QList <QWidget *> tabPage;                      //!< Array of : pointer to Tab Page Widget
    QList <TabController *> tabController;          //!< Tab Page Widget Controller. (Private Istance of #TabController)
    ConfigController * configController;            //!< CFG reader. (Private Istance of #ConfigController)
    CommunicationController * communicationController;

    UiParser uiParser;                      //!< XML reader. (Private Istance of #UiParser)

};

#endif // CENTRALWIDGETCONTROLLER_H
