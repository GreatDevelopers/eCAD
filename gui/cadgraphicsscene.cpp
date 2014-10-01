#include "cadgraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

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
    case NoMode:
        qDebug() << "No Mode" ;
        break;

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

void CadGraphicsScene::writeStream(QXmlStreamWriter *stream)
{
    foreach(QGraphicsItem* item, items())
    {
//        if(item->type() == Point::Type )
//        {
            Point* myItem = dynamic_cast<Point*>(item);
            stream->writeStartElement("Point");
            stream->writeAttribute("xCoord", QString::number(myItem->x()));
            stream->writeAttribute("yCoord", QString::number(myItem->y()));
            stream->writeEndElement();  //end of Point Item
//        }
    }
}

void CadGraphicsScene::readStream(QXmlStreamReader *stream)
{
    while (!stream->atEnd())
    {
        stream->readNext();
        if (stream->isStartElement() && stream->name() == "Point")
        {
            qreal x = 0.0, y = 0.0;
            foreach(QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "xCoord") x = attribute.value().toString().toDouble();
                if (attribute.name() == "yCoord") y = attribute.value().toString().toDouble();
            }
            Point *myPoint = new Point;
            addItem(myPoint);
            myPoint->setPos(x,y);
        }
    }
}
