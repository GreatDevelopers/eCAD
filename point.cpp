#include "point.h"
#include <QDebug>
#include "mainwindow.h"

point::point()
{
    mClick = true;
    mPaintFlag = false;
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

void point::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) {
        if(mClick){
            x1 = event->pos().x();
            y1 = event->pos().y();
            coordinateX = QString::number(x1, 'g', 5);
            coordinateY = QString::number(y1, 'g', 5);
            qDebug() << coordinateX << "," <<coordinateY;
            mClick = false;
            mPaintFlag = true;
            update();
        }
    }
    QGraphicsItem::mousePressEvent(event);
    update();
}

void point::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void point::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
