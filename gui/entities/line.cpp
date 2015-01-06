#include "line.h"

Line::Line(QPointF p1, QPointF p2)
{
    // set values of start point and end point of line
    startP = p1;
    endP = p2;

    linePath();
}

Line::Line(int i, QPointF p1, QPointF p2)
{
    // assigns id
    id = i;

    // set values of start point and end point of line
    startP = p1;
    endP = p2;

    linePath();
}

void Line::linePath()
{
    path.moveTo(startP);
    path.lineTo(endP);
}

int Line::type() const
{
    // Enable the use of qgraphicsitem_cast with line item.
    return Type;
}

QPainterPath Line::shape() const
{
    // sets the shape of the line for selection
    QPainterPathStroker stroker;
    stroker.setWidth(10);
    return stroker.createStroke(path);
}

QRectF Line::boundingRect() const
{
    qreal extra = 10.0;

    // bounding rectangle for line
    return QRectF(startP, QSizeF(endP.x() - startP.x(), endP.y() - startP.y()))
            .normalized().adjusted(-extra, -extra, extra, extra);
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // draws/paints the path of line
    QPen paintpen;
    painter->setRenderHint(QPainter::Antialiasing);
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);

    if (option->state & QStyle::State_Selected)
    {
        // sets brush for end points
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(startP, 2, 2);
        painter->drawEllipse(endP, 2, 2);

        // sets pen for line path
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawPath(path);
    }

    else
    {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(startP, 2, 2);
        painter->drawEllipse(endP, 2, 2);
        painter->drawPath(path);
    }
}

getEntity *Line::clone(int i)
{
    Line *l = new Line;
    l->id = i;
    l->startP;
    l->endP;
    return l;
}

QVector<QPointF> Line::getEndPoints()
{
    // returns vector containing end points of line
    endPoints.append(startP);
    endPoints.append(endP);
    return endPoints;
}

QPointF Line::getMiddlePoint()
{
    // returns middle point of line
    midP.setX((startP.x() + endP.x()) / 2);
    midP.setY((startP.y() + endP.y()) / 2);
    return midP;
}
