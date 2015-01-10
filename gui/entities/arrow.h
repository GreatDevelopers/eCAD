#ifndef ARROW_H
#define ARROW_H

#include <QPainter>

#include "qmath.h"

class Arrow
{
public:
    Arrow(int, QPointF, QPointF);
    Arrow(int, QPointF, QPointF, QPointF);
    qreal getAngle(const qreal, const qreal);
    QPainterPath getArrowPath();
    void calculate(int);

    bool horizontal, vertical, radial;
    QPointF startP, midP, endP;
    QPointF middle;
    QLineF line;
    QPolygonF headPolygon, tailPolygon;
    qreal lineAngle, textHeight, textWidth, padding;
    QString value;
    QFont font;
};

#endif // ARROW_H
