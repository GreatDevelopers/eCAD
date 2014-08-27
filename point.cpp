#include "point.h"
#include <QDebug>
#include "mainwindow.h"

point::point()
{
    mClick = true;
    mPaintFlag = false;
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF point::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,1450,1400);
}

void point:: paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(mPaintFlag){
        {
            QPen paintpen(Qt::red);
            paintpen.setWidth(4);
            QPointF p1;
            p1.setX(x1);
            p1.setY(y1);

            painter->setPen(paintpen);
            painter->drawPoint(p1);
            update();
        }
    }
}

void point::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if(e->button()==Qt::LeftButton) {
        if(mClick){
            x1 = e->pos().x();
            y1 = e->pos().y();
            coordinateX = QString::number(x1, 'g', 5);
            coordinateY = QString::number(y1, 'g', 5);
            qDebug() << coordinateX << "," <<coordinateY;
            mClick = false;
            mPaintFlag = true;
            update();
        }
    }
    QGraphicsItem::mousePressEvent(e);
    update();
}

void point::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseMoveEvent(e);
    update();
}

void point::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mouseReleaseEvent(e);
    update();
}
