/**
 * arc.h
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

#ifndef ARC_H
#define ARC_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "qmath.h"
#include "getEntity.h"

class Arc : public getEntity
{
public:
    Arc(QObject *parent = 0) : getEntity(parent) {}
    Arc(int i, QPointF point1, QPointF point2, QPointF point3);
    Arc(QPointF point1, QPointF point2, QPointF point3);
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    enum { Type = UserType + 6 };
    int type() const;
    getEntity *clone(int);
    QVector<QPointF> getEndPoints();
    QPointF getCenter();
    QPointF getMiddlePoint();

    int id;
    QPointF p1, p2, p3, center;
    QLineF lineBC, lineAC, lineBA, lineOA, lineOB, lineOC;
    QLineF bisectorBC, bisectorBA;
    qreal startAngle, spanAngle, rad;
    QRectF circle, boundingRectTemp;
    QVector<QPointF> endPoints;
    QPointF middlePoint;
    QPainterPath path;

private:
    void init();
};

#endif // ARC_H
