#-------------------------------------------------
#
# Project created by QtCreator 2024-12-24T17:24:32
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVDL2_Qt
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
        serialconnect.cpp \
        strobethread.cpp

HEADERS  += dialog.h \
    serialconnect.h \
    strobethread.h

FORMS    += dialog.ui
