#include "missile.h"
#include "ui_missile.h"

missile::missile(QWidget *parent, QString p)
    : QWidget(parent)
    , ui(new Ui::missile)
{
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer,SIGNAL (timeout()),this, SLOT(OnTimer()));
    planet=p;
    sky=QImage(planet+"_sky_2");
    soil=QImage(planet+"_soil");
    aim=QImage("aim");
    x0=y0=size/2;
    xStart=1920/4;
    h=this->height();
    w=this->width();
    updt();
    connect(this,SIGNAL(st(int)),parent,SLOT(setCurrentIndex(int)));
    connect(this,SIGNAL(sendScore(double)),parent->parent(),SLOT(getScore(double)));
    this->setMinimumSize(1800,1000);
    connect(this,SIGNAL(amount(int)),parent->parent(),SLOT(amount(int)));
    r=new Restart(this);
    r->setStyleSheet("QWidget#Restart {border: 3px solid rgba(0,0,0,255);"
                        "border-radius: 8px;}");
    r->hide();
    connect(this,SIGNAL(sendInfo(double,double,double,double,double,double,double,double)),
            r,SLOT(getInfo(double,double,double,double,double,double,double,double)));
    emit amount(1);
}

void missile::paintEvent(QPaintEvent *)
{
    update();
    QPainter paint(this);
    h=this->height();
    w=this->width();
    xcord=w/4;
    ycord=h*0.6;
    sky=sky.scaled(w,ycord+size+1);
    paint.drawImage(0,0,sky);
    soil=soil.scaled(w,h-(ycord+size));
    paint.drawImage(0,ycord+size,soil);
        changeStart();
    if(!ready){
        QFont font("Verdana",20,QFont::Bold);
        paint.setFont(font);
        paint.setPen(Qt::black);
        paint.drawText(this->width()/4+3,100+3,width()/2,50,Qt::AlignHCenter,"Press A to move the cannon to the left");
        paint.drawText(this->width()/4+3,210+3,width()/2,50,Qt::AlignHCenter,"Press D to move the cannon to the right");
        paint.drawText(this->width()/4+3,320+3,width()/2,50,Qt::AlignHCenter,"Press Enter or drag your mouse to set the position");
        paint.setPen(QColor(228,27,27));
        paint.drawText(this->width()/4,100,width()/2,50,Qt::AlignHCenter,"Press A to move the cannon to the left");
        paint.drawText(this->width()/4,210,width()/2,50,Qt::AlignHCenter,"Press D to move the cannon to the right");
        paint.drawText(this->width()/4,320,width()/2,50,Qt::AlignHCenter,"Press Enter or drag your mouse to set the position");
    }
    paint.translate(xcord,ycord*1.1);

    if(ready){
        drawAim(paint);
        draw(paint);
    }
    drawMissile(paint);

}

void missile::draw(QPainter &paint)
{
    if(/*num==0&&*/showt){
        pen1.setWidth(3);
        paint.setPen(pen1);
        for (int k=0;k<traj+1;k++){
            int i=v0*cos(ang)*k+size/2.0;
            int j=-v0*sin(ang)*k+g*(pow(k,2))/2.0+size/2.0;
            if(j<size)
                paint.drawEllipse(i,j,3,3);
        }

    }
    paint.setPen(Qt::NoPen);
    if(1){
        paint.setBrush(QColor(0,0,0,128));
        int mvm=(y+size/2)/tan(M_PI/36*15)-size/(2*sin(M_PI/36*15))+size/2;
        paint.drawEllipse(x+size/6-mvm,size,size/sin(M_PI/36*15),size/6);
    }
    if(!dir){
        paint.setBrush(QBrush(QColor(102,52,8),Qt::Dense3Pattern));
        for(int i=0;i<abs(xStop-xFall)-size*2/3;i+=size/3){
            paint.drawEllipse(xFall+i+size*2/3,size,size*2/3,size/6);
        }
        paint.drawEllipse(xStop-size/6,size,size*2/3,size/6);
    }
    QPoint p(x+size/2-size/18,y+size/2);
    QRadialGradient gr(p,size/2);
    gr.setColorAt(1,Qt::black);
    gr.setColorAt(0.6,QColor(91,91,91));
    gr.setColorAt(0.3,QColor(108,108,108));
    gr.setColorAt(0,QColor(206,206,206));
    paint.setBrush(gr);
    paint.drawEllipse(x,y+size/6,size,size);
}

missile::~missile()
{
    emit amount(-1);
    delete ui;
}

void missile::mouseReleaseEvent(QMouseEvent *pe)
{
    if (ready){
        num++;
        if(pe->button()==1&&num==1&&!(w*0.2<pe->x()&&pe->x()<w*0.24&&h*5/36<pe->y()&&pe->y()<h*7/36)){
            x=y=0;
            t=0;
            x0=pe->x();
            y0=pe->y();
            double r=pow(pow(x0-xcord-size/2.0,2)+pow(y0-ycord-size/2.0,2),0.5);
            vel=pow(800*r*2/mass,0.5);
            v0=vel;
            ang=acos((xcord+size/2.0-x0)/r);
            double tp=2*vel*sin(ang)/g;
            if(ang<M_PI/2&&y0>ycord+size/6&&vel*cos(ang)*tp>size*1.75)
                timer->start();
            else{
                num--;
                x0=size/2;
                y0=size/2;
                traj=0;

            }
        }
    }
}

void missile::mouseMoveEvent(QMouseEvent *pe)
{
    if(num<1&&ready){
        x0=pe->x();
        y0=pe->y();
        double r=pow(pow(x0-xcord-size/2.0,2)+pow(y0-ycord-size/2.0,2),0.5);
        vel=pow(800*r*2/mass,0.5);
        v0=vel;
        ang=acos((xcord+size/2.0-x0)/r);
        traj=2*vel*sin(ang)/g;
        if(ang<M_PI/2&&y0>ycord+size/6&&vel*cos(ang)*traj>size*1.75)
            pen1.setColor(Qt::white);
        else
            pen1.setColor(Qt::red);
        x0=pe->x()-xcord;
        y0=pe->y()-ycord-size-size/3;

    }
    ready=true;
}

void missile::mousePressEvent(QMouseEvent *pe)
{
    int xs=pe->x();
    int ys=pe->y();
    if(dir&&w*0.2<xs&&xs<w*0.24&&h*5/36<ys&&ys<h*7/36&&planet=="earth"){
        if(state=="day"){
            sky=QImage("night_sky");
            state="night";
        }
        else{
            sky=QImage("earth_sky_2");
            state="day";
        }
        if(ready)
            num--;
        update();
    }

}

void missile::resizeEvent(QResizeEvent *re)
{
    if(planet=="earth"&&state=="night")
        sky=QImage("night_sky");
    else
        sky=QImage(planet+"_sky_2");
    soil=QImage(planet+"_soil");
    h=re->size().height();
    w=re->size().width();
    if(!dir){
        r->setMaximumSize(w/3,h/2);
        r->setMinimumSize(w/3,h/2);
        r->move(w/3,h/4);
    }
    update();
}

void missile::keyPressEvent(QKeyEvent *ke)
{
    int k=ke->key();
    if((ke->key()==Qt::Key_D||k==1042)&&!ready&&xStart<w/2-size*1.5)
        xStart+=10;
    else if((ke->key()==Qt::Key_A||k==1060)&&!ready&&xStart>=size*2.5)
        xStart-=10;
    else if(ke->key()==Qt::Key_Return)
        ready=true;
}

void missile::OnTimer()
{
    double dt=timer->interval()/100.0;
    t+=dt;
    if(abs(vel_x)<1.7&&!dir){
        vel=0;
        timer->stop();
        res=this->grab(QRect(0,0,width(),height()));
        double temp=abs(xFall-aimCord-size*2.5-size*0.25)/(aimCord+size*7/4+size*1.5);
        if(temp>1)
            score=0;
        else
            score=10-10*temp;
        if(score<7){
            double a=(aimCord+/*size*5/4*/size*3)*g/pow(v0,2);
            if(a>1)
                idlAng=M_PI/4;
            else
                idlAng=M_PI/2-asin(a)/2.0;
            idlVel=sqrt((aimCord+/*size*5/4*/size*3)*g/sin(2*ang));
        }
//        r=new Restart(this);
//        r->setStyleSheet("QWidget#Restart {border: 3px solid rgba(0,0,0,255);"
//                            "border-radius: 8px;}");

        r->setMaximumSize(this->width()/3,this->height()/2);
        r->setMinimumSize(this->width()/3,this->height()/2);
        r->move(w/3,h/4);
//        connect(this,SIGNAL(sendInfo(double,double,double,double,double,double,double,double)),
//                r,SLOT(getInfo(double,double,double,double,double,double,double,double)));
        emit sendInfo(score,v0,ang,idlVel,idlAng,aimCord+size*3,xFall+size/2,g);
//        disconnect(this,SIGNAL(sendInfo(double,double,double,double,double,double,double,double)),
//                   r,SLOT(getInfo(double,double,double,double,double,double,double,double)));
        r->show();
    }
    if(y<0.1){
        y=-vel*sin(ang)*t+g*(pow(t,2))/2.0;
        vel_x=vel*cos(ang);
        x+=vel_x*dt;
        xFall=x;
        t0=t;
    }
    else{
        dir=false;
        t=0;
        y=size*1/6*0+0.1;
        vel_x=vel_x -abs(cos(ang))/cos(ang)*0.55*g*dt;
        x+=vel_x*dt;
        xStop=x;
    }
    update();
}

void missile::setP(int s, int m, double g, QString p,QString u, bool sh)
{

    size=s;
    mass=m;
    this->g=g;
    planet=p;
    showt=sh;
    username=u;
    this->update();
    emit st(1);
}

void missile::restart()
{
    setFocus();
    xStart=1920/4;
    ready=false;
    x=0;
    y=0;
    vel=0;
    ang=0;
    num=0;
    idlVel=-1;
    idlAng=-1;
    x0=size/2;
    y0=size/2;
    updt();
    xFall=0;
    xStop=0;
    dir=true;
}

void missile::save(QPixmap& prtscr)
{
    QString fileName1 = QFileDialog::getSaveFileName(this,
    tr("Сохранить файл"), "",
    tr("PDF (*.pdf)"));
    res.save("res.jpg");
    prtscr.save("prtscr.jpg");
    res.save(fileName1);
    if (fileName1 != ""){
          QString html ="<h1 align=left>""Planet: "+planet+"</h1>"
                  "<h2 align=center>"+g+"</h2>"
                  "<h3 align=left>""User: "+ username +"<br>""Mass: "+ QString::number(mass) +"kg"+"<br>""Size: "+ QString::number(size)+"cm"+"</h3>"
                  "<h2 align=left>""Results:""</h2>"
                  "<h3 align=left>"+QString::number(score)+"</h3>"
                  "<h2 align=left>""Trajectory:""</h2>"
                  "<br>"
                  "<img src=\"res.jpg\" width=\"600\">";
                  if (score<7){
                      html+="<h2 align=left>""Ideal trajectories:""</h2>"
                      "<br>"
                      "<img src=\"prtscr.jpg\" width=\"600\">";
                  }
          QTextDocument document;
          document.setHtml(html);
          QPrinter printer(QPrinter::PrinterResolution);
          printer.setOutputFormat(QPrinter::PdfFormat);
          printer.setPageSize(QPrinter::A4);
          printer.setOutputFileName(fileName1);
          printer.setPageMargins(QMarginsF(15, 15, 15, 15));
          document.print(&printer);
    }
}

void missile::updt()
{
    const unsigned seed = unsigned(std::time(nullptr));
    std::mt19937 engine;
    engine.seed(seed);
    int max;
    if(this->width()<1800)
        max=1800;
    else
        max=this->width();
    double mult=abs(xStart-max)/max;
    std::uniform_int_distribution<unsigned> distribution(0,1600*mult-size*1.5);
    aimCord=distribution(engine);
}

void missile::drawAim(QPainter &paint)
{
    aim=aim.scaled(size*1.5,size/3);
    paint.setBrush(Qt::NoBrush);
    paint.drawImage(size*2.5+aimCord,size-size/12,aim);
}

void missile::drawMissile(QPainter &paint)
{
    paint.setBrush(QBrush(QColor(0,0,0,128)));
    paint.drawChord(QRect(-size,size*5/6,size*3.1,size*2/3),180*16,200*16);
    if(!ready)
        paint.setPen(QColor(Qt::red));
    QPoint p(-size*2/3,-size/3);
    QRadialGradient gr(p,size*2);
    gr.setColorAt(1,QColor(0,0,0/*,128*/));
    gr.setColorAt(0.6,QColor(91,91,91/*,128*/));
    gr.setColorAt(0.3,QColor(108,108,108/*,128*/));
    gr.setColorAt(0,QColor(206,206,206/*,128*/));
    paint.setBrush(gr);
    paint.drawEllipse(-size,size*11/12,size*3,size/2);
    paint.drawPie(QRect(-size,-size/3/*-size*1.5*/,size*3,size*3),0,180*16);
    paint.setBrush(QBrush(QColor(50,50,50)));
    paint.drawEllipse(0,size/6,size,size);
    paint.translate(size/2.0,size*2/3.0);
    paint.rotate(-ang*180/M_PI);
    paint.drawRect(0,-size/2.0,size*1.5,size);
    paint.rotate(ang*180/M_PI);
    paint.translate(-size/2.0,-size*2/3.0);

}

void missile::changeStart()
{
    xcord=xStart;
    if(!ready)
        updt();
}

