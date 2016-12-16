#include "mainwindow.h"
#include "logwindow.h"
#include <QApplication>

#include "ble_source.h"

LogWindow *log;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BLEReader reader;

    reader.deviceSearch();

    MainWindow w;
    reader.setTarget(w.plotter);
    w.resize(1000, 600);
    w.show();

    log = new LogWindow();
    log->resize(400, 400);
    log->show();

    return a.exec();
}



int blog (const char * format, ... )
{
    char buf[512];

    va_list args;
    va_start (args, format);
    vsprintf (buf ,format, args);
    va_end (args);
    log->addText(buf);
}
