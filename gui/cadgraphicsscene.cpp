#include "cadgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>

clipboardStack *clipboardStack::inst = 0;

CadGraphicsScene::CadGraphicsScene(QObject *parent, QUndoStack *undoStack)
    : QGraphicsScene(parent)
{
    setFlags();
    id = 0;
    mUndoStack = undoStack;
    contextMenu = new QMenu;
    cutAction = contextMenu->addAction("cut");
    copyAction = contextMenu->addAction("copy");
    pasteAction = contextMenu->addAction("paste");
    contextItem = 0;

    // connects context menu items to action slots
    connect(contextMenu, SIGNAL(triggered(QAction *)),
            this, SLOT(menuAction(QAction *)));
    // connects selectionChanged signal to selectItems slot
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

void CadGraphicsScene::editorLostFocus(Text *item)
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
    selectedEntities.clear();

    foreach (QGraphicsItem *item, itemList)
    {
        if (item->isSelected())
        {
            if (item->type() == Point::Type)
            {
                Point *itemPtr = dynamic_cast<Point *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == Line::Type)
            {
                Line *itemPtr = dynamic_cast<Line *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == Circle::Type)
            {
                Circle *itemPtr = dynamic_cast<Circle *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == Ellipse::Type)
            {
                Ellipse *itemPtr = dynamic_cast<Ellipse *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == Text::Type)
            {
                Text *itemPtr = dynamic_cast<Text *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == Arc::Type)
            {
                Arc *itemPtr = dynamic_cast<Arc *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }
        }
    }
}

void CadGraphicsScene::selectDeselectAllItems(bool b)
{
    // sets selection for all items
    foreach (QGraphicsItem *item, items())
    {
        item->setSelected(b);
    }
}

void CadGraphicsScene::invertSelection()
{
    // inverts selection of all items in the scene
    setMode(NoMode);

    foreach (QGraphicsItem *item, itemList)
    {
        if (item->isSelected())
            item->setSelected(false);
        else
            item->setSelected(true);
    }
}

void CadGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (isGridVisible)
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
}

void CadGraphicsScene::drawEntity(QGraphicsItem *item)
{
    if (item->type() == Point::Type)
    {
        Point *itemPtr = dynamic_cast<Point *>(item);
        itemPtr->setPos(itemPtr->position.x(), itemPtr->position.y());
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Line::Type)
    {
        Line *itemPtr = dynamic_cast<Line *>(item);
        itemPtr->setLine(itemPtr->startP.x(), itemPtr->startP.y(),
                         itemPtr->endP.x(), itemPtr->endP.y());
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Circle::Type)
    {
        Circle *itemPtr = dynamic_cast<Circle *>(item);
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Ellipse::Type)
    {
        Ellipse *itemPtr = dynamic_cast<Ellipse *>(item);
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Text::Type)
    {
        Text *itemPtr = dynamic_cast<Text *>(item);
        itemPtr->setPos(itemPtr->position);
        itemPtr->setPlainText(itemPtr->str);
        itemList.append(itemPtr);
        itemPtr->setTextInteractionFlags(Qt::TextEditorInteraction);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
        connect(itemPtr, SIGNAL(lostFocus(Text *)),
                this, SLOT(editorLostFocus(Text *)));
        connect(itemPtr, SIGNAL(selectedChange(QGraphicsItem *)),
                this, SIGNAL(itemSelected(QGraphicsItem *)));
    }

    else if (item->type() == Arc::Type)
    {
        Arc *itemPtr = dynamic_cast<Arc *>(item);
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    setFlags();
}

void CadGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // mousePressEvent in the graphicsScene
    if(mouseEvent->button() == Qt::LeftButton)
    {
        switch (entityMode)
        {
        case NoMode:
            if (mFirstClick)
                startP = mouseEvent->scenePos();
            break;

        case PointMode:
            startP = mouseEvent->scenePos();
            pointItem = new Point(++id, startP);
            drawEntity(pointItem);
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
                drawEntity(lineItem);
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
                drawEntity(circleItem);
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
                drawEntity(ellipseItem);
            }
            break;

        case TextMode:
            startP = mouseEvent->scenePos();
            textItem = new Text(++id, startP, str);
            drawEntity(textItem);
            break;

        case ArcMode:
            if (mFirstClick)
            {
                startP = mouseEvent->scenePos();
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                midP = mouseEvent->scenePos();
                mSecondClick = false;
                mThirdClick = true;
            }

            else if (!mSecondClick && mThirdClick)
            {
                endP = mouseEvent->scenePos();
                mPaintFlag = true;
                mThirdClick = false;
            }

            if (mPaintFlag)
            {
                arcItem = new Arc(++id, startP, midP, endP);
                drawEntity(arcItem);
            }
            break;

        default:
            ;
        }
    }

    else
    {
        if (mouseEvent->button() == Qt::RightButton)
        {
            contextItem = itemAt(mouseEvent->scenePos().toPoint(), QTransform());
            contextPosition = mouseEvent->scenePos();

            if (!contextItem)
            {
                cutAction->setEnabled(false);
                copyAction->setEnabled(false);

                if (clipboardStack::instance()->isEmpty())
                {
                    pasteAction->setEnabled(false);
                }

                else
                {
                    pasteAction->setEnabled(true);
                }
            }

            else
            {
                cutAction->setEnabled(true);
                copyAction->setEnabled(true);
                pasteAction->setEnabled(false);
            }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CadGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *mouseEvent)
{
    contextMenu->exec(mouseEvent->screenPos());
}

void CadGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // if any items moved, then create undo commands
    foreach (entityPos item, selectedEntities)
    {
        if (item.first->type() == Point::Type)
        {
            Point *itemPtr = dynamic_cast<Point *>(item.first);
            mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                itemPtr->scenePos()));
        }

        else if (item.first->type() == Line::Type)
        {
            Line *itemPtr = dynamic_cast<Line *>(item.first);
            mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                itemPtr->scenePos()));
        }

        else if (item.first->type() == Circle::Type)
        {
            Circle *itemPtr = dynamic_cast<Circle *>(item.first);
            mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                itemPtr->scenePos()));
        }

        else if (item.first->type() == Ellipse::Type)
        {
            Ellipse *itemPtr = dynamic_cast<Ellipse *>(item.first);
            mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                itemPtr->scenePos()));
        }

        else if (item.first->type() == Text::Type)
        {
            Text *itemPtr = dynamic_cast<Text *>(item.first);
            mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                itemPtr->scenePos()));
        }

        else if (item.first->type() == Arc::Type)
        {
            Arc *itemPtr = dynamic_cast<Arc *>(item.first);
            mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                itemPtr->scenePos()));
        }
    }

    // refresh record of selected items and call base mouseReleaseEvent
    selectItems();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CadGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (entityMode == NoMode)
    {
        endP = mouseEvent->scenePos();

        // sets isInvertedSelection according to direction of mouse drag
        if (startP.x() - endP.x() < 0)
            isInvertedSelection = false;
        else
            isInvertedSelection = true;
    }

    emit(setSelectionSignal());
    QGraphicsScene::mouseMoveEvent(mouseEvent);
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
                Point *itemPtr = dynamic_cast<Point *>(item);
                stream->writeStartElement("Point");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("x", QString::number(itemPtr->x()));
                stream->writeAttribute("y", QString::number(itemPtr->y()));
                stream->writeEndElement();  //end of Point Item
            }

            else if (item->type() == Line::Type)
            {
                Line *itemPtr = dynamic_cast<Line *>(item);
                stream->writeStartElement("Line");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("x1", QString::number(itemPtr->startP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y1", QString::number(itemPtr->startP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("x2", QString::number(itemPtr->endP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y2", QString::number(itemPtr->endP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeEndElement();  //end of Line Item
            }

            else if (item->type() == Circle::Type)
            {
                Circle *itemPtr = dynamic_cast<Circle *>(item);
                stream->writeStartElement("Circle");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("cx", QString::number(itemPtr->centerP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("cy", QString::number(itemPtr->centerP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("radius", QString::number(itemPtr->radius));
                stream->writeEndElement();  //end of Circle Item
            }

            else if (item->type() == Ellipse::Type)
            {
                Ellipse *itemPtr = dynamic_cast<Ellipse *>(item);
                stream->writeStartElement("Ellipse");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("cx", QString::number(itemPtr->p1.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("cy", QString::number(itemPtr->p1.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("majR", QString::number(itemPtr->majRadius));
                stream->writeAttribute("minR", QString::number(itemPtr->minRadius));
                stream->writeEndElement();  //end of Ellipse Item
            }

            else if (item->type() == Text::Type)
            {
                Text *itemPtr = dynamic_cast<Text *>(item);
                stream->writeStartElement("Text");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("x", QString::number(itemPtr->x()));
                stream->writeAttribute("y", QString::number(itemPtr->y()));
                stream->writeAttribute("text", itemPtr->toPlainText());
                stream->writeEndElement();  //end of Text Item
            }

            else if (item->type() == Arc::Type)
            {
                Arc *itemPtr = dynamic_cast<Arc *>(item);
                stream->writeStartElement("Arc");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("x1", QString::number(itemPtr->p1.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y1", QString::number(itemPtr->p1.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("x2", QString::number(itemPtr->p2.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y2", QString::number(itemPtr->p2.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("x3", QString::number(itemPtr->p3.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y3", QString::number(itemPtr->p3.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeEndElement();  //end of Arc Item
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
                    startP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y")
                    startP.setY(attribute.value().toString().toDouble());
            }

            pointItem = new Point(id, startP);
            drawEntity(pointItem);
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
            drawEntity(lineItem);
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
            drawEntity(circleItem);
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
            drawEntity(ellipseItem);
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

            textItem = new Text(id, startP, str);
            drawEntity(textItem);
        }

        if (stream->isStartElement() && stream->name() == "Arc")
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
                    midP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y2")
                    midP.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "x3")
                    endP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y3")
                    endP.setY(attribute.value().toString().toDouble());
            }

            arcItem = new Arc(id, startP, midP, endP);
            drawEntity(arcItem);
        }
    }
}

void CadGraphicsScene::cut(getEntity *obj)
{
    removeItem(obj);
    clipboardStack::instance()->push(obj->clone());
}

void CadGraphicsScene::copy(getEntity *obj)
{
    clipboardStack::instance()->push(obj->clone());
}

void CadGraphicsScene::paste(const QPointF &pos)
{
    getEntity *pasteEntity = clipboardStack::instance()->pop();

    if (pasteEntity)
    {
        pasteEntity->setPos(pos);
        pasteEntity->setFlag(QGraphicsItem::ItemIsSelectable);
        pasteEntity->setFlag(QGraphicsItem::ItemIsMovable);
        mUndoStack->push(new CadCommandAdd(this, pasteEntity));
    }
}

void CadGraphicsScene::menuAction(QAction *action)
{
    if (action == cutAction)
    {
        cut(static_cast<getEntity *>(contextItem));
    }

    else if (action == copyAction)
    {
        copy(static_cast<getEntity *>(contextItem));
    }

    else if (action == pasteAction)
    {
        paste(contextPosition);
    }
}
