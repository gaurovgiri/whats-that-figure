#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMutex>
class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QObject* parent = nullptr);
    void run();
    void stop();
private:
    bool stopFlag;

signals:
    void doSomething();

public slots:
};

#endif // THREAD_H
