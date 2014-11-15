#include "circle.h"

Circle::Circle(int i, QPointF p1, QPointF p2) {
    // assigns id
    id = i;

    /* set values of center point, end point
    and calculate radius of circle */
    centerP = p1;
    endP = p2;
    radius = qSqrt(qPow((endP.x() - centerP.x()), 2)
                   + qPow((endP.y() - centerP.y()), 2));
}

Circle::Circle(int i, QPointF p1, qreal rad) {
    // assigns id
    id = i;

    /* set values of center point
       and radius of circle */
    centerP = p1;
    radius = rad;
}

int Circle::type() const {
    // Enable the use of qgraphicsitem_cast with circle item.
    return Type;
}

QRectF Circle::boundingRect() const {
    // bounding rectangle for circle
    return QRectF((centerP.x() - radius), (centerP.y() - radius),
                  (2 * radius), (2 * radius));
}

void Circle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget) {
    // draws/paints the path of circle
    QPen paintpen(Qt::black);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);

    if (isSelected()) {
        // sets brush for center point
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(centerP, 2, 2);

        // sets pen for circumference
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawEllipse(centerP, radius, radius);
    } else {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(centerP, 2, 2);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(centerP, radius, radius);
    }
}

getEntity* Circle::clone() {
    Circle* c = new Circle;
    c->centerP = centerP;
    c->radius = radius;
    return c;
}

