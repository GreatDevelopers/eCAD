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
    CadCommandMove(QGraphicsItem *item, qreal fromX, qreal fromY,
                   qreal toX, qreal toY)
    {
        m_item = item;
        mFrom = QPointF(fromX, fromY);
        mTo = QPointF(toX, toY);
        setText(QString("Point move (%1,%2) -> (%3,%4)").arg(fromX).arg(fromY)
                .arg(toX).arg(toY));
    }

    virtual void undo()
    {
        m_item->setPos(mFrom);
    }

    virtual void redo()
    {
        m_item->setPos(mTo);
    }

private:
    QGraphicsItem *m_item;
    QPointF mFrom;
    QPointF mTo;
};

#endif // CADCOMMANDMOVE_H
