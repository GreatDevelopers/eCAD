#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPainter>
#include <QGraphicsItem>

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
    enum { Type = UserType + 3 };
    int type() const;

    QPointF center_p, end_p, move_p;
    qreal radius;

private:
    QVector<QPointF> stuff;
};

#endif // CIRCLE_H
