#ifndef ARC_H
#define ARC_H

#include <QPainter>
#include <QGraphicsItem>

#include "qmath.h"

class Arc : public QGraphicsItem
{
public:
    Arc(int, QPointF, QPointF, QPointF);
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 6 };
    int type() const;
    int id;

    QPointF p1, p2, p3, mid12, mid23, centerP;
    float centerX, centerY;
    double radius, startAngle, endAngle;
    double slope12, slope23, slopePerp12, slopePerp23;
    double constant1, constant2;
};

#endif // ARC_H
