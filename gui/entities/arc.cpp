#include "arc.h"

Arc::Arc(int i, QPointF point1, QPointF point2, QPointF point3)
{
    // assigns id
    id = i;

    /**
     * set values of three points
     * and calls init() to do calculation for arc
     */
    p1 = point1;
    p2 = point2;
    p3 = point3;
    init();
}

int Arc::type() const
{
    // Enable the use of qgraphicsitem_cast with arc item.
    return Type;
}

Arc::Arc(QPointF point1, QPointF point2, QPointF point3)
{
    /**
     * set values of three points
     * and calls init() to do calculation for arc
     */
    p1 = point1;
    p2 = point2;
    p3 = point3;
    init();
}

void Arc::init()
{
    // Calculates startangle and spanangle
    lineBC = QLineF(p2, p3);
    lineAC = QLineF(p1, p3);
    lineBA = QLineF(p2, p1);

    rad = qAbs(lineBC.length()/(2 * qSin(qDegreesToRadians
                                       (lineAC.angleTo(lineBA)))));

    bisectorBC = QLineF(lineBC.pointAt(0.5), lineBC.p2());
    bisectorBC.setAngle(lineBC.normalVector().angle());

    bisectorBA = QLineF(lineBA.pointAt(0.5), lineBA.p2());
    bisectorBA.setAngle(lineBA.normalVector().angle());
    bisectorBA.intersect(bisectorBC, &center);

    circle = QRectF(center.x() - rad, center.y() - rad, rad * 2, rad * 2);

    lineOA = QLineF(center, p1);
    lineOB = QLineF(center, p2);
    lineOC = QLineF(center, p3);

    startAngle = lineOA.angle();
    spanAngle = lineOA.angleTo(lineOC);

    /**
     * Make sure that the span angle covers all three points with the
     * second point in the middle
     */
    if (qAbs(spanAngle) < qAbs(lineOA.angleTo(lineOB)) ||
            qAbs(spanAngle) < qAbs(lineOB.angleTo(lineOC)))
    {
        // swap the end point and invert the spanAngle
        startAngle = lineOC.angle();
        spanAngle = 360 - spanAngle;
    }

    int w = 10;
    boundingRectTemp = circle.adjusted(-w, -w, w, w);
}

QRectF Arc::boundingRect() const
{
    // bounding rectangle for arc
    return boundingRectTemp;
}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    QPen paintpen;
    painter->setRenderHint(QPainter::Antialiasing);
    paintpen.setWidth(1);

    // Draw arc
    if (option->state & QStyle::State_Selected)
    {
        // sets brush for end points
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, 2, 2);
        painter->drawEllipse(p2, 2, 2);
        painter->drawEllipse(p3, 2, 2);

        // sets pen for arc path
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
    }

    else
    {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, 2, 2);
        painter->drawEllipse(p2, 2, 2);
        painter->drawEllipse(p3, 2, 2);
    }

    QPainterPath path;
    path.arcMoveTo(circle, startAngle);
    path.arcTo(circle, startAngle, spanAngle);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

getEntity *Arc::clone(int i)
{
    Arc *a = new Arc;
    a->id = i;
    a->p1 = p1;
    a->p2 = p2;
    a->p3 = p3;
    a->lineAC = lineAC;
    a->lineBA = lineBA;
    a->lineBC = lineBC;
    a->lineOA = lineOA;
    a->lineOB = lineOB;
    a->lineOC = lineOC;
    a->bisectorBA = bisectorBA;
    a->bisectorBC = bisectorBC;
    a->center = center;
    a->circle = circle;
    a->startAngle = startAngle;
    a->spanAngle = spanAngle;
    return a;
}
