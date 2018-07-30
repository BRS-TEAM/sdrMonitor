#ifndef FRAMES_H
#define FRAMES_H

#include <QFrame>

namespace Ui {
class Frames;
}

class Frames : public QFrame
{
    Q_OBJECT

public:
    explicit Frames(QWidget *parent = 0);
    ~Frames();

    void insertWidget(QWidget*, int, int, int, int);
    QWidget * getFrames();
    void setFrameTitle(QString);
    void setIndex(int);
    int getIndex();

    void addSpace();

private:
    Ui::Frames *ui;
    int index;
};

#endif // FRAMES_H
