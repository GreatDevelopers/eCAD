#ifndef LINE_H
#define LINE_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "qmath.h"
#include "getEntity.h"

class Line : public getEntity
{
    Q_OBJECT

public:
    Line(QObject *parent = 0) : getEntity(parent) {}
    Line(int, QPointF, QPointF);
    QPainterPath shape() const;
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 2 };
    int type() const;
    getEntity *clone(int);

    int id;
    QPointF startP, endP;
};

#endif // LINE_H
