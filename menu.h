#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QObject>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class menu; }
QT_END_NAMESPACE

class menu : public QWidget
{
    Q_OBJECT

public:
    menu(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    ~menu();

private:
    QImage bg;
    QLineEdit* usernm;
    QRadioButton *earth,*moon,*mars;
    double g=9.8;
    int s=60,m=60,amount=0;
    QString planet="earth",state="day",username="";
    bool showt=true, open=false;
    QTableWidget* leaderboard;
    Ui::menu *ui;
signals:
    void st(int);
    void transfer(int,int,double,QString,QString,bool);
public slots:
    void massChanged(int mass){m=mass;};
    void sizeChanged(int size);;
    void pressEarth(bool);
    void pressMoon(bool);
    void pressMars(bool);
    void showtr(bool);
    void start();
    void getScore(double);
};
#endif // MENU_H
