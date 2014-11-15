#ifndef CADCOMMANDDELETE_H
#define CADCOMMANDDELETE_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "mtext.h"

class CadCommandDelete : public QUndoCommand {
    public:
        CadCommandDelete(QGraphicsScene* scene, QGraphicsItem* item) {
            cadScene = scene;
            cadItem = item;

            if (cadItem->type() == Point::Type) {
                setText(QString("Point delete p(%1,%2)")
                        .arg(cadItem->scenePos().x())
                        .arg(cadItem->scenePos().y()));
            }

            if (cadItem->type() == Line::Type) {
                Line* lineItem = dynamic_cast<Line*>(cadItem);
                setText(QString("Line delete p1(%1,%2), p2(%3,%4)")
                        .arg(lineItem->startP.x()).arg(lineItem->startP.y())
                        .arg(lineItem->endP.x()).arg(lineItem->endP.x()));
            }

            if (cadItem->type() == Circle::Type) {
                Circle* circleItem = dynamic_cast<Circle*>(cadItem);
                setText(QString("Circle delete c(%1,%2), r(%3)")
                        .arg(circleItem->centerP.x())
                        .arg(circleItem->centerP.y())
                        .arg(circleItem->radius));
            }

            if (cadItem->type() == Ellipse::Type) {
                Ellipse* ellipseItem = dynamic_cast<Ellipse*>(cadItem);
                setText(QString("Ellipse delete c(%1,%2), mjR(%3), mnR(%4)")
                        .arg(ellipseItem->p1.x())
                        .arg(ellipseItem->p1.y())
                        .arg(ellipseItem->majRadius)
                        .arg(ellipseItem->minRadius));
            }

            if (cadItem->type() == mText::Type) {
                mText* mTextItem = dynamic_cast<mText*>(cadItem);
                setText(QString("Text delete"));
            }
        }

        virtual void undo() {
            cadScene->addItem(cadItem);
        }

        virtual void redo() {
            cadScene->removeItem(cadItem);
        }

    private:
        QGraphicsItem* cadItem;
        QGraphicsScene* cadScene;
};

#endif // CADCOMMANDDELETE_H
