#ifndef CADCOMMANDDELETE_H
#define CADCOMMANDDELETE_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"

class CadCommandDelete : public QUndoCommand
{
public:
    CadCommandDelete(QGraphicsScene *scene, QGraphicsItemGroup *group )
    {
        m_scene   = scene;
        itemGroup = group;
        if (itemGroup->type() == Point::Type)
        {
            setText(QString("Point delete p(%1,%2)")
                    .arg(itemGroup->scenePos().x())
                    .arg(itemGroup->scenePos().y()));
        }
        if (itemGroup->type() == Line::Type)
        {
            Line *lineGroup = dynamic_cast<Line *>(itemGroup);
            setText(QString("Line delete p1(%1,%2), p2(%3,%4)")
                    .arg(lineGroup->start_p.x()).arg(lineGroup->start_p.y())
                    .arg(lineGroup->end_p.x()).arg(lineGroup->end_p.x()));
        }
        if (itemGroup->type() == Circle::Type)
        {
            Circle *circleGroup = dynamic_cast<Circle *>(itemGroup);
            setText(QString("Circle delete c(%1,%2), r(%3)")
                    .arg(circleGroup->center_p.x())
                    .arg(circleGroup->center_p.y())
                    .arg(circleGroup->radius));
        }
        if (itemGroup->type() == Ellipse::Type)
        {
            Ellipse *ellipseGroup = dynamic_cast<Ellipse *>(itemGroup);
            setText(QString("Ellipse delete c(%1,%2), mjR(%3), mnR(%4)")
                    .arg(ellipseGroup->p1.x())
                    .arg(ellipseGroup->p1.y())
                    .arg(ellipseGroup->majRadius)
                    .arg(ellipseGroup->minRadius));
        }
    }

    virtual void undo()
    {
        m_scene->addItem(itemGroup);
    }

    virtual void redo()
    {
        m_scene->removeItem(itemGroup);
    }

private:
    QGraphicsItemGroup *itemGroup;
    QGraphicsScene *m_scene;
};

#endif // CADCOMMANDDELETE_H
