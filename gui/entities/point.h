#ifndef POINT_H
#define POINT_H

#include <QPainter>
#include <QGraphicsItem>

#include "getEntity.h"
#include "clipboardstack.h"

class Point : public getEntity
{
    Q_OBJECT

public:
    Point(QObject *parent = 0) : getEntity(parent) {}
    Point(int, QPointF);
    enum { Type = UserType + 1 };
    int type() const;
    getEntity *clone();

    int id;
    QPointF position;

protected:
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
};

#endif // POINT_H
