#ifndef LINE_H
#define LINE_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "qmath.h"
#include "getEntity.h"

class Line : public getEntity
{
    Q_OBJECT

public:
    Line(QObject *parent = 0) : getEntity(parent) {}
    Line(int, QPointF, QPointF);
    Line(QPointF, QPointF);
    QPainterPath shape() const;
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 2 };
    int type() const;
    getEntity *clone(int);
    QVector<QPointF> getEndPoints();
    QPointF getMiddlePoint();

    int id;
    QPointF startP, endP, midP;
    QVector<QPointF> endPoints;
};

#endif // LINE_H
