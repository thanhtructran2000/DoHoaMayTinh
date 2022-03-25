#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>

class graphics : public QWidget
{
    Q_OBJECT
public:
    explicit graphics(QWidget *parent = 0);
    void timerEvent(QTimerEvent *);
    void paintEvent(QPaintEvent *);
    double timerID;
    double position;
    float angle_sh;
    float angle_mh;
    float angle_hh;
    float angle_star;
    float angle_bell;
    int count;

    QPointF quay(QPointF p, QPointF c, float deta);
    QPointF dichuyen(QPointF p, double kc, int angle);
    QPointF tinhtien(QPointF p, int tx, int ty);
    int random(int n);

    void drawWall(QPainter &painter);
    void drawHouse(QPainter &painter, QPointF p);
    void drawClock(QPainter &painter, QPointF p);
    void drawWindow1_clock(QPainter &painter, QPointF p);
    void drawWindow2_clock(QPainter &painter, QPointF p);
    void drawStar(QPainter &painter, QPointF p);
    void drawBell(QPainter &painter, QPointF p);
    void drawWindow(QPainter &painter);
    void drawSky(QPainter &painter, QPointF p, int w, int h);
    void drawSnowman(QPainter &painter, QPointF p);
    void light(QPainter &painter, QPointF p2);
    void drawPine(QPainter &painter, QPointF p);
    void star(QPainter &painter, QPointF p);
    void light1(QPainter &painter, QPointF p, int r, double kc);
    void light2(QPainter &painter, QPointF q);
    void light3(QPainter &painter, QPointF q);
    void snowflake(QPainter &painter, QPointF p, int L);
    void gift(QPainter &painter, QPointF p1);

signals:
    
public slots:
    
};

#endif // GRAPHICS_H
