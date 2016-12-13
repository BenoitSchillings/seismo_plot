#ifndef QPLOTTER_H
#define QPLOTTER_H

#include <QWidget>
#include <QScrollArea>
#include <QFile>

class QPlotter : public QWidget
{
    Q_OBJECT

private:
    double           time_per_sample;
    double           time_per_pixel;
    double           large_tick_time;
    double           small_tick_time;
    double           fscale;
    QFile            *file;
    bool             rt_mode;
public:
    QScrollArea     *dad;

    QVector<float>  *data;
public:
    explicit QPlotter(QWidget *parent = 0);
    double   SampleToTime(int sample);
    double   TimeToSample(double time);
    double   PixelToTime(int pixel);
    int     TimeToPixel(double time);


    void    SetArray(QVector<float> *array);
    float   CenterTime();
    void    SetCenterTime(float t);

private:
    double   fx(double x, double height);
    int     TickHeight(float t0, float t1);
    QString TimeToString(float time);
    void    InitLoad();

signals:

public slots:
    void load();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    //void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
};

#endif // QPLOTTER_H
