#include "dimvertical.h"

DimVertical::DimVertical(int i, QPointF p1, QPointF p2, QPointF p3)
{
    // Assigns id
    id = i;

    // Set values of startPoint, midPoint and endPoint
    if (p2.y() > p1.y())
    {
        startP = p1;
        midP = p2;
    }

    else
    {
        startP = p2;
        midP = p1;
    }

    endP = p3;

    eval();
}

DimVertical::DimVertical(QPointF p1, QPointF p2, QPointF p3)
{
    // Set values of startPoint, midPoint and endPoint
    if (p2.y() > p1.y())
    {
        startP = p1;
        midP = p2;
    }

    else
    {
        startP = p2;
        midP = p1;
    }

    endP = p3;

    eval();
}

void DimVertical::eval()
{
    // Creates arrow and extension lines
    arrow = new Arrow(2, startP, midP, endP);

    extLine1.setLine(startP.x(), startP.y(), endP.x(), startP.y());
    extLine2.setLine(midP.x(), midP.y(), endP.x(), midP.y());
}

int DimVertical::type() const
{
    // Enable the use of qgraphicsitem_cast with vertical dimension item.
    return Type;
}

QPainterPath DimVertical::shape() const
{
    QPainterPath s;
    QPainterPathStroker stroker;
    QTransform t;
    t.scale(1, -1);
    t.translate(0, -2 * arrow->middle.y());
    s.addPath(t.map(arrow->getArrowPath()));
    s.addPath(extLine1.shape());
    s.addPath(extLine2.shape());
    stroker.setWidth(10);
    return stroker.createStroke(s);
}

QRectF DimVertical::boundingRect() const
{
    // Bounding rectangle of the vertical dimension
    qreal extra = 10.0 + arrow->textWidth;

    if (startP.x() > midP.x())
    {
        if (endP.x() >= startP.x() && endP.x() > midP.x())
        {
            return QRectF(QPointF(midP.x(), startP.y()),
                          QSizeF(endP.x() - midP.x(), midP.y() - startP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else if (endP.x() > midP.x() && endP.x() < startP.x())
        {
            return QRectF(QPointF(startP.x(), startP.y()),
                          QSizeF(midP.x() - startP.x(), midP.y() - startP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else
        {
            return QRectF(QPointF(endP.x(), startP.y()),
                          QSizeF(startP.x() - endP.x(), midP.y() - startP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }
    }

    else if (startP.x() < midP.x())
    {
        if (endP.x() >= midP.x() && endP.x() > startP.x())
        {
            return QRectF(QPointF(startP.x(), midP.y()),
                          QSizeF(endP.x() - startP.x(), startP.y() - midP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else if (endP.x() < midP.x() && endP.x() > startP.x())
        {
            return QRectF(QPointF(startP.x(), midP.y()),
                          QSizeF(midP.x() - startP.x(), startP.y() - midP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else
        {
            return QRectF(QPointF(endP.x(), midP.y()),
                          QSizeF(midP.x() - endP.x(), startP.y() - midP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }
    }
}

void DimVertical::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // Draws/paints the vertical dimension
    QPen paintpen;
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);

    if (option->state & QStyle::State_Selected)
        paintpen.setStyle(Qt::DashLine);

    paintpen.setColor(Qt::black);
    painter->setPen(paintpen);
    painter->drawLine(startP.x(), startP.y(), endP.x(), startP.y());
    painter->drawLine(midP.x(), midP.y(), endP.x(), midP.y());
    painter->setBrush(Qt::SolidPattern);
    painter->drawPath(arrow->getArrowPath());
    painter->scale(-1, 1);
    painter->rotate(180);
    painter->translate(0, -2 * arrow->middle.y());
    painter->drawText(arrow->middle, arrow->value);
}
