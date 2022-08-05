#ifndef NOTIFICATIONBOX_H
#define NOTIFICATIONBOX_H

#include <QMessageBox>
#include <QTimer>

class NotificationBox : public QMessageBox
{
public:
    NotificationBox(const QString _text, const int _showTime = 1000, QWidget* parent = nullptr);
private:
    int showTime;
    QString text;
private:
    void intialize();
public:
    void display();
};

#endif // NOTIFICATIONBOX_H
