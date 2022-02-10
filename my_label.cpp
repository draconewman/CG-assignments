#include "my_label.h"

my_label::my_label(QWidget *parent):QLabel(parent)
{
    this->setMouseTracking(true);
    GridSize=5;
}

void my_label::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint mouse_pos=ev->pos(),p;
    if(mouse_pos.x()<=this->size().width() && mouse_pos.y()<=this->size().height()){
        if(mouse_pos.x()>0 && mouse_pos.y()>0){
            p=transformCoordinate(mouse_pos);
            emit sendMousePosition(p);
        }
    }
}

void my_label::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        this->x=ev->x();
        this->y=ev->y();
        emit Mouse_Pos();
    }
}
QPoint my_label::transformCoordinate(QPoint& pos )
{
    QPoint p;
    int leftx,lefty;
    leftx=-(600/(2*GridSize));lefty=-leftx;
    p.setX(leftx+pos.x()/GridSize);
    p.setY(lefty-pos.y()/GridSize);
    return(p);
}
