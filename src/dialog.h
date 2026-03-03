#ifndef DIALOG_H
#define DIALOG_H

#include <QByteArray>
#include <QDialog>
#include <QPointer>
#include <QSerialPort>
#include <map>
#include <qtypes.h>


#include "datastream.hpp"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

  using CallbackType = void (Dialog::*)(QString);

  public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

  private slots:
    void onTimerTick();
    void onDataReceived(QByteArray);
    void setConsoleText(QString);

    void on_btnQuit_clicked();
    void on_btnSerialConnect_clicked();
    void on_cmbSerialPorts_currentTextChanged(QString);

  private:
    qint64 m_ticks{0};
    Ui::Dialog *ui;

    void parseKeyValuePair(const QByteArray &);

    void updateCarID(QString);
    void updateRES(QString);
    void updateMotorMomentActual(QString);
    void updateMotorMomentTarget(QString);
    void updateAutonomousState(QString);
    void updateSpeedActual(QString);
    void updateSpeedTarget(QString);
    void updateBrakeTarget(QString);
    void updateBrakeActual(QString);
    void updateSteeringTarget(QString);
    void updateSteeringActual(QString);
    void updateAutonomousMission(QString);
    void updateEbsState(QString);

    void updateSerialPortOptions();
    void registerMessageType(QString name, CallbackType callback);
    void closeConnection();

    QByteArray m_serialBuffer;
    QPointer<QThread> m_ioThread{nullptr};
    QPointer<QTimer> m_flashingTimer{nullptr};
    QPointer<DataLoggerStream> m_stream{nullptr};

    std::map<QString, CallbackType> m_callbackRegistry;
};

#endif // DIALOG_H
