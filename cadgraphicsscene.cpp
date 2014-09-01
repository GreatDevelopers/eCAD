#include "cadgraphicsscene.h"
#include <QGraphicsSceneMouseEvent>

CadGraphicsScene::CadGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void CadGraphicsScene::setMode(Mode mode)
{
    entityMode = mode;
}

void CadGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Point *pointItem;

    switch (entityMode){
    case PointMode:
        pointItem = new Point;
        addItem(pointItem);
        pointItem->setPos(mouseEvent->scenePos());
        emit createdPoint(pointItem);
        break;

    default:
        ;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}
