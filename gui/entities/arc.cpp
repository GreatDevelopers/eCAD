/**
 * arc.cpp
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

#include "arc.h"

Arc::Arc(int i, QPointF point1, QPointF point2, QPointF point3)
{
    // assigns id
    id = i;

    /**
     * set values of three points
     * and calls init() to do calculation for arc
     */
    p1 = point1;
    p2 = point2;
    p3 = point3;
    init();
}

int Arc::type() const
{
    // Enable the use of qgraphicsitem_cast with arc item.
    return Type;
}

Arc::Arc(QPointF point1, QPointF point2, QPointF point3)
{
    /**
     * set values of three points
     * and calls init() to do calculation for arc
     */
    p1 = point1;
    p2 = point2;
    p3 = point3;
    init();
}

void Arc::init()
{
    // Calculates startangle and spanangle
    lineBC = QLineF(p2, p3);
    lineAC = QLineF(p1, p3);
    lineBA = QLineF(p2, p1);

    rad = qAbs(lineBC.length()/(2 * qSin(qDegreesToRadians
                                       (lineAC.angleTo(lineBA)))));

    bisectorBC = QLineF(lineBC.pointAt(0.5), lineBC.p2());
    bisectorBC.setAngle(lineBC.normalVector().angle());

    bisectorBA = QLineF(lineBA.pointAt(0.5), lineBA.p2());
    bisectorBA.setAngle(lineBA.normalVector().angle());
    bisectorBA.intersect(bisectorBC, &center);

    circle = QRectF(center.x() - rad, center.y() - rad, rad * 2, rad * 2);

    lineOA = QLineF(center, p1);
    lineOB = QLineF(center, p2);
    lineOC = QLineF(center, p3);

    startAngle = lineOA.angle();
    spanAngle = lineOA.angleTo(lineOC);

    /**
     * Make sure that the span angle covers all three points with the
     * second point in the middle
     */
    if (qAbs(spanAngle) < qAbs(lineOA.angleTo(lineOB)) ||
            qAbs(spanAngle) < qAbs(lineOB.angleTo(lineOC)))
    {
        // swap the end point and invert the spanAngle
        startAngle = lineOC.angle();
        spanAngle = 360 - spanAngle;
    }

    int w = 10;
    boundingRectTemp = circle.adjusted(-w, -w, w, w);

    // sets the path of arc and middle point of its path
    path.arcMoveTo(circle, startAngle);
    path.arcTo(circle, startAngle, spanAngle);
    middlePoint = path.pointAtPercent(0.5);
}

QPainterPath Arc::shape() const
{
    // sets the shape of the arc for selection
    QPainterPathStroker stroker;
    stroker.setWidth(10);
    return stroker.createStroke(path);
}

QRectF Arc::boundingRect() const
{
    // bounding rectangle for arc
    return boundingRectTemp;
}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    QPen paintpen;
    painter->setRenderHint(QPainter::Antialiasing);
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);

    // Draw arc
    if (option->state & QStyle::State_Selected)
    {
        // sets brush for end points
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, 2, 2);
        painter->drawEllipse(p2, 2, 2);
        painter->drawEllipse(p3, 2, 2);

        // sets pen for arc path
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
    }

    else
    {
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::black);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, 2, 2);
        painter->drawEllipse(p2, 2, 2);
        painter->drawEllipse(p3, 2, 2);
    }

    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

getEntity *Arc::clone(int i)
{
    Arc *a = new Arc;
    a->id = i;
    return a;
}

QVector<QPointF> Arc::getEndPoints()
{
    // returns vector containing end points of arc
    endPoints.append(p1);
    endPoints.append(p3);
    return endPoints;
}

QPointF Arc::getCenter()
{
    // returns center of arc
    return center;
}

QPointF Arc::getMiddlePoint()
{
    // returns middle point of arc
    return middlePoint;
}
