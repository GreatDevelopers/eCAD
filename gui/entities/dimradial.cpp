#include "dimradial.h"

DimRadial::DimRadial(int i, qreal v, QPointF p1, QPointF p2)
{
    // Assigns id
    id = i;

    // Set values of startPoint and endPoint
    startP = p1;
    endP = p2;

    eval(v);
}

DimRadial::DimRadial(qreal v, QPointF p1, QPointF p2)
{
    // Set values of startPoint and endPoint
    startP = p1;
    endP = p2;

    eval(v);
}

void DimRadial::eval(qreal v)
{
    // Creates arrow and set value of radius
    arrow = new Arrow(3, startP, endP);
    radValue = "R " + QString::number(v);

    // Set the font and font metrics for the text
    font.setPointSize(10);
    font.setWeight(25);
    QFontMetrics fm(font);
    textHeight = fm.height();
    textWidth = fm.width(radValue);

    if (arrow->line.p1().x() > arrow->line.p2().x())
    {
        extLine.setLine(arrow->line.p2().x(), arrow->line.p2().y(),
                        arrow->line.p2().x() - 10, arrow->line.p2().y());
        textPadding = -textWidth - 10;
    }

    else
    {
        extLine.setLine(arrow->line.p2().x(), arrow->line.p2().y(),
                        arrow->line.p2().x() + 10, arrow->line.p2().y());
        textPadding = 10;
    }

    textPoint.setX(arrow->line.p2().x() + textPadding);
    textPoint.setY(arrow->line.p2().y() + textHeight / 4);
    text.addText(textPoint, font, radValue);
}

int DimRadial::type() const
{
    // Enable the use of qgraphicsitem_cast with radial dimension item.
    return Type;
}

QPainterPath DimRadial::shape() const
{
    // Returns the shape of the radial dimension
    QPainterPath s;
    QPainterPathStroker stroker;
    QTransform t;
    t.scale(1, -1);
    t.translate(0, -2 * arrow->line.p2().y());
    s.addPath(arrow->getArrowPath());
    s.moveTo(extLine.p1());
    s.lineTo(extLine.p2());
    s.addPath(t.map(text));
    stroker.setWidth(10);
    return stroker.createStroke(s);
}

QRectF DimRadial::boundingRect() const
{
    // Bounding rectangle of the radial dimension
    qreal extra = 10.0 + textWidth;

    return QRectF(startP, QSizeF(endP.x() - startP.x(), endP.y() - startP.y()))
            .normalized().adjusted(-extra, -extra, extra, extra);
}

void DimRadial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // Draws/paints the radial dimension
    QPen paintpen;
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);

    if (option->state & QStyle::State_Selected)
        paintpen.setStyle(Qt::DashLine);

    paintpen.setColor(Qt::black);
    painter->setPen(paintpen);
    painter->setBrush(Qt::SolidPattern);
    painter->drawPath(arrow->getArrowPath());
    painter->drawLine(extLine);
    painter->scale(1, -1);
    painter->translate(0, -2 * arrow->line.p2().y());
    painter->drawPath(text);
}
