#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include "strobethread.h"
//#include "serialconnect.h"
#include "serialportfacade.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

   // sAVDLser = new SerialConnect();

    m_serial = new QSerialPort(this);


    qInfo() << this << Q_FUNC_INFO << QThread::currentThread();



    sThread = new StrobeThread(this); // thread is created on heap to prevent it goes out of scope
    connect(sThread,SIGNAL(FlashEstopN(int)),this, SLOT(FlashEstop(int)));

    qInfo() << this << Q_FUNC_INFO << QThread::currentThread();

    //Started
    sThread->Stop=false;

    sThread->start();



    ui->CarNumber_lcdNumber->display("----");
    ui->RES_lcdNumber->display("-");
    ui->MMA_Text_Actual->setPlainText("___");
    ui->MMA_Text_Target->setPlainText("___");
    ui->ST_Text_Actual->setPlainText("___");
    ui->SA_Text_Actual->setPlainText("___");


    QPixmap Background ("C:/Users/user/Downloads/Qt-Temperature-Sensor-master/Qt-Temperature-Sensor-master/DS18B20_Qt/Background.jpg");
    ui->Background->setPixmap(Background);
    QPixmap NUVO_LOGO ("C:/Users/user/Downloads/Qt-Temperature-Sensor-master/Qt-Temperature-Sensor-master/DS18B20_Qt/Nuvomotorsportlogo.png");
    ui->NUVO_LOGO->setPixmap(NUVO_LOGO);



    arduino = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";
    temperature_value = 0.0;






}

Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close(); //    Close the serial port if it's open.
    }
    delete ui;
}

void Dialog::readSerial(QByteArray Sstring)
{

    // serialData = arduino->readAll();
    //serialData=printReadData;
    //qDebug() << serialData;
    qDebug() << "PARSING DATA";
    serialData=Sstring;
    qDebug() << serialData;
    ui->Console->document()->setPlainText(serialData);
    /*
     * readyRead() doesn't guarantee that the entire message will be received all at once.
     * The message can arrive split into parts.  Need to buffer the serial data and then parse for the temperature value.
     *
     */
    QList<QByteArray> lines = Sstring.replace("\r\n", "\n").split('\n');
    for(const auto &line : lines) {
        readSerialLine(line);
    }

}
void Dialog::readSerialLine(const QByteArray& serialData) {





    QRegularExpression re("(ID=)+(\\w\\d\\d)");
    QRegularExpressionMatch match = re.match(serialData);
    if (match.hasMatch()) {
         serialBuffer = match.captured(0); // matched == "ID=A##"
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateCarID(buffer_split[1]);
    }

    qDebug() << serialBuffer;
    serialBuffer="";

    QRegularExpression re2("(RES=)+(\\d)");
    QRegularExpressionMatch match2 = re2.match(serialData);
    if (match2.hasMatch()) {
        serialBuffer = match2.captured(0); // matched == "RES=#"
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateRES(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";

    QRegularExpression re3("(MMA=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match3 = re3.match(serialData);
    if (match3.hasMatch()) {
        serialBuffer = match3.captured(0); // matched == "MMA=###"
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateMMA(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";


    QRegularExpression re4("(MMT=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match4 = re4.match(serialData);
    if (match4.hasMatch()) {
        serialBuffer = match4.captured(0); // matched == "MMA=###"
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateMmmTarget(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";


    QRegularExpression re5("(AST=)+(\\d)");
    QRegularExpressionMatch match5 = re5.match(serialData);
    if (match5.hasMatch()) {
        serialBuffer = match5.captured(0); // matched == "AST=#"
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateAutonomousState(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";

    QRegularExpression re6("(SA=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match6 = re6.match(serialData);
    if (match6.hasMatch()) {
        serialBuffer = match6.captured(0); // matched == "SA=###"
        qDebug() << serialBuffer;
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateSaActual(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";

    QRegularExpression re7("(ST=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match7 = re7.match(serialData);
    if (match7.hasMatch()) {
        serialBuffer = match7.captured(0); // matched == "S&=###"
        qDebug() << serialBuffer;
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateStTarget(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";

    QRegularExpression re8("(BRT=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match8 = re8.match(serialData);
    if (match8.hasMatch()) {
        serialBuffer = match8.captured(0); // matched == "S&=###"
        qDebug() << serialBuffer;
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateBrtTarget(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";

    QRegularExpression re9("(BRA=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match9 = re9.match(serialData);
    if (match9.hasMatch()) {
        serialBuffer = match9.captured(0); // matched == "S&=###"
        qDebug() << serialBuffer;
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateBraActual(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";


    QRegularExpression re10("(STT=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match10 = re10.match(serialData);
    if (match10.hasMatch()) {
        serialBuffer = match10.captured(0); // matched == "S&=###"
        qDebug() << serialBuffer;
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateSttTarget(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";

    QRegularExpression re11("(STA=)+(\\d\\d\\d|\\d\\d|\\d)");
    QRegularExpressionMatch match11 = re11.match(serialData);
    if (match11.hasMatch()) {
        serialBuffer = match11.captured(0); // matched == "S&=###"
        qDebug() << serialBuffer;
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateStaActual(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";


    QRegularExpression re12("(AMI=)+(\\d)");
    QRegularExpressionMatch match12 = re12.match(serialData);
    if (match12.hasMatch()) {
        serialBuffer = match12.captured(0); // matched == "AST=#"
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateAutonomousMission(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";



    QRegularExpression re13("(EBS=)+(\\d)");
    QRegularExpressionMatch match13 = re13.match(serialData);
    if (match13.hasMatch()) {
        serialBuffer = match13.captured(0); // matched == "AST=#"
        QStringList buffer_split = serialBuffer.split("="); //  split the serialBuffer string, parsing with '=' as the separator
        Dialog::UpdateEbsState(buffer_split[1]);
    }
    qDebug() << serialBuffer;
    serialBuffer="";









    /*
    //  Check to see if there less than 3 tokens in buffer_split.
    //  If there are at least 3 then this means there were 2 commas,
    //  means there is a parsed temperature value as the second token (between 2 commas)
    if(buffer_split.length() < 3){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
        serialData = arduino->readAll();
        qDebug() << serialData;
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{
        // the second element of buffer_split is parsed correctly, update the temperature value on temp_lcdNumber
        serialBuffer = "";
        qDebug() << buffer_split << "\n";
        parsed_data = buffer_split[1];
        temperature_value = (9/5.0) * (parsed_data.toDouble()) + 32; // convert to fahrenheit
        qDebug() << "Temperature: " << temperature_value << "\n";
        parsed_data = QString::number(temperature_value, 'g', 4); // format precision of temperature_value to 4 digits or fewer
        Dialog::updateTemperature(parsed_data);
    }
*/

}

void Dialog::UpdateCarID(QString CarID)
{
    //  update the value displayed on the car ID
    ui->CarNumber_lcdNumber->display(CarID);

}


void Dialog::UpdateRES(QString Res_Status)
{
    //  update the value displayed on the RES status

    ui->RES_lcdNumber->display(Res_Status);

    if (Res_Status.toInt()==1){
        QPixmap GreenEstop ("C:/Users/user/Downloads/Qt-Temperature-Sensor-master/Qt-Temperature-Sensor-master/DS18B20_Qt/emergency-stop-green.png");
        ui->RES_ESTOP_RED->setPixmap(GreenEstop);
        ui->RES_ESTOP_RED_2->setPixmap(GreenEstop);

        //ui->RES_ESTOP_RED->setVisible(false);
        qDebug() << "ESTOP ACTIVATED";
        ui->Estop_Text->setPlainText("ESTOP NOT ACTIVATED");
    }
    else {
       // ui->RES_ESTOP_RED->setHidden(true);
        qDebug() << "ESTOP NOT - ACTIVATED";
        ui->Estop_Text->setPlainText("ESTOP ACTIVATED");
        QPixmap RedEstop ("C:/Users/user/Downloads/Qt-Temperature-Sensor-master/Qt-Temperature-Sensor-master/DS18B20_Qt/emergency-stop-red.png");
        ui->RES_ESTOP_RED->setPixmap(RedEstop);
        ui->RES_ESTOP_RED_2->setPixmap(RedEstop); // Big Estop


    }

}

void Dialog::UpdateMMA(QString MMA)
{
    //  update the value displayed on the MMA status
    int MMAi=MMA.split(" ")[0].toInt();

    ui->MMA_Slider_Actual->setValue(MMAi);
    ui->MMA_Text_Actual->document()->setPlainText(MMA);

}

void Dialog::UpdateMmmTarget(QString MMA)
{
    //  update the value displayed on the MMA status
    int MMAi=MMA.split(" ")[0].toInt();

    ui->MMA_Slider_Target->setValue(MMAi);
    ui->MMA_Text_Target->document()->setPlainText(MMA);

}

void Dialog::UpdateAutonomousState(QString Astate)
{
    //  update the value displayed on the AS status


    switch(Astate.toInt()){
    case 0:
        ui->AS_Text->document()->setPlainText("0 - No Mission Selected");
        break;

    case 1:
        ui->AS_Text->document()->setPlainText("1 - AS_State_OFF");
        break;

    case 2:
        ui->AS_Text->document()->setPlainText("2 - AS_State_READY");
        break;

    case 3:
        ui->AS_Text->document()->setPlainText("3 - AS_State_DRIVING");
        break;

    case 4:
        ui->AS_Text->document()->setPlainText("4 - AS_state_emergency_brake");
        break;

    case 5:
        ui->AS_Text->document()->setPlainText("5 - AS_state_finish");
        break;

    default:

            break;
    }
}


void Dialog::UpdateSaActual(QString SA)
{
    //  update the value displayed on the SA status
    int SAi=SA.split(" ")[0].toInt();
    float f = SAi;
    //f=f/0.5;
    if( f > 127 ) {
        f -= 256;
    }
    ui->SA_Slider->setValue(f);
    ui->SA_Text_Actual->setPlainText(QString::number(f));
}


void Dialog::UpdateStTarget(QString ST)
{
    //  update the value displayed on the SA status
    int STi=ST.split(" ")[0].toInt();
    float f = STi;
    //f=f/0.5;
    if( f > 127 ) {
        f -= 256;
    }
    ui->ST_Slider->setValue(f);
    ui->ST_Text_Actual->setPlainText(QString::number(f));
}

void Dialog::UpdateBrtTarget(QString BRT)
{
    //  update the value displayed on the BRT status
    int BRTi=BRT.split(" ")[0].toInt();
    float f = BRTi;
    //f=f/0.5;
    if( f > 127 ) {
        f -= 256;
    }
    ui->BRT_Slider->setValue(f);
    ui->BRT_Text_Actual->setPlainText(QString::number(f));
}


void Dialog::UpdateBraActual(QString BRA)
{
    //  update the value displayed on the SA status
    int BRAi=BRA.split(" ")[0].toInt();
    float f = BRAi;
    //f=f/0.5;
    if( f > 127 ) {
        f -= 256;
    }
    ui->BRA_Slider->setValue(f);
    ui->BRA_Text_Actual->setPlainText(QString::number(f));
}



void Dialog::UpdateSttTarget(QString STT)
{
    //  update the value displayed on the BRT status
    int STTi=STT.split(" ")[0].toInt();
    float f = STTi;
    //f=f/0.5;
    if( f > 127 ) {
        f -= 256;
    }
    ui->STT_Slider->setValue(f);
    ui->STT_Text_Target->setPlainText(QString::number(f));
}


void Dialog::UpdateStaActual(QString STA)
{
    //  update the value displayed on the SA status
    int STAi=STA.split(" ")[0].toInt();
    float f = STAi;
    //f=f/0.5;
    if( f > 127 ) {
        f -= 256;
    }
    ui->STA_Slider->setValue(f);
    ui->STA_Text_Actual->setPlainText(QString::number(f));
}



void Dialog::UpdateAutonomousMission(QString Astate)
{
    //  update the value displayed on the AS status


    switch(Astate.toInt()){
    case 0:
        ui->AMI_Text->document()->setPlainText("0 - No Mission Selected");
        break;

    case 1:
        ui->AMI_Text->document()->setPlainText("1 - AMI_state_acceleration");
        break;

    case 2:
        ui->AMI_Text->document()->setPlainText("2 - AMI_state_skidpad");
        break;

    case 3:
        ui->AMI_Text->document()->setPlainText("3 - AMI_state_trackdrive");
        break;

    case 4:
        ui->AMI_Text->document()->setPlainText("4 - AMI_state_braketest");
        break;

    case 5:
        ui->AMI_Text->document()->setPlainText("5 - AMI_state_inspection");
        break;

    case 6:
        ui->AMI_Text->document()->setPlainText("5 - AMI_state_autocross");
        break;


    default:

        break;
    }
}





void Dialog::UpdateEbsState(QString Astate)
{
    //  update the value displayed on the AS status


    switch(Astate.toInt()){
    case 0:
        ui->EBS_Text->document()->setPlainText("0 - No Mission Selected");
        break;

    case 1:
        ui->EBS_Text->document()->setPlainText("1 - EBS_State_Unavailiable");
        break;

    case 2:
        ui->EBS_Text->document()->setPlainText("2 - EBS_State_Armed");
        break;

    case 3:
        ui->EBS_Text->document()->setPlainText("3 - EBS_State_Activated");
        break;


    default:

        break;
    }
}


void Dialog::FlashEstop(int number)
{
    //  Flash the Estop pixmaps


QPixmap RedEstop ("C:/Users/user/Downloads/Qt-Temperature-Sensor-master/Qt-Temperature-Sensor-master/DS18B20_Qt/emergency-stop-red.png");

    if (number<5){
   // qDebug() << "greater than 5";

        ui->RES_ESTOP_RED_2->setHidden(false);

        ui->RES_ESTOP_RED_2->setPixmap(RedEstop); // Big Estop
    }
    else
    {
       // qDebug() << "less than 5";

        ui->RES_ESTOP_RED_2->setHidden(true);
    }

}

void Dialog::on_pushButton_clicked()
{
    sThread->Stop=true;
    //sThread->terminate(); //Very extream tells the OS to kill the thread

    sThread->quit();
    sThread->deleteLater();
    sThread->wait();

    qApp->exit();
}


void Dialog::on_pushButton_2_clicked()
{
   // sAVDLser->readSerial();

   //sAVDLser->run();

   // sAVDLser->close();
   openSerialPort();
}




void Dialog::openSerialPort()
{

    p.name="COM19";
    p.baudRate=QSerialPort::Baud115200;
    p.dataBits=QSerialPort::Data8;
    p.parity=QSerialPort::NoParity;
    p.stopBits=QSerialPort::OneStop;
    p.flowControl=QSerialPort::NoFlowControl;

    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setFlowControl(p.flowControl);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);

    qInfo() << this << Q_FUNC_INFO << QThread::currentThread();

            m_multithreadserial = new SerialPortFacade();
            m_serialPortThread = new QThread();
            m_multithreadserial->moveToThread(m_serialPortThread);
            initMultiThreadingConnections();
            m_serialPortThread->start();
            m_serialPortThread->setObjectName("serial thread");
            qInfo() << m_serialPortThread << Q_FUNC_INFO << QThread::currentThread();
            qInfo() << sThread << Q_FUNC_INFO << QThread::currentThread();

            emit openSerialPort_MT(p.name, p.baudRate, p.dataBits, p.parity, p.stopBits, p.flowControl);


}



// Initializes Multi-Threading signal/slot connections
void Dialog::initMultiThreadingConnections()
{
    // Serial Port behavior specific connections
    connect(this, &Dialog::openSerialPort_MT, m_multithreadserial, &SerialPortFacade::open);
    connect(this, &Dialog::closeSerialPort_MT, m_multithreadserial, &SerialPortFacade::close, Qt::ConnectionType::BlockingQueuedConnection); // Blocking guarantees serial port has a chance to wrap everything up. Negligible blocking cost
   // connect(m_console, &Console::getData, m_multithreadserial, &SerialPortFacade::write);
    //connect(m_multithreadserial, &SerialPortFacade::printReadData,  this,  &Dialog::readSerial);

    connect(m_multithreadserial,SIGNAL(printReadData(QByteArray)),this, SLOT(readSerial(QByteArray)));


   // connect(m_multithreadserial, &SerialPortFacade::connectionChange, this, &MainWindow::handleConnectionChange);
    // Thread specific connections. Ordering matters here.
    connect(m_multithreadserial, &SerialPortFacade::endThread, m_serialPortThread, &QThread::quit, Qt::ConnectionType::DirectConnection); // DirectConnection forces the thread to quit instantly on signal emition
    connect(m_multithreadserial, &SerialPortFacade::endThread, m_multithreadserial, &SerialPortFacade::deleteLater);
    connect(m_serialPortThread, &QThread::finished, m_serialPortThread, &QThread::deleteLater);
}
