#include "line.h"
#include <QDebug>
#include <QMouseEvent>

line::line()
{
    mFirstClick = true;
    mSecondClick = false;
    mPaintFlag = false;

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF line::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,1450,1400);
}

void line::mousePressEvent(QGraphicsSceneMouseEvent* e){

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
        check_p = e->pos();
    }

    QGraphicsItem::mousePressEvent(e);
    update();
}

void line:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    setCursor(Qt::CrossCursor);
    if(mPaintFlag){

        paintpen.setColor(Qt::red);
        paintpen.setWidth(6);

        linePen.setColor(Qt::black);
        linePen.setWidth(1);

        painter->setRenderHint(QPainter::Antialiasing, true);

        p1.setX(x1);
        p1.setY(y1);

        painter->setPen(paintpen);
        painter->drawPoint(p1);

        p2.setX(move_p.x());
        p2.setY(move_p.y());

        painter->setPen(paintpen);
        painter->drawPoint(p2);

        painter->setPen(linePen);
        painter->drawLine(p1, p2);
    }
}

void line::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    move_p = e->pos();
    update();

    if(check_p == e->pos())
    {
        qDebug()  << "Item exists";
    }

    if (e->modifiers() & Qt::ShiftModifier) {
        stuff << e->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(e);
}

void line::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseReleaseEvent(e);
    update();
}
