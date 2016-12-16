#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T12:40:38
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seismo_plot
TEMPLATE = app
CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    qplotter.cpp \
    ble_source.cpp \
    deviceinfo.cpp \
    logwindow.cpp

QMAKE_LFLAGS += -v

HEADERS  += mainwindow.h \
    qplotter.h \
    ble_source.h \
    deviceinfo.h \
    logwindow.h

FORMS    += mainwindow.ui \
    logwindow.ui
