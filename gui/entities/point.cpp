#include "point.h"

Point::Point(int i, QPointF p)
{
    // assigns id
    id = i;

    // sets position of point
    position = p;
}

int Point::type() const
{
    // Enable the use of qgraphicsitem_cast with point item.
    return Type;
}

QRectF Point::boundingRect() const
{
    // bounding rectangle for point
    qreal penwidth = 1;
    return QRectF(-1 - penwidth / 2, -1 - penwidth / 2,
                  2 + penwidth, 2 + penwidth);
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    // draws/paints the point
    QPen paintpen;

    if (option->state & QStyle::State_Selected)
        paintpen.setColor(Qt::red);
    else
        paintpen.setColor(Qt::black);

    paintpen.setWidth(2);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::SolidPattern);
    painter->setPen(paintpen);
    painter->drawEllipse(boundingRect());
}

getEntity *Point::clone(int i)
{
    Point *p = new Point;
    p->id = i;
    p->scenePos();
    return p;
}
