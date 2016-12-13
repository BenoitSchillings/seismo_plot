#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T12:40:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seismo_plot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qplotter.cpp

HEADERS  += mainwindow.h \
    qplotter.h

FORMS    += mainwindow.ui
