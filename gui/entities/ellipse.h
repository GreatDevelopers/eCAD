#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QPainter>
#include <QGraphicsItem>

#include "qmath.h"
#include "getEntity.h"
#include "clipboardstack.h"

class Ellipse : public getEntity {
        Q_OBJECT
    public:
        Ellipse(QObject* parent = 0) : getEntity(parent) {}
        Ellipse(int, QPointF, QPointF, QPointF);
        Ellipse(int, QPointF, qreal, qreal);
        QRectF boundingRect() const;
        virtual void paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget);
        enum { Type = UserType + 4 };
        int type() const;
        int id;

        QPointF p1, p2, p3;
        float d12, d13, majRadius, minRadius;
        float theta;

        getEntity* clone();

    private:
        QVector<QPointF> stuff;
};

#endif // ELLIPSE_H
