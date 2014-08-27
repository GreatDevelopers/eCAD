#include "ellipse.h"

ellipse::ellipse()
{
    mFirstClick = true;
    mSecondClick = false;
    mThirdClick = false;
    mPaintFlag = false;
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF ellipse::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,1450,1400);
}

void ellipse::mousePressEvent(QGraphicsSceneMouseEvent *e)
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
            mFirstClick = false;
            mSecondClick = false;
            mThirdClick = true;

        }

        else if(!mSecondClick && mThirdClick){
            x3 = e->pos().x();
            y3 = e->pos().y();
            mThirdClick = false;
            mPaintFlag = true;
            update();
            emit DrawFinished();
        }
    }
    QGraphicsItem::mousePressEvent(e);
    update();
}

void ellipse::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (e->modifiers() & Qt::ShiftModifier) {
        stuff << e->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(e);
}

void ellipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseReleaseEvent(e);
    update();
}

void ellipse:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
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
        p2.setX(x2);
        p2.setY(y2);

        painter->setPen(paintpen);
        painter->drawPoint(p2);

        QPoint p3;
        p3.setX(x3);
        p3.setY(y3);

        painter->setPen(paintpen);
        painter->drawPoint(p3);

        majRadius = qSqrt(qPow((p2.x()-x1), 2) + qPow((p2.y()-y1), 2));
        minRadius = qSqrt(qPow((p3.x()-x1), 2) + qPow((p3.y()-y1), 2));

        painter->setPen(linepen);
        painter->drawEllipse(p1, majRadius, minRadius);
    }
}
