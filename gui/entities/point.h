#ifndef POINT_H
#define POINT_H

#include <QPainter>
#include <QGraphicsItem>

#include "gEntity.h"

class Point : public gEntity
{
    Q_OBJECT
public:
    Point(QObject* parent = 0) : gEntity(parent)
    {

    }
    Point(int);
    enum { Type = UserType + 1 };
    int type() const;
    int id;

    gEntity *my_clone();

protected:
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
};

#endif // POINT_H
