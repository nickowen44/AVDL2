#pragma once

#include <QObject>

class DataLoggerStream : public QObject {
    Q_OBJECT
  public:
    using QObject::QObject;
    virtual void close() = 0;
  signals:
    void dataReceived(QByteArray);
    void errorMessage(QString);
};
