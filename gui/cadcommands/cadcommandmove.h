#ifndef CADCOMMANDMOVE_H
#define CADCOMMANDMOVE_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "mtext.h"

class CadCommandMove : public QUndoCommand
{
public:
    CadCommandMove(QGraphicsItem *item, QPointF oldPos, QPointF newPos)
    {
        oPos = oldPos;
        nPos = newPos;

        cadItem = dynamic_cast<QGraphicsItem *>(item);
        if (cadItem->type() == Point::Type)
        {
            pointItem = dynamic_cast<Point *>(cadItem);
            setText(QString("Point moved to (%1,%2)")
                    .arg(nPos.x()).arg(nPos.y()));
        }
        if (cadItem->type() == Circle::Type)
        {
            circleItem = dynamic_cast<Circle *>(cadItem);
            setText(QString("Circle's center moved to (%1,%2)")
                    .arg(nPos.x() + circleItem->centerP.x())
                    .arg(nPos.y() + circleItem->centerP.y()));
        }
        if (cadItem->type() == Ellipse::Type)
        {
            ellipseItem = dynamic_cast<Ellipse *>(cadItem);
            setText(QString("Ellipse's center moved to (%1,%2)")
                    .arg(nPos.x() + ellipseItem->p1.x())
                    .arg(nPos.y() + ellipseItem->p1.y()));
        }
        if (cadItem->type() == mText::Type)
        {
            textItem = dynamic_cast<mText *>(cadItem);
            setText(QString("Text moved to (%1,%2)")
                    .arg(nPos.x()).arg(nPos.y()));
        }
    }

    CadCommandMove(QGraphicsLineItem *item, QPointF oldStart, QPointF oldEnd,
                   QPointF newStart,QPointF newEnd)
    {
        cadItem = dynamic_cast<Line *>(item);
        oStart = oldStart;
        oEnd = oldEnd;
        nStart = newStart;
        nEnd = newEnd;
        setText(QString("Line moved to p1((%1,%2), p2(%3,%4))")
                .arg(nStart.x()).arg(nStart.y()).arg(nEnd.x()).arg(nEnd.y()));
    }

    virtual void undo()
    {
        if (cadItem->type() == Line::Type)
        {
            cadItem->setPos(oStart.x() - nStart.x(), oStart.y() - nStart.y());
        }
        else
        {
            cadItem->setPos(oPos);
        }
    }

    virtual void redo()
    {
        if (cadItem->type() == Line::Type)
        {
            cadItem->setPos(nStart.x() - oStart.x(), nStart.y() - oStart.y());
        }
        else
        {
            cadItem->setPos(nPos);
        }
    }

private:
    QGraphicsItem *cadItem;
    QPointF oPos, nPos;
    QPointF oStart, oEnd, nStart, nEnd;
    Point *pointItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
    mText *textItem;
};

#endif // CADCOMMANDMOVE_H
