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
    qreal penwidth = 1;
    return QRectF(-1 - penwidth /2, -1 - penwidth/2,
                  2 + penwidth, 2 + penwidth);
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
