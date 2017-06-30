#ifndef MYMOUSEEVENT_H
#define MYMOUSEEVENT_H

#include <QObject>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

class MyMouseEvent : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit MyMouseEvent(QGraphicsPixmapItem *parent = 0);

private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

public:
    void reSet();
};

#endif // MYMOUSEEVENT_H
