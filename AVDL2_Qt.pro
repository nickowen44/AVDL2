#-------------------------------------------------
#
# Project created by QtCreator 2024-12-24T17:24:32
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVDL2_Qt
TEMPLATE = app


SOURCES += src/datastream.cpp \
        src/dialog.cpp \
        src/main.cpp \
        src/serial.cpp \


HEADERS  += src/datatream.hpp \
        src/datastream.hpp \
        src/dialog.h \
        src/serial.h \
        src/serial.hpp \
        src/util.hpp

FORMS    += src/dialog.ui
