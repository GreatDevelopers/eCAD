/**
 * line.h
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
    void linePath();

    int id;
    QPointF startP, endP, midP;
    QVector<QPointF> endPoints;
    QPainterPath path;
};

#endif // LINE_H
