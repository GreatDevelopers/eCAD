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
        m_item = dynamic_cast<Point *>(item);
        oPos = oldPos;
        nPos = newPos;
        setText(QString("Point move (%1,%2) -> (%3,%4)").arg(oPos.x())
                .arg(oPos.y()).arg(nPos.x()).arg(nPos.y()));
    }

    CadCommandMove(QGraphicsLineItem *item, QPointF oldStart, QPointF oldEnd,
                   QPointF newStart,QPointF newEnd)
    {
        m_item = dynamic_cast<Line *>(item);
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
        if (m_item->type() == Point::Type)
        {
            m_item->setPos(oPos);
        }
        if (m_item->type() == Line::Type)
        {
            m_item->setPos(oStart.x() - nStart.x(), oStart.y() - nStart.y());
        }
    }

    virtual void redo()
    {
        if (m_item->type() == Point::Type)
        {
            m_item->setPos(nPos);
        }
        if (m_item->type() == Line::Type)
        {
            m_item->setPos(nStart.x() - oStart.x(), nStart.y() - oStart.y());
        }
    }

private:
    QGraphicsItem *m_item;
    QPointF oPos, nPos;
    QPointF oStart, oEnd, nStart, nEnd;
};

#endif // CADCOMMANDMOVE_H
