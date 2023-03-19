#include "app.h"
#include "ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    mWidget=new QStackedWidget(this);
    missile1= new missile(mWidget);
    menu1= new menu(mWidget);
    mWidget->addWidget(menu1);
    mWidget->addWidget(missile1);
    mWidget->setCurrentWidget(menu1);
    this->setCentralWidget(mWidget);
    menuBar1=new QMenuBar();
    pmnu = new QMenu("Menu");
    pmnu->addAction("&Menu", this, SLOT(goMenu()));
    menuActive=true;
    pmnu->addAction("&Quit",this, SLOT(close()));
    menuBar1->addMenu(pmnu);
    this->setMenuBar(menuBar1);
    menuBar1->setVisible(false);
    connect(this,SIGNAL(transferForvard(int,int,double,QString,QString,bool)),missile1,SLOT(setP(int,int,double,QString,QString,bool)));
    connect(this,SIGNAL(sendScore(double)),menu1,SLOT(getScore(double)));
    this->setStyleSheet("QMenuBar {"
                        "font: bold 16px;}");
}

App::~App()
{
    delete ui;
//    delete mWidget;
}

void App::amount(int h)
{
    if (h==1)
        n++;
    if (h==-1)
        n--;
}

void App::transfer(int s, int m, double g, QString p, QString u, bool sh)
{
    username=u;
    mWidget->removeWidget(missile1);
    delete missile1;
    missile1=new missile(mWidget,p);
    mWidget->addWidget(missile1);
    menuBar1->setVisible(true);
    connect(this,SIGNAL(transferForvard(int,int,double,QString,QString,bool)),missile1,SLOT(setP(int,int,double,QString,QString,bool)));
    emit transferForvard(s,m,g,p,username,sh);
}

void App::getScore(double sc)
{
    sendScore(sc);
    if(menuActive){
        menuBar1->setVisible(false);
    }
    update();
}

void App::goMenu()
{
    mWidget->setCurrentIndex(0);
    if(menuActive){
        menuBar1->setVisible(false);
    }
    update();
}


