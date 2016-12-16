#ifndef LOGWINDOW_H
#define LOGWINDOW_H
#include <QMainWindow>
#include <QTextEdit>

namespace Ui {
class LogWindow;
}


class LogWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();
    void    addText(char *buf);

private:
    QTextEdit *content;
    Ui::LogWindow *ui;
};


#endif
