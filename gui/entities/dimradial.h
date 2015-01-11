/**
 * dimradial.h
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
