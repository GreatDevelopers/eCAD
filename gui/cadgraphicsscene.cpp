#include "cadgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QDebug>

MyClipBoard *MyClipBoard::inst = 0;

CadGraphicsScene::CadGraphicsScene(QObject *parent, QUndoStack *undoStack)
    : QGraphicsScene(parent)
{
    setFlags();
    id = 0;
    mUndoStack = undoStack;
    contextmenu = new QMenu;
    cutaction = contextmenu->addAction("cut");
    copyaction = contextmenu->addAction("copy");
    pasteaction = contextmenu->addAction("paste");
    contextitem = 0;

    //connect context menu items to action slots
    connect(contextmenu, SIGNAL(triggered(QAction*)),
            this, SLOT(contextMenuaction(QAction*)));
    // connect selectionChanged signal to selectItems slot
    connect(this, SIGNAL(selectionChanged()), this, SLOT(selectItems()));
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

void CadGraphicsScene::editorLostFocus(mText *item)
{
    //sets the text cursor
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);
}

void CadGraphicsScene::areItemsSelectable(bool b)
{
    // make items selectable
    foreach (QGraphicsItem *item, itemList)
    {
        item->setFlag(QGraphicsItem::ItemIsSelectable, b);
        item->setFlag(QGraphicsItem::ItemIsMovable, b);
    }
}

void CadGraphicsScene::deleteItems()
{
    // delete selected items
    foreach (QGraphicsItem *item, itemList)
    {
        if (item->isSelected())
        {
            mUndoStack->push(new CadCommandDelete(this, item));
            item->setSelected(false);
        }
    }
}

void CadGraphicsScene::selectItems()
{
    // refresh record of selected items and their starting positions
    selectedPoints.clear();
    selectedLines.clear();
    foreach (QGraphicsItem *item, itemList)
    {
        if (item->isSelected())
        {
            if (item->type() == Point::Type)
            {
                Point *myItem = dynamic_cast<Point *>(item);
                selectedPoints.append(qMakePair(myItem, myItem->scenePos()));
            }
            if(item->type() == Line::Type)
            {
                Line *myItem = dynamic_cast<Line *>(item);
                selectedLines.append(qMakePair(myItem, myItem->line()));
            }
        }
    }
}

void CadGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    const int gridSize = 50;
    const int realLeft = static_cast<int>(std::floor(rect.left()));
    const int realRight = static_cast<int>(std::ceil(rect.right()));
    const int realTop = static_cast<int>(std::floor(rect.top()));
    const int realBottom = static_cast<int>(std::ceil(rect.bottom()));

    // Draw grid.
    const int firstLeftGridLine = realLeft - (realLeft % gridSize);
    const int firstTopGridLine = realTop - (realTop % gridSize);
    QVarLengthArray<QLine, 100> lines;

    for (qreal x = firstLeftGridLine; x <= realRight; x += gridSize)
        lines.append(QLine(x, realTop, x, realBottom));
    for (qreal y = firstTopGridLine; y <= realBottom; y += gridSize)
        lines.append(QLine(realLeft, y, realRight, y));

    painter->setPen(QPen(QColor(220, 220, 220), 0.0));
    painter->drawLines(lines.data(), lines.size());

    // Draw axes.
    painter->setPen(QPen(Qt::lightGray, 0.0));
    painter->drawLine(0, realTop, 0, realBottom);
    painter->drawLine(realLeft, 0, realRight, 0);
}

void CadGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // mousePressEvent in the graphicsScene
    if(mouseEvent->button() == Qt::LeftButton)
    {
        switch (entityMode)
        {
        case NoMode:
            qDebug() << "No Mode";
            break;

        case PointMode:
            pointItem = new Point(++id);
            pointItem->setPos(mouseEvent->scenePos());
            itemList.append(pointItem);
            mUndoStack->push(new CadCommandAdd(this, pointItem));
            break;

        case LineMode:
            if (mFirstClick)
            {
                startP = mouseEvent->scenePos();
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                endP = mouseEvent->scenePos();
                mPaintFlag = true;
                mSecondClick = false;
            }

            if (mPaintFlag)
            {
                lineItem = new Line(++id, startP, endP);
                lineItem->setLine(startP.x(), startP.y(), endP.x(), endP.y());
                itemList.append(lineItem);
                mUndoStack->push(new CadCommandAdd(this, lineItem));
                setFlags();
            }
            break;

        case CircleMode:
            if (mFirstClick)
            {
                startP = mouseEvent->scenePos();
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                endP = mouseEvent->scenePos();
                mPaintFlag = true;
                mSecondClick = false;
            }

            if (mPaintFlag)
            {
                circleItem = new Circle(++id, startP, endP);
                itemList.append(circleItem);
                mUndoStack->push(new CadCommandAdd(this, circleItem));
                setFlags();
            }
            break;

        case EllipseMode:
            if (mFirstClick)
            {
                startP = mouseEvent->scenePos();
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                midP = mouseEvent->scenePos();
                mFirstClick = false;
                mSecondClick = false;
                mThirdClick = true;
            }

            else if (!mSecondClick && mThirdClick)
            {
                endP = mouseEvent->scenePos();
                mThirdClick = false;
                mPaintFlag = true;
            }

            if (mPaintFlag)
            {
                ellipseItem = new Ellipse(++id, startP, midP, endP);
                itemList.append(ellipseItem);
                mUndoStack->push(new CadCommandAdd(this, ellipseItem));
                setFlags();
            }
            break;

        case TextMode:
            textItem = new mText(++id);
            textItem->setPos(mouseEvent->scenePos());
            itemList.append(textItem);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            mUndoStack->push(new CadCommandAdd(this, textItem));
            connect(textItem, SIGNAL(lostFocus(mText *)),
                    this, SLOT(editorLostFocus(mText *)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
                    this, SIGNAL(itemSelected(QGraphicsItem *)));
            setFlags();

        default:
            ;
        }
    }
    else
    {
        if (mouseEvent->button() == Qt::RightButton)
        {
            contextitem = itemAt(mouseEvent->scenePos().toPoint(), QTransform());
            cpos = mouseEvent->scenePos();

            if (!contextitem)
            {
                cutaction->setEnabled(false);
                copyaction->setEnabled(false);

                if (MyClipBoard::instance()->isempty())
                {
                    pasteaction->setEnabled(false);
                }
                else
                {
                    pasteaction->setEnabled(true);
                }
            }
            else
            {
                cutaction->setEnabled(true);
                copyaction->setEnabled(true);
                pasteaction->setEnabled(false);
            }
        }
    }


    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CadGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *mouseEvent)
{
    contextmenu->exec(mouseEvent->screenPos());
}

void CadGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // if any items moved, then create undo commands
    foreach (pointPos item, selectedPoints)
    {
        if(item.first->type() == Point::Type)
        {
            Point *myItem = dynamic_cast<Point *>(item.first);
            mUndoStack->push(new CadCommandMove(myItem, item.second,
                                                myItem->scenePos()));
        }
    }

    foreach (linePos item, selectedLines)
    {
        if(item.first->type() == Line::Type)
        {
            Line *myItem = dynamic_cast<Line *>(item.first);
            mUndoStack->push(new CadCommandMove(myItem, item.second.p1(),
                                                item.second.p2(),
                                                item.second.p1()
                                                + item.first->scenePos(),
                                                item.second.p2()
                                                + item.first->scenePos()));
        }
    }

    // refresh record of selected items and call base mouseReleaseEvent
    selectItems();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CadGraphicsScene::writeStream(QXmlStreamWriter *stream)
{
    // write entities in a file
    foreach (QGraphicsItem *item, itemList)
    {
        if (items().contains(item))
        {
            if (item->type() == Point::Type)
            {
                Point *myItem = dynamic_cast<Point *>(item);
                stream->writeStartElement("Point");
                stream->writeAttribute("id", QString::number(myItem->id));
                stream->writeAttribute("x", QString::number(myItem->x()));
                stream->writeAttribute("y", QString::number(myItem->y()));
                stream->writeEndElement();  //end of Point Item
            }

            else if (item->type() == Line::Type)
            {
                Line *myItem = dynamic_cast<Line *>(item);
                stream->writeStartElement("Line");
                stream->writeAttribute("id", QString::number(myItem->id));
                stream->writeAttribute("x1", QString::number(myItem->startP.x()
                                                             + myItem->scenePos()
                                                             .x()));
                stream->writeAttribute("y1", QString::number(myItem->startP.y()
                                                             + myItem->scenePos()
                                                             .y()));
                stream->writeAttribute("x2", QString::number(myItem->endP.x()
                                                             + myItem->scenePos()
                                                             .x()));
                stream->writeAttribute("y2", QString::number(myItem->endP.y()
                                                             + myItem->scenePos()
                                                             .y()));
                stream->writeEndElement();  //end of Line Item
            }

            else if (item->type() == Circle::Type)
            {
                Circle *myItem = dynamic_cast<Circle *>(item);
                stream->writeStartElement("Circle");
                stream->writeAttribute("id", QString::number(myItem->id));
                stream->writeAttribute("cx", QString::number(myItem->centerP.x()));
                stream->writeAttribute("cy", QString::number(myItem->centerP.y()));
                stream->writeAttribute("radius", QString::number(myItem->radius));
                stream->writeEndElement();  //end of Circle Item
            }

            else if (item->type() == Ellipse::Type)
            {
                Ellipse *myItem = dynamic_cast<Ellipse *>(item);
                stream->writeStartElement("Ellipse");
                stream->writeAttribute("id", QString::number(myItem->id));
                stream->writeAttribute("cx", QString::number(myItem->p1.x()));
                stream->writeAttribute("cy", QString::number(myItem->p1.y()));
                stream->writeAttribute("majR", QString::number(myItem->majRadius));
                stream->writeAttribute("minR", QString::number(myItem->minRadius));
                stream->writeEndElement();  //end of Ellipse Item
            }

            else if (item->type() == mText::Type)
            {
                mText *myItem = dynamic_cast<mText *>(item);
                stream->writeStartElement("Text");
                stream->writeAttribute("id", QString::number(myItem->id));
                stream->writeAttribute("x", QString::number(myItem->x()));
                stream->writeAttribute("y", QString::number(myItem->y()));
                stream->writeAttribute("text", myItem->toPlainText());
                stream->writeEndElement();  //end of Text Item
            }
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
            foreach (QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "id")
                    id = attribute.value().toString().toDouble();
                if (attribute.name() == "x")
                    x = attribute.value().toString().toDouble();
                if (attribute.name() == "y")
                    y = attribute.value().toString().toDouble();
            }
            pointItem = new Point(id);
            pointItem->setPos(x, y);
            itemList.append(pointItem);
            mUndoStack->push(new CadCommandAdd(this, pointItem));
        }

        if (stream->isStartElement() && stream->name() == "Line")
        {
            foreach (QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "id")
                    id = attribute.value().toString().toDouble();
                if (attribute.name() == "x1")
                    startP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y1")
                    startP.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "x2")
                    endP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y2")
                    endP.setY(attribute.value().toString().toDouble());
            }
            lineItem = new Line(id, startP, endP);
            lineItem->setLine(startP.x(), startP.y(), endP.x(), endP.y());
            itemList.append(lineItem);
            mUndoStack->push(new CadCommandAdd(this, lineItem));
        }

        if (stream->isStartElement() && stream->name() == "Circle")
        {
            foreach (QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "id")
                    id = attribute.value().toString().toDouble();
                if (attribute.name() == "cx")
                    startP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "cy")
                    startP.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "radius")
                    rad = attribute.value().toString().toDouble();
            }
            circleItem = new Circle(id, startP, rad);
            itemList.append(circleItem);
            mUndoStack->push(new CadCommandAdd(this, circleItem));
        }

        if (stream->isStartElement() && stream->name() == "Ellipse")
        {
            foreach (QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "id")
                    id = attribute.value().toString().toDouble();
                if (attribute.name() == "cx")
                    startP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "cy")
                    startP.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "majR")
                    radM = attribute.value().toString().toDouble();
                if (attribute.name() == "minR")
                    rad = attribute.value().toString().toDouble();
            }
            ellipseItem = new Ellipse(id, startP, rad, radM);
            itemList.append(ellipseItem);
            mUndoStack->push(new CadCommandAdd(this, ellipseItem));
        }

        if (stream->isStartElement() && stream->name() == "Text")
        {
            foreach (QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "id")
                    id = attribute.value().toString().toDouble();
                if (attribute.name() == "x")
                    startP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y")
                    startP.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "text")
                    str = attribute.value().toString();
            }
            textItem = new mText(id);
            textItem->setPos(startP.x(), startP.y());
            textItem->setPlainText(str);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            itemList.append(textItem);
            mUndoStack->push(new CadCommandAdd(this, textItem));
        }
    }
}

void CadGraphicsScene::cut(gEntity *obj)
{
    removeItem(obj);
    MyClipBoard::instance()->push(obj);
}

void CadGraphicsScene::copy(gEntity *obj)
{
    MyClipBoard::instance()->push(obj->clone());
}

void CadGraphicsScene::paste(const QPointF &pos)
{
    gEntity *last = MyClipBoard::instance()->pop();
    if (last)
    {
        addItem(static_cast<QGraphicsItem *>(last));
        last->setPos(pos);
        last->setFlag(QGraphicsItem::ItemIsSelectable);
        last->setFlag(QGraphicsItem::ItemIsMovable);
        mUndoStack->push(new CadCommandAdd(this, last));
    }
}

void CadGraphicsScene::contextMenuaction(QAction *a)
{
    if (a == cutaction)
    {
        cut(static_cast<gEntity *>(contextitem));
    }
    else if (a == copyaction)
    {
        copy(static_cast<gEntity *>(contextitem));
    }
    else if (a == pasteaction)
    {
        paste(cpos);
    }
}

