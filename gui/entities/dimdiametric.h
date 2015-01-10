#ifndef DIMDIAMETRIC_H
#define DIMDIAMETRIC_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFont>

#include "getEntity.h"
#include "qmath.h"
#include "arrow.h"

class DimDiametric: public getEntity
{
    Q_OBJECT

public:
    DimDiametric(QObject *parent = 0) : getEntity(parent) {}
    DimDiametric(int i, qreal, QPointF, QPointF);
    DimDiametric(qreal, QPointF, QPointF);
    QPainterPath shape() const;
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 11 };
    int type() const;
    void eval(qreal);

    int id;
    QString diaValue;
    qreal dia, textPadding, textHeight, textWidth;
    Arrow *arrow;
    QLineF extLine;
    QPointF startP, endP, textPoint;
    QFont font;
    QPainterPath text;
};

#endif // DIMDIAMETRIC_H
