#ifndef CADCOMMANDMOVE_H
#define CADCOMMANDMOVE_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "mtext.h"


class CadCommandMove : public QUndoCommand
{
public:
    CadCommandMove(QGraphicsItemGroup *group, qreal fromX, qreal fromY,
                   qreal toX, qreal toY)
    {
        itemGroup = group;
        mFrom = QPointF(fromX, fromY);
        mTo = QPointF(toX, toY);
        setText(QString("Point move (%1,%2) -> (%3,%4)").arg(fromX).arg(fromY)
                .arg(toX).arg(toY));
    }

    virtual void undo()
    {
        itemGroup->setPos(mFrom);
    }

    virtual void redo()
    {
        itemGroup->setPos(mTo);
    }

private:
    QGraphicsItemGroup *itemGroup;
    QPointF mFrom;
    QPointF mTo;
};

#endif // CADCOMMANDMOVE_H
