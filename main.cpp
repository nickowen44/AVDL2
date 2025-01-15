#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("NUVOTION - AV DATALOGGER");
    w.setFixedSize(1600,900);
    w.show();


    return a.exec();
}
