#ifndef SERIALCONNECT_H
#define SERIALCONNECT_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>
//#include <QThread>

class SerialConnect
{



public:
    SerialConnect();
    //explicit SerialConnect(QObject *parent = nullptr);
    bool SerialSimulation = true; // true to enable simulated serial port, false to disable
    QString SimulatedSerialAVDL_port_name = "COM4";
    void run();
    bool Stop;


public slots:
    void readSerial();
    void close();


private:
    QSerialPort *SerialAVDL;
    static const quint16 SerialAVDL_vendor_id = 4292; //4292
    static const quint16 SerialAVDL_product_id = 60000; //6000
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;


signals:
    void SerialData(QByteArray);


};

#endif // SERIALCONNECT_H
