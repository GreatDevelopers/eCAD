#include "cadgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

CadGraphicsScene::CadGraphicsScene(QObject *parent, QUndoStack *undoStack)
    : QGraphicsScene(parent)
{
    setFlags();
    mUndoStack = undoStack;
}

void CadGraphicsScene::setFlags()
{
    // set/unset all the flags
    mFirstClick = true;
    mSecondClick = false;
    mThirdClick = false;
    mPaintFlag = false;
}

void CadGraphicsScene::setMode(Mode mode)
{
    // determine the mode set
    entityMode = mode;

    if (entityMode == NoMode)
        areItemsSelectable(true);
    else
        areItemsSelectable(false);
}

void CadGraphicsScene::areItemsSelectable(bool b)
{
    foreach (QGraphicsItemGroup* item, groupList)
    {
        item->setFlag(QGraphicsItemGroup::ItemIsSelectable, b);
        item->setFlag(QGraphicsItemGroup::ItemIsMovable, b);
    }
}

void CadGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // mousePressEvent in the graphicsScene
    static int id = 0;

    switch (entityMode)
    {
    case NoMode:
        qDebug() << "No Mode";
        break;

    case PointMode:
        pointItem = new Point(++id);
        pointItem->setPos(mouseEvent->scenePos());
        groupList.append(pointItem);
        mUndoStack->push(new CadCommandAdd(this, pointItem));
        break;

    case LineMode:
        if (mFirstClick)
        {
            start_p = mouseEvent->scenePos();
            mFirstClick = false;
            mSecondClick = true;
        }

        else if (!mFirstClick && mSecondClick)
        {
            end_p = mouseEvent->scenePos();
            mPaintFlag = true;
            mSecondClick = false;
        }

        if (mPaintFlag)
        {
            lineItem = new Line(++id, start_p, end_p);
            groupList.append(lineItem);
            mUndoStack->push(new CadCommandAdd(this, lineItem));
            setFlags();
        }
        break;

    case CircleMode:
        if (mFirstClick)
        {
            start_p = mouseEvent->scenePos();
            mFirstClick = false;
            mSecondClick = true;
        }

        else if (!mFirstClick && mSecondClick)
        {
            end_p = mouseEvent->scenePos();
            mPaintFlag = true;
            mSecondClick = false;
        }

        if (mPaintFlag)
        {
            circleItem = new Circle(++id, start_p, end_p);
            groupList.append(circleItem);
            mUndoStack->push(new CadCommandAdd(this, circleItem));
            setFlags();
        }
        break;

    case EllipseMode:
        if (mFirstClick)
        {
            start_p = mouseEvent->scenePos();
            mFirstClick = false;
            mSecondClick = true;
        }

        else if (!mFirstClick && mSecondClick)
        {
            mid_p = mouseEvent->scenePos();
            mFirstClick = false;
            mSecondClick = false;
            mThirdClick = true;
        }

        else if (!mSecondClick && mThirdClick)
        {
            end_p = mouseEvent->scenePos();
            mThirdClick = false;
            mPaintFlag = true;
        }

        if (mPaintFlag)
        {
            ellipseItem = new Ellipse(++id, start_p, mid_p, end_p);
            groupList.append(ellipseItem);
            mUndoStack->push(new CadCommandAdd(this, ellipseItem));
            setFlags();
        }
        break;

    default:
        ;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CadGraphicsScene::writeStream(QXmlStreamWriter *stream)
{
    foreach (QGraphicsItemGroup* item, groupList )
    {
        if(item->type() == Point::Type )
        {
            Point* myItem = dynamic_cast<Point*>(item);
            stream->writeStartElement("Point");
            stream->writeAttribute("id", QString::number(myItem->id));
            stream->writeAttribute("x", QString::number(myItem->x()));
            stream->writeAttribute("y", QString::number(myItem->y()));
            stream->writeEndElement();  //end of Point Item
        }

        else if(item->type() == Line::Type )
        {
            Line* myItem = dynamic_cast<Line*>(item);
            stream->writeStartElement("Line");
            stream->writeAttribute("id", QString::number(myItem->id));
            stream->writeAttribute("x1", QString::number(myItem->start_p.x()));
            stream->writeAttribute("y1", QString::number(myItem->start_p.y()));
            stream->writeAttribute("x2", QString::number(myItem->end_p.x()));
            stream->writeAttribute("y2", QString::number(myItem->end_p.y()));
            stream->writeEndElement();  //end of Line Item
        }

        else if(item->type() == Circle::Type )
        {
            Circle* myItem = dynamic_cast<Circle*>(item);
            stream->writeStartElement("Circle");
            stream->writeAttribute("id", QString::number(myItem->id));
            stream->writeAttribute("cx", QString::number(myItem->center_p.x()));
            stream->writeAttribute("cy", QString::number(myItem->center_p.y()));
            stream->writeAttribute("radius", QString::number(myItem->radius));
            stream->writeEndElement();  //end of Circle Item
        }

        else if(item->type() == Ellipse::Type )
        {
            Ellipse* myItem = dynamic_cast<Ellipse*>(item);
            stream->writeStartElement("Ellipse");
            stream->writeAttribute("id", QString::number(myItem->id));
            stream->writeAttribute("cx", QString::number(myItem->p1.x()));
            stream->writeAttribute("cy", QString::number(myItem->p1.y()));
            stream->writeAttribute("majR", QString::number(myItem->majRadius));
            stream->writeAttribute("minR", QString::number(myItem->minRadius));
            stream->writeEndElement();  //end of Ellipse Item
        }
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
            foreach (QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "xCoord")
                    x = attribute.value().toString().toDouble();
                if (attribute.name() == "yCoord")
                    y = attribute.value().toString().toDouble();
            }
//            Point *myPoint = new Point;
//            addItem(myPoint);
//            myPoint->setPos(x,y);
        }
    }
}

