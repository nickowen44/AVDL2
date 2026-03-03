#include "dialog.h"
#include <QApplication>
#include <QDateTime>
#include <fstream>

class FileLogger {
  public:
    FileLogger(std::string fileName) : m_stream{fileName} {}

    void log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        if (!m_stream) {
            return;
        }

        QString typeStr;
        switch (type) {
        case QtDebugMsg:
            typeStr = "DEBUG";
            break;
        case QtInfoMsg:
            typeStr = "INFO";
            break;
        case QtWarningMsg:
            typeStr = "WARNING";
            break;
        case QtCriticalMsg:
            typeStr = "CRITICAL";
            break;
        case QtFatalMsg:
            typeStr = "FATAL";
            break;
        }

        // Format the timestamp
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

        // Write the log entry: [TIME] [TYPE] [FILE:LINE] MESSAGE
        m_stream << QString("[%1] [%2] [%3:%4] %5").arg(timestamp).arg(typeStr).arg(context.file).arg(context.line).arg(msg).toStdString();
        m_stream.flush();
    }

    static FileLogger &getInstance() {
        static FileLogger instance{"avdl.log"};
        return instance;
    }

  private:
    std::ofstream m_stream;
};

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    FileLogger::getInstance().log(type, context, msg);
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(customMessageOutput);

    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("NUVOTION - AV DATALOGGER");
    w.setFixedSize(1600, 900);
    w.show();

    return a.exec();
}
