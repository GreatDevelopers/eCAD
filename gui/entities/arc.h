#ifndef ARC_H
#define ARC_H

#include <QGraphicsItem>
#include <QLineF>
#include <QPointF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class Arc : public QGraphicsItem
{
public:
    Arc();
    Arc(int i, QPointF point0, QPointF point1, QPointF point2);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    enum { Type = UserType + 6 };
    int type() const;
    int id;

    QPointF p1, p2, p3, center;
    QLineF lineBC, lineAC, lineBA, lineOA, lineOB, lineOC, bisectorBC, bisectorBA;
    qreal startAngle, spanAngle, rad;
    QRectF circle, boundingRectTemp;

private:
    void init();
};

#endif // ARC_H
