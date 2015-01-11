/**
 * point.h
 *
 * Copyright (C) 2014  GreatDevelopers
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

#ifndef POINT_H
#define POINT_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "getEntity.h"

class Point : public getEntity
{
    Q_OBJECT

public:
    Point(QObject *parent = 0) : getEntity(parent) {}
    Point(int, QPointF);
    Point(QPointF);
    enum { Type = UserType + 1 };
    int type() const;
    getEntity *clone(int);
    QPointF getPoint();

    int id;
    QPointF position;

protected:
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
};

#endif // POINT_H
