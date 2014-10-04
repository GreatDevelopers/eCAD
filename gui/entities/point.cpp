#include "point.h"

Point::Point()
{
}

QRectF Point::boundingRect() const
{
    // bounding rectangle for point
    qreal penwidth = 1;
    return QRectF(-1 - penwidth /2, -1 - penwidth/2,
                  2 + penwidth, 2 + penwidth);
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    // draws/paints the point
    QPen paintpen(Qt::black);
    paintpen.setWidth(2);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::SolidPattern);
    painter->setPen(paintpen);
    painter->drawEllipse(boundingRect());
}
