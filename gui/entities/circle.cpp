#include "circle.h"

Circle::Circle(int i, QPointF p1, QPointF p2)
{
    // assigns id
    id = i;

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
    // draws/paints the path of circle
    QPen paintpen(Qt::black);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);

    if(isSelected())
    {
        // sets brush for center point
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(center_p, 2, 2);

        // sets pen for circumference
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawEllipse(center_p, radius, radius);
    }
    else
    {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(center_p, 2, 2);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(center_p, radius, radius);
    }
}
