#ifndef DIMVERTICAL_H
#define DIMVERTICAL_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFont>

#include "getEntity.h"
#include "qmath.h"
#include "arrow.h"

class DimVertical: public getEntity
{
    Q_OBJECT

public:
    DimVertical(QObject *parent = 0) : getEntity(parent) {}
    DimVertical(int i, QPointF, QPointF, QPointF);
    DimVertical(QPointF, QPointF, QPointF);
    QPainterPath shape() const;
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 9 };
    int type() const;
    void eval();

    int id;
    Arrow *arrow;
    QPointF startP, midP, endP;
    QGraphicsLineItem extLine1, extLine2;
};

#endif // DIMVERTICAL_H
