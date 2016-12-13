#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qLabel>
#include <qFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollBar>
#include "qplotter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QPlotter *plotter = new QPlotter();
    plotter->resize(2114000, 596);
    QScrollArea *scrollArea = new QScrollArea;
    //scrollArea->setBackgroundRole(QPalette::Light);
    plotter->dad = scrollArea;
    scrollArea->setWidget(plotter);
    setCentralWidget(scrollArea);
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);

}

MainWindow::~MainWindow()
{
    delete ui;
}
