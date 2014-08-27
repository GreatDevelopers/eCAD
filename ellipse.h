#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include "qmath.h"

class ellipse: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    ellipse();
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    int x1, y1, x2, y2, x3, y3;
    int majRadius, minRadius;
    bool mFirstClick;
    bool mSecondClick;
    bool mThirdClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;

signals:
    void DrawFinished();
};

#endif // ELLIPSE_H
