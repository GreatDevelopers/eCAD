#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPainter>
#include <QGraphicsItem>

#include "qmath.h"
#include "getEntity.h"
#include "clipboardstack.h"

class Circle : public getEntity
{
    Q_OBJECT
public:
    Circle(QObject *parent = 0) : getEntity(parent) {}
    Circle(int, QPointF, QPointF);
    Circle(int, QPointF, qreal);
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 3 };
    int type() const;
    int id;

    QPointF centerP, endP;
    qreal radius;

    getEntity *clone();

private:
    QVector<QPointF> stuff;
};

#endif // CIRCLE_H
