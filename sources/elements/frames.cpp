#include "headers/elements/frames.h"
#include "ui_frames.h"
#include "headers/core/errorlogger.h"

#include <QTextStream>

Frames::Frames(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frames)
{
    ui->setupUi(this);
}

Frames::~Frames()
{
    delete ui;
}

void Frames::insertWidget(QWidget* w, int row, int col, int rowSpan, int colSpan) {
    ui->gridLayout->addWidget(w, row, col, rowSpan, colSpan);
    printDebugString("insert :: " + QString().setNum(row) + ", " + QString().setNum(col) + ", " + QString().setNum(rowSpan) + ", " + QString().setNum(colSpan) );
}

void Frames::addSpace() {


        ui->gridLayout->setColumnStretch(ui->gridLayout->columnCount(),1);

        ui->gridLayout->setRowStretch(ui->gridLayout->rowCount(),1);

    printDebugString( "insert Space " );
}


QWidget* Frames::getFrames() {
    return this;
}

void Frames::setFrameTitle(QString title) {
    if ((title == " ")||(title == ""))
        ui->FrameLabel->hide();
    else
        ui->FrameLabel->setText(title);
}
void Frames::setIndex(int index) {
    this->index = index;
}

int Frames::getIndex() {
    return index;
}
