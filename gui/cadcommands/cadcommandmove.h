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
