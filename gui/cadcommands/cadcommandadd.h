#ifndef CADCOMMANDADD_H
#define CADCOMMANDADD_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "mtext.h"

class CadCommandAdd : public QUndoCommand
{
public:
    CadCommandAdd(QGraphicsScene *scene, QGraphicsItem *item)
    {
        cadItem = item;
        cadScene = scene;

        if (cadItem->type() == Point::Type)
        {
            setText(QString("Point add p(%1,%2)")
                    .arg(cadItem->scenePos().x())
                    .arg(cadItem->scenePos().y()));
        }

        if (cadItem->type() == Line::Type)
        {
            Line *lineItem = dynamic_cast<Line *>(cadItem);
            setText(QString("Line add p1(%1,%2), p2(%3,%4)")
                    .arg(lineItem->startP.x()).arg(lineItem->startP.y())
                    .arg(lineItem->endP.x()).arg(lineItem->endP.y()));
        }

        if (cadItem->type() == Circle::Type)
        {
            Circle *circleItem = dynamic_cast<Circle *>(cadItem);
            setText(QString("Circle add c(%1,%2), r(%3)")
                    .arg(circleItem->centerP.x())
                    .arg(circleItem->centerP.y())
                    .arg(circleItem->radius));
        }

        if (cadItem->type() == Ellipse::Type)
        {
            Ellipse *ellipseItem = dynamic_cast<Ellipse *>(cadItem);
            setText(QString("Ellipse add c(%1,%2), mjR(%3), mnR(%4)")
                    .arg(ellipseItem->p1.x())
                    .arg(ellipseItem->p1.y())
                    .arg(ellipseItem->majRadius)
                    .arg(ellipseItem->minRadius));
        }

        if (cadItem->type() == mText::Type)
        {
            mText *mTextItem = dynamic_cast<mText *>(cadItem);
            setText(QString("Text add p(%1,%2)")
                    .arg(mTextItem->scenePos().x())
                    .arg(mTextItem->scenePos().y()));
        }
    }

    virtual void undo()
    {
        cadScene->removeItem(cadItem);
    }

    virtual void redo()
    {
        cadScene->addItem(cadItem);
    }

private:
    QGraphicsItem *cadItem;
    QGraphicsScene *cadScene;
};

#endif // CADCOMMANDADD_H
