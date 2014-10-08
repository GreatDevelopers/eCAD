#include "ellipse.h"

Ellipse::Ellipse(int i, QPointF point1, QPointF point2, QPointF point3)
{
    // assigns id
    id = i;

    /* set values of three points
    and calculate radii of ellipse */
    p1 = point1;
    p2 = point2;
    p3 = point3;

    d12 = qSqrt(qPow((p2.x()-p1.x()), 2) + qPow((p2.y()-p1.y()), 2));
    d13 = qSqrt(qPow((p3.x()-p1.x()), 2) + qPow((p3.y()-p1.y()), 2));

    if(d12>d13)
    {
        majRadius = d12;
        minRadius = d13;
        theta = atan2((p2.y()-p1.y()),(p2.x()-p1.x())) * (180/M_PI);
    }
    else
    {
        majRadius = d13;
        minRadius = d12;
        theta = atan2((p3.y()-p1.y()),(p3.x()-p1.x())) * (180/M_PI);
    }
}

int Ellipse::type() const
{
    // Enable the use of qgraphicsitem_cast with ellipse item.
    return Type;
}

QRectF Ellipse::boundingRect() const
{
    // bounding rectangle for ellipse
    return QRectF(p1.x()-majRadius, p1.y()-minRadius,
                  2*majRadius, 2*minRadius).normalized();
}

void Ellipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    // creates the path of ellipse
    QPainterPath ellipse;
    ellipse.moveTo(p1.x()+majRadius, p1.y());
    ellipse.arcTo(boundingRect(), 0 , 360);

    // draws/paints the path of ellipse
    QPen paintpen(Qt::black);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(paintpen);
    painter->save();
    painter->translate(p1.x(), p1.y());
    painter->rotate(theta);
    painter->translate(-p1.x(), -p1.y());
    painter->drawPath(ellipse);
    painter->restore();
}
