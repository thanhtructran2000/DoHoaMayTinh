#include "graphics.h"
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QPainterPath>
#include <QString>
#include "math.h"
#include <QPolygon>
#include <QGradient>

graphics::graphics(QWidget *parent) :
    QWidget(parent)
{
    position = 0;
    angle_sh = 0;
    angle_mh = 0;
    angle_hh = 300;
    angle_bell = 20;
    angle_star = 0;
    count = 0;
    timerID = startTimer(100);
}

void graphics::timerEvent(QTimerEvent *)
{
    position += 1;
    angle_sh += 6;
    if(angle_sh >= 360.0) angle_sh=0;
    angle_mh += 0.1;
    if(angle_mh >= 360.0) angle_mh=0;
    angle_hh += 0.008333;
    if(angle_hh >= 360.0) angle_hh=0;
    angle_star += 20;
    if(angle_bell > 0 && count > -40)
    {
        count -= 10;
        if (count == -40)
        {
            angle_bell = -20;
            count = 0;
        }
    }
    if(angle_bell < 0 && count < 40)
    {
        count += 10;
        if(count == 40)
        {
            angle_bell = 20;
            count = 0;
        }
    }
    repaint();
}

QPointF graphics::quay(QPointF p, QPointF c, float deta)
{
    QPointF pnew;
    double goc = deta * 3.14 / 180;
    pnew.setX(c.x() + (p.x() - c.x())*cos(goc) - (p.y() - c.y())*sin(goc));
    pnew.setY(c.y() + (p.x() - c.x())*sin(goc) + (p.y() - c.y())*cos(goc));
    return pnew;
}

QPointF graphics::dichuyen(QPointF p, double kc, int angle)
{
    QPointF pnew;
    pnew.setX(p.x() + kc*cos(M_PI*angle/180));
    pnew.setY(p.y() + kc*sin(M_PI*angle/180));
    return pnew;
}

QPointF graphics::tinhtien(QPointF p, int tx, int ty)
{
    QPointF pnew;
    pnew.setX(p.x() + tx);
    pnew.setY(p.y() + ty);
    return pnew;
}

int graphics::random(int n)
{
    return rand()%n;
}

void graphics::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    drawWall(painter);
    // draw things
    QPointF p(250, 380);
    drawBell(painter, p);
    drawHouse(painter, p);
    drawClock(painter, p);
    if(angle_mh >= 0 && angle_mh <= 12)
    {
        drawWindow2_clock(painter, p);
        drawStar(painter, p);
    }
    else drawWindow1_clock(painter, p);
    drawWindow(painter);

    QPointF p2(650, 0);
    light(painter, p2);

    // decorate pine
    QPointF q(1250, 50);
    drawPine(painter, q);

    QPointF p1(500, 450);
    gift(painter, p1);
}

// --------------------------------------------------------------------------------------- //

void graphics::drawWall(QPainter &painter)
{
    painter.setBrush(QBrush("#FFA07A"));
    painter.drawRect(0, 0, width(), height());
    painter.setBrush(QBrush("#FF6347"));
    painter.drawRect(0, height()-400, width(), 400);
    painter.setBrush(QBrush("#FF3300"));
    painter.drawRect(0, height()-50, width(), 50);
}

void graphics::drawHouse(QPainter &painter, QPointF p)
{
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.setBrush(QBrush("#CD853F"));
    static const QPointF pointer1[5] = {
        QPointF(p.x()-150, p.y()-200),
        QPointF(p.x()-150, p.y()+150),
        QPointF(p.x()+150, p.y()+150),
        QPointF(p.x()+150, p.y()-200),
        QPointF(p.x(), p.y()-350),
    };
    painter.drawPolygon(pointer1, 5);
    // draw floor
    painter.setBrush(QBrush("#A0522D"));
    painter.drawRect(p.x()-200, p.y()+120, 400, 30);
    // draw roop
    static const QPointF pointer2[6] = {
        QPointF(p.x(), p.y()-350),
        QPointF(p.x()-190, p.y()-160),
        QPointF(p.x()-190, p.y()-200),
        QPointF(p.x(), p.y()-390),
        QPointF(p.x()+190, p.y()-200),
        QPointF(p.x()+190, p.y()-160),
    };
    painter.drawPolygon(pointer2, 6);
}

void graphics::drawClock(QPainter &painter, QPointF p)
{
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    // Draw circle
    painter.setBrush(Qt::white);
    painter.drawEllipse(p.x()-100, p.y()-100, 200, 200);

    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    // Draw second-hand
    QPointF sh(p.x(), p.y()-70);
    sh = quay(sh, p, angle_sh);
    painter.drawLine(p, sh);
    // Draw minute-hand
    QPointF mh(p.x(), p.y()-60);
    mh = quay(mh, p, angle_mh);
    painter.drawLine(p, mh);
    // Draw hour-hand
    QPointF hh(p.x(), p.y()-50);
    hh = quay(hh, p, angle_hh);
    painter.drawLine(p, hh);
    // Draw lines
    painter.setPen(QPen(Qt::red, 4, Qt::SolidLine));
    QPointF m(p.x(), p.y()-90);
    QPointF n(p.x(), p.y()-80);
    for(int i=0; i<12; i++)
    {
        m = quay(m, p, 30);
        n = quay(n, p, 30);
        painter.drawLine(m, n);
    }
}

void graphics::drawWindow1_clock(QPainter &painter, QPointF p)
{
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.setBrush(QBrush("#A0522D"));
    painter.drawRect(p.x()-50, p.y()-220, 100, 100);
    painter.drawLine(p.x(), p.y()-220, p.x(), p.y()-120);
}

void graphics::drawWindow2_clock(QPainter &painter, QPointF p)
{
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.setBrush(QBrush("#8B4513"));
    painter.drawRect(p.x()-50, p.y()-220, 100, 100);

    painter.setBrush(QBrush("#A0522D"));
    painter.drawRect(p.x()-100, p.y()-220, 50, 100);
    painter.drawRect(p.x()+50, p.y()-220, 50, 100);
}

void graphics::drawStar(QPainter &painter, QPointF p)
{
    painter.setPen(QPen(Qt::yellow, 5, Qt::SolidLine));
    QPointF q(p.x(), p.y()-200);
    QPointF c(p.x(), p.y()-170);
    q = quay(q, c, angle_star);
    for (int i=0; i<5; i++)
    {
        QPointF t = quay(q, c, 144);
        painter.drawLine(q, t);
        q = t;
    }
}

void graphics::drawBell(QPainter &painter, QPointF p)
{
    // draw cord
    painter.setPen(QPen(Qt::black, 20, Qt::SolidLine));
    QPointF q(p.x(), p.y()+250);
    QPointF r(q.x(), q.y()-12);
    q = quay(q, p, angle_bell + count);
    r = quay(r, p, angle_bell + count);
    painter.drawLine(p, q);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.setBrush(Qt::red);
    // draw bell
    for(int i=1; i<=5; i++)
    {
        r = quay(r, q, 72);
        painter.drawEllipse(r, 24, 24);
    }
}

void graphics::drawWindow(QPainter &painter)
{
    // draw window frame
    painter.setPen(QPen(Qt::white, 12, Qt::SolidLine));
    painter.setBrush(Qt::black);
    QPointF u(width()/2-250, 100);
    painter.drawRect(u.x(), u.y(), 600, 420);

    // draw moon
    QRadialGradient grad1(u.x()+100, u.y()+100, 110);
    grad1.setColorAt(0, Qt::white);
    grad1.setColorAt(0.5, Qt::black);
    grad1.setColorAt(1, Qt::black);
    painter.fillRect(u.x()+50, u.y()+50, 160, 160, grad1);
    QPixmap moon("D://moon.png");
    painter.drawPixmap(u.x()+80, u.y()+80, 40, 40, moon);

    // draw night sky with stars
    drawSky(painter, u, 600, 420);

    // draw snowman
    drawSnowman(painter, u);

    // draw window
    painter.setPen(QPen(Qt::white, 8, Qt::SolidLine));
    painter.drawLine(u.x(), u.y(), u.x()+80, u.y()+100);
    painter.drawLine(u.x()+80, u.y()+100, u.x()+80, u.y()+420);
    painter.drawLine(u.x()+40, u.y()+50, u.x()+40, u.y()+420);
    painter.drawLine(u.x(), u.y()+140, u.x()+80, u.y()+240);
    painter.drawLine(u.x(), u.y()+280, u.x()+80, u.y()+380);

    painter.drawLine(u.x()+600, u.y(), u.x()+520, u.y()+100);
    painter.drawLine(u.x()+520, u.y()+100, u.x()+520, u.y()+420);
    painter.drawLine(u.x()+560, u.y()+50, u.x()+560, u.y()+420);
    painter.drawLine(u.x()+600, u.y()+140, u.x()+520, u.y()+240);
    painter.drawLine(u.x()+600, u.y()+280, u.x()+520, u.y()+380);
}

void graphics::drawSky(QPainter &painter, QPointF p, int w, int h)
{
    for(int i=1; i<=100; i++)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        int r = random(6);
        int x = int(fmod(rand(), (w + p.x()) - p.x() + 1 ) + p.x());
        int y = int(fmod(rand(), (h + p.y()) - p.y() + 1) + p.y());
        painter.drawEllipse(x, y, r, r);
    }
}

void graphics::drawSnowman(QPainter &painter, QPointF p)
{
    QPointF n(p.x()+400, p.y()+300);
    painter.setOpacity(1);
    painter.setPen(QPen(Qt::white, 8, Qt::SolidLine));
    painter.drawLine(n.x(), n.y()+115, n.x()-120, n.y()+20);
    painter.drawLine(n.x(), n.y()+115, n.x()+120, n.y()+20);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawChord(n.x()-100, n.y()+20, 200, 200, 0, 180*16);
    painter.drawEllipse(n.x()-50, n.y()-50, 100, 100);
    // draw nose
    painter.setBrush(Qt::red);
    painter.drawEllipse(n.x()-10, n.y()-10, 20, 20);
    // draw shawl
    painter.setPen(QPen(Qt::red, 16, Qt::SolidLine));
    painter.drawLine(n.x()-55, n.y()+35, n.x()+55, n.y()+35);
    painter.drawLine(n.x()+30, n.y()+35, n.x()+30, n.y()+80);
    // draw mouth
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    painter.drawArc(n.x()-20, n.y()-20, 40, 40, -30*16, -120*16);
    // draw eyes
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawEllipse(n.x()-25, n.y()-25, 8, 8);
    painter.drawEllipse(n.x()+17, n.y()-25, 8, 8);
    // draw hat
    painter.setBrush(Qt::red);
    QPolygon polygon;
    polygon << QPoint(n.x(), n.y()-100) << QPoint(n.x()-40, n.y()-30) << QPoint(n.x()+40, n.y()-30);
    painter.drawPolygon(polygon);
    painter.setBrush(Qt::white);
    painter.drawEllipse(n.x()-10, n.y()-110, 20, 20);
    painter.setPen(QPen(Qt::white, 12, Qt::SolidLine));
    painter.drawLine(n.x()-40, n.y()-30, n.x()+40, n.y()-30);
}

void graphics::light(QPainter &painter,QPointF p2)
{
    //day 1
    painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
    painter.drawLine(p2.x(),p2.y(),p2.x(),p2.y()+50);
    int r=50;
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.drawEllipse(p2.x()-25,p2.y()+50,r,r);
    snowflake(painter,QPointF(p2.x(),p2.y()+3*r/2),20);
    //day 2
    painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
    painter.drawLine(p2.x()+90,p2.y(),p2.x()+90,p2.y()+80);
    star(painter,QPointF(p2.x()+90,p2.y()+120));
    //day 3
    painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
    painter.drawLine(p2.x()+180,p2.y(),p2.x()+180,p2.y()+40);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.drawEllipse(p2.x()+160,p2.y()+40,r-10,r-10);
    snowflake(painter,QPointF(p2.x()+180,p2.y()+3*(r-10)/2),18);
    //day 4
    painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
    painter.drawLine(p2.x()+250,p2.y(),p2.x()+250,p2.y()+120);
    star(painter,QPointF(p2.x()+250,p2.y()+155));
    //day 5
    painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
    painter.drawLine(p2.x()+320,p2.y(),p2.x()+320,p2.y()+90);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.drawEllipse(p2.x()+290,p2.y()+60,r+10,r+10);
    snowflake(painter,QPointF(p2.x()+320,p2.y()+3*(r+10)/2),30);
}

void graphics::drawPine(QPainter &painter, QPointF p)
{
   painter.setPen(Qt::NoPen);
   painter.setBrush(QBrush("#228B22"));
   int r = 50;
   static const QPointF pointer[21] = {
       QPointF(p.x(), p.y()),
       QPointF(p.x()-2*r, p.y()+3*r), //
       QPointF(p.x()-r, p.y()+3*r),
       QPointF(p.x()-3*r, p.y()+6*r), //
       QPointF(p.x()-2*r, p.y()+6*r),
       QPointF(p.x()-4*r, p.y()+9*r), //
       QPointF(p.x()-3*r, p.y()+9*r),
       QPointF(p.x()-5*r, p.y()+12*r), //
       QPointF(p.x()-4*r, p.y()+12*r),
       QPointF(p.x()-6*r, p.y()+15*r), //
       QPointF(p.x()-5*r, p.y()+15*r),

       QPointF(p.x()+5*r, p.y()+15*r),
       QPointF(p.x()+6*r, p.y()+15*r),
       QPointF(p.x()+4*r, p.y()+12*r),
       QPointF(p.x()+5*r, p.y()+12*r),
       QPointF(p.x()+3*r, p.y()+9*r),
       QPointF(p.x()+4*r, p.y()+9*r),
       QPointF(p.x()+2*r, p.y()+6*r),
       QPointF(p.x()+3*r, p.y()+6*r),
       QPointF(p.x()+r, p.y()+3*r),
       QPointF(p.x()+2*r, p.y()+3*r),
   };
   painter.drawPolygon(pointer, 21);

   light1(painter, QPointF(p.x()-2*r, p.y()+3*r), 5, 200);
   light1(painter, QPointF(p.x()-3*r, p.y()+6*r), 5, 300);
   light1(painter, QPointF(p.x()-4*r, p.y()+9*r), 5, 400);
   light1(painter, QPointF(p.x()-5*r, p.y()+12*r), 5, 500);

   light2(painter, p);
   light3(painter, p);

   star(painter, p);
}

void graphics::star(QPainter &painter, QPointF p)
{
    painter.setPen(QColor(random(255),random(255),random(255)));
    int angle=0;
    double kc=0;
    QPointF BD=p;
    for(int i=0;i<150;i++)
    {
        BD=p;
        p=dichuyen(p,kc,angle);
        painter.drawLine(BD,p);
        kc+=0.5;
        angle+=144;
    }
}

void graphics::light1(QPainter &painter, QPointF p, int r, double kc)
{
    painter.setPen(QColor(random(255),random(255),random(255)));
    painter.setBrush(Qt::white);
    QPointF q(p.x(), p.y()-r);
    while (q.x() <= p.x() + kc)
    {
        painter.drawEllipse(q, 2*r, 2*r);
        q = tinhtien(q, 3*r, 0);
    }
}

void graphics::light2(QPainter &painter, QPointF q)
{
    //day den
    painter.setPen(QPen(Qt::black,3,Qt::SolidLine));

    painter.drawLine(q.x()-30,q.y()+50,q.x()+55,q.y()+90);

    painter.drawLine(q.x()+67,q.y()+180,q.x()-97,q.y()+230);
    painter.drawLine(q.x()-118,q.y()+330,q.x()+150,q.y()+380);
    painter.drawLine(q.x()+170,q.y()+480,q.x()-197,q.y()+530);
    painter.drawLine(q.x()-220,q.y()+630,q.x()+250,q.y()+680);
}

void graphics::light3(QPainter &painter, QPointF q)
{
    //day bong den
    QPointF q0(q.x()-18,q.y()+62);
    int r=5;
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    for(int i=0;i<4;i++)
    {
        painter.drawEllipse(q0,r,r);
        q0 = tinhtien(q0,4*r,10);
    }
    QPointF q1(q.x()-70,q.y()+230);
    painter.setBrush(Qt::yellow);
    for(int i=0;i<6;i++)
    {

        painter.drawEllipse(q1,r+2,r+2);
        q1 = tinhtien(q1,5*r,-(8));
    }
    QPointF q2(q.x()-90,q.y()+345);
    painter.setBrush(Qt::yellow);
    for(int i=0;i<6;i++)
    {

        painter.drawEllipse(q2,r+4,r+4);
        q2 = tinhtien(q2,8*r,8);
    }
    QPointF q3(q.x()-170,q.y()+540);
    painter.setBrush(Qt::red);
    for(int i=0;i<7;i++)
    {

        painter.drawEllipse(q3,r+6,r+6);
        q3 = tinhtien(q3,10*r,-7);
    }
    QPointF q4(q.x()-190,q.y()+645);
    painter.drawEllipse(q4,r+6,r+6);
    for(int i=0;i<8;i++)
    {
        painter.drawEllipse(q4,r+6,r+6);
        q4 = tinhtien(q4,12*r,7);
    }
}

void graphics::snowflake(QPainter &painter, QPointF p, int L)
{
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    QPoint c(p.x(), p.y());
    int angle = -60;
    QPolygon polygon;
    polygon << QPoint(c.x(), c.y() - L/40/sqrt(2)/sin(30*3.14/180));
    for(int i=0; i<6; i++)
    {
        // Phần trên
        QPointF p(polygon.value(i).x(), polygon.value(i).y());
        QPointF BD = p;
        p = dichuyen(p, L*4/10, 0+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*3/10, -60+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L/40, 30+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*3/10 - L/40/tan(60*3.14/180), 120+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10, 0+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10, -60+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L/40, 30+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10 - L/40/tan(60*3.14/180), 120+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10, 0+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L/40, 45+angle);
        painter.drawLine(BD, p);

        // Phần dưới
        BD = p;
        p = dichuyen(p, L/40, 135+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10, 180+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10 - L/40/tan(60*3.14/180), 60+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L/40, 150+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10, 240+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*2/10, 180+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*3/10 - L/40/tan(60*3.14/180), 60+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L/40, 150+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*3/10, 240+angle);
        painter.drawLine(BD, p);

        BD = p;
        p = dichuyen(p, L*4/10, 180+angle);
        painter.drawLine(BD, p);

        // Quay điểm thứ i trong polygon
        angle += 60;
        polygon << QPoint(p.x(), p.y());
    }
}

void graphics::gift(QPainter &painter, QPointF p1)
{
    painter.setPen(Qt::NoPen);
    //hop qua 1
    int r=150;
    painter.setBrush(QBrush("#B80000"));
    painter.drawRect(p1.x()+10,p1.y()+200,r,r-30);
    painter.setBrush(QBrush("#FFE4E1"));
    painter.drawRect(p1.x(),p1.y()+185,r+20,r/5);
    //
    painter.setBrush(QBrush("#00BFFF"));
    painter.drawRect(p1.x()+35,p1.y()+100,r-50,r-65);
    painter.setBrush(QBrush("#0000CD"));
    painter.drawRect(p1.x()+20,p1.y()+90,r-20,20);
    //
    painter.setBrush(QBrush("#FF00FF"));
    painter.drawRect(p1.x()+45,p1.y()+40,r-70,r-100);
    painter.setBrush(QBrush("#8B008B"));
    painter.drawRect(p1.x()+35,p1.y()+25,r-50,r/10);

    // hop qua 2
    painter.setBrush(QBrush("#FFD700"));
    painter.drawRect(p1.x()+170,p1.y()+150,r+80,r/5);
    painter.setBrush(QBrush("#FFFF00"));
    painter.drawRect(p1.x()+185,p1.y()+180,r+50,r-30);
    //no hop qua 2
    painter.setBrush(Qt::red);
    static const QPointF point[4]={
        QPointF(p1.x()+285,p1.y()+150),
        QPointF(p1.x()+285,p1.y()+120),
        QPointF(p1.x()+245,p1.y()+100),
        QPointF(p1.x()+225,p1.y()+120),
    };
    painter.drawPolygon(point, 4);
    static const QPointF point1[4]={
        QPointF(p1.x()+285,p1.y()+150),
        QPointF(p1.x()+285,p1.y()+120),
        QPointF(p1.x()+325,p1.y()+100),
        QPointF(p1.x()+345,p1.y()+120),
    };
    painter.drawPolygon(point1, 4);

    painter.drawRect(p1.x()+270,p1.y()+180,r/5,r-30);
    //
    //hop qua 3
    painter.setBrush(QBrush("#3CB371"));
    painter.drawRect(p1.x()+340,p1.y()+220,r-30,r-50);
    painter.setBrush(QBrush("#C0C0C0"));
    painter.drawRect(p1.x()+390,p1.y()+220,r-130,r-50);
    painter.drawRect(p1.x()+340,p1.y()+260,r-30,r-130);
}
