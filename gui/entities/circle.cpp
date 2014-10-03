#include "circle.h"

Circle::Circle(QPointF p1, QPointF p2)
{
    center_p = p1;
    end_p = p2;
    radius = qSqrt(qPow((end_p.x()-center_p.x()), 2)
                             + qPow((end_p.y()-center_p.y()), 2));
}

QRectF Circle::boundingRect() const
{
    return QRectF((center_p.x()-radius),(center_p.y()-radius),
                              (2*radius),(2*radius));
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath circle;
    circle.moveTo(center_p.x()+radius, center_p.y());
    circle.arcTo(boundingRect(),0,360);

    QPen paintpen(Qt::black);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(paintpen);
    painter->drawPath(circle);
}
