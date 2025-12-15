#pragma once

#include "datastream.hpp"

#include <QPointer>
#include <QSerialPort>
#include <QString>
#include <QThread>

struct SerialConfig {
    QString name;
    qint32 baudRate{QSerialPort::Baud115200};
    QSerialPort::DataBits dataBits{QSerialPort::Data8};
    QSerialPort::Parity parity{QSerialPort::NoParity};
    QSerialPort::StopBits stopBits{QSerialPort::OneStop};
    QSerialPort::FlowControl flowControl{QSerialPort::NoFlowControl};
};

class SerialDataStream : public DataLoggerStream {
    Q_OBJECT
  public:
    using DataLoggerStream::DataLoggerStream;
    ~SerialDataStream() { close(); }
    void close() override;
    bool open(const SerialConfig &config);

  private slots:
    void handleReadyRead();
    void handleSerialError(QSerialPort::SerialPortError error);

  private:
    SerialConfig m_config;
    QPointer<QSerialPort> m_serial{nullptr};
};
