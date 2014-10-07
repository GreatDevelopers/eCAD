#include "line.h"

Line::Line(QPointF p1, QPointF p2)
{
    // set values of start point and end point of line
    start_p = p1;
    end_p = p2;
}

int Line::type() const
{
    // Enable the use of qgraphicsitem_cast with line item.
    return Type;
}

QRectF Line::boundingRect() const
{
    // bounding rectangle for line
    return QRectF(start_p, end_p).normalized();
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // creates the path of line
    QPainterPath line;
    line.moveTo(start_p);
    line.lineTo(end_p);

    // draws/paints the path of line
    QPen paintpen(Qt::black);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::SolidPattern);
    painter->setPen(paintpen);
    painter->drawPath(line);
}
