#include "arrow.h"

Arrow::Arrow(int i, QPointF p1, QPointF p2)
{
    // Set values of startPoint and endPoint
    startP = p1;
    endP = p2;

    horizontal = false;
    vertical = false;
    radial = false;

    calculate(i);
}

Arrow::Arrow(int i, QPointF p1, QPointF p2, QPointF p3)
{
    // Set values of startPoint, midPoint and endPoint
    startP = p1;
    midP = p2;
    endP = p3;

    horizontal = false;
    vertical = false;
    radial = false;

    calculate(i);
}

qreal Arrow::getAngle(const qreal dx, const qreal dy)
{
    // Returns the angle of the line
    return M_PI - atan(dx / dy);
}

void Arrow::calculate(int i)
{
    switch (i)
    {
    case 1:
        horizontal = true;
        break;

    case 2:
        vertical = true;
        break;

    case 3:
        radial = true;
        break;

    default:
        break;
    }

    // Sets font of text
    font.setPointSize(10);
    font.setWeight(25);
    QFontMetrics fm(font);
    textHeight = fm.height();

    if (horizontal)
        line.setLine(startP.x(), endP.y(), midP.x(), endP.y());
    else if (vertical)
        line.setLine(endP.x(), startP.y(), endP.x(), midP.y());
    else if (radial)
        line.setLine(startP.x(), startP.y(), endP.x(), endP.y());

    value = QString::number(line.length());
    textWidth = fm.width(value);
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

        if (!radial)
            p.addPolygon(headPolygon);

        p.moveTo(line.p1());
        p.lineTo(line.p2());
        p.addPolygon(tailPolygon);
    }

    else
    {
        padding = 25;

        if (horizontal)
        {
            QTransform t1, t2;
            t1.translate(-line.dx(), 0);
            p.addPolygon(t1.map(headPolygon));
            t2.translate(line.dx(), 0);
            p.addPolygon(t2.map(tailPolygon));
            p.moveTo(QPointF(line.p1().x() - 30, line.p1().y()));
            p.lineTo(QPointF(line.p1()));
            p.moveTo(QPointF(line.p2().x() + 30, line.p2().y()));
            p.lineTo(QPointF(line.p2()));
        }

        else if (vertical)
        {
            QTransform t1, t2;
            t1.translate(0, -line.dy());
            p.addPolygon(t1.map(headPolygon));
            t2.translate(0, line.dy());
            p.addPolygon(t2.map(tailPolygon));
            p.moveTo(QPointF(line.p1().x(), line.p1().y() - 30));
            p.lineTo(QPointF(line.p1()));
            p.moveTo(QPointF(line.p2().x(), line.p2().y() + 30));
            p.lineTo(QPointF(line.p2()));
        }

        else if (radial)
        {
            p.moveTo(line.p1());
            p.lineTo(line.p2());
            p.addPolygon(tailPolygon);
        }
    }

    return p;
}
