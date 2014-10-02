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
    // outer most edges
    if(start_p.x() <= end_p.x() && start_p.y() <= end_p.y())
    {
        return QRectF(start_p.x(),
                      start_p.y(),end_p.x(), end_p.y());
    }
    else if(start_p.x() >= end_p.x() && start_p.y() >= end_p.y())
    {
        return QRectF(end_p.x(), end_p.y(),start_p.x(),
                      start_p.y());
    }
    else if(start_p.x() >= end_p.x() && start_p.y() <= end_p.y())
    {
        return QRectF(end_p.x(), start_p.y(),start_p.x(),
                      end_p.y());
    }
    else if(start_p.x() <= end_p.x() && start_p.y() >= end_p.y())
    {
        return QRectF(start_p.x(), end_p.y(),end_p.x(),
                      start_p.y());
    }

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
