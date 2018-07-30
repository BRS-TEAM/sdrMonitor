#include "headers/elements/audiofilters.h"
#include "ui_audiofilters.h"

// Data Processing
#include "headers/staticdependencies/valueconversion.h"
#include "headers/staticdependencies/datatypeconversion.h"

// Dialog Display
#include <QTextStream>
// Connection
#include "headers/elements/inputfield.h"

// Parent
#include "headers/core/tabcontroller.h"




AudioFilters::AudioFilters(TabController *parent) :
    QWidget((QWidget*)parent),
    ui(new Ui::AudioFilters)
{
    ui->setupUi(this);
    ui->Value_indicator->setMaximum(20);
    ui->Value_indicator->setMinimum(0);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Frequency(kHz)");
    ui->customPlot->yAxis->setLabel("Level(dB)");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 4);
    ui->customPlot->yAxis->setRange(0, 24);
    Default();
    dStructure = 0;
    tabControllerParent = (TabController*) parent;
    connect(ui->Value_indicator, SIGNAL(sliderMoved(int)), this, SLOT(Cursor_moved(int)));
    connect(ui->Value_indicator, SIGNAL(valueChanged(int)), this, SLOT(Cursor_moved(int)));
    connect(ui->Value_indicator, SIGNAL(sliderPressed()), this, SLOT(Cursor_moved(int)));
    ui->setButton->setIcon(QPixmap("../SDRToolResources/icons/send.png"));
    ui->setButton->setIconSize(ui->setButton->rect().size());
}

AudioFilters::~AudioFilters()
{
    delete ui;
}

DataStructure * AudioFilters::getStructure() {
    return dStructure;
}

void AudioFilters::setIndex(int index) {
    this->index = index;
}

int AudioFilters::getIndex() {
    return index;
}
void AudioFilters::setNameIdentifier(QString name)
{
     nameIdentifier = name;
}

/************************ Getters & Setters ************************/
QString AudioFilters::getNameIdentifier()
{
    return nameIdentifier;
}
bool AudioFilters::setStructure(DataStructure * structure) {
    dStructure = 0;
    dStructure = structure;
    if (dStructure != 0)
        return true;
    return false;
}
void AudioFilters::resetDataSource()
{
    dSource.clear();
}
int AudioFilters::getDataSourceIdentifierCount()
{
    return dSourceIdentifier.count();
}
bool AudioFilters::setDataSource(DataStructure * structure) {

    if (structure != 0) {
        dSource.append(structure);
        return true;
    }
    return false;
}
QString AudioFilters::getDataSourceIdentifier(int index) {
    if(dSourceIdentifier.count() > index)
        return dSourceIdentifier.at(index);
    return "";
}
void AudioFilters::disable()
{
    //this->hide();
    this->setDisabled(true);
}

void AudioFilters::enable(QString state)
{
    if(state == "disable") {
        this->setDisabled(true);
    }
    else {
        this->setEnabled(true);
    }
    //this->hide();
    //this->setEnabled(true);
}

void AudioFilters::hideWidget()
{
    this->hide();
}

void AudioFilters::showWidget()
{
    this->show();
}
// Response to 'valueChanged()' action signal.
void AudioFilters::updateEvent()
{
//    ui->Value_label->setText(QString::number(8));
//    ui->Value_indicator->setValue(8);
}
void AudioFilters::RD45()
{
     value = 10;
     ui->Value_label->setText(QString::number(value));
     ui->Value_indicator->setValue(value);
     drawGraph(value);
}

void AudioFilters::Wave()
{
    value = 15;
    ui->Value_label->setText(QString::number(value));
    ui->Value_indicator->setValue(value);
    drawGraph(value);
}
void AudioFilters::Default()
{
     value = 18;
     ui->Value_label->setText(QString::number(value));
     ui->Value_indicator->setValue(value);
     ui->Value_indicator->setEnabled(true);
     drawGraph(value);
}
void AudioFilters::Cursor_moved(int val)
{
    value = val;
    ui->Value_label->setText(QString::number(value));
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
        x[i] = i/50.0 + i/100.0 + i/100; // x goes from -1 to 1
        y[i] = value * qSin(x[i] * 1); // let's plot a quadratic function
    }
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->replot();

}
void AudioFilters::on_setButton_clicked()
{    
    if (dStructure != 0) {

        QList<DataStructure *> dS;
        QList<QString> values;
        dS.append(getStructure());

        values.append(QString().setNum(value));

        if(!dS.isEmpty())
        {
            tabControllerParent->initHMIWriteReq(dS, values);
        }
    }
}
void AudioFilters::drawGraph(int val)
{
    value = val;
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
        x[i] = i/50.0 + i/100.0 + i/100; // x goes from -1 to 1
        y[i] = value * qSin(x[i] * 1); // let's plot a quadratic function
    }
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->replot();
}
