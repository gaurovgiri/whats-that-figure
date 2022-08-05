#include "notificationbox.h"
#include "notificationbox.h"

NotificationBox::NotificationBox(const QString _text, const int _showTime, QWidget* parent)
    :
      text{_text},
      showTime{_showTime},
      QMessageBox(parent)
{
    intialize();
    setText(text);
}

void NotificationBox::intialize()
{
    setWindowTitle("Notification");
    setIcon(QMessageBox::NoIcon);
    setAttribute(Qt::WA_DeleteOnClose);
}

void NotificationBox::display()
{
    show();
    QTimer::singleShot(showTime, this, SLOT(close()));
}
