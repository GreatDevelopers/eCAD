#ifndef CADCOMMANDDELETE_H
#define CADCOMMANDDELETE_H

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

class CadCommandDelete : public QUndoCommand
{
public:
    CadCommandDelete(QGraphicsScene *scene, QGraphicsItem *item)
    {
        cadScene = scene;
        cadItem = item;

        if (cadItem->type() == Point::Type)
        {
            setText(QString("Point delete p(%1,%2)")
                    .arg(cadItem->scenePos().x())
                    .arg(cadItem->scenePos().y()));
        }

        if (cadItem->type() == Line::Type)
        {
            Line *lineItem = dynamic_cast<Line *>(cadItem);
            setText(QString("Line delete p1(%1,%2), p2(%3,%4)")
                    .arg(lineItem->startP.x()).arg(lineItem->startP.y())
                    .arg(lineItem->endP.x()).arg(lineItem->endP.x()));
        }

        if (cadItem->type() == Circle::Type)
        {
            Circle *circleItem = dynamic_cast<Circle *>(cadItem);
            setText(QString("Circle delete c(%1,%2), r(%3)")
                    .arg(circleItem->centerP.x())
                    .arg(circleItem->centerP.y())
                    .arg(circleItem->radius));
        }

        if (cadItem->type() == Ellipse::Type)
        {
            Ellipse *ellipseItem = dynamic_cast<Ellipse *>(cadItem);
            setText(QString("Ellipse delete c(%1,%2), mjR(%3), mnR(%4)")
                    .arg(ellipseItem->p1.x())
                    .arg(ellipseItem->p1.y())
                    .arg(ellipseItem->majRadius)
                    .arg(ellipseItem->minRadius));
        }

        if (cadItem->type() == Text::Type)
        {
            Text *TextItem = dynamic_cast<Text *>(cadItem);
            setText(QString("Text delete"));
        }

        if (cadItem->type() == Arc::Type)
        {
            Arc *arcItem = dynamic_cast<Arc *>(cadItem);
            setText(QString("Arc delete p1(%1,%2), p2(%3,%4), p3(%5,%6)")
                    .arg(arcItem->p1.x())
                    .arg(arcItem->p1.y())
                    .arg(arcItem->p2.x())
                    .arg(arcItem->p2.y())
                    .arg(arcItem->p3.x())
                    .arg(arcItem->p3.y()));
        }

        if (cadItem->type() == Image::Type)
        {
            Image *imageItem = dynamic_cast<Image *>(cadItem);
            setText(QString("Image delete"));
        }

        if (cadItem->type() == DimHorizontal::Type)
        {
            DimHorizontal *dimHorizontalItem =
                    dynamic_cast<DimHorizontal *>(cadItem);
            setText(QString("DimHorizontal delete p1(%1,%2), p2(%3,%4)")
                    .arg(dimHorizontalItem->startP.x())
                    .arg(dimHorizontalItem->endP.y())
                    .arg(dimHorizontalItem->midP.x())
                    .arg(dimHorizontalItem->endP.y()));
        }

        if (cadItem->type() == DimVertical::Type)
        {
            DimVertical *dimVerticalItem = dynamic_cast<DimVertical *>(cadItem);
            setText(QString("DimVertical delete p1(%1,%2), p2(%3,%4)")
                    .arg(dimVerticalItem->endP.x())
                    .arg(dimVerticalItem->startP.y())
                    .arg(dimVerticalItem->endP.x())
                    .arg(dimVerticalItem->midP.y()));
        }
    }

    virtual void undo()
    {
        cadScene->addItem(cadItem);
    }

    virtual void redo()
    {
        cadScene->removeItem(cadItem);
    }

private:
    QGraphicsItem *cadItem;
    QGraphicsScene *cadScene;
};

#endif // CADCOMMANDDELETE_H
