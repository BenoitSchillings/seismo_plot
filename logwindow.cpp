#include "logwindow.h"
#include "ui_logwindow.h"
#include <qLabel>
#include <qFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollBar>
#include <QTextEdit>

LogWindow::LogWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogWindow)
{
    content = new QTextEdit();
    this->setCentralWidget(content);
}

LogWindow::~LogWindow()
{
    delete ui;
}

void LogWindow::addText(char *buf)
{
    content->append(QString(buf));
}
