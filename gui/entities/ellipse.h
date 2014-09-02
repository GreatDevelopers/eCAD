#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "qmath.h"

class Ellipse: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Ellipse();
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int center_x, center_y, x1, y1, x2, y2;
    int majRadius, minRadius;
    bool mFirstClick;
    bool mSecondClick;
    bool mThirdClick;
    bool mPaintFlag;
    QPointF center, p1, p2;
    QVector<QPointF> stuff;
};

#endif // ELLIPSE_H
