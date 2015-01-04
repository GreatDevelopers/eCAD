#include "arrow.h"

Arrow::Arrow(QPointF p1, QPointF p2, QPointF p3)
{
    // Set values of startPoint, midPoint and endPoint
    startP = p1;
    midP = p2;
    endP = p3;

    calculate();
}

qreal Arrow::getAngle(const qreal dx, const qreal dy)
{
    // Returns the angle of the line
    return M_PI - atan(dx/dy);
}

void Arrow::calculate()
{
    // Sets font of text
    font.setPointSize(10);
    font.setWeight(25);
    QFontMetrics fm(font);
    textHeight = fm.height();

    line.setLine(startP.x(), endP.y(), midP.x(), endP.y());
    value = QString::number(line.length());
    middle = line.pointAt(0.45);
    lineAngle = getAngle(line.dx(), line.dy());

    if (line.dy() >= 0.0)
        lineAngle = (1.0 * M_PI) + lineAngle;

    const qreal size = 10.0;

    // Tail polygon
    const QPointF t0 = line.p1();
    const QPointF t1 = t0 + QPointF(qSin(lineAngle + M_PI + (M_PI * 0.1)) * size,
                                    -qCos(lineAngle + M_PI + (M_PI * 0.1)) * size);
    const QPointF t2 = t0 + QPointF(qSin(lineAngle + M_PI - (M_PI * 0.1)) * size,
                                    -qCos(lineAngle + M_PI - (M_PI * 0.1)) * size);
    tailPolygon << t0 << t1 << t2 << t0;


    // Head polygon
    const QPointF h0 = line.p2();
    const QPointF h1 = h0 + QPointF(qSin(lineAngle + 0.0 + (M_PI * 0.1)) * size,
                                    -qCos(lineAngle + 0.0 + (M_PI * 0.1)) * size);
    const QPointF h2 = h0 + QPointF(qSin(lineAngle + 0.0 - (M_PI * 0.1)) * size,
                                    -qCos(lineAngle + 0.0 - (M_PI * 0.1)) * size);
    headPolygon << h0 << h1 << h2 << h0;
}

QPainterPath Arrow::getArrowPath()
{
    // Arrow Path
    QPainterPath p;

    if (line.length() >= 30)
    {
        padding = 0;
        p.addPolygon(headPolygon);
        p.moveTo(line.p1());
        p.lineTo(line.p2());
        p.addPolygon(tailPolygon);
    }

    else
    {
        padding = 25;
        QTransform t, t2;
        t.translate(-line.dx(), 0);
        p.addPolygon(t.map(headPolygon));
        t2.translate(line.dx(), 0);
        p.addPolygon(t2.map(tailPolygon));
        p.moveTo(QPointF(line.p1().x() - 30, line.p1().y()));
        p.lineTo(QPointF(line.p1()));
        p.moveTo(QPointF(line.p2().x() + 30, line.p2().y()));
        p.lineTo(QPointF(line.p2()));
    }

    p.addText(middle * 0.995, font, value);
    return p;
}
