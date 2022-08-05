#ifndef MSGBOX_H
#define MSGBOX_H
#include <QMessageBox>

class MsgBox : public QMessageBox
{
    int timeout;
    bool autoClose;
    int currentTime;

    void MsgBox::showEvent (QShowEvent * )
    {
        currentTime = 0;
        if (autoClose)
            this->startTimer(1000);
    }

    void MsgBox::timerEvent(QTimerEvent *event)
    {
        currentTime++;
        if (currentTime>=timeout)
            this->done(0);
    }
};

#endif // MSGBOX_H
