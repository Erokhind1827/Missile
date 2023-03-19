#ifndef MISSILE_H
#define MISSILE_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QObject>
#include <QTimer>
#include <ctime>
#include <random>
#include <cmath>
#include <QDebug>
#include <restart.h>
#include <QPrinter>
//#include <menu.h>

QT_BEGIN_NAMESPACE
namespace Ui { class missile; }
QT_END_NAMESPACE
//int n{0};
class missile : public QWidget
{
    Q_OBJECT

public:
    missile(QWidget *parent = nullptr,QString p="earth");
    void paintEvent(QPaintEvent*);
    void draw(QPainter & paint);
    ~missile();
protected:
    void mouseReleaseEvent (QMouseEvent* pe);
    void mouseMoveEvent (QMouseEvent* pe);
    void mousePressEvent (QMouseEvent* pe);
    void resizeEvent(QResizeEvent* re);
    void keyPressEvent(QKeyEvent* ke);
private slots:
    void OnTimer();
    void setP(int,int,double,QString,QString,bool);
public slots:
    void restart();
    void send(){emit sendScore(score);};
    void save(QPixmap&);
signals:
    void st(int);
    void sendInfo(double,double,double,double,double,double,double,double);
    void sendScore(double);
    void amount(int);
private:
    double score=0;
    QPen pen1;
    Restart* r;
    QImage sky,soil,aim;
    QPixmap res;
    QBrush brush1;
    void updt();;
    void drawAim(QPainter& paint);
    void drawMissile(QPainter& paint);
    void changeStart();
    double vel,vel_y,vel_x,ang=0,idlVel=-1,idlAng=-1,v0;
    double t=0,g=9.8,h,w,traj,t0;
    double x=0,y=0,xcord=0,ycord=0,aimCord,x0,y0,xFall,xStop,xStart;
    int num=0,size=60,mass=50;
    QString planet="earth",state="day",username;
    bool dir=true,showt=false,ready=false;
    QTimer *timer;
    Ui::missile *ui;
};
#endif // MISSILE_H
