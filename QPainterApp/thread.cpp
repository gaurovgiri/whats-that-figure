#include "thread.h"


Thread::Thread(QObject* parent)
:
      QThread(parent)
{
    stopFlag = false;
}


void Thread::run()
{
    for(int i = 0; i < 10000; i++)
    {
        QMutex mutex;
        mutex.lock();
        if(this->stopFlag)
           break;
        mutex.unlock();

        emit doSomething();

        this->msleep(100);
    }
}

void Thread::stop()
{
    stopFlag = true;
}


