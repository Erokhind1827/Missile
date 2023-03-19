#include "restart.h"

Restart::Restart(QWidget *parent)
    : QWidget(parent)
{
    bg=QImage("menubg2");
    restart=new QPushButton("Restart");
    menu=new QPushButton("Menu");
    save=new QPushButton("Save");
    QFont fontbtns("Helvetica");
    fontbtns.setBold(true);
    restart->setFont(fontbtns);
    menu->setFont(fontbtns);
    restart->setMinimumHeight(50);
    save->setFont(fontbtns);
    save->setMinimumHeight(50);
    menu->setMinimumHeight(50);
    QHBoxLayout* btns=new QHBoxLayout();
    btns->addWidget(restart);
    btns->addWidget(menu);
    btns->addWidget(save);
    connect(restart,SIGNAL(clicked()),this,SLOT(goRestart()));
    connect(menu,SIGNAL(clicked()),this,SLOT(goMenu()));
    connect(save,SIGNAL(clicked()),this,SLOT(goSave()));
    connect(this,SIGNAL(sendPrtSc(QPixmap&)),parent,SLOT(save(QPixmap&)));
    QLabel* ysi=new QLabel("Your score is:");
    ysi->setMaximumHeight(parent->height()/12);
    scr=new QLabel();
    scr->setMaximumHeight(parent->height()/12);
    QFont font("Helvetica",12,QFont::DemiBold);
    ysi->setFont(font);
    scr->setFont(font);
    QHBoxLayout* traj=new QHBoxLayout();
    QVBoxLayout* vl=new QVBoxLayout[2];
    QLabel* tr=new QLabel[4];
    QFont font1("Helvetica",9,QFont::DemiBold);
    for(int i=0;i<4;i++)
        tr[i].setFont(font1);
    tr[0].setText("Your trajectory:");
    tr[1].setText("Ideal velocity trajectory:");
    tr[2].setText("Your trajectory:");
    tr[3].setText("Ideal angle trajectory:");
    vl[0].addWidget(&tr[0]);
    vl[0].addWidget(&tr[1]);
    vl[1].addWidget(&tr[2]);
    vl[1].addWidget(&tr[3]);
    traj->addLayout(&vl[0]);
    traj->addSpacing(this->width()*0.47);
    traj->addLayout(&vl[1]);
    QVBoxLayout* layout=new QVBoxLayout();
    layout->addWidget(ysi,Qt::AlignHCenter);
    layout->addWidget(scr,Qt::AlignHCenter);
    layout->addSpacing(parent->height()*0.02);
    layout->addLayout(traj);
    layout->addSpacing(parent->height()*0.02);
    layout->addSpacing(parent->height()*0.50);
    layout->addLayout(btns);
    connect(this,SIGNAL(setActive(int)),parent->parent(),SLOT(setCurrentIndex(int)));
    this->setStyleSheet("QPushButton { background-color : #E3FDFD;"
                                      "border: 2px solid rgba(0,0,0,255);"
                                      "border-radius: 8px;"
                                      "font-size: 16px;}"
                        "QPushButton:hover {background-color : #71C9CE;"
                                           "color : rgb(255,255,255);}"
                        "QLabel {color: #364F6B;}"
                        );
    connect(restart,SIGNAL(clicked()),parent,SLOT(restart()));
    connect(menu,SIGNAL(clicked()),parent,SLOT(send()));
    setLayout(layout);
}

void Restart::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    bg=bg.scaled(this->width(),this->height());
    paint.drawImage(0,0,bg);
    QPen pen1;
    pen1.setWidth(3);
    paint.setPen(pen1);
    paint.drawLine(0,75,this->width(),75);
    paint.drawLine(0,150,this->width(),150);
    paint.drawLine(0,this->height()-75,this->width(),this->height()-75);
    paint.drawLine(this->width()/2,75,this->width()/2,this->height()-75);
    pen1.setWidth(1);
    paint.setPen(pen1);
    paint.setBrush(QColor(Qt::red));
    paint.drawEllipse(this->width()*0.23,100,5,5);
    paint.drawEllipse(this->width()*0.23+10,100,5,5);
    paint.drawEllipse(this->width()*0.23+20,100,5,5);
    paint.drawEllipse(this->width()*0.75,100,5,5);
    paint.drawEllipse(this->width()*0.75+10,100,5,5);
    paint.drawEllipse(this->width()*0.75+20,100,5,5);
    paint.setBrush(QColor(Qt::green));
    paint.drawEllipse(this->width()*0.33,126,5,5);
    paint.drawEllipse(this->width()*0.33+10,126,5,5);
    paint.drawEllipse(this->width()*0.33+20,126,5,5);
    paint.drawEllipse(this->width()*0.82,126,5,5);
    paint.drawEllipse(this->width()*0.82+10,126,5,5);
    paint.drawEllipse(this->width()*0.82+20,126,5,5);
    if(score<7){
        int t=2*v0*sin(ang)/g;
        int idlt=2*idlVel*sin(ang)/g;
        int k=std::max(t,idlt);
        double mult=(this->width()/2*0.7+10)/idlLen;
        paint.translate(10,this->height()-75);
        paint.setBrush(QColor(Qt::black));
        paint.drawEllipse(-10,-3,20,6);
        paint.setBrush(QColor(Qt::red));
        paint.drawEllipse(idlLen*mult-10,-3,20,6);
        paint.setBrush(QColor(Qt::white));
        paint.drawEllipse(idlLen*mult-5,-1,10,3);
        for(double j=0;j<k+1;j+=0.2){
            double x1=v0*cos(ang)*j*mult;
            double y1=(-v0*sin(ang)*j+g*(pow(j,2))/2.0)*mult;
            if(y1<-0.1&&y1>-(this->height()-225)&&x1<this->width()/2-10){
                paint.setBrush(QColor(Qt::red));
                paint.drawEllipse(x1,y1,3,3);
            }
            double x2=idlVel*cos(ang)*j*mult;
            double y2=(-idlVel*sin(ang)*j+g*(pow(j,2))/2.0)*mult;
            if(y2<-0.1&&y2>-(this->height()-225)&&x2<this->width()/2){
                paint.setBrush(QColor(Qt::green));
                paint.drawEllipse(x2,y2,3,3);
            }
        }
        paint.translate(this->width()/2,0);
        idlt=2*v0*sin(idlAng)/g;
        k=std::max(t,idlt);
        paint.setBrush(QColor(Qt::black));
        paint.drawEllipse(-10,-3,20,6);
        paint.setBrush(QColor(Qt::red));
        paint.drawEllipse(idlLen*mult-10,-3,20,6);
        paint.setBrush(QColor(Qt::white));
        paint.drawEllipse(idlLen*mult-5,-1,10,3);
        for(double j=0;j<k+1;j+=0.2){
            double x1=v0*cos(ang)*j*mult;
            double y1=(-v0*sin(ang)*j+g*(pow(j,2))/2.0)*mult;
            if(y1<-0.1&&y1>-(this->height()-225)&&x1<this->width()/2-10){
                paint.setBrush(QColor(Qt::red));
                paint.drawEllipse(x1,y1,3,3);
            }
            double x2=v0*cos(idlAng)*j*mult;
            double y2=(-v0*sin(idlAng)*j+g*(pow(j,2))/2.0)*mult;
            if(y2<-0.1&&y2>-(this->height()-225)&&x2<this->width()/2){
                paint.setBrush(QColor(Qt::green));
                paint.drawEllipse(x2,y2,3,3);
            }
        }
    }
    else{
        paint.drawImage(width()*0.055,162,platon);
        paint.drawImage(width()/2+width()*0.055,162,rock);
    }
}

void Restart::resizeEvent(QResizeEvent *re)
{
    bg=bg.scaled(re->size().width(),re->size().height());
    if(score>=7){
        platon=platon.scaled(re->size().width()*0.39,re->size().width()*0.39);
        rock=rock.scaled(re->size().width()*0.39,re->size().width()*0.39);
    }
    update();
}

Restart::~Restart()
{
    int n=0;
    n++;
}

void Restart::goMenu()
{
    setActive(0);
    this->close();
}

void Restart::goRestart()
{
    this->close();
}

void Restart::goSave()
{
    pix=new QPixmap();
    *pix=this->grab(QRect(0,75,width(),height()-145));
    emit sendPrtSc(*pix);
}

void Restart::getInfo(double s, double v, double a, double idV, double idA, double idlL, double l, double gfall)
{
    bg=QImage("menubg2");
    bg=bg.scaled(width(),height());
    score=s;
    if(s>=7&&!opened){
        platon=QImage("platon1.jpg");
        rock=QImage("rock.jpg");
        platon=platon.scaled(width()*0.39,width()*0.39);
        rock=rock.scaled(width()*0.39,width()*0.39);
    }
    v0=v;
    ang=a;
    idlVel=idV;
    idlAng=idA;
    idlLen=idlL;
    len=l;
    g=gfall;
    scr->setText(QString::number(score));
    update();
}

