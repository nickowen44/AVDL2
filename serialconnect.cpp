#include "serialconnect.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QtCore>


/*
SerialConnect::SerialConnect(QObject *parent) {
     //: QThread{parent}
}

*/


SerialConnect::SerialConnect() {
    //: QThread{parent}
}




void SerialConnect::readSerial()
{
    SerialAVDL = new QSerialPort();
    serialBuffer = "";
    parsed_data = "";

/*
     *  Testing code, prints the description, vendor id, and product id of all ports.
     *  Used it to determine the values for the arduino uno.
     *

*/
    qDebug() << "Serial Connect";
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Name: " << serialPortInfo.portName() << "\n";

        qDebug() << "Description: " << serialPortInfo.description() << "\n";
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";

    }



    /*
     *   Identify the port the SerialAVDL is on.
     */
    bool SerialAVDL_is_available = false;
    QString SerialAVDL_port_name;
    //
    //  Search availbel commports of AVDL
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        //  check if the serialport has both a product identifier and a vendor identifier
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            //  check if the product ID and the vendor ID match those of the SerialAVDL
            if((serialPortInfo.productIdentifier() == SerialAVDL_product_id)
                && (serialPortInfo.vendorIdentifier() == SerialAVDL_vendor_id)){
                SerialAVDL_is_available = true; //    SerialAVDL is available on this port

                SerialAVDL_port_name = serialPortInfo.portName();
                //SerialAVDL_port_name = "COM7";

            }
        }
    }

    if (SerialSimulation==true){
        SerialAVDL_is_available = true; //    SerialAVDL is available on this port
         SerialAVDL_port_name = SimulatedSerialAVDL_port_name; // overwrite detected serial port with simualted port name
    }


    /*
     *  Open and configure the SerialAVDL port if available
     */
    if(SerialAVDL_is_available){
        qDebug() << "Found the SerialAVDL port on : " <<SerialAVDL_port_name;
        //SerialAVDL->setPortName(SerialAVDL_name);
        SerialAVDL->setPortName(SerialAVDL_port_name);

        SerialAVDL->open(QSerialPort::ReadOnly);
        SerialAVDL->setBaudRate(QSerialPort::Baud115200);
        SerialAVDL->setDataBits(QSerialPort::Data8);
        SerialAVDL->setFlowControl(QSerialPort::NoFlowControl);
        SerialAVDL->setParity(QSerialPort::NoParity);
        SerialAVDL->setStopBits(QSerialPort::OneStop);
  //      QObject::connect(SerialAVDL, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the arduino.\n";
        //QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to SerialAVDL.");
    }


}



void SerialConnect::run()
{
    qInfo() << this << Q_FUNC_INFO << QThread::currentThread();
    while(1){
        if(SerialAVDL->isOpen()){
            serialData = SerialAVDL->readAll();

            qDebug() << serialData;
        }
        //QMutex mutex;
        //mutex.lock();
        if(this->Stop) break;
        //qDebug()  << i;
        // mutex.unlock();
        //emit SerialData(serialData);
        //this->msleep(10);

    }

}


void SerialConnect::close(){

    if(SerialAVDL->isOpen()){
        SerialAVDL->close();
            //    Close the serial port if it's open.
    }

}


