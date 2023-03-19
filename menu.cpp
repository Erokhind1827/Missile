#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menu)
{
    connect(this,SIGNAL(st(int)),parent,SLOT(setCurrentIndex(int)));
    QHBoxLayout* layout=new QHBoxLayout;
    bg=QImage("menubg2");
    layout->addSpacing(200);
    QVBoxLayout* columns=new QVBoxLayout[3];
    QGroupBox* gbx= new QGroupBox("Planet");
    earth = new QRadioButton("Earth");
    moon = new QRadioButton("Moon");
    mars = new QRadioButton("Mars");
    connect(earth, SIGNAL(toggled(bool)),this,SLOT(pressEarth(bool)));
    connect(moon, SIGNAL(toggled(bool)),this,SLOT(pressMoon(bool)));
    connect(mars, SIGNAL(toggled(bool)),this,SLOT(pressMars(bool)));
    earth->setChecked(true);
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(earth);
    pvbxLayout->addWidget(moon);
    pvbxLayout->addWidget(mars);
    gbx->setLayout(pvbxLayout);
    QSlider* mass = new QSlider(Qt::Horizontal);
    QLabel* massValue = new QLabel("60");
    mass->setRange(20, 100);
    mass->setSingleStep(10);
    mass->setValue(60);
    mass->setTickInterval(20);
    mass->setTickPosition(QSlider::TicksBelow);
    connect(mass, SIGNAL(valueChanged(int)),massValue, SLOT(setNum(int)));
    connect(mass, SIGNAL(valueChanged(int)),this, SLOT(massChanged(int)));
    QHBoxLayout* slider=new QHBoxLayout();
    QLabel* lm=new QLabel("Mass");
    lm->setObjectName("l");
    slider->addWidget(mass);
    slider->addWidget(massValue);
    columns[0].addSpacing(200);
    columns[0].addWidget(lm,1,Qt::Alignment(Qt::AlignHCenter));
    columns[0].addLayout(slider,1);
    QSlider* size = new QSlider(Qt::Horizontal);
    QLabel* sizeValue = new QLabel("60");
    size->setRange(40, 80);
    size->setSingleStep(5);
    size->setValue(60);
    size->setTickInterval(10);
    size->setTickPosition(QSlider::TicksBelow);
    connect(size, SIGNAL(valueChanged(int)),sizeValue, SLOT(setNum(int)));
    connect(size, SIGNAL(valueChanged(int)),this, SLOT(sizeChanged(int)));
    QHBoxLayout* slider1=new QHBoxLayout();
    QCheckBox* traj=new QCheckBox("Show trajectory");
    traj->setChecked(true);
    connect(traj,SIGNAL(toggled(bool)),this,SLOT(showtr(bool)));
    QLabel* ls=new QLabel("Size");
    ls->setObjectName("l");
    slider1->addWidget(size);
    slider1->addWidget(sizeValue);
    columns[0].addWidget(ls,1,Qt::Alignment(Qt::AlignHCenter));
    columns[0].addLayout(slider1,1);
    columns[0].addSpacing(15);
    columns[0].addWidget(traj,1);
    columns[0].addSpacing(15);
    columns[0].addWidget(gbx,6);
    columns[0].addSpacing(200);
    layout->addLayout(&columns[0],1);
    layout->addSpacing(this->width()/12);
    QLabel* grandLabel=new QLabel("Cannon simulator");
    QFont font("Verdana",20,QFont::Bold);
    grandLabel->setFont(font);
    grandLabel->setMaximumHeight(300);
    grandLabel->setObjectName("gL");
    QPushButton* start=new QPushButton("Start");
    start->setObjectName("b");
    start->setMaximumWidth(300);
    start->setMinimumWidth(300);
    start->setMinimumHeight(100);
    QPushButton* exit=new QPushButton("Close");
    exit->setObjectName("b");
    exit->setMaximumWidth(300);
    exit->setMinimumWidth(300);
    exit->setMinimumHeight(100);
    QFont fontbtns("Verdana");
    fontbtns.setBold(true);
    start->setFont(fontbtns);
    exit->setFont(fontbtns);
    massValue->setFont(fontbtns);
    sizeValue->setFont(fontbtns);
    traj->setFont(fontbtns);
    massValue->setObjectName("l");
    sizeValue->setObjectName("l");
    traj->setObjectName("l");
    gbx->setFont(fontbtns);
    connect(exit, SIGNAL(clicked()),parent->parent(), SLOT(close()));
    connect(start,SIGNAL(clicked()),this,SLOT(start()));   
    connect(this,SIGNAL(transfer(int,int,double,QString,QString,bool)),parent->parent(),SLOT(transfer(int,int,double,QString,QString,bool)));
    columns[1].addSpacing(100);
    columns[1].addWidget(grandLabel,3,Qt::Alignment(Qt::AlignHCenter));
    columns[1].addWidget(start,3,Qt::Alignment(Qt::AlignVCenter|Qt::AlignHCenter));
    columns[1].addWidget(exit,3,Qt::Alignment(Qt::AlignTop|Qt::AlignHCenter));
    columns[1].addSpacing(100);
    layout->addLayout(&columns[1],1);
    layout->addSpacing(this->width()/12);
    QLabel* user=new QLabel("Username");
    usernm=new QLineEdit;
    user->setObjectName("l");
    QLabel* records=new QLabel("Leaderboard");
    records->setObjectName("l");
    leaderboard=new QTableWidget(5,3);
    for(int i=0;i<5;i++){
        QTableWidgetItem* el=new QTableWidgetItem("");
        el->setFlags(el->flags() ^ Qt::ItemIsEditable);
        leaderboard->setItem(i,0,el);
        QTableWidgetItem* pl=new QTableWidgetItem("");
        pl->setFlags(pl->flags() ^ Qt::ItemIsEditable);
        leaderboard->setItem(i,1,pl);
        QTableWidgetItem* el1=new QTableWidgetItem("");
        el1->setFlags(el1->flags() ^ Qt::ItemIsEditable);
        leaderboard->setItem(i,2,el1);
    }
    leaderboard->horizontalHeader()->setVisible(false);
    leaderboard->verticalHeader()->setVisible(false);
    leaderboard->setMinimumHeight(190);
    leaderboard->setMaximumHeight(190);
    leaderboard->setMinimumWidth(380);
    leaderboard->setMaximumWidth(380);
    columns[2].addSpacing(200);
    columns[2].addWidget(user,1,Qt::Alignment(Qt::AlignCenter));
    columns[2].addWidget(usernm,1,Qt::Alignment(Qt::AlignVCenter));
    columns[2].addWidget(records,1,Qt::Alignment(Qt::AlignCenter));
    columns[2].addWidget(leaderboard,6,Qt::Alignment(Qt::AlignJustify));
    columns[2].addSpacing(200);
    layout->addLayout(&columns[2],1);
    layout->addSpacing(200);
    QVBoxLayout* bigLayout=new QVBoxLayout;
    bigLayout->addSpacing(this->height()/3);
    bigLayout->addLayout(layout);
    bigLayout->addSpacing(this->height()/3);
    this->setStyleSheet("QPushButton#b { background-color : #E3FDFD;"
                                      "border: 2px solid #364F6B;"
                                      "border-radius: 8px;"
                                      "font-size: 16px;}"
                        "QPushButton#b:hover {background-color : #71C9CE;"
                                           "color : rgb(255,255,255);}"
                        "QSlider::groove:horizontal {border: 1px solid #364F6B;"
                                                    "height: 8px;" /* по умолчанию бороздка расширяется до размеров ползунка. задав высоту она принимает фиксированный размер */
                                                    "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E3FDFD, stop:1 #71C9CE);"
                                                    "margin: 2px 0;}"
                        "QSlider::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #364F6B, stop:1 #11999E);"
                                                    "border: 1px solid #364F6B;"
                                                    "width: 18px;"
                                                    "margin: -2px 0;" /* рукоятка располагается по умолчанию в прямоугольнике содержимого бороздки. Расширяется наружу от бороздки */
                                                    "border-radius: 3px;}"
                        "QLineEdit {border: 2px solid #364F6B;"
                                   "border-radius: 10px;"
                                   "color: #364F6B;"
                                   "font: bold 16px;"
                                   "padding: 0 8px;"
                                   "background: #E3FDFD;"
                                   "selection-background-color: #364F6B;}"
                        "QTableView {color: #364F6B;"
                                 "background-color: #E3FDFD;" /* background color in the table*/
                                 "font: bold 16px;"
                                 "border: 2px solid #364F6B;"
                                 "border-radius: 10px;}"
                        "QLabel#l {color: #364F6B;"
                                 "font: bold 24px;}"
                        "QLabel#gL {color: #364F6B;}"
                      "QGroupBox {background-color: #E3FDFD;"/*qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FAE3D9, stop: 1 #FFB6B9)*/
                        "border: 2px solid #364F6B;"
                        "border-radius: 5px;"
                       " margin-top: 1ex;" /* оставляем пространство вверху для заголовка */
                    "}"

                    "QGroupBox::title {subcontrol-origin: margin;"
                        "subcontrol-position: top center;" /* помещаем вверху по центру */
                        "padding: 0 3px;"
                        "color: #364F6B;"
                        "font: bold 24px;"
                        "border-radius: 2px;"
                        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E3FDFD, stop: 1 #71C9CE);"
                    "}"
                    "QCheckBox {color: #364F6B;"
                        "font: bold 16px;}"
                    "QRadioButton {"
                        "font: 16px;"
                        "color:                  black;"
                    "}"

                    "QRadioButton::indicator {"
                        "width:                  12px;"
                        "height:                 12px;"
                        "border-radius:          7px;"
                    "}"

                    "QRadioButton::indicator:checked {"
                        "background-color:       #11999E;"
                        "border:                 2px solid black;"
                    "}"

                    "QRadioButton::indicator:unchecked {"
                        "background-color:       #364F6B;"
                        "border:                 2px solid black;"
                    "}"
                        );
    this->setLayout(bigLayout);
    ui->setupUi(this);
}

void menu::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    bg=bg.scaled(this->width(),this->height());
    paint.drawImage(0,0,bg);
}

menu::~menu()
{
    delete ui;
}

void menu::sizeChanged(int size)
{
    s=size;
}


void menu::pressEarth(bool b)
{
    if(b){
        planet="earth";
        g=9.82;
    }
}

void menu::pressMoon(bool b)
{
    if(b){
        planet="Moon";
        g=1.62;
    }
}

void menu::pressMars(bool b)
{
    if(b){
        planet="Mars";
        g=3.71;
    }
}

void menu::showtr(bool b)
{
    if(b)
        showt=true;
    else
        showt=false;
}

void menu::start()
{
    username=usernm->text();
    if(username=="")
        username="NoName";
    emit transfer(s,m,g,planet,username,showt);
}

void menu::getScore(double score)
{
    for(int i=0;i<5;i++){
        QString result=leaderboard->item(i,2)->text();
        if(planet=="earth")
            planet="Earth";
        double s;
        if (result=="")
            s=-1;
        else
            s=result.toDouble();
        if(score>s&&i==4){
            QTableWidgetItem* el=new QTableWidgetItem(username);
            el->setFlags(el->flags() ^ Qt::ItemIsEditable);
            leaderboard->setItem(i,0,el);
            QTableWidgetItem* pl=new QTableWidgetItem(planet);
            pl->setFlags(pl->flags() ^ Qt::ItemIsEditable);
            leaderboard->setItem(i,1,pl);
            QTableWidgetItem* el1=new QTableWidgetItem(QString::number(score));
            el1->setFlags(el1->flags() ^ Qt::ItemIsEditable);
            leaderboard->setItem(i,2,el1);
        }
        else if(score>s){
            for(int j=3;j>=i;j--){
                QTableWidgetItem* el=new QTableWidgetItem(*leaderboard->item(j,0));
                leaderboard->setItem(j+1,0,el);
                QTableWidgetItem* pl=new QTableWidgetItem(*leaderboard->item(j,1));
                leaderboard->setItem(j+1,1,pl);
                QTableWidgetItem* el1=new QTableWidgetItem(*leaderboard->item(j,2));
                leaderboard->setItem(j+1,2,el1);
            }
            QTableWidgetItem* el=new QTableWidgetItem(username);
            el->setFlags(el->flags() ^ Qt::ItemIsEditable);
            leaderboard->setItem(i,0,el);
            QTableWidgetItem* pl=new QTableWidgetItem(planet);
            pl->setFlags(pl->flags() ^ Qt::ItemIsEditable);
            leaderboard->setItem(i,1,pl);
            QTableWidgetItem* el1=new QTableWidgetItem(QString::number(score));
            el1->setFlags(el1->flags() ^ Qt::ItemIsEditable);
            leaderboard->setItem(i,2,el1);
            break;
        }
    }

}

