#ifndef POINT_H
#define POINT_H

#include <QPainter>
#include <QGraphicsItemGroup>

class Point: public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    Point(int);
     enum { Type = UserType + 1 };
     int type() const;
     int id;

protected:
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget);
};

#endif // POINT_H
