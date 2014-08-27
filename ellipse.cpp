#include "ellipse.h"

ellipse::ellipse()
{
    mFirstClick = true;
    mSecondClick = false;
    mThirdClick = false;
    mPaintFlag = false;
}

QRectF ellipse::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,1450,1400);
}

void ellipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) {
        if(mFirstClick){
            center_x = event->pos().x();
            center_y= event->pos().y();
            mFirstClick = false;
            mSecondClick = true;
        }

        else if(!mFirstClick && mSecondClick){
            x1 = event->pos().x();
            y1 = event->pos().y();
            mFirstClick = false;
            mSecondClick = false;
            mThirdClick = true;

        }

        else if(!mSecondClick && mThirdClick){
            x2 = event->pos().x();
            y2 = event->pos().y();
            mThirdClick = false;
            mPaintFlag = true;
            update();
        }
    }
    QGraphicsItem::mousePressEvent(event);
    update();
}

void ellipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ellipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void ellipse:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(mPaintFlag){
        QPen paintpen(Qt::red);
        paintpen.setWidth(4);

        QPen linepen(Qt::black);
        linepen.setWidth(1);

        center.setX(center_x);
        center.setY(center_y);

        painter->setPen(paintpen);
        painter->drawPoint(center);

        p1.setX(x1);
        p1.setY(y1);

        painter->setPen(paintpen);
        painter->drawPoint(p1);

        p2.setX(x2);
        p2.setY(y2);

        painter->setPen(paintpen);
        painter->drawPoint(p2);

        majRadius = qSqrt(qPow((p1.x()-center_x), 2) + qPow((p2.y()-center_y), 2));
        minRadius = qSqrt(qPow((p2.x()-center_x), 2) + qPow((p2.y()-center_y), 2));

        painter->setPen(linepen);
        painter->drawEllipse(center, majRadius, minRadius);
    }
}
