#ifndef DIMRADIAL_H
#define DIMRADIAL_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFont>

#include "getEntity.h"
#include "qmath.h"
#include "arrow.h"

class DimRadial: public getEntity
{
    Q_OBJECT

public:
    DimRadial(QObject *parent = 0) : getEntity(parent) {}
    DimRadial(int i, qreal, QPointF, QPointF);
    DimRadial(qreal, QPointF, QPointF);
    QPainterPath shape() const;
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 10 };
    int type() const;
    void eval(qreal);

    int id;
    QString radValue;
    qreal rad, textPadding, textHeight, textWidth;
    Arrow *arrow;
    QLineF extLine;
    QPointF startP, endP, textPoint;
    QFont font;
    QPainterPath text;
};

#endif // DIMRADIAL_H
