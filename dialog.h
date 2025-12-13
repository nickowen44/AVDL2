#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>
#include "strobethread.h"
#include "serialconnect.h"
#include "serialportfacade.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    StrobeThread *sThread;
    SerialConnect *sAVDLser;
    struct  {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
        bool multithreading;
    }p;


private slots:
    void readSerial(QByteArray);
    void readSerialLine(const QByteArray&);
    void UpdateCarID(QString);
    void UpdateRES(QString);
    void UpdateMMA(QString);
    void UpdateMmmTarget(QString);
    void UpdateAutonomousState(QString);
    void UpdateSaActual(QString);
    void UpdateStTarget(QString);
    void UpdateBrtTarget(QString);
    void UpdateBraActual(QString);
    void UpdateSttTarget(QString);
    void UpdateStaActual(QString);
    void UpdateAutonomousMission(QString);
    void UpdateEbsState(QString);
    void FlashEstop(int number);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void openSerialPort();
    void initMultiThreadingConnections();

private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 4292; //4292
    static const quint16 arduino_uno_product_id = 60000; //6000
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    double temperature_value;
    QSerialPort *m_serial = nullptr;
    // Multi-Threading members
    QPointer<SerialPortFacade> m_multithreadserial = nullptr;
    QPointer<QThread> m_serialPortThread = nullptr;


signals:
        void openSerialPort_MT(
            const QString& portname,
            const qint32 baudrate,
            const QSerialPort::DataBits dbits,
            const QSerialPort::Parity parity,
            const QSerialPort::StopBits stopbits,
            const QSerialPort::FlowControl flow);
    void closeSerialPort_MT();

};

#endif // DIALOG_H
