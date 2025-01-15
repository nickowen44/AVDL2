#ifndef STROBETHREAD_H
#define STROBETHREAD_H


#include <QThread>

class StrobeThread : public QThread
{
    Q_OBJECT

public:
    explicit StrobeThread(QObject *parent = nullptr);
    void run();
    bool Stop;

signals:
    void FlashEstopN(int);

public slots:

};

#endif // STROBETHREAD_H
