#include "circle.h"

Circle::Circle(QPointF p1, QPointF p2)
{
    /* set values of center point, end point
    and calculate radius of circle */
    center_p = p1;
    end_p = p2;
    radius = qSqrt(qPow((end_p.x()-center_p.x()), 2)
                   + qPow((end_p.y()-center_p.y()), 2));
}

int Circle::type() const
{
    // Enable the use of qgraphicsitem_cast with circle item.
    return Type;
}

QRectF Circle::boundingRect() const
{
    // bounding rectangle for circle
    return QRectF((center_p.x()-radius), (center_p.y()-radius),
                  (2*radius), (2*radius));
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    // creates the path of circle
    QPainterPath circle;
    circle.moveTo(center_p.x()+radius, center_p.y());
    circle.arcTo(boundingRect(),0,360);

    // draws/paints the path of circle
    QPen paintpen(Qt::black);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(paintpen);
    painter->drawPath(circle);
}
