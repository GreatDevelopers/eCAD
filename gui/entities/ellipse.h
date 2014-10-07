#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QPainter>
#include <QGraphicsItem>

#include "qmath.h"

class Ellipse: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Ellipse(QPointF, QPointF, QPointF);
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget);

private:
    QPen paintpen, linepen;
    QPointF p1, p2, p3;
    QVector<QPointF> stuff;
    float d12, d13, majRadius, minRadius;
    float theta;
};

#endif // ELLIPSE_H
