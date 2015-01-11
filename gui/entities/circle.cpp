/**
 * circle.cpp
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

#include "circle.h"

Circle::Circle(QPointF p1, QPointF p2)
{
    /**
     * set values of center point, end point
     * and calculate radius of circle
    */
    centerP = p1;
    endP = p2;
    radius = qSqrt(qPow((endP.x()-centerP.x()), 2)
                   + qPow((endP.y()-centerP.y()), 2));
}

Circle::Circle(int i, QPointF p1, QPointF p2)
{
    // assigns id
    id = i;

    /**
     * set values of center point, end point
     * and calculate radius of circle
    */
    centerP = p1;
    endP = p2;
    radius = qSqrt(qPow((endP.x()-centerP.x()), 2)
                   + qPow((endP.y()-centerP.y()), 2));
}

Circle::Circle(int i, QPointF p1, qreal rad)
{
    // assigns id
    id = i;

    /**
     * set values of center point
     * and radius of circle
    */
    centerP = p1;
    radius = rad;
}

int Circle::type() const
{
    // Enable the use of qgraphicsitem_cast with circle item.
    return Type;
}

QPainterPath Circle::shape() const
{
    // sets the shape of the circle for selection
    QPainterPath path, path2;
    qreal adjust = 10;
    path.addEllipse(centerP, radius + adjust, radius + adjust);
    path2.addEllipse(centerP, radius - adjust, radius - adjust);
    return path - path2;
}

QRectF Circle::boundingRect() const
{
    // bounding rectangle for circle
    return QRectF((centerP.x() - radius), (centerP.y() - radius),
                  (2 * radius), (2 * radius));
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    // draws/paints the path of circle
    QPen paintpen(Qt::black);
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);

    if (option->state & QStyle::State_Selected)
    {
        // sets brush for center point
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(centerP, 2, 2);

        // sets pen for circumference
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawEllipse(centerP, radius, radius);
    }

    else
    {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(centerP, 2, 2);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(centerP, radius, radius);
    }
}

getEntity *Circle::clone(int i)
{
    Circle *c = new Circle;
    c->id = i;
    c->centerP.x();
    c->centerP.y();
    c->radius = radius;
    return c;
}

QPointF Circle::getCenter()
{
    // returns center of circle
    return centerP;
}
