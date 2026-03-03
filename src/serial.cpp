#include "serial.hpp"

namespace {
QString toString(QSerialPort::SerialPortError error) {
    switch (error) {
    case QSerialPort::SerialPortError::NoError:
        return "NoError";
    case QSerialPort::SerialPortError::DeviceNotFoundError:
        return "DeviceNotFoundError";
    case QSerialPort::SerialPortError::PermissionError:
        return "PermissionError";
    case QSerialPort::SerialPortError::OpenError:
        return "OpenError";
    case QSerialPort::SerialPortError::WriteError:
        return "WriteError";
    case QSerialPort::SerialPortError::ReadError:
        return "ReadError";
    case QSerialPort::SerialPortError::ResourceError:
        return "ResourceError";
    case QSerialPort::SerialPortError::UnsupportedOperationError:
        return "UnsupportedOperationError";
    case QSerialPort::SerialPortError::UnknownError:
        return "UnknownError";
    case QSerialPort::SerialPortError::TimeoutError:
        return "TimeoutError";
    case QSerialPort::SerialPortError::NotOpenError:
        return "NotOpenError";
    }
    return "Unhandled error code: " + QString::number(error);
}
} // namespace

void SerialDataStream::close() {
    if (m_serial) {
        if (m_serial->isOpen()) {
            m_serial->close();
        }
        m_serial->deleteLater();
        m_serial = nullptr;
    }
}

bool SerialDataStream::open(const SerialConfig &config) {
    close();

    m_config = config;

    m_serial = new QSerialPort(this);

    connect(m_serial, &QSerialPort::readyRead, this, &SerialDataStream::handleReadyRead);
    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialDataStream::handleSerialError);

    m_serial->setPortName(m_config.name);
    m_serial->open(QSerialPort::ReadOnly);
    m_serial->setBaudRate(m_config.baudRate);
    m_serial->setDataBits(m_config.dataBits);
    m_serial->setParity(m_config.parity);
    m_serial->setStopBits(m_config.stopBits);
    m_serial->setFlowControl(m_config.flowControl);

    return m_serial->isOpen();
}

void SerialDataStream::handleReadyRead() {
    if (m_serial && m_serial->isOpen()) {
        const QByteArray data = m_serial->readAll();
        emit dataReceived(data);
    }
}

void SerialDataStream::handleSerialError(QSerialPort::SerialPortError error) {
    emit errorMessage(QString("Serial error for %1 occurred: %2").arg(m_config.name).arg(toString(error)));
}
