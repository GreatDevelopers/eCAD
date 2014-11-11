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
        cadItem = dynamic_cast<Point *>(item);
        oPos = oldPos;
        nPos = newPos;
        setText(QString("Point move (%1,%2) -> (%3,%4)").arg(oPos.x())
                .arg(oPos.y()).arg(nPos.x()).arg(nPos.y()));
    }

    CadCommandMove(QGraphicsLineItem *item, QPointF oldStart, QPointF oldEnd,
                   QPointF newStart,QPointF newEnd)
    {
        cadItem = dynamic_cast<Line *>(item);
        oStart = oldStart;
        oEnd = oldEnd;
        nStart = newStart;
        nEnd = newEnd;
        setText(QString("Line move ((%1,%2),(%3,%4)) -> ((%5,%6),(%7,%8))")
                .arg(oStart.x()).arg(oStart.y()).arg(oEnd.x()).arg(oEnd.y())
                .arg(nStart.x()).arg(nStart.y()).arg(nEnd.x()).arg(nEnd.y()));
    }

    virtual void undo()
    {
        if (cadItem->type() == Point::Type)
        {
            cadItem->setPos(oPos);
        }
        if (cadItem->type() == Line::Type)
        {
            cadItem->setPos(oStart.x() - nStart.x(), oStart.y() - nStart.y());
        }
    }

    virtual void redo()
    {
        if (cadItem->type() == Point::Type)
        {
            cadItem->setPos(nPos);
        }
        if (cadItem->type() == Line::Type)
        {
            cadItem->setPos(nStart.x() - oStart.x(), nStart.y() - oStart.y());
        }
    }

private:
    QGraphicsItem *cadItem;
    QPointF oPos, nPos;
    QPointF oStart, oEnd, nStart, nEnd;
};

#endif // CADCOMMANDMOVE_H
