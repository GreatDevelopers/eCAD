#ifndef ARROW_H
#define ARROW_H

#include <QPainter>

#include "qmath.h"

class Arrow
{
public:
    Arrow(QPointF, QPointF, QPointF);
    qreal getAngle(const qreal, const qreal);
    QPainterPath getArrowPath();
    void calculate();

    QPointF startP, midP, endP;
    QPointF middle;
    QLineF line;
    QPolygonF headPolygon, tailPolygon;
    qreal lineAngle, textHeight, padding;
    QString value;
    QFont font;
};

#endif // ARROW_H
