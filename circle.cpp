#include "circle.h"

circle::circle()
{
    mFirstClick = true;
    mSecondClick = false;
    mPaintFlag = false;
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF circle::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,1450,1400);
}

void circle::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if(e->button()==Qt::LeftButton) {
        if(mFirstClick){
            x1 = e->pos().x();
            y1 = e->pos().y();
            mFirstClick = false;
            mSecondClick = true;
        }

        else if(!mFirstClick && mSecondClick){
            x2 = e->pos().x();
            y2 = e->pos().y();
            mPaintFlag = true;
            mSecondClick = false;
            update();
            emit DrawFinished();
        }
    }
    QGraphicsItem::mousePressEvent(e);
    update();
}

void circle:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(mPaintFlag){
        QPen paintpen(Qt::red);
        paintpen.setWidth(4);

        QPen linepen(Qt::black);
        linepen.setWidth(1);

        QPoint p1;
        p1.setX(x1);
        p1.setY(y1);

        painter->setPen(paintpen);
        painter->drawPoint(p1);

        QPoint p2;
        p2.setX(move_p.x());
        p2.setY(move_p.y());

        radius = qSqrt(qPow((p2.x()-x1), 2) + qPow((p2.y()-y1), 2));

        painter->setPen(paintpen);
        painter->drawPoint(p2);

        painter->setPen(linepen);
        painter->drawEllipse(p1,radius,radius);
    }
}

void circle::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    move_p = e->pos();
    update();
    if (e->modifiers() & Qt::ShiftModifier) {
        stuff << e->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(e);
}

void circle::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseReleaseEvent(e);
    update();
}
