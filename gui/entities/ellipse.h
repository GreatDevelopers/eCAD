/**
 * ellipse.h
 *
 * Copyright (C) 2014 GreatDevelopers
 *
 * eCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "qmath.h"
#include "getEntity.h"

class Ellipse : public getEntity
{
    Q_OBJECT

public:
    Ellipse(QObject *parent = 0) : getEntity(parent) {}
    Ellipse(int, QPointF, QPointF, QPointF);
    Ellipse(int, QPointF, qreal, qreal, qreal);
    Ellipse(QPointF, QPointF, QPointF);
    void calculate();
    QPainterPath shape() const;
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 4 };
    int type() const;
    getEntity *clone(int);
    QPointF getCenter();

    int id;
    QPointF p1, p2, p3, topLeft, bottomRight;
    qreal d12, d13, majRadius, minRadius;
    qreal theta;
};

#endif // ELLIPSE_H
