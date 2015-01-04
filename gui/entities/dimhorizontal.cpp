#include "dimhorizontal.h"

DimHorizontal::DimHorizontal(int i, QPointF p1, QPointF p2, QPointF p3)
{
    // Assigns id
    id = i;

    // Set values of startPoint, midPoint and endPoint
    if (p2.x() > p1.x())
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

DimHorizontal::DimHorizontal(QPointF p1, QPointF p2, QPointF p3)
{
    // Set values of startPoint, midPoint and endPoint
    if (p2.x() > p1.x())
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

void DimHorizontal::eval()
{
    // Create arrow and get values associated to it
    Arrow arrow(startP, midP, endP);
    arrowPath = arrow.getArrowPath();
    arrowMiddle = arrow.middle;
    arrowTextHeight = arrow.textHeight;
    arrowPadding = arrow.padding;

    extLine1.setLine(startP.x(), startP.y(), startP.x(), endP.y());
    extLine2.setLine(midP.x(), midP.y(), midP.x(), endP.y());
}

int DimHorizontal::type() const
{
    // Enable the use of qgraphicsitem_cast with horizontal dimension item.
    return Type;
}

QPainterPath DimHorizontal::shape() const
{
    QPainterPath s;
    QPainterPathStroker stroker;
    QTransform t;
    t.scale(1, -1);
    t.translate(0, - 2 * arrowMiddle.y());
    s.addPath(t.map(arrowPath));
    s.addPath(extLine1.shape());
    s.addPath(extLine2.shape());
    stroker.setWidth(10);
    return stroker.createStroke(s);
}

QRectF DimHorizontal::boundingRect() const
{
    // Bounding rectangle of the horizontal dimension
    qreal extra = 10.0;

    if (startP.y() > midP.y())
    {
        if (endP.y() > midP.y() && endP.y() > startP.y())
        {
            return QRectF(QPointF(startP.x() - arrowPadding,
                                  endP.y() + arrowTextHeight/2),
                          QSizeF(midP.x() - startP.x() + 2 * arrowPadding,
                                 midP.y() - endP.y() - arrowTextHeight/2))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        if (endP.y() > midP.y() && endP.y() < startP.y())
        {
            return QRectF(QPointF(startP.x() - arrowPadding,
                                  startP.y() + arrowTextHeight/2),
                          QSizeF(midP.x() - startP.x() + 2 * arrowPadding,
                                 midP.y() - startP.y() - arrowTextHeight/2))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else
        {
            return QRectF(QPointF(startP.x() - arrowPadding, startP.y()),
                          QSizeF(midP.x() - startP.x() + 2 * arrowPadding,
                                 endP.y() - startP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }
    }

    else if (startP.y() < midP.y())
    {
        if (endP.y() > midP.y() && endP.y() > startP.y())
        {
            return QRectF(QPointF(startP.x() - arrowPadding,
                                  endP.y() + arrowTextHeight/2),
                          QSizeF(midP.x() - startP.x() + 2 * arrowPadding,
                                 startP.y() - endP.y() - arrowTextHeight/2))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        if (endP.y() < midP.y() && endP.y() < startP.y())
        {
            return QRectF(QPointF(startP.x() - arrowPadding, midP.y()),
                          QSizeF(midP.x() - startP.x() + 2 * arrowPadding,
                                 endP.y() - midP.y() - arrowTextHeight/2))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else
        {
            return QRectF(QPointF(startP.x() - arrowPadding, midP.y()),
                          QSizeF(midP.x() - startP.x() + 2 * arrowPadding,
                                 startP.y() - midP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }
    }
}

void DimHorizontal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // Draws/paints the horizontal dimension
    QPen paintpen;
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);

    if (option->state & QStyle::State_Selected)
        paintpen.setStyle(Qt::DashLine);

    paintpen.setColor(Qt::black);
    painter->setPen(paintpen);

    painter->drawLine(startP.x(), startP.y(), startP.x(), endP.y());
    painter->drawLine(midP.x(), midP.y(), midP.x(), endP.y());
    painter->setBrush(Qt::SolidPattern);
    painter->scale(1, -1);
    painter->translate(0, -2 * arrowMiddle.y());
    painter->drawPath(arrowPath);
}
