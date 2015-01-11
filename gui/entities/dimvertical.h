/**
 * dimvertical.h
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
