#include "circle.h"

circle::circle()
{
    mFirstClick = true;
    mSecondClick = false;
    mPaintFlag = false;
}

QRectF circle::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,1450,1400);
}

void circle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) {
        if(mFirstClick){
            center_x = event->pos().x();
            center_y = event->pos().y();
            mFirstClick = false;
            mSecondClick = true;
        }

        else if(!mFirstClick && mSecondClick){
            x1 = event->pos().x();
            y1 = event->pos().y();
            mPaintFlag = true;
            mSecondClick = false;
            update();
        }
    }
    QGraphicsItem::mousePressEvent(event);
    update();
}

void circle:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(mPaintFlag){
        QPen paintpen(Qt::red);
        paintpen.setWidth(4);

        QPen linepen(Qt::black);
        linepen.setWidth(1);

        center.setX(center_x);
        center.setY(center_y);

        painter->setPen(paintpen);
        painter->drawPoint(center);

        p1.setX(move_p.x());
        p1.setY(move_p.y());

        radius = qSqrt(qPow((p1.x()-center_x), 2) + qPow((p1.y()-center_y), 2));

        painter->setPen(paintpen);
        painter->drawPoint(p1);

        painter->setPen(linepen);
        painter->drawEllipse(center,radius,radius);
    }
}

void circle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    move_p = event->pos();
    update();
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void circle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
