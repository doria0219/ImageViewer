#include "mymouseevent.h"

MyMouseEvent::MyMouseEvent(QGraphicsPixmapItem *parent) :
    QGraphicsPixmapItem(parent)
{

}

void MyMouseEvent::mousePressEvent(QGraphicsSceneMouseEvent *event){

}

void MyMouseEvent::mouseMoveEvent(QGraphicsSceneMouseEvent *event){

    setPos(pos() + mapToParent(event->pos()) - mapToParent(event->lastPos()));
}

void MyMouseEvent::wheelEvent(QGraphicsSceneWheelEvent *event){

    int delta = event->delta();
    double factor = scale();
    double rate = 0.1;

    if(delta > 0){
        factor *= (1 + rate);
    }else if(delta < 0){
        factor *= (1 - rate);
    }
    setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2);
    setScale(factor);
}

void MyMouseEvent::reSet()
{
    setPos(0, 0);
    setScale(1.0);
}
