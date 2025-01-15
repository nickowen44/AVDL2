#include "strobethread.h"
#include <QtCore>

StrobeThread::StrobeThread(QObject *parent)
    : QThread{parent}
{


}


void StrobeThread::run()
{
    qInfo() << this << Q_FUNC_INFO << QThread::currentThread();
    while(1){
        for (int i=0; i <10; i++)
        {
            QMutex mutex;
            mutex.lock();
            if(this->Stop) break;
            //qDebug()  << i;
            mutex.unlock();
            emit FlashEstopN(i);
            this->msleep(100);
        }
        if(this->Stop) break;

    }

}


