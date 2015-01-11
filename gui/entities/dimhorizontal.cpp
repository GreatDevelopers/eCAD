/**
 * dimhorizontal.cpp
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

#include "dimhorizontal.h"

DimHorizontal::DimHorizontal(int i, QPointF p1, QPointF p2, QPointF p3)
{
    // Assigns id
    id = i;

    // Set values of startPoint, midPoint and endPoint
    if (p2.x() > p1.x())
    {
        startP = p1;
        midP = p2;
    }

    else
    {
        startP = p2;
        midP = p1;
    }

    endP = p3;

    eval();
}

DimHorizontal::DimHorizontal(QPointF p1, QPointF p2, QPointF p3)
{
    // Set values of startPoint, midPoint and endPoint
    if (p2.x() > p1.x())
    {
        startP = p1;
        midP = p2;
    }

    else
    {
        startP = p2;
        midP = p1;
    }

    endP = p3;

    eval();
}

void DimHorizontal::eval()
{
    // Creates arrow and extension lines
    arrow = new Arrow(1, startP, midP, endP);

    extLine1.setLine(startP.x(), startP.y(), startP.x(), endP.y());
    extLine2.setLine(midP.x(), midP.y(), midP.x(), endP.y());
}

int DimHorizontal::type() const
{
    // Enable the use of qgraphicsitem_cast with horizontal dimension item.
    return Type;
}

QPainterPath DimHorizontal::shape() const
{
    QPainterPath s;
    QPainterPathStroker stroker;
    QTransform t;
    t.scale(1, -1);
    t.translate(0, -2 * arrow->middle.y());
    s.addPath(t.map(arrow->getArrowPath()));
    s.addPath(extLine1.shape());
    s.addPath(extLine2.shape());
    stroker.setWidth(10);
    return stroker.createStroke(s);
}

QRectF DimHorizontal::boundingRect() const
{
    // Bounding rectangle of the horizontal dimension
    qreal extra = 10.0 + arrow->textHeight;

    if (startP.y() > midP.y())
    {
        if (endP.y() > midP.y() && endP.y() > startP.y())
        {
            return QRectF(QPointF(startP.x(), endP.y()),
                          QSizeF(midP.x() - startP.x(), midP.y() - endP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else if (endP.y() > midP.y() && endP.y() < startP.y())
        {
            return QRectF(QPointF(startP.x(), startP.y()),
                          QSizeF(midP.x() - startP.x(), midP.y() - startP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else
        {
            return QRectF(QPointF(startP.x(), startP.y()),
                          QSizeF(midP.x() - startP.x(), endP.y() - startP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }
    }

    else if (startP.y() < midP.y())
    {
        if (endP.y() > midP.y() && endP.y() > startP.y())
        {
            return QRectF(QPointF(startP.x(), endP.y()),
                          QSizeF(midP.x() - startP.x(), startP.y() - endP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else if (endP.y() < midP.y() && endP.y() < startP.y())
        {
            return QRectF(QPointF(startP.x(), midP.y()),
                          QSizeF(midP.x() - startP.x(), endP.y() - midP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }

        else
        {
            return QRectF(QPointF(startP.x(), midP.y()),
                          QSizeF(midP.x() - startP.x(), startP.y() - midP.y()))
                    .normalized().adjusted(-extra, -extra, extra, extra);
        }
    }
}

void DimHorizontal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // Draws/paints the horizontal dimension
    QPen paintpen;
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);

    if (option->state & QStyle::State_Selected)
        paintpen.setStyle(Qt::DashLine);

    paintpen.setColor(Qt::black);
    painter->setPen(paintpen);
    painter->drawLine(startP.x(), startP.y(), startP.x(), endP.y());
    painter->drawLine(midP.x(), midP.y(), midP.x(), endP.y());
    painter->setBrush(Qt::SolidPattern);
    painter->drawPath(arrow->getArrowPath());
    painter->scale(1, -1);
    painter->translate(0, -2 * arrow->middle.y());
    painter->drawText(arrow->middle, arrow->value);
}
