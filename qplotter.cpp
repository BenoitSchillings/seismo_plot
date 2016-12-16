#include <QtWidgets>
#include "qplotter.h"
#include <QTime>

#define BIGTICK     250
#define SMALLTICK   5

int blog (const char * format, ... );

QPlotter::QPlotter(QWidget *parent) : QWidget(parent)
{
    time_per_sample = 0.06;
    time_per_pixel = 0.06;
    large_tick_time = time_per_pixel * 200;
    small_tick_time = time_per_pixel * 50;
    setWindowTitle(tr("Sismo"));
    InitLoad();
    fscale = 100;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(load()));
    timer->start(110);
    rt_mode = true;
    start_time = new QTime(QTime::currentTime());

}

void QPlotter::NewValue(float v)
{
    data->append(v);

    //blog("new value %f", v);

    if (rt_mode) {
        SetCenterTime(data->size() * time_per_sample);
        this->update();
    }
    else {
        this->update();
    }
}

void QPlotter::InitLoad()
{
    data = new QVector<float>();
    data->reserve(1000000);

    file = new QFile("/Users/benoit/log.txt");
    if (file->exists()) {
        printf("Exist\n");
    }

    file->open(QIODevice::ReadOnly);
    QTextStream in(file);

    QString line = in.readLine();
    while(!in.atEnd()) {
        float v;

        v = line.toFloat();
        data->append(v);
        line = in.readLine();
    }
}


void QPlotter::load()
{
    int cnt = 0;

    this->setFocus();


    QTextStream in(file);

    QString line;
    int capacity = data->capacity();


    while(!in.atEnd()) {
        float v;
        line = in.readLine();

       if (line != QString::null) {
           v = line.toFloat();
            data->append(v);
        }

        cnt++;
    }

    if (cnt > 0) {
        if (rt_mode) {
         SetCenterTime(data->size() * time_per_sample);
         this->update();
        }
        else
            this->update();
    }
}

double QPlotter::fx(double time, double height)
{
    int cnt = (time_per_pixel/time_per_sample);
    if (cnt < 1) cnt = 1;

    int sample_pos = round(TimeToSample(time));

    if ((sample_pos + cnt + 1) > data->size())
        return height;

    float v = 0;


    for (int i = 0; i < cnt; i++) {
        v = v + data->at(sample_pos);
        sample_pos++;
    }
    v = height + v / (cnt * fscale);

/*
    for (int i = 0; i <= cnt; i++) {
        float cv = data->at(sample_pos);

        if (fabs(cv)>v) v = cv;
        sample_pos++;
    }


    v = v/fscale + height;
*/
    return v;
}

int QPlotter::TickHeight(float t0, float t1)
{
    if (trunc(t0/large_tick_time) != trunc(t1/large_tick_time)) {
        return BIGTICK;
    }
    if (trunc(t0/small_tick_time) != trunc(t1/small_tick_time)) {
        return SMALLTICK;
    }
    else
        return 0;
}

double QPlotter::SampleToTime(int sample)
{
    return sample * time_per_sample;
}

double QPlotter::TimeToSample(double time)
{
    return round(time / time_per_sample);
}

double QPlotter::PixelToTime(int pixel)
{
    return pixel * time_per_pixel;
}

int QPlotter::TimeToPixel(double time)
{
    return time / time_per_pixel;
}

QString QPlotter::TimeToString(float time)
{
   QTime t(*start_time);

    t = t.addMSecs((int)(1000*time));

    return t.toString("hh:mm:ss");
}

void QPlotter::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);

    QRect  clip = evt->rect();
    int height = dad->height() / 2;
    int left = clip.left();
    int right = clip.right();


    QPen pen = QPen(Qt::black);
    pen.setWidthF(0.25);
    painter.setPen(pen);


    int x;

    left -= 20;
    right += 20;

    if (left < 0) left = 0;

    int last_x = left;

    double cur_v;
    double cur_v1;

    cur_v = fx(PixelToTime(left), height);

    for (x = left; x <= right; x++) {
        painter.setPen(pen);
        cur_v1 = fx(PixelToTime(x), height);
        if (cur_v1 != cur_v) {
            painter.drawLine(last_x, cur_v, x, cur_v1);
            cur_v = cur_v1;
            last_x = x;
        }


        int tick = TickHeight(PixelToTime(x), PixelToTime(x + 1));
        if (tick > 0) {
            painter.setPen(QColor(170, 170, 240));
            painter.drawLine(x, height - tick, x, height + tick);
            if (tick == BIGTICK) {
                QString time;
                time = TimeToString(PixelToTime(x+1));
                QRectF r;

                r.setRect(x - 100, height + height * 0.9, 200, 30);
                painter.setPen(Qt::red);
                painter.drawText(r, Qt::AlignCenter, time);
            }
        }
    }
     painter.setPen(QColor(140, 140, 140));
     painter.drawLine(left, height, right, height);

     //this->grabMouse();
}

float QPlotter::CenterTime()
{
    QRect r = dad->geometry();
    QScrollBar *bar = dad->horizontalScrollBar();

    int hp = bar->sliderPosition();

    hp = hp + r.width()/2;

    return PixelToTime(hp);
}

void QPlotter::SetCenterTime(float t)
{
    QRect r = dad->geometry();

    QScrollBar *bar = dad->horizontalScrollBar();

    int hp = TimeToPixel(t);

    hp = hp -  r.width()/2;

    int hp0 = bar->value();

    if (abs(hp0 - hp) >= (0))
        bar->setValue(hp);

}
/*
void QPlotter::wheelEvent(QWheelEvent * event)
{
    float delta = event->delta();

    if (delta > 0) {
        float center_time = CenterTime();
        time_per_pixel *= (1.0 + delta/100.0);
        SetCenterTime(center_time);
        large_tick_time = time_per_pixel * 200;
        small_tick_time = time_per_pixel * 50;
        this->repaint();
    }

    if (delta < 0) {
        float center_time = CenterTime();
        time_per_pixel /= (1.0 - delta/100.0);
        SetCenterTime(center_time);
        large_tick_time = time_per_pixel * 200;
        small_tick_time = time_per_pixel * 50;
        this->repaint();
    }

}
*/
void QPlotter::keyPressEvent(QKeyEvent * event)
{
    QString txt = event->text();

    switch(event->key()) {
        case Qt::Key_Up:
         fscale *= 1.1;
         if (fscale > 1240) fscale = 1240.0;
         this->update();
         break;
        case Qt::Key_Down:
         fscale /= 1.1;
         if (fscale < 0.01) fscale = 0.01;
         this->update();
         break;
        case Qt::Key_Right:
         break;
        case Qt::Key_Left:
         break;
    }

    if (txt.compare("-") == 0) {
        float center_time = CenterTime();
        time_per_pixel *= 1.1;
        SetCenterTime(center_time);
        large_tick_time = time_per_pixel * 200;
        small_tick_time = time_per_pixel * 50;
        //this->repaint();
        this->update();
    }

    if (txt.compare("t") == 0) {
        rt_mode ^= 1;
    }

    if (txt.compare("+") == 0) {
        float center_time = CenterTime();
        time_per_pixel /= 1.1;
        SetCenterTime(center_time);
        large_tick_time = time_per_pixel * 200;
        small_tick_time = time_per_pixel * 50;

        this->update();
    }

}
