#ifndef LINE_H
#define LINE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

class Line: public QObject, public QGraphicsItem{
    Q_OBJECT
public:
    Line(QPointF, QPointF);
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

private:
    QVector<QPointF> stuff;
    QPointF start_p, end_p, move_p, check_p;
    QPen paintpen, linePen;
};

#endif // LINE_H
