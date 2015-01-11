/**
 * point.cpp
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

#include "point.h"

Point::Point(QPointF p)
{
    // sets position of point
    position = p;
}

Point::Point(int i, QPointF p)
{
    // assigns id
    id = i;

    // sets position of point
    position = p;
}

int Point::type() const
{
    // Enable the use of qgraphicsitem_cast with point item.
    return Type;
}

QRectF Point::boundingRect() const
{
    // bounding rectangle for point
    qreal penwidth = 1;
    return QRectF(-1 - penwidth / 2, -1 - penwidth / 2,
                  2 + penwidth, 2 + penwidth);
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    // draws/paints the point
    QPen paintpen;

    if (option->state & QStyle::State_Selected)
        paintpen.setColor(Qt::red);
    else
        paintpen.setColor(Qt::black);

    paintpen.setCosmetic(true);
    paintpen.setWidth(2);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::SolidPattern);
    painter->setPen(paintpen);
    painter->drawEllipse(boundingRect());
}

getEntity *Point::clone(int i)
{
    Point *p = new Point;
    p->id = i;
    p->scenePos();
    return p;
}

QPointF Point::getPoint()
{
    // returns point's position
    return position;
}
