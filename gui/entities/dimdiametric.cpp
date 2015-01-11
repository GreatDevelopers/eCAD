/**
 * dimdiametric.cpp
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

#include "dimdiametric.h"

DimDiametric::DimDiametric(int i, qreal v, QPointF p1, QPointF p2)
{
    // Assigns id
    id = i;

    // Set values of startPoint and endPoint
    startP = p1;
    endP = p2;

    eval(v);
}

DimDiametric::DimDiametric(qreal v, QPointF p1, QPointF p2)
{
    // Set values of startPoint and endPoint
    startP = p1;
    endP = p2;

    eval(v);
}

void DimDiametric::eval(qreal v)
{
    // Creates arrow and set value of diameter
    arrow = new Arrow(3, startP, endP);
    dia = v;
    diaValue = "D " + QString::number(v);

    // Set the font and font metrics for the text
    font.setPointSize(10);
    font.setWeight(25);
    QFontMetrics fm(font);
    textHeight = fm.height();
    textWidth = fm.width(diaValue);

    if (arrow->line.p1().x() > arrow->line.p2().x())
    {
        extLine.setLine(arrow->line.p2().x(), arrow->line.p2().y(),
                        arrow->line.p2().x() - 10, arrow->line.p2().y());
        textPadding = -textWidth - 10;
    }

    else
    {
        extLine.setLine(arrow->line.p2().x(), arrow->line.p2().y(),
                        arrow->line.p2().x() + 10, arrow->line.p2().y());
        textPadding = 10;
    }

    textPoint.setX(arrow->line.p2().x() + textPadding);
    textPoint.setY(arrow->line.p2().y() + textHeight / 4);
    text.addText(textPoint, font, diaValue);
}

int DimDiametric::type() const
{
    // Enable the use of qgraphicsitem_cast with diametric dimension item.
    return Type;
}

QPainterPath DimDiametric::shape() const
{
    // Returns the shape of the diametric dimension
    QPainterPath s;
    QPainterPathStroker stroker;
    QTransform t;
    t.scale(1, -1);
    t.translate(0, -2 * arrow->line.p2().y());
    s.addPath(arrow->getArrowPath());
    s.moveTo(extLine.p1());
    s.lineTo(extLine.p2());
    s.addPath(t.map(text));
    stroker.setWidth(10);
    return stroker.createStroke(s);
}

QRectF DimDiametric::boundingRect() const
{
    // Bounding rectangle of the diametric dimension
    qreal extra = 10.0 + textWidth;

    return QRectF(startP, QSizeF(endP.x() - startP.x(), endP.y() - startP.y()))
            .normalized().adjusted(-extra, -extra, extra, extra);
}

void DimDiametric::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    // Draws/paints the diametric dimension
    QPen paintpen;
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);

    if (option->state & QStyle::State_Selected)
        paintpen.setStyle(Qt::DashLine);

    paintpen.setColor(Qt::black);
    painter->setPen(paintpen);
    painter->setBrush(Qt::SolidPattern);
    painter->drawPath(arrow->getArrowPath());
    painter->drawLine(extLine);
    painter->scale(1, -1);
    painter->translate(0, -2 * arrow->line.p2().y());
    painter->drawPath(text);
}
