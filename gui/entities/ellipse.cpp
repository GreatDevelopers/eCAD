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

Ellipse::Ellipse(int i, QPointF point1, qreal rad, qreal radM)
{
    // assigns id
    id = i;

    /* set values of center point
    and radii of ellipse */
    p1 = point1;
    minRadius = rad;
    majRadius = radM;
}

int Ellipse::type() const
{
    // Enable the use of qgraphicsitem_cast with ellipse item.
    return Type;
}

QRectF Ellipse::boundingRect() const
{
    // bounding rectangle for ellipse
    float topLeftX = majRadius * cos(theta);
    float topLeftY = majRadius * sin(theta);
    float bottomRightX = minRadius * cos(theta + M_PI/2);
    float bottomRightY = minRadius * sin(theta + M_PI/2);

    float halfWidth = sqrt((topLeftX * topLeftX) + (bottomRightX * bottomRightX));
    float halfHeight = sqrt((topLeftY * topLeftY) + (bottomRightY * bottomRightY));

    return QRectF(p1.x() - 1.5 * halfWidth, p1.y() - 1.5 * halfHeight,
                  3 * halfWidth, p1.y() + 3 * halfHeight);
}

void Ellipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    // draws/paints the path of ellipse
    QPen paintpen;
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);

    if (isSelected())
    {
        // sets brush for center point
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, 2, 2);
        painter->save();
        painter->translate(p1.x(), p1.y());
        painter->rotate(theta);
        painter->translate(-p1.x(), -p1.y());

        // sets pen for circumference
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, majRadius, minRadius);
        painter->restore();
    }
    else
    {
        painter->setBrush(Qt::SolidPattern);
        painter->drawEllipse(p1, 2, 2);
        painter->save();
        painter->translate(p1.x(), p1.y());
        painter->rotate(theta);
        painter->translate(-p1.x(), -p1.y());

        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, majRadius, minRadius);
        painter->restore();
    }
}

getEntity *Ellipse::clone()
{
    Ellipse *e = new Ellipse;
    e->p1 = p1;
    e->minRadius = minRadius;
    e->majRadius = majRadius;
    return e;
}
