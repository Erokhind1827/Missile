#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <menu.h>
#include <missile.h>
#include <restart.h>

QT_BEGIN_NAMESPACE
namespace Ui { class App; }
QT_END_NAMESPACE

class App : public QMainWindow
{
    Q_OBJECT

public:
    App(QWidget *parent = nullptr);
    ~App();
    int n=0;
public slots:
    void amount(int);
    void transfer(int,int,double,QString,QString,bool);
    void getScore(double);
    void goMenu();
signals:
    void restart();
    void transferForvard(int,int,double,QString,QString,bool);
    void sendResults(double,double,double,double,double);
    void sendScore(double);
private:
    menu* menu1;
    missile* missile1;
    QStackedWidget* mWidget;
    QMenuBar *menuBar1;
    QMenu* pmnu;
    QString username;
    double score;
    bool menuActive=false;
    Ui::App *ui;
};
#endif // APP_H
