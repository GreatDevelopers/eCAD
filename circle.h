#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "qmath.h"

class circle: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    circle();
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    int x1, y1, x2, y2, radius;
    bool mFirstClick;
    bool mSecondClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF move_p;

signals:
    void DrawFinished();
};

#endif // CIRCLE_H
