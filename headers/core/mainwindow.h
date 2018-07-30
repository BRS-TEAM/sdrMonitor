/*!
 * \file     mainwindow.h
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

// MainWondow
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Essential Headers
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>

#include <headers/core/centralwidgetcontroller.h>
#include "headers/core/createprojectwindow.h"
#include "headers/core/errorlogger.h"

class QPlainTextEdit;

namespace Ui {
class MainWindow;
}

/*!
 * \brief MainWindow class for handling application structure & services
 */
class MainWindow : public QMainWindow
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
    explicit MainWindow(QWidget *parent = 0);

    //! Destructor
    ~MainWindow();

    /************************ General Methods specific to this class. ************************/

    //! Open selected project file.
    /*!
     * Read .proj file
     * Initialize #CentralWidgetController
     * Create Necessary Interface
     *  -   Create COMMON SENSOR DOCK window by calling addCommonWindow(QWidget * widget, QString name)
     *  -   Create MAIN DOCK window by calling addMainWindow(QWidget * widget, QString name)
     *  -   Add project to recent project list by calling addRecentProject(QString fileName)
     *
     */
    void openProject(QString projName);

    //! Create COMMON SENSOR DOCK window.
    /*!
     * Add \a widget widget to COMMON SENSOR DOCK window having \a name name
     */
    int addCommonWindow(QWidget * widget, QString name);

    //! Create MAIN DOCK window.
    /*!
     * Add \a widget widget to MAIN DOCK window having \a name name
     */
    int addMainWindow(QWidget * widget, QString name);

    //! Create ERROR LOG window.
    void createLogWindow();

    //! Add \a fileName to recent project list
    void addRecentProject(QString fileName);

    //! Initialize Recent Project List
    void loadRecentProjectList();

    //! Reset necessary Flags
    void resetErrorFlags();

    //! Delete Temporary Files
    void clearTemporaryFiles();

    // *********************************************************************** //
    //                                                                         //
    //                           Private Slots                                 //
    //                                                                         //
    // *********************************************************************** //
private slots:
    /************************ Slots are special methods which execute in response to the connected signal. ************************/
    //! Response to 'Open' action signal.
    /*!
    * This SLOT method :
    * - Loads the selected file.
    * - Assigns memory to CentralWidgetController
    * - Passes the loaded file to CentralWidgetController.
    * - Creates the layout to contain central widget.
    */
    void on_actionOpen_triggered();

    //! Response to 'Close' action signal.
    /*!
     * Close currently open project.
     */
    void on_actionClose_triggered();

    //! Response to 'Quit' action signal.
    /*!
     * Quit Application
     */
    void on_actionQuit_triggered();

    //! Response to 'NetSettings' action signal.
    /*!
     * Show/Hide Network Setting Window
     */
    void on_actionNetSettings_triggered();

    //! Response to 'About' action signal.
    void on_actionAbout_triggered();

    //! Response to 'LoadConfig' action signal.
    /*!
     * Load Config File
     */
    void on_actionLoadConfig_triggered();

    //! Response to 'SaveConfig' action signal.
    /*!
     * Save Config File
     */
    void on_actionSaveConfig_triggered();

    //! Response to 'SetupConfig' action signal.
    /*!
     * Show/Hide Config Editor Window
     */
    void on_actionSetupConfig_triggered();

    //! Response to 'Start' action signal.
    /*!
     * Start Sampling
     */
    void on_actionStart_triggered();

    //! Response to 'Stop' action signal.
    /*!
     * Stop Sampling
     */
    void on_actionStop_triggered();

    //! Response to 'Read_Coefficient' action signal.
    /*!
     * Show/Hide READ/WRITE COEFFICIENT window
     */
    void on_actionRead_Coefficient_triggered();

    //! Response to 'MainWindow' action signal.
    /*!
     * Show/Hide MAIN DOCK window
     */
    void on_actionMainWindow_triggered();

    //! Response to 'CommonWindow' action signal.
    /*!
     * Show/Hide COMMON SENSOR DOCK window
     */
    void on_actionCommonWindow_triggered();

    //! Response to 'Log_Window' action signal.
    /*!
     * Show/Hide ERROR LOG window
     */
    void on_actionLog_Window_triggered();

    //! Response to 'New' action signal.
    /*!
     * Used to open CREATE NEW PROJECT window
     */
    void on_actionNew_triggered();

    //! Response to a signal to open recent project selected from recent project list.
    void actionRecentProject();

    //! Response to 'FM' action signal.
    /*!
     * Used to change radio mode
     */
    void on_actionFM_triggered();

    //! Response to 'AM' action signal.
    /*!
     * Used to change radio mode
     */
    void on_actionAM_triggered();

    //! Response to 'DAB' action signal.
    /*!
     * Used to change radio mode
     */
    void on_actionDAB_triggered();

    //! Response to 'WriteCoeffs' action signal.
    /*!
     * Used for MEMORY DUMP operation
     */
    void on_actionWriteCoeffs_triggered();

    //! Response to 'ReadCoeffs' action signal.
    /*!
     * Used for MEMORY DUMP operation
     */
    void on_actionReadCoeffs_triggered();

    //! Response to a signal to update ERROR LOG.
    void updateErrorLog(QString str);

    // *********************************************************************** //
    //                                                                         //
    //                           Private Data Members                          //
    //                                                                         //
    // *********************************************************************** //
private:

    /************************ Class Attributes ************************/
    Ui::MainWindow *ui;                         //!< Private Istance to access '.ui' file elements
    bool centralWidgetOpen;                     //!< Central Widget open/close status
    CentralWidgetController * cWController;     //!< Private Istance of #CentralWidgetController
    int FM_MAIN_INDEX;                          //!< INDEX number to locate FM Tab in MAIN DOCK window
    int AM_MAIN_INDEX;                          //!< INDEX number to locate AM Tab in MAIN DOCK window
    int DAB_MAIN_INDEX;                          //!< INDEX number to locate DAB Tab in MAIN DOCK window
    int FM_COMM_INDEX;                          //!< INDEX number to locate FM Tab in COMMON SENSOR DOCK window
    int AM_COMM_INDEX;                          //!< INDEX number to locate AM Tab in COMMON SENSOR DOCK window
    int DAB_COMM_INDEX;                          //!< INDEX number to locate DAB Tab in COMMON SENSOR DOCK window

    QList<QDockWidget *> mainDock ;              //!< List of MAIN DOCK window widgets.
    QList<QDockWidget *> commonDock ;              //!< List of COMMON SENSOR DOCK window widgets.
    QDockWidget * logDock ;                     //!< ERROR LOG DOCK widget.
    QPlainTextEdit * logViewer;                  //!< Text Viewer to show ERROR LOG
    CreateProjectWindow * createProjectWindow;  //!< Private Istance of #CreateProjectWindow
    QList<QString> recentProjects;              //!< List of recent projects.
    QFileSystemWatcher watcher;                 //!< Watcher to monitor the Error Log File changes.
};

#endif // MAINWINDOW_H


// QMdiSubWindow * createSubWindow(QWidget *);
