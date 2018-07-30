#ifndef AUDIOFILTERS_H
#define AUDIOFILTERS_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QSlider>


// Conections
class InputField;
// Parent
class TabController;

// Data Structure
#include "headers/core/datastructure.h"

namespace Ui {
class AudioFilters;
}

class AudioFilters : public QWidget
{
    Q_OBJECT

public:
    //explicit AudioFilters(QWidget *parent = 0);
    explicit AudioFilters(TabController *parent = 0);
    ~AudioFilters();

    void setIndex(int index);
    int getIndex();
    void setNameIdentifier(QString);
    bool setStructure(DataStructure *);
    //! Get the indentification name of XYPlot
    /*!
     * It is used by #ConfigController to identify element.
     * \sa setNameIdentifier()
     */
    QString getNameIdentifier();
    int getDataSourceIdentifierCount();
    bool setDataSource(DataStructure *);
    QString getDataSourceIdentifier(int);
    void resetDataSource();
    DataStructure * getStructure();
    void updateEvent();

    private slots:
        /************************ Slots are special methods which execute in response to the connected signal. ************************/
        /*!
         * Disable the TimeValuePlot
         */
        void disable();
        /*!
         * Enable the TimeValuePlot
         */
        void enable(QString state);
        void hideWidget();

        void showWidget();
        void RD45();
        void Wave();
        void Default();
        void Cursor_moved(int);
        void on_setButton_clicked();
        void drawGraph(int val);

    private:
        Ui::AudioFilters *ui;
        int index;
        int value;
        QString nameIdentifier;
        QList<InputField*> outputEditField;
        QList<QString> outputEditFieldOldValue;


        /************************ Parent ************************/
        TabController * tabControllerParent; //!< Pointer to parent class.

        DataStructure * dStructure;
        QList<DataStructure *> dSource;
        QList<QString> dSourceIdentifier;

    };

    #endif // AUDIOFILTERS_H
