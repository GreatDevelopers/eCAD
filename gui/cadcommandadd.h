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
        m_item = item;
        m_scene = scene;

        if (m_item->type() == Point::Type)
        {
            setText(QString("Point add p(%1,%2)")
                    .arg(m_item->scenePos().x())
                    .arg(m_item->scenePos().y()));
        }
        if (m_item->type() == Line::Type)
        {
            Line *lineItem = dynamic_cast<Line *>(m_item);
            setText(QString("Line add p1(%1,%2), p2(%3,%4)")
                    .arg(lineItem->start_p.x()).arg(lineItem->start_p.y())
                    .arg(lineItem->end_p.x()).arg(lineItem->end_p.y()));
        }
        if (m_item->type() == Circle::Type)
        {
            Circle *circleItem = dynamic_cast<Circle *>(m_item);
            setText(QString("Circle add c(%1,%2), r(%3)")
                    .arg(circleItem->center_p.x())
                    .arg(circleItem->center_p.y())
                    .arg(circleItem->radius));
        }
        if (m_item->type() == Ellipse::Type)
        {
            Ellipse *ellipseItem = dynamic_cast<Ellipse *>(m_item);
            setText(QString("Ellipse add c(%1,%2), mjR(%3), mnR(%4)")
                    .arg(ellipseItem->p1.x())
                    .arg(ellipseItem->p1.y())
                    .arg(ellipseItem->majRadius)
                    .arg(ellipseItem->minRadius));
        }
        if (m_item->type() == mText::Type)
        {
            mText *mTextItem = dynamic_cast<mText *>(m_item);
            setText(QString("Text added"));
        }
    }

    virtual void undo()
    {
        m_scene->removeItem(m_item);
    }

    virtual void redo()
    {
        m_scene->addItem(m_item);
    }

private:
    QGraphicsItem *m_item;
    QGraphicsScene *m_scene;
};


#endif // CADCOMMANDADD_H
