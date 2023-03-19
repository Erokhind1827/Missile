#ifndef RESTART_H
#define RESTART_H

#include <QWidget>
#include <QObject>
#include <QtWidgets>
#include <cmath>

class Restart : public QWidget
{
    Q_OBJECT

public:
    Restart(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    ~Restart();
private:
    QImage bg,platon,rock;
    double score,len,idlLen,g;
    double v0,idlVel;
    double ang,idlAng;
    bool opened=false;
    double space;
    QPushButton *restart, *menu, *save;
    QLabel* scr;
    QLabel *pl, *ri;
    QPixmap* pix;
signals:
    void setActive(int);
    void sendPrtSc(QPixmap&);
private slots:
    void goMenu();
    void goRestart();
    void goSave();
public slots:
    void getInfo(double,double,double,double,double,double,double,double);
};
#endif // RESTART_H
