#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "qmath.h"

class Circle: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Circle(QPointF, QPointF);
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget);

private:
    QVector<QPointF> stuff;
    QPointF center_p, end_p, move_p;
    qreal radius;
};

#endif // CIRCLE_H
