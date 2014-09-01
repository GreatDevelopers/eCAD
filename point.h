#ifndef POINT_H
#define POINT_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class Point: public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    Point();

protected:
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

private:

};

#endif // POINT_H
