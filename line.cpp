#include "line.h"
#include <QDebug>
#include <QMouseEvent>

Line::Line()
{
    mFirstClick = true;
    mSecondClick = false;
    mPaintFlag = false;

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF Line::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,1450,1400);
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent* event){

    if(event->button()==Qt::LeftButton) {
        if(mFirstClick){

            start_x = event->pos().x();
            start_y = event->pos().y();

            mFirstClick = false;
            mSecondClick = true;
        }

        else if(!mFirstClick && mSecondClick){
            end_x = event->pos().x();
            end_y = event->pos().y();
            mPaintFlag = true;
            mSecondClick = false;
            update();
        }
        check_p = event->pos();
    }

    QGraphicsItem::mousePressEvent(event);
    update();
}

void Line:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //setCursor(Qt::CrossCursor);
    if(mPaintFlag){

        paintpen.setColor(Qt::red);
        paintpen.setWidth(6);

        linePen.setColor(Qt::black);
        linePen.setWidth(1);

        painter->setRenderHint(QPainter::Antialiasing, true);

        start_p.setX(start_x);
        start_p.setY(start_y);

        painter->setPen(paintpen);
        painter->drawPoint(start_p);

        end_p.setX(move_p.x());
        end_p.setY(move_p.y());

        painter->setPen(paintpen);
        painter->drawPoint(end_p);

        painter->setPen(linePen);
        painter->drawLine(start_p, end_p);
    }
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    move_p = event->pos();
    update();

    if(check_p == event->pos())
    {
        qDebug()  << "Item exists";
    }

    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
