#include "line.h"

Line::Line(int i, QPointF p1, QPointF p2)
{
    // assigns id
    id = i;

    // set values of start point and end point of line
    start_p = p1;
    end_p = p2;
}

int Line::type() const
{
    // Enable the use of qgraphicsitem_cast with line item.
    return Type;
}

QRectF Line::boundingRect() const
{
    // bounding rectangle for line
    return QRectF(start_p, end_p).normalized();
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // draws/paints the path of line
    QPen paintpen;
    painter->setRenderHint(QPainter::Antialiasing);
    paintpen.setWidth(1);

    if(isSelected())
    {
        // sets brush for end points
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(start_p, 2, 2);
        painter->drawEllipse(end_p, 2, 2);

        // sets pen for line path
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawLine(start_p, end_p);
    }
    else
    {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(start_p, 2, 2);
        painter->drawEllipse(end_p, 2, 2);
        painter->drawLine(start_p, end_p);
    }
}
