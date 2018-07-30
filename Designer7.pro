#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T09:11:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QT += uitools
QT += xml
QT += network

TARGET = Designer7
TEMPLATE = app
RC_FILE = myapp.rc

# Tell the qcustomplot header that it will be used as library:
DEFINES += QCUSTOMPLOT_USE_LIBRARY

FMK_PATH = "D:\14-03 Framework\framework"

include($$FMK_PATH/fmkQt.pri)

# include pqth to library files
INCLUDEPATH+= ../SDRToolResources/sharedlib

INCLUDEPATH += \
    $$FMK_PATH/osAbstractionLayer/Qt/includes


LIBS += -L../SDRToolResources/sharedlib

# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd1
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot1
  else: QCPLIB = qcustomplot
}

LIBS += -L./ -l$$QCPLIB

SOURCES += \
    main.cpp \
    sources/communication/ethernetcomprocess_copy.cpp \
    sources/communication/clientdemothread.cpp \
    sources/core/centralwidgetcontroller.cpp \
    sources/core/communicationcontroller.cpp \
    sources/core/configcontroller.cpp \
    sources/core/configwindow.cpp \
    sources/core/connectionwindow.cpp \
    sources/core/createprojectwindow.cpp \
    sources/core/datamanager.cpp \
    sources/core/mainwindow.cpp \
    sources/core/readwritewindow.cpp \
    sources/core/tabcontroller.cpp \
    sources/core/uipainter.cpp \
    sources/core/uiparser.cpp \
    sources/elements/agccontrol.cpp \
    sources/elements/agcrf.cpp \
    sources/elements/bargraphh.cpp \
    sources/elements/button.cpp \
    sources/elements/checkbox.cpp \
    sources/elements/frames.cpp \
    sources/elements/indicator.cpp \
    sources/elements/inputfield.cpp \
    sources/elements/progressbargraph.cpp \
    sources/elements/radiobutton.cpp \
    sources/elements/slidebar.cpp \
    sources/elements/spinbox.cpp \
    sources/elements/testelement.cpp \
    sources/elements/textbox.cpp \
    sources/elements/timevalueplot.cpp \
    sources/elements/xyplot.cpp \
    sources/elements/audiofilters.cpp \
    sources/staticdependencies/datatypeconversion.cpp \
    sources/staticdependencies/linearplotfunctions.cpp \
    sources/staticdependencies/nonlinearplotfunctions.cpp \
    sources/staticdependencies/valueconversion.cpp \
    sources/core/errorlogger.cpp


HEADERS  += \
    headers/communication/ethernetcomprocess_copy.h \
    headers/communication/clientdemothread.h \
    headers/core/centralwidgetcontroller.h \
    headers/core/communicationcontroller.h \
    headers/core/configcontroller.h \
    headers/core/configwindow.h \
    headers/core/connectionwindow.h \
    headers/core/createprojectwindow.h \
    headers/core/datamanager.h \
    headers/core/datastructure.h \
    headers/core/mainwindow.h \
    headers/core/readwritewindow.h \
    headers/core/sharedflags.h \
    headers/core/tabcontroller.h \
    headers/core/uipainter.h \
    headers/core/uiparser.h \
    headers/elements/agccontrol.h \
    headers/elements/agcrf.h \
    headers/elements/bargraphh.h \
    headers/elements/button.h \
    headers/elements/checkbox.h \
    headers/elements/frames.h \
    headers/elements/indicator.h \
    headers/elements/inputfield.h \
    headers/elements/progressbargraph.h \
    headers/elements/radiobutton.h \
    headers/elements/slidebar.h \
    headers/elements/spinbox.h \
    headers/elements/testelement.h \
    headers/elements/textbox.h \
    headers/elements/timevalueplot.h \
    headers/elements/xyplot.h \
    headers/elements/audiofilters.h \
    headers/staticdependencies/datatypeconversion.h \
    headers/staticdependencies/linearplotfunctions.h \
    headers/staticdependencies/nonlinearplotfunctions.h \
    headers/staticdependencies/valueconversion.h \
    headers/core/errorlogger.h



FORMS    += \
    forms/bargraphh.ui \
    forms/button.ui \
    forms/checkbox.ui \
    forms/configwindow.ui \
    forms/connectionwindow.ui \
    forms/frames.ui \
    forms/inputfield.ui \
    forms/mainwindow.ui \
    forms/progressbargraph.ui \
    forms/readwritewindow.ui \
    forms/slidebar.ui \
    forms/timevalueplot.ui \
    forms/xyplot.ui \
    forms/audiofilters.ui \
    forms/createprojectwindow.ui \
    forms/indicator.ui \
    forms/radiobutton.ui \
    forms/textbox.ui \
    forms/spinbox.ui \
    forms/agccontrol.ui \
    forms/agcrf.ui
