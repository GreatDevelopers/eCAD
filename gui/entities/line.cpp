#include "line.h"
#include <QDebug>
#include <QMouseEvent>

Line::Line(QPointF p1, QPointF p2)
{
    start_p = p1;
    end_p = p2;
}

QRectF Line::boundingRect() const
{
    return QRectF(start_p,end_p).normalized();

}

void Line:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath line;
    line.moveTo(start_p);
    line.lineTo(end_p);

    QPen paintpen(Qt::black);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::SolidPattern);
    painter->setPen(paintpen);
    painter->drawPath(line);

}
