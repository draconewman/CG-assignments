#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#include<chrono>
#include<cmath>
#include<math.h>
#include<QApplication>

#define maxHt 1800
#define maxWd 1000
#define maxVer 10000

using namespace std;
using namespace std::chrono;

int gridsize=1;
std::vector<std::pair<int,int> > vertex_list;

QImage img=QImage(600,600,QImage::Format_RGB888);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    img=QImage(ui->frame->height(),ui->frame->width(),QImage::Format_RGB888);
    ui->x_axis->hide();
    ui->y_axis->hide();
    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::point(int x,int y,int r)
{
    int a,b;
    QImage img=QImage(ui->frame->height(),ui->frame->width(),QImage::Format_RGB888);
    r=0;
    for(a=-r;a<=r;a++){
        for(b=-r;b<=r;b++){
                img.setPixelColor(x+a,y+b,qRgb(255,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::showMousePosition(QPoint &pos)
{
    ui->mouse_movement->setText(" X : "+QString::number(pos.x())+", Y : "+QString::number(pos.y()));
}
void MainWindow::Mouse_Pressed()
{
    QPoint p(ui->frame->x,ui->frame->y);
    p=ui->frame->transformCoordinate(p);
    ui->mouse_pressed->setText(" X : "+QString::number(p.x())+", Y : "+QString::number(p.y()));
}

void MainWindow::on_show_axes_clicked()
{
    int s=(600/2);
    if(ui->show_axes->isChecked())
    {
        int i,j;
        for(i=s;i<s+GridSize;i++)
        {
            for(j=0;j<600;j++)
            {
               img.setPixelColor(i,j,qRgb(255,255,255));
            }
        }
        for(j=s;j<s+GridSize;j++)
        {
            for(i=0;i<600;i++)
            {
               img.setPixelColor(i,j,qRgb(255,255,255));
            }
        }
        ui->frame->setPixmap(QPixmap::fromImage(img));
    }
    else{
    img=QImage(ui->frame->height(),ui->frame->width(),QImage::Format_RGB888);
    on_grid_clicked();
    }
}
void MainWindow::on_set_point1_clicked()
{
    if(ui->draw_line->isChecked()||ui->dda->isChecked()||ui->bresenham->isChecked()
   ||ui->midpoint_circle->isChecked()||ui->Bresenham_circle->isChecked()||ui->Polar_circle->isChecked()||ui->midpoint_ellipse->isChecked()
   ||ui->polar_ellipse->isChecked())
    {
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        p1=ui->frame->transformCoordinate(p1);
        QColor c=QColor(235,250,10);
        fillColor(p1,c);
        p4=p1;
    }
}

void MainWindow::on_set_point2_clicked()
{
    if(ui->draw_line->isChecked()||ui->dda->isChecked()||ui->bresenham->isChecked()){
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
        p2=ui->frame->transformCoordinate(p2);
        QColor c=QColor(235,250,10);
        fillColor(p2,c);
    }
}

void MainWindow::on_Draw_clicked()
{
    //int r0=ui->circle_radius->value();
//    QPixmap *pix=new QPixmap(450,450);
//    QPainter *paint=new QPainter();
//    if(ui->draw_circle->isChecked()){
//        p1.setX(ui->frame->x);
//        p1.setY(ui->frame->y);
//        /*int a,b;
//        QImage img=QImage(450,450,QImage::Format_RGB888);
//        for(a=-r0;a<=r0;a++){
//            for(b=-r0;b<=r0;b++){
//                if((a*a)+(b*b)<=(r0*r0)){
//                    img.setPixelColor(p1.x()+a,p1.y()+b,qRgb(255,255,255));
//                }
//            }
//        }*/
//        paint->begin(pix);
//        paint->setPen(Qt::lightGray);
//        paint->drawEllipse(p1,r0,r0);
//        ui->frame->setPixmap(*pix);
//        paint->end();
        //ui->frame->setPixmap(QPixmap::fromImage(img));

    if(ui->dda->isChecked())
    {
        dda();
    }
    else if(ui->bresenham->isChecked())
    {
        bresenham();
    }
    else if(ui->midpoint_circle->isChecked())
    {
        midpoint_circle();
    }
    else if(ui->Bresenham_circle->isChecked())
    {
        bresenham_circle();
    }
    else if(ui->Polar_circle->isChecked())
    {
        polar_circle();
    }
    else if(ui->midpoint_ellipse->isChecked())
    {
        midpoint_ellipse();
    }
    else if(ui->polar_ellipse->isChecked())
    {
        polar_ellipse();
    }
    else if(ui->Polygon->isChecked())
    {
        drawPolygon();
    }
    else if(ui->Scanline->isChecked())
    {
        scanLine();
    }
    else if(ui->BoundaryFill->isChecked())
    {
        int i,j;for(i=0;i<1000;i++)for(j=0;j<1000;j++)visited[i][j]=false;
        boundaryFill(seed.rx(),seed.ry());
    }
    else if(ui->Translation->isChecked())
    {
        translation();
    }
    else if(ui->Rotation->isChecked())
    {
        rotate();
    }
    else if(ui->Scaling->isChecked())
    {
        scale();
    }
    else if(ui->Reflection_x->isChecked())
    {
        reflectx();
    }
    else if(ui->Reflection_y->isChecked())
    {
        reflecty();
    }



}

void MainWindow::delay() {
    int SLEEP_ms = ui->delay->value();
    QTime dieTime= QTime::currentTime().addMSecs(SLEEP_ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::dda()
{
    auto start = high_resolution_clock::now();
    double x1 = p1.x()/gridsize;
    double y1 = p1.y()/gridsize;
    double x2 = p2.x()/gridsize;
    double y2 = p2.y()/gridsize;
    double xinc, yinc, step;
    double slope = fabs(y2-y1)/fabs(x2-x1);
    if(slope  <= 1.00) {
        xinc = 1;
        yinc = slope;
        step = fabs(x2 - x1);
    } else {
        xinc = 1/slope;
        yinc = 1;
        step = fabs(y2 - y1);
    }
    if(x1 > x2) xinc *= -1;
    if(y1 > y2) yinc *= -1;
    double x = x1*gridsize + gridsize/2;
    double y = y1*gridsize + gridsize/2;
    auto stop = high_resolution_clock::now();
    auto intervals = (stop - start);
    for(int i=0;i<=step;i++){
        delay();
        QPoint q(x,y);
        QColor c=QColor(0,255,0);
        fillColor(q,c);
        start = high_resolution_clock::now();
        x += xinc * gridsize;
        y += yinc * gridsize;
        stop = high_resolution_clock::now();
        intervals += (stop - start);
        auto duration = duration_cast<microseconds>(intervals);
        //cout << "Time taken by DDA: "<< duration.count() << " μs" << endl;
        ui->label_8->setText(QString::number(duration.count()) + " μs");
    }
}

//void MainWindow::on_DDAButton_clicked()
//{
//    QPainter painter(&img);
//    QPen pen;
//    pen.setWidth(1);
//    pen.setColor(Qt::red);
//    painter.setPen(Qt::yellow);
////  painter.drawLine(p1,p2);
//    dda();
//}

//void swap(int &a, int &b) {
//    int temp = b;
//    b = a;
//    a = temp;
//}

void MainWindow::bresenham()
{
    auto start = high_resolution_clock::now();
    int x1 = p1.x()/gridsize;
    int y1 = p1.y()/gridsize;
    int x2 = p2.x()/gridsize;
    int y2 = p2.y()/gridsize;
    int dx = fabs(x2 - x1);
    int dy = fabs(y2 - y1);
    int xinc = (x1 > x2 ? -1 : 1);
    int yinc = (y1 > y2 ? -1 : 1);
    bool flag = 1;
    int x = x1*gridsize + gridsize/2;
    int y = y1*gridsize + gridsize/2;
    if(dy > dx) {
        swap(dx,dy);
        swap(x,y);
        swap(xinc,yinc);
        flag = 0;
    }
    int decision = 2*dy - dx;
    int step = dx, twice_dy = 2*dy, diff_dydx = 2*dy - 2*dx;
    int xstep = xinc*gridsize, ystep = yinc*gridsize;
    auto stop = high_resolution_clock::now();
    auto intervals = (stop - start);
    for(int i=0;i<=step;i++) {
        delay();
        if(flag){
            QPoint q(x,y);
            QColor c=QColor(255,0,0);
            fillColor(q,c);
        }
        else{
            QPoint q(y,x);
            QColor c=QColor(255,0,0);
            fillColor(q,c);
        }
        start = high_resolution_clock::now();
        if(decision < 0) {
            x += xstep;
            decision += twice_dy;
        } else {
            x += xstep;
            y += ystep;
            decision += diff_dydx;
        }
        stop = high_resolution_clock::now();
        intervals += (stop - start);        
    }

}

void MainWindow::polar_circle()
{
    auto start = high_resolution_clock::now();
    int xc=p1.x()/gridsize,yc=p1.y()/gridsize;
    int x,y;
    int radius=ui->circle_radius->value();
    auto stop = high_resolution_clock::now();
    auto intervals = (stop - start);
    for(float i=0; i<3.14; i=i+(1/(float)radius))
    {
        start = high_resolution_clock::now();
        x=round(radius*cos(i));
        y=round(radius*sin(i));
        stop = high_resolution_clock::now();
        intervals += (stop - start);


        delay();
        QPoint q(x+xc,y+yc);
        QColor c=QColor(157,82,255);
        fillColor(q,c);
        delay();
        QPoint q1(-x+xc,y+yc);
        QColor c1=QColor(157,82,255);
        fillColor(q1,c1);
        delay();
        QPoint q2(x+xc,-y+yc);
        QColor c2=QColor(157,82,255);
        fillColor(q2,c2);
        delay();
        QPoint q3(-x+xc,-y+yc);
        QColor c3=QColor(157,82,255);
        fillColor(q3,c3);
    }
    auto duration = duration_cast<microseconds>(intervals);
    ui->label_12->setText(QString::number(duration.count()) + " μs");
}

void MainWindow::midpoint_circle()
{
    auto start = high_resolution_clock::now();
    double radius=ui->circle_radius->value();
    double centerx=p1.x()/gridsize,centery=p1.y()/gridsize;
    centerx=(centerx/gridsize)*gridsize+gridsize/2;
    centery=(centery/gridsize)*gridsize+gridsize/2;
    double x1=radius*gridsize,y1=0;
    auto stop = high_resolution_clock::now();
    auto intervals = (stop - start);
    delay();
        QPoint q(x1+centerx,y1+centery);
        QColor c=QColor(255,0,255);
        fillColor(q,c);
    if(radius>0)
    {
        delay();
        QPoint q(-x1+centerx,-y1+centery);
        QColor c=QColor(255,0,255);
        fillColor(q,c);
        delay();
        QPoint q1(y1+centerx,x1+centery);
        QColor c1=QColor(255,0,255);
        fillColor(q1,c1);
        delay();
        QPoint q2(-y1+centerx,-x1+centery);
        QColor c2=QColor(255,0,255);
        fillColor(q2,c2);
    }
    start = high_resolution_clock::now();
    double p=(1-radius)*gridsize;
    while(x1>y1)
    {
        y1+=gridsize;
        if(p<=0)
            p=p+2*y1+1;
        else
        {
            x1-=gridsize;
            p=p+2*y1-2*x1+1;
        }
        if(x1<y1) break;
        stop = high_resolution_clock::now();
        intervals += (stop - start);
        delay();
            QPoint q(x1+centerx,y1+centery);
            QColor c=QColor(255,0,255);
            fillColor(q,c);
        delay();
            QPoint q1(-x1+centerx,y1+centery);
            QColor c1=QColor(255,0,255);
            fillColor(q1,c1);
        delay();
            QPoint q2(x1+centerx,-y1+centery);
            QColor c2=QColor(255,0,255);
            fillColor(q2,c2);
        delay();
            QPoint q3(-x1+centerx,-y1+centery);
            QColor c3=QColor(255,0,255);
            fillColor(q3,c3);
        if(x1!=y1)
        {
            delay();
            QPoint q(y1+centerx,x1+centery);
            QColor c=QColor(255,0,255);
            fillColor(q,c);
            delay();
            QPoint q1(-y1+centerx,x1+centery);
            QColor c1=QColor(255,0,255);
            fillColor(q1,c1);
            delay();
            QPoint q2(y1+centerx,-x1+centery);
            QColor c2=QColor(255,0,255);
            fillColor(q2,c2);
            delay();
            QPoint q3(-y1+centerx,-x1+centery);
            QColor c3=QColor(255,0,255);
            fillColor(q3,c3);
        }
    }
    auto duration = duration_cast<microseconds>(intervals);
    ui->label_10->setText(QString::number(duration.count()) + " μs");
}

void MainWindow::drawCircle(int xc,int yc, int x1,int y1)
{
delay();
    QPoint q(xc+x1,yc+y1);
    QColor c=QColor(0,255,255);
    fillColor(q,c);
delay();
    QPoint q1(xc-x1,yc+y1);
    QColor c1=QColor(0,255,255);
    fillColor(q1,c1);
delay();
    QPoint q2(xc+x1,yc-y1);
    QColor c2=QColor(0,255,255);
    fillColor(q2,c2);
delay();
    QPoint q3(xc-x1,yc-y1);
    QColor c3=QColor(0,255,255);
    fillColor(q3,c3);
delay();
    QPoint q4(xc+y1,yc+x1);
    QColor c4=QColor(0,255,255);
    fillColor(q4,c4);
delay();
    QPoint q5(xc-y1,yc+x1);
    QColor c5=QColor(0,255,255);
    fillColor(q5,c5);
delay();
    QPoint q6(xc+y1,yc-x1);
    QColor c6=QColor(0,255,255);
    fillColor(q6,c6);
delay();
    QPoint q7(xc-y1,yc-x1);
    QColor c7=QColor(0,255,255);
    fillColor(q7,c7);
}

void MainWindow::bresenham_circle()
{
    auto start = high_resolution_clock::now();
    int radius=ui->circle_radius->value();
    int centerx=p1.x()/gridsize,centery=p1.y()/gridsize;
    centerx=(centerx/gridsize)*gridsize+gridsize/2;
    centery=(centery/gridsize)*gridsize+gridsize/2;

    int x1 = 0, y1 = radius*gridsize;
    int decision_parameter = (3 - 2 * radius)*gridsize;
    auto stop = high_resolution_clock::now();
    auto intervals = (stop - start);
    while (y1 >= x1)
    {
        drawCircle(centerx,centery, x1, y1);
        start = high_resolution_clock::now();
        x1+=gridsize;
        if (decision_parameter > 0)
        {
            y1-=gridsize;
            decision_parameter = decision_parameter + 4 * (x1 - y1) + 10;
        }
        else
            decision_parameter = decision_parameter + 4 * x1 + 6;
        stop = high_resolution_clock::now();
        intervals += (stop - start);
        drawCircle(centerx,centery, x1, y1);
    }
    auto duration = duration_cast<microseconds>(intervals);
    ui->label_11->setText(QString::number(duration.count()) + " μs");
}

void MainWindow::draw_m_ellipse(int centerx,int centery,int x1,int y1)
{
delay();
    QPoint q(centerx+x1*gridsize,centery+y1*gridsize);
    QColor c=QColor(200,240,100);
    fillColor(q,c);
delay();
    QPoint q1(centerx+x1*gridsize,centery-y1*gridsize);
    QColor c1=QColor(200,240,100);
    fillColor(q1,c1);
delay();
    QPoint q2(centerx-x1*gridsize,centery+y1*gridsize);
    QColor c2=QColor(200,240,100);
    fillColor(q2,c2);
delay();
    QPoint q3(centerx-x1*gridsize,centery-y1*gridsize);
    QColor c3=QColor(200,240,100);
    fillColor(q3,c3);
}

void MainWindow::midpoint_ellipse() //this is Bresenham's Midpoint Ellipse
{
    auto start = high_resolution_clock::now();
    int x1, y1, p;
    int rx=ui->Elli_axis_1->value();
    int ry=ui->Elli_axis_2->value();
    int xc=p1.x()/gridsize,yc=p1.y()/gridsize;
    xc=(xc/gridsize)*gridsize+gridsize/2;
    yc=(yc/gridsize)*gridsize+gridsize/2;
    x1=0;
    y1=ry;
    p=(ry*ry)-(rx*rx*ry)+((rx*rx)/4);
    auto stop = high_resolution_clock::now();
    auto intervals = (stop - start);
    while((2*x1*ry*ry)<(2*y1*rx*rx))
    {
        draw_m_ellipse(xc,yc,x1,y1);
        start = high_resolution_clock::now();
        if(p<0)
        {
            x1=x1+1;
            p=p+(2*ry*ry*x1)+(ry*ry);
        }
        else
        {
            x1=x1+1;
            y1=y1-1;
            p=p+(2*ry*ry*x1+ry*ry)-(2*rx*rx*y1);
        }
    }
    p=((float)x1+0.5)*((float)x1+0.5)*ry*ry+(y1-1)*(y1-1)*rx*rx-rx*rx*ry*ry;
    stop = high_resolution_clock::now();
    intervals += (stop - start);
    while(y1>=0)
    {
        draw_m_ellipse(xc,yc,x1,y1);
        start = high_resolution_clock::now();
        if(p>0)
        {
            y1=y1-1;
            p=p-(2*rx*rx*y1)+(rx*rx);
        }
        else
        {
            y1=y1-1;
            x1=x1+1;
        p=p+(2*ry*ry*x1)-(2*rx*rx*y1)-(rx*rx);
        }
   }
    stop = high_resolution_clock::now();
    intervals += (stop - start);
    auto duration = duration_cast<microseconds>(intervals);
    ui->label_13->setText(QString::number(duration.count()) + " μs");
}

void MainWindow::draw_p_ellipse(int centerx,int centery,int x1,int y1)
{
delay();
    QPoint q(centerx+x1*gridsize,centery+y1*gridsize);
    QColor c=QColor(255,96,59);
    fillColor(q,c);
delay();
    QPoint q1(centerx+x1*gridsize,centery-y1*gridsize);
    QColor c1=QColor(255,96,59);
    fillColor(q1,c1);
delay();
    QPoint q2(centerx-x1*gridsize,centery+y1*gridsize);
    QColor c2=QColor(255,96,59);
    fillColor(q2,c2);
delay();
    QPoint q3(centerx-x1*gridsize,centery-y1*gridsize);
    QColor c3=QColor(255,96,59);
    fillColor(q3,c3);
}

void MainWindow::polar_ellipse()
{
    auto start = high_resolution_clock::now();
    int xc=p1.x()/gridsize,yc=p1.y()/gridsize;
    float x=0,y=ui->Elli_axis_2->value();
    float angle=0,range=3.14;
    float inc=1/float(ui->Elli_axis_1->value());
    auto stop = high_resolution_clock::now();
    auto intervals = (stop - start);
    do{

        draw_p_ellipse(xc,yc,x,y);
        start = high_resolution_clock::now();
        angle+=inc;
        x=round(ui->Elli_axis_1->value()*cos(angle));
        y=round(ui->Elli_axis_2->value()*sin(angle));
    }while(angle<=range);
    stop = high_resolution_clock::now();
    intervals += (stop - start);
    auto duration = duration_cast<microseconds>(intervals);
    ui->label_14->setText(QString::number(duration.count()) + " μs");
}

//***********************
void MainWindow::on_setVertexButton_clicked()
{
        p3.setX(ui->frame->x);
        p3.setY(ui->frame->y);
        p3=ui->frame->transformCoordinate(p3);
        QColor c=QColor(255,128,0);
        fillColor(p3,c);
        PolygonPoints.push_back(p3);
}

void MainWindow::on_vertices_valueChanged(int arg1)
{
    PolygonPoints.clear();Edges.clear();
    vertices=arg1;
}

void MainWindow::drawPolygon()
{
        int j,s=vertices;
        for(j=0;j<vertices;j++)
        {
            p1=PolygonPoints[j];p2=PolygonPoints[(j+1)%s];
            bresenham();
        }
}

void MainWindow::scanLine()
{
    int i,j,e,v;bool paint=false,pie=false,piv=false;
    vector<int> edges,fedges;cout<<"&&&&&&&&&\n";
    for(auto p:PolygonPoints){cout<<p.x()<<"*"<<p.y()<<"---->\n";}
    delay();
    int s=PolygonPoints.size();
    for(i=350;i>(-350);i--)
    {paint=false;edges.clear();fedges.clear();
        int x,y;float cx=i;
        for(x=0;x<s;x++)
        {
            float y2,y1,x1,x2,p;x1=PolygonPoints[x].x();
            y1=PolygonPoints[x].y();
            x2=PolygonPoints[(x+1)%s].x();
            y2=PolygonPoints[(x+1)%s].y();
            if(y1==y2)
            {
                if(x1>x2){int t=x2;x2=x1;x1=t;}
                for(int j=x1;j<=x2;j++)
                {
                    if(y1==i)
                    edges.push_back(j);
                }
            }
            else if(((i>=min(y1,y2))&&(i<=max(y1,y2)))){
            p=(x2-x1)*(i-y1);
            p=round(p/(y2-y1));
            p+=x1;
            if((p>=min(x1,x2))&&(p<=max(x1,x2)))
            edges.push_back(p);
            }
        }
        sort(edges.begin(),edges.end());
        int siz=edges.size();
        j=0;
        while(j<siz)
        {int x=edges[j];
            while((j<siz)&&(x==edges[j])){j++;}
            fedges.push_back(x);
        }
        edges.clear();
        for(auto p:fedges)edges.push_back(p);
        cout<<i<<"---->";
        for(auto p:edges){cout<<p<<" ";}cout<<"\n";
        //scanline and illuminate pixels
        for(j=-120;j<(120);j++)
        {
            pie=false;piv=false;
            QPoint p(j,i);QColor c=QColor(255,220,0);delay();
            //p=ui->frame->transformCoordinate(p);//cout<<p.x()<<" "<<p.y()<<"&\n";
            for(e=0;e<edges.size();e++)
            {
                if(edges[e]==j)pie=true;
            }
            for(v=0;v<PolygonPoints.size();v++)
            {
                if((PolygonPoints[v].x()==p.x())&&((PolygonPoints[v].y()==p.y()))){piv=true;break;}
            }

            if(piv)
            {
                int prevy,nexty,cury, prevx,nextx,curx;
                prevy=PolygonPoints[(v+vertices-1)%vertices].ry();
                nexty=PolygonPoints[(v+1)%vertices].ry();
                cury=PolygonPoints[(v)%vertices].ry();
                prevx=PolygonPoints[(v+vertices-1)%vertices].rx();
                nextx=PolygonPoints[(v+1)%vertices].rx();
                curx=PolygonPoints[(v)%vertices].rx();
                if((nexty==cury&&nextx>curx)){j=nextx;}
                else if((prevy==cury&&prevx>curx)){j=prevx;}
                else if(((prevy<=cury)&&(nexty>=cury))||((prevy>=cury)&&(nexty<=cury)))paint=!paint;
                continue;
            }
            else if(pie)
            {
                paint=!paint;
                continue;
            }
            else
            {
                if(paint){fillColor(p,c);delay();}
            }
        }
    }
}

void MainWindow::on_setSeedButton_clicked()
{
    seed.setX(ui->frame->x);
    seed.setY(ui->frame->y);
    seed=ui->frame->transformCoordinate(seed);
    cout<<"INfirst\n"<<seed.rx()<<" "<<seed.ry()<<"&&&&\n";
    QColor c=QColor(255,220,0);
    fillColor(seed,c);
}

bool MainWindow::present(int x,int y)
{
    for(auto p:Edges)
    {
        if((p.rx()==x)&&(p.ry()==y))return(true);
    }
    if(visited[x+350][y+350])return(true);
    return(false);
}

void MainWindow::boundaryFill(int x,int y)
{
    QColor c=QColor(255,220,0);visited[x+350][y+350]=true;
    QPoint p(x,y);fillColor(p,c);delay();
    if(!present(x,y+1)){boundaryFill( x,y+1);}
    if(!present(x+1,y)){boundaryFill( x+1,y);}
    if(!present(x,y-1)){boundaryFill( x,y-1);}
    if(!present(x-1,y)){boundaryFill( x-1,y);}
}

//void MainWindow::on_spinBox_4_valueChanged(int arg1)
//{
//    del=arg1;
//}

void MainWindow::MatrixMultiplication(int a[3][3],int b[3])
{
    int i,j,temp[3];
    for(i=0;i<3;i++)temp[i]=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            temp[i]+=(a[i][j]*b[j]);
        }
    }
    for(i=0;i<3;i++)mat[i]=temp[i];
}
void MainWindow::MatrixMultiplication(float a[3][3],float b[3])
{
    int i,j;float temp[3];
    for(i=0;i<3;i++)temp[i]=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            temp[i]+=(a[i][j]*b[j]);
        }
    }
    for(i=0;i<3;i++)fmat[i]=temp[i];
}

void MainWindow::on_x_change_valueChanged(int arg1)
{
    transformx=arg1;
}
void MainWindow::on_y_change_valueChanged(int arg1)
{
    transformy=arg1;
}

void MainWindow::translation()
{int i,j;
    int m[3][3];
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(i==j)m[i][j]=1;
            else m[i][j]=0;
        }
    }
    m[0][2]=transformx;
    m[1][2]=transformy;
    vector<QPoint> np;
    int od[3];
    for(j=0;j<vertices;j++)
    {
        od[0]=PolygonPoints[j].x();od[1]=PolygonPoints[j].y();od[2]=1;
        MatrixMultiplication(m,od);
        np.push_back(QPoint(mat[0],mat[1]));
    }
    on_clearGrid_clicked();on_show_axes_clicked();
    PolygonPoints.clear();
    for(auto p:np){PolygonPoints.push_back(p);}
    Edges.clear();
    drawPolygon();
}

void MainWindow::rotate()
{
        int i,j;
        float m[3][3];
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {
                if(i==j)m[i][j]=1;
                else m[i][j]=0;
            }
        }
        m[0][0]=cos(angle);
        m[1][1]=cos(angle);
        m[1][0]=sin(angle);
        m[0][1]=-sin(angle);
        vector<QPoint> np;
        float od[3];
        for(j=0;j<vertices;j++)
        {
            od[0]=PolygonPoints[j].x();od[1]=PolygonPoints[j].y();od[2]=1;
            MatrixMultiplication(m,od);
            np.push_back(QPoint(round(fmat[0]),round(fmat[1])));
        }
        on_clearGrid_clicked();on_show_axes_clicked();
        PolygonPoints.clear();
        for(auto p:np){PolygonPoints.push_back(p);}
        Edges.clear();
        drawPolygon();
}

void MainWindow::scale()
{
    int i,j;
        int m[3][3];
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {
                if(i==j)m[i][j]=1;
                else m[i][j]=0;
            }
        }
        m[0][0]=transformx;
        m[1][1]=transformy;
        vector<QPoint> np;
        int od[3];
        for(j=0;j<vertices;j++)
        {
            od[0]=PolygonPoints[j].x();od[1]=PolygonPoints[j].y();od[2]=1;
            MatrixMultiplication(m,od);
            np.push_back(QPoint(mat[0],mat[1]));
        }
        on_clearGrid_clicked();on_show_axes_clicked();
        PolygonPoints.clear();
        for(auto p:np){PolygonPoints.push_back(p);}
        Edges.clear();
        drawPolygon();
}

void MainWindow::reflectx()
{
    int i,j;
        int m[3][3];
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {
                if(i==j)m[i][j]=1;
                else m[i][j]=0;
            }
        }
        m[1][1]=-1;

        vector<QPoint> np;
        int od[3];
        for(j=0;j<vertices;j++)
        {
            od[0]=PolygonPoints[j].x();od[1]=PolygonPoints[j].y();od[2]=1;
            MatrixMultiplication(m,od);
            np.push_back(QPoint(mat[0],mat[1]));
        }
        on_clearGrid_clicked();on_show_axes_clicked();
        PolygonPoints.clear();
        for(auto p:np){PolygonPoints.push_back(p);}
        Edges.clear();
        drawPolygon();
}

void MainWindow::reflecty()
{
    int i,j;
        int m[3][3];
        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
            {
                if(i==j)m[i][j]=1;
                else m[i][j]=0;
            }
        }
        m[0][0]=-1;

        vector<QPoint> np;
        int od[3];
        for(j=0;j<vertices;j++)
        {
            od[0]=PolygonPoints[j].x();od[1]=PolygonPoints[j].y();od[2]=1;
            MatrixMultiplication(m,od);
            np.push_back(QPoint(mat[0],mat[1]));
        }
        on_clearGrid_clicked();on_show_axes_clicked();
        PolygonPoints.clear();
        for(auto p:np){PolygonPoints.push_back(p);}
        Edges.clear();
        drawPolygon();
}

void MainWindow::on_rotate_valueChanged(double arg1)
{
    angle=arg1;
}



//*********************************
void MainWindow::on_grid_clicked()
{
    int i,j;
    img=QImage(ui->frame->height(),ui->frame->width(),QImage::Format_RGB888);
    for(i=0;i<600;i+=GridSize)
    {
        for(j=0;j<600;j++)
        {
            img.setPixelColor(i,j,qRgb(200,200,200));
        }
    }
    for(i=0;i<600;i+=GridSize)
    {
        for(j=0;j<600;j++)
        {
            img.setPixelColor(j,i,qRgb(200,200,200));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::fillColor(QPoint& p,QColor &c)
{
    int i,j,x,y,leftx,lefty;
    leftx=600/(2*ui->frame->GridSize);lefty=-600/(2*ui->frame->GridSize);
    x=p.x();y=p.y();
    x+=leftx;y+=lefty;
    x*=ui->frame->GridSize;y*=-ui->frame->GridSize;
    for(i=x;i<x+GridSize;i++)
    {
        for(j=y;j<y+GridSize;j++)
        {
            img.setPixelColor(i,j,c);
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_gridsizespinBox_valueChanged(int arg1)
{
    ui->frame->GridSize=arg1;
    GridSize=arg1;
}

void MainWindow::on_clearGrid_clicked()
{
    on_grid_clicked();
}




