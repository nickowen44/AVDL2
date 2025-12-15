#include "dialog.h"
#include "serial.hpp"
#include "ui_dialog.h"
#include "util.hpp"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTimer>
#include <qserialportinfo.h>

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);

    ui->CarNumber_lcdNumber->display("----");
    ui->RES_lcdNumber->display("-");
    ui->MMA_Text_Actual->setPlainText("___");
    ui->MMA_Text_Target->setPlainText("___");
    ui->ST_Text_Actual->setPlainText("___");
    ui->SA_Text_Actual->setPlainText("___");

    ui->Background->setPixmap(QPixmap{"Background.jpg"});
    ui->NUVO_LOGO->setPixmap(QPixmap{"Nuvomotorsportlogo.png"});

    registerMessageType("ID", &Dialog::updateCarID);
    registerMessageType("RES", &Dialog::updateRES);
    registerMessageType("AMI", &Dialog::updateAutonomousMission);
    registerMessageType("EBS", &Dialog::updateEbsState);
    registerMessageType("MMA", &Dialog::updateMotorMomentActual);
    registerMessageType("MMT", &Dialog::updateMotorMomentTarget);
    registerMessageType("AST", &Dialog::updateAutonomousState);
    registerMessageType("SA", &Dialog::updateSpeedActual);
    registerMessageType("ST", &Dialog::updateSpeedTarget);
    registerMessageType("BRT", &Dialog::updateBrakeTarget);
    registerMessageType("BRA", &Dialog::updateBrakeActual);
    registerMessageType("STT", &Dialog::updateSteeringTarget);
    registerMessageType("STA", &Dialog::updateSteeringActual);

    m_flashingTimer = new QTimer(this);
    m_flashingTimer->setInterval(500);
    connect(m_flashingTimer, &QTimer::timeout, this, &Dialog::onTimerTick);
    m_flashingTimer->start();

    m_serialBuffer = "";
}

Dialog::~Dialog() {
    if (m_stream) {
        m_stream->close();
        m_stream = nullptr;
    }
}

void Dialog::registerMessageType(QString name, CallbackType callback) { m_callbackRegistry.insert(std::make_pair(name, callback)); }

void Dialog::setConsoleText(QString error) {
    ui->Console->document()->setPlainText(error);
    qDebug() << error << "\n";
}

void Dialog::onDataReceived(QByteArray payload) {
    // qDebug() << "Received data: " << payload << "\n";
    m_serialBuffer += payload;
    ui->Console->document()->setPlainText(payload);

    QByteArray buffer{};
    buffer.reserve(16);

    int consumed = 0;
    int skipped = 0;

    for (int i = 0; i < m_serialBuffer.size(); ++i) {
        const char data = m_serialBuffer[i];
        if (data == '\r') {
            // Skip the carriage return from `\r\n`
            skipped += 1;
        } else if (data == '\n' || data == '|') {
            // Skip the new line or intermediate
            skipped += 1;
            consumed += buffer.length() + skipped;

            if (buffer.length() > 0) {
                parseKeyValuePair(buffer);
            }

            buffer.clear();
            skipped = 0;
        } else {
            buffer += data;
        }
    }
    // Delete what we consumed from the buffer
    m_serialBuffer = m_serialBuffer.last(m_serialBuffer.size() - consumed);
}

void Dialog::parseKeyValuePair(const QByteArray &serialData) {
    QList<QByteArray> parts = serialData.split('=');
    if (parts.size() != 2) {
        qDebug() << "Bad kv pair: `" << serialData << "`\n";
        return;
    }

    const auto callback = m_callbackRegistry.find(parts[0]);
    if (callback != m_callbackRegistry.end()) {
        (this->*(callback->second))(parts[1]);
    } else {
        qDebug() << "Bad key: `" << serialData << "`\n";
    }
}

void Dialog::onTimerTick() {

    if ((m_ticks % 5) == 0) {
        updateSerialPortOptions();
    }

    if ((m_ticks % 2) == 0) {
        ui->RES_ESTOP_RED_2->setHidden(false);
        ui->RES_ESTOP_RED_2->setPixmap(QPixmap{"emergency-stop-red.png"}); // Big Estop
    } else {
        ui->RES_ESTOP_RED_2->setHidden(true);
    }

    ++m_ticks;
}

void Dialog::updateSerialPortOptions() {
    const auto availablePorts = QSerialPortInfo::availablePorts();
    if (availablePorts.size() == 0) {
        ui->cmbSerialPorts->setDisabled(true);
        ui->btnSerialConnect->setDisabled(true);

        ui->cmbSerialPorts->clear();
        ui->cmbSerialPorts->addItem("No COM ports found");
        return;
    }

    ui->cmbSerialPorts->setDisabled(false);
    ui->btnSerialConnect->setDisabled(false);

    QStringList currentPortNames{};
    for (int i = 0; i < ui->cmbSerialPorts->count(); ++i) {
        currentPortNames.append(ui->cmbSerialPorts->itemText(i));
    }
    QStringList availablePortNames{};
    for (const auto &option : availablePorts) {
        availablePortNames.append(option.portName());
    }

    if (currentPortNames == availablePortNames) {
        // No update required so don't refresh the model
        return;
    }

    QString currentSelected = ui->cmbSerialPorts->currentText();
    ui->cmbSerialPorts->clear();

    for (const auto &option : availablePorts) {
        ui->cmbSerialPorts->addItem(option.portName());
    }

    const int selectIndex = ui->cmbSerialPorts->findText(currentSelected);
    if (selectIndex >= 0) {
        ui->cmbSerialPorts->setCurrentIndex(selectIndex);
    }
}

void Dialog::on_btnQuit_clicked() { qApp->exit(); }

void Dialog::on_cmbSerialPorts_currentTextChanged(QString text) { qDebug() << "Selected " << text << "\n"; }

void Dialog::closeConnection() {
    if (m_stream) {
        m_stream->close();
        m_stream = nullptr;
    }
}

void Dialog::on_btnSerialConnect_clicked() {
    if(m_stream)
    {
        closeConnection();
        return;
    }

    SerialConfig config;
    config.name = ui->cmbSerialPorts->currentText();
    if (config.name == "") {
        return;
    }
    config.baudRate = QSerialPort::Baud115200;
    config.dataBits = QSerialPort::Data8;
    config.parity = QSerialPort::NoParity;
    config.stopBits = QSerialPort::OneStop;
    config.flowControl = QSerialPort::NoFlowControl;

    SerialDataStream *serial = new SerialDataStream(this);
    m_stream = serial;
    connect(m_stream, &DataLoggerStream::dataReceived, this, &Dialog::onDataReceived);
    connect(m_stream, &DataLoggerStream::errorMessage, this, &Dialog::setConsoleText);
    if (serial->open(config)) {
        setConsoleText("Connected to " + config.name + " at " + QString::number(config.baudRate));
    } else {
        closeConnection();
    }
}

// =======================================
//          Update methods
// =======================================

void Dialog::updateCarID(QString CarID) {
    //  update the value displayed on the car ID
    ui->CarNumber_lcdNumber->display(CarID);
}

void Dialog::updateRES(QString Res_Status) {
    //  update the value displayed on the RES status

    ui->RES_lcdNumber->display(Res_Status);

    if (Res_Status.toInt() == 1) {
        QPixmap GreenEstop("emergency-stop-green.png");
        ui->RES_ESTOP_RED->setPixmap(GreenEstop);
        ui->RES_ESTOP_RED_2->setPixmap(GreenEstop);
        ui->Estop_Text->setPlainText("ESTOP NOT ACTIVATED");
    } else {
        ui->Estop_Text->setPlainText("ESTOP ACTIVATED");
        QPixmap RedEstop("emergency-stop-red.png");
        ui->RES_ESTOP_RED->setPixmap(RedEstop);
        ui->RES_ESTOP_RED_2->setPixmap(RedEstop); // Big Estop
    }
}

void Dialog::updateAutonomousState(QString value) {
    const int state = value.toInt();
    ui->AS_Text->document()->setPlainText(QString::number(state) + " - " + util::intToAutonomousState(state));
}
void Dialog::updateAutonomousMission(QString value) {
    const int mission = value.toInt();
    ui->AMI_Text->document()->setPlainText(QString::number(mission) + " - " + util::intToMission(mission));
}
void Dialog::updateEbsState(QString value) {
    const int state = value.toInt();
    ui->EBS_Text->document()->setPlainText(QString::number(state) + " - " + util::intToEbsState(state));
}

void Dialog::updateMotorMomentActual(QString value) {
    ui->MMA_Slider_Actual->setValue(value.toInt());
    ui->MMA_Text_Actual->document()->setPlainText(value);
}

void Dialog::updateMotorMomentTarget(QString value) {
    ui->MMA_Slider_Target->setValue(value.toInt());
    ui->MMA_Text_Target->document()->setPlainText(value);
}

void Dialog::updateSpeedActual(QString value) {
    const float speed{util::normalizeSlider(value.toInt())};
    ui->SA_Slider->setValue(speed);
    ui->SA_Text_Actual->setPlainText(QString::number(speed));
}

void Dialog::updateSpeedTarget(QString value) {
    const float speed{util::normalizeSlider(value.toInt())};
    ui->ST_Slider->setValue(speed);
    ui->ST_Text_Actual->setPlainText(QString::number(speed));
}

void Dialog::updateSteeringTarget(QString value) {
    const float degrees{util::normalizeSlider(value.toInt())};
    ui->STT_Slider->setValue(degrees);
    ui->STT_Text_Target->setPlainText(QString::number(degrees));
}

void Dialog::updateSteeringActual(QString value) {
    const float degrees{util::normalizeSlider(value.toInt())};
    ui->STA_Slider->setValue(degrees);
    ui->STA_Text_Actual->setPlainText(QString::number(degrees));
}

void Dialog::updateBrakeTarget(QString value) {
    const float percent{util::normalizeSlider(value.toInt())};
    ui->BRT_Slider->setValue(percent);
    ui->BRT_Text_Actual->setPlainText(QString::number(percent));
}

void Dialog::updateBrakeActual(QString value) {
    const float percent{util::normalizeSlider(value.toInt())};
    ui->BRA_Slider->setValue(percent);
    ui->BRA_Text_Actual->setPlainText(QString::number(percent));
}
