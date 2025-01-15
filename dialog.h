#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>
#include "strobethread.h"

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


private slots:
    void readSerial();
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

private:
    Ui::Dialog *ui;

    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 4292; //4292
    static const quint16 arduino_uno_product_id = 60000; //6000
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    double temperature_value;
};

#endif // DIALOG_H
