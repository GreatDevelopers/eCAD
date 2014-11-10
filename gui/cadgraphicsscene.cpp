#include "cadgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QDebug>

MyClipBoard* MyClipBoard::inst = 0;

CadGraphicsScene::CadGraphicsScene(QObject *parent, QUndoStack *undoStack)
    : QGraphicsScene(parent)
{
    setFlags();
    id = 0;
    mUndoStack = undoStack;
    m_context = new QMenu;
    a_cut = m_context->addAction("cut");
    a_copy = m_context->addAction("copy");
    a_paste = m_context->addAction("paste");
    context_item = 0;

    //connect context menu items to action slots
    connect(m_context, SIGNAL(triggered(QAction*)), this, SLOT(contmenu(QAction*)));
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
                lineItem->setLine(start_p.x(), start_p.y(), end_p.x(), end_p.y());
                itemList.append(lineItem);
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
                itemList.append(circleItem);
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
            connect(textItem, SIGNAL(lostFocus(mText*)),
                    this, SLOT(editorLostFocus(mText*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            setFlags();

        default:
            ;
        }
    }
    else
    {
        if (mouseEvent->button() & Qt::RightButton)
        {
            context_item = itemAt(mouseEvent->scenePos().toPoint(), QTransform());
            cpos = mouseEvent->scenePos();

            if (!context_item)
             {
                 a_cut->setEnabled(false);
                 a_copy->setEnabled(false);

                 if(MyClipBoard::instance()->isempty())
                     a_paste->setEnabled(false);
                 else a_paste->setEnabled(true);
             }
             else
             {
                 a_cut->setEnabled(true);
                 a_copy->setEnabled(true);
                 a_paste->setEnabled(false);
             }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CadGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    m_context->exec(event->screenPos());
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
                stream->writeAttribute("x1", QString::number(myItem->start_p.x()
                                                             + myItem->scenePos()
                                                             .x()));
                stream->writeAttribute("y1", QString::number(myItem->start_p.y()
                                                             + myItem->scenePos()
                                                             .y()));
                stream->writeAttribute("x2", QString::number(myItem->end_p.x()
                                                             + myItem->scenePos()
                                                             .x()));
                stream->writeAttribute("y2", QString::number(myItem->end_p.y()
                                                             + myItem->scenePos()
                                                             .y()));
                stream->writeEndElement();  //end of Line Item
            }

            else if (item->type() == Circle::Type)
            {
                Circle *myItem = dynamic_cast<Circle *>(item);
                stream->writeStartElement("Circle");
                stream->writeAttribute("id", QString::number(myItem->id));
                stream->writeAttribute("cx", QString::number(myItem->center_p.x()));
                stream->writeAttribute("cy", QString::number(myItem->center_p.y()));
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
                    _x = attribute.value().toString().toDouble();
                if (attribute.name() == "y")
                    _y = attribute.value().toString().toDouble();
            }
            pointItem = new Point(id);
            pointItem->setPos(_x, _y);
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
                    start_p.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y1")
                    start_p.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "x2")
                    end_p.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y2")
                    end_p.setY(attribute.value().toString().toDouble());
            }
            lineItem = new Line(id, start_p, end_p);
            lineItem->setLine(start_p.x(), start_p.y(), end_p.x(), end_p.y());
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
                    start_p.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "cy")
                    start_p.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "radius")
                    _rad = attribute.value().toString().toDouble();
            }
            circleItem = new Circle(id, start_p, _rad);
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
                    start_p.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "cy")
                    start_p.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "majR")
                    _radM = attribute.value().toString().toDouble();
                if (attribute.name() == "minR")
                    _rad = attribute.value().toString().toDouble();
            }
            ellipseItem = new Ellipse(id, start_p, _rad, _radM);
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
                    start_p.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y")
                    start_p.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "text")
                    _str = attribute.value().toString();
            }
            textItem = new mText(id);
            textItem->setPos(start_p.x(), start_p.y());
            textItem->setPlainText(_str);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            itemList.append(textItem);
            mUndoStack->push(new CadCommandAdd(this, textItem));
        }
    }
}

void CadGraphicsScene::cut(gEntity* obj)
{
    removeItem(obj);
    MyClipBoard::instance()->push(obj);
}

void CadGraphicsScene::copy(gEntity* obj)
{
    MyClipBoard::instance()->push(obj->my_clone());
}

void CadGraphicsScene::paste(const QPointF& pos)
{
    gEntity* last = MyClipBoard::instance()->pop();
    if(last)
    {
        addItem(static_cast<QGraphicsItem*>(last));
        last->setPos(pos);
        last->setFlag(QGraphicsItem::ItemIsSelectable);
        last->setFlag(QGraphicsItem::ItemIsMovable);
        mUndoStack->push(new CadCommandAdd(this, last));
    }
}

void CadGraphicsScene::contmenu(QAction* a)
{
    if(a == a_cut)
    {
        cut(static_cast<gEntity*>(context_item));
    }
    else if(a == a_copy)
    {
        copy(static_cast<gEntity*>(context_item));
    }
    else if(a == a_paste)
    {
        paste(cpos);
    }
}
