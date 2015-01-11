/**
 * cadcommandmove.h
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

#ifndef CADCOMMANDMOVE_H
#define CADCOMMANDMOVE_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "text.h"
#include "arc.h"
#include "image.h"
#include "dimhorizontal.h"
#include "dimvertical.h"
#include "dimradial.h"
#include "dimdiametric.h"

class CadCommandMove : public QUndoCommand
{
public:
    CadCommandMove(QGraphicsItem *item, QPointF oldPos, QPointF newPos)
    {
        oPos = oldPos;
        nPos = newPos;
        cadItem = item;

        if (cadItem->type() == Point::Type)
        {
            Point *pointItem = dynamic_cast<Point *>(cadItem);
            setText(QString("Point moved to (%1,%2)")
                    .arg(nPos.x()).arg(nPos.y()));
        }

        if (cadItem->type() == Line::Type)
        {
            Line *lineItem = dynamic_cast<Line *>(cadItem);
            setText(QString("Line moved to p1((%1,%2), p2(%3,%4))")
                    .arg(lineItem->startP.x() + nPos.x())
                    .arg(lineItem->startP.y() + nPos.y())
                    .arg(lineItem->endP.x() + nPos.x())
                    .arg(lineItem->endP.y() + nPos.y()));
        }

        if (cadItem->type() == Circle::Type)
        {
            Circle *circleItem = dynamic_cast<Circle *>(cadItem);
            setText(QString("Circle's center moved to (%1,%2)")
                    .arg(nPos.x() + circleItem->centerP.x())
                    .arg(nPos.y() + circleItem->centerP.y()));
        }

        if (cadItem->type() == Ellipse::Type)
        {
            Ellipse *ellipseItem = dynamic_cast<Ellipse *>(cadItem);
            setText(QString("Ellipse's center moved to (%1,%2)")
                    .arg(nPos.x() + ellipseItem->p1.x())
                    .arg(nPos.y() + ellipseItem->p1.y()));
        }

        if (cadItem->type() == Text::Type)
        {
            Text *textItem = dynamic_cast<Text *>(cadItem);
            setText(QString("Text moved to (%1,%2)")
                    .arg(nPos.x()).arg(nPos.y()));
        }

        if (cadItem->type() == Arc::Type)
        {
            Arc *arcItem = dynamic_cast<Arc *>(cadItem);
            setText(QString("Arc moved to p1(%1,%2), p2(%3,%4), p3(%5,%6)")
                    .arg(nPos.x() + arcItem->p1.x())
                    .arg(nPos.y() + arcItem->p1.y())
                    .arg(nPos.x() + arcItem->p2.x())
                    .arg(nPos.y() + arcItem->p2.y())
                    .arg(nPos.x() + arcItem->p3.x())
                    .arg(nPos.y() + arcItem->p3.y()));
        }

        if (cadItem->type() == Image::Type)
        {
            Image *imageItem = dynamic_cast<Image *>(cadItem);
            setText(QString("Image moved to (%1,%2)")
                    .arg(nPos.x() + imageItem->startP.x())
                    .arg(nPos.y() + imageItem->startP.y()));
        }

        if (cadItem->type() == DimHorizontal::Type)
        {
            DimHorizontal *dimHorizontalItem =
                    dynamic_cast<DimHorizontal *>(cadItem);
            setText(QString("DimHorizontal moved to p1(%1,%2), p2(%3,%4)")
                    .arg(dimHorizontalItem->startP.x() + nPos.x())
                    .arg(dimHorizontalItem->endP.y() + nPos.y())
                    .arg(dimHorizontalItem->midP.x() + nPos.x())
                    .arg(dimHorizontalItem->endP.y() + nPos.y()));
        }

        if (cadItem->type() == DimVertical::Type)
        {
            DimVertical *dimVerticalItem = dynamic_cast<DimVertical *>(cadItem);
            setText(QString("DimVertical moved to p1(%1,%2), p2(%3,%4)")
                    .arg(dimVerticalItem->endP.x() + nPos.x())
                    .arg(dimVerticalItem->startP.y() + nPos.y())
                    .arg(dimVerticalItem->endP.x() + nPos.x())
                    .arg(dimVerticalItem->midP.y() + nPos.y()));
        }

        if (cadItem->type() == DimRadial::Type)
        {
            DimRadial *dimRadialItem = dynamic_cast<DimRadial *>(cadItem);
            setText(QString("DimRadial moved to p1(%1,%2), p2(%3,%4)")
                    .arg(dimRadialItem->startP.x() + nPos.x())
                    .arg(dimRadialItem->startP.y() + nPos.y())
                    .arg(dimRadialItem->endP.x() + nPos.x())
                    .arg(dimRadialItem->endP.y() + nPos.y()));
        }

        if (cadItem->type() == DimDiametric::Type)
        {
            DimDiametric *dimDiametricItem =
                    dynamic_cast<DimDiametric *>(cadItem);
            setText(QString("DimDiametric moved to p1(%1,%2), p2(%3,%4)")
                    .arg(dimDiametricItem->startP.x() + nPos.x())
                    .arg(dimDiametricItem->startP.y() + nPos.y())
                    .arg(dimDiametricItem->endP.x() + nPos.x())
                    .arg(dimDiametricItem->endP.y() + nPos.y()));
        }
    }

    virtual void undo()
    {
        cadItem->setPos(oPos);
    }

    virtual void redo()
    {
        cadItem->setPos(nPos);
    }

private:
    QGraphicsItem *cadItem;
    QPointF oPos, nPos;
    QPointF oStart, oEnd, nStart, nEnd;
};

#endif // CADCOMMANDMOVE_H
