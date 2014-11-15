#include "line.h"

Line::Line(int i, QPointF p1, QPointF p2) {
    // assigns id
    id = i;

    // set values of start point and end point of line
    startP = p1;
    endP = p2;
}

int Line::type() const {
    // Enable the use of qgraphicsitem_cast with line item.
    return Type;
}

QRectF Line::boundingRect() const {
    qreal extra = 1.0;

    // bounding rectangle for line
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
           .normalized()
           .adjusted(-extra, -extra, extra, extra);
}

void Line::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget) {
    // draws/paints the path of line
    QPen paintpen;
    painter->setRenderHint(QPainter::Antialiasing);
    paintpen.setWidth(1);

    if (isSelected()) {
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
        painter->drawLine(startP, endP);
    } else {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(startP, 2, 2);
        painter->drawEllipse(endP, 2, 2);
        painter->drawLine(startP, endP);
    }
}
