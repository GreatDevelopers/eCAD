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
    Circle();
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int center_x, center_y, x1, y1, radius;
    bool mFirstClick;
    bool mSecondClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF center, p1, move_p;
};

#endif // CIRCLE_H
