#include "arc.h"

Arc::Arc(int i, QPointF point1, QPointF point2, QPointF point3)
{
    // assigns id
    id = i;

    /* set values of three points
     * and calculate radius of arc */
    p1 = point1;
    p2 = point2;
    p3 = point3;

    /* finds mid points of line segment
     * joining p1 and p2, p2 and p3 */
    mid12 = (p1 + p2) / 2;
    mid23 = (p2 + p3) / 2;

    /* calculates the slope of line segment
     * joining p1 and p2, p2 and p3 */
    slope12 = (p2.y() - p1.y()) / (p2.x() - p1.x());
    slope23 = (p3.y()-p2.y()) / (p3.x() - p2.x());

    /* calculates the slope of perpendicular to
     * line segment joining p1 and p2, p2 and p3 */
    slopePerp12 = -(1 / slope12);
    slopePerp23 = -(1 / slope23);

    // constants from equation of perpendiculars
    constant1 = mid12.y() - (slopePerp12 * mid12.x());
    constant2 = mid23.y() - (slopePerp23 * mid23.x());

    // calculates center and radius of arc
    centerX = (constant1 - constant2) / (slopePerp23 - slopePerp12);
    centerY = (slopePerp12 * centerX) + constant1;
    centerP = QPointF(centerX, centerY);
    radius = sqrt(pow(centerX - p1.x(), 2) + pow(centerY-p1.y(), 2));

    // calculates start angle and end angle
    startAngle = (180/M_PI) * atan2((p1.y()-centerY) , (p1.x()-centerX));
    endAngle = (180/M_PI) * atan2((p3.y()-centerY) , (p3.x()-centerX));
}

int Arc::type() const
{
    // Enable the use of qgraphicsitem_cast with arc item.
    return Type;
}

QRectF Arc::boundingRect() const
{
    // bounding rectangle for arc
    return QRectF((centerX-radius), (centerY-radius),
                  (2*radius), (2*radius));
}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    // draws/paints the path of arc
    QPen paintpen;
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);

    if (isSelected())
    {
        //sets brush for three points
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, 2, 2);
        painter->drawEllipse(p2, 2, 2);
        painter->drawEllipse(p3, 2, 2);

        // sets pen for circumference
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawArc(boundingRect(), -startAngle * 16,
                         (startAngle - endAngle) * 16);
    }
    else
    {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->drawEllipse(p1, 2, 2);
        painter->drawEllipse(p2, 2, 2);
        painter->drawEllipse(p3, 2, 2);

        painter->setBrush(Qt::NoBrush);
        painter->drawArc(boundingRect(), -startAngle * 16,
                         (startAngle - endAngle) * 16);
    }
}
