#include "cadgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>

clipboardStack *clipboardStack::inst = 0;

CadGraphicsScene::CadGraphicsScene(QObject *parent, QUndoStack *undoStack)
    : QGraphicsScene(parent)
{
    setFlags();
    id = 0;
    setMode(NoMode);
    mUndoStack = undoStack;
    contextMenu = new QMenu;
    cutAction = contextMenu->addAction("cut");
    copyAction = contextMenu->addAction("copy");
    pasteAction = contextMenu->addAction("paste");
    contextItem = 0;
    snapTo = 1;
    endPointSnap = false;
    centerSnap = false;
    middleSnap = false;
    installEventFilter(this);

    dx = 30;
    dy = 30;

    // connects context menu items to action slots
    connect(contextMenu, SIGNAL(triggered(QAction *)),
            this, SLOT(menuAction(QAction *)));
    // connects selectionChanged signal to selectItems slot
    connect(this, SIGNAL(selectionChanged()), this, SLOT(selectItems()));
}

qreal CadGraphicsScene::roundOff(qreal value, qreal multipleOf)
{
    return qFloor((value + multipleOf / 2) / multipleOf) * multipleOf;
}

bool CadGraphicsScene::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMouseMove)
    {
        QGraphicsSceneMouseEvent *mouseEvent =
                static_cast<QGraphicsSceneMouseEvent *>(event);

        // shows mouse position and status bar message tooltip
        if(entityMode)
        {
            QToolTip::showText(mouseEvent->screenPos(),
                               QString("%1, %2 %3")
                               .arg(tempPoint.x())
                               .arg(tempPoint.y())
                               .arg(message));
        }

        else
        {
            QToolTip::showText(mouseEvent->screenPos(),
                               QString("%1, %2")
                               .arg(mouseEvent->scenePos().x())
                               .arg(mouseEvent->scenePos().y()));
        }

        if (!previewList.isEmpty())
        {
            foreach (QGraphicsItem *item, previewList)
                removeItem(item);

            previewList.clear();
        }

        if (entityMode != NoMode)
        {
            /**
             * snaps the mouse to the end points of entities if the distance
             * between any of the end points of any entity and mouse's position
             * is less than 30 units along both axes
             */
            if (endPointSnap)
            {
                foreach (endPoint, endPointsList)
                {
                    if ((endPoint.x() - dx <= mouseEvent->scenePos().x())
                            && (mouseEvent->scenePos().x() <= endPoint.x() + dx)
                            && (endPoint.y() - dy <= mouseEvent->scenePos().y())
                            && (mouseEvent->scenePos().y() <= endPoint.y() + dy))
                        tempPoint = endPoint;
                }
            }

            /**
             * snaps the mouse to the center points of entities if the distance
             * between any of the end points of any entity and mouse's position
             * is less than 30 units along both axes
             */
            else if (centerSnap)
            {
                foreach (centerPoint, centerPointsList)
                {
                    if ((centerPoint.x() - dx <= mouseEvent->scenePos().x())
                            && (mouseEvent->scenePos().x() <= centerPoint.x() + dx)
                            && (centerPoint.y() - dy <= mouseEvent->scenePos().y())
                            && (mouseEvent->scenePos().y() <= centerPoint.y() + dy))
                        tempPoint = centerPoint;
                }
            }

            /**
             * snaps the mouse to the middle points of entities if the distance
             * between any of the end points of any entity and mouse's position
             * is less than 30 units along both axes
             */
            else if (middleSnap)
            {
                foreach (middlePoint, midPointsList)
                {
                    if ((middlePoint.x() - dx <= mouseEvent->scenePos().x())
                            && (mouseEvent->scenePos().x() <= middlePoint.x() + dx)
                            && (middlePoint.y() - dy <= mouseEvent->scenePos().y())
                            && (mouseEvent->scenePos().y() <= middlePoint.y() + dy))
                        tempPoint = middlePoint;
                }
            }

            else
            {
                tempPoint = QPointF(roundOff(mouseEvent->scenePos().x(), snapTo),
                                    roundOff(mouseEvent->scenePos().y(), snapTo));
            }

            horizontalAxis = new Line(QPointF(sceneRect().bottomLeft().x(),
                                              tempPoint.y()),
                                      QPointF(sceneRect().bottomRight().x(),
                                              tempPoint.y()));
            verticalAxis = new Line(QPointF(tempPoint.x(),
                                            sceneRect().topLeft().y()),
                                    QPointF(tempPoint.x(),
                                            sceneRect().bottomLeft().y()));

            if (entityMode != DeleteMode && entityMode != DimHorizontalMode &&
                    entityMode != DimVerticalMode && entityMode != DimRadialMode
                    && entityMode != DimDiametricMode)
            {
                addItem(horizontalAxis);
                addItem(verticalAxis);
                previewList.append(horizontalAxis);
                previewList.append(verticalAxis);
            }
        }

        emit setMessage();

        if (entityMode == PointMode)
        {
            pointItem = new Point(tempPoint);
            pointItem->setPos(tempPoint);
            previewList.append(pointItem);
            addItem(pointItem);
        }

        else if (entityMode == LineMode)
        {
            if (mSecondClick)
            {
                lineItem = new Line(startP, tempPoint);
                previewList.append(lineItem);
                addItem(lineItem);
            }
        }

        else if (entityMode == CircleMode)
        {
            if (mSecondClick)
            {
                circleItem = new Circle(startP, tempPoint);
                previewList.append(circleItem);
                addItem(circleItem);
            }
        }

        else if (entityMode == EllipseMode)
        {
            if (mSecondClick)
            {
                ellipseItem = new Ellipse(startP, tempPoint, tempPoint);
                previewList.append(ellipseItem);
                addItem(ellipseItem);
            }

            if (mThirdClick)
            {
                ellipseItem = new Ellipse(startP, midP, tempPoint);
                previewList.append(ellipseItem);
                addItem(ellipseItem);
            }
        }

        else if (entityMode == ArcMode)
        {
            if (mSecondClick)
            {
                lineItem = new Line(startP, tempPoint);
                previewList.append(lineItem);
                addItem(lineItem);
            }

            if (mThirdClick)
            {
                arcItem = new Arc(startP, midP, tempPoint);
                previewList.append(arcItem);
                addItem(arcItem);
            }
        }

        else if (entityMode == DimHorizontalMode)
        {
            if (mSecondClick)
            {
                lineItem = new Line(startP, tempPoint);
                previewList.append(lineItem);
                addItem(lineItem);
            }

            if (mThirdClick)
            {
                dimHorizontalItem = new DimHorizontal(startP, midP, tempPoint);
                previewList.append(dimHorizontalItem);
                addItem(dimHorizontalItem);
            }
        }

        else if (entityMode == DimVerticalMode)
        {
            if (mSecondClick)
            {
                lineItem = new Line(startP, tempPoint);
                previewList.append(lineItem);
                addItem(lineItem);
            }

            if (mThirdClick)
            {
                dimVerticalItem = new DimVertical(startP, midP, tempPoint);
                previewList.append(dimVerticalItem);
                addItem(dimVerticalItem);
            }
        }

        else if (entityMode == DimRadialMode)
        {
            if (mSecondClick)
            {
                dimRadialItem = new DimRadial(radValue, startP, tempPoint);
                previewList.append(dimRadialItem);
                addItem(dimRadialItem);
            }
        }

        else if (entityMode == DimDiametricMode)
        {
            if (mSecondClick)
            {
                dimDiametricItem = new DimDiametric(diaValue, startP, tempPoint);
                previewList.append(dimDiametricItem);
                addItem(dimDiametricItem);
            }
        }
    }
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

    if (item->toPlainText().isEmpty())
    {
        --id;
        removeItem(item);
        itemList.removeOne(item);
        item->deleteLater();
    }
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

            else if (item->type() == Image::Type)
            {
                Image *itemPtr = dynamic_cast<Image *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == DimHorizontal::Type)
            {
                DimHorizontal *itemPtr = dynamic_cast<DimHorizontal *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == DimVertical::Type)
            {
                DimVertical *itemPtr = dynamic_cast<DimVertical *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == DimRadial::Type)
            {
                DimRadial *itemPtr = dynamic_cast<DimRadial *>(item);
                selectedEntities.append(qMakePair(itemPtr,
                                                  itemPtr->scenePos()));
            }

            else if (item->type() == DimDiametric::Type)
            {
                DimDiametric *itemPtr = dynamic_cast<DimDiametric *>(item);
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

void CadGraphicsScene::selectDeselectIntersectedEntities(bool b)
{
    setMode(NoMode);

    // checks if any items existing in the scene intersects with another
    foreach (QGraphicsItem *item, items())
    {
        foreach (QGraphicsItem *i, collidingItems(item))
            item->setSelected(b);
    }
}

void CadGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (isGridVisible)
    {
        const int gridSize = 50;
        const int realLeft = static_cast<int>(qFloor(rect.left()));
        const int realRight = static_cast<int>(qCeil(rect.right()));
        const int realTop = static_cast<int>(qFloor(rect.top()));
        const int realBottom = static_cast<int>(qCeil(rect.bottom()));

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

    // creates cross at (0,0) to depict origin
    painter->setPen(QPen(Qt::red, 0.0));
    painter->drawLine(-25, 0, 25, 0);
    painter->drawLine(0, -25, 0, 25);
}

void CadGraphicsScene::drawEntity(QGraphicsItem *item)
{
    if (item->type() == Point::Type)
    {
        Point *itemPtr = dynamic_cast<Point *>(item);
        itemPtr->setPos(itemPtr->position.x(), itemPtr->position.y());
        itemList.append(itemPtr);
        endPointsList.append(itemPtr->getPoint());
        centerPointsList.append(itemPtr->getPoint());
        midPointsList.append(itemPtr->getPoint());
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Line::Type)
    {
        Line *itemPtr = dynamic_cast<Line *>(item);
        itemList.append(itemPtr);

        foreach (QPointF i, itemPtr->getEndPoints())
            endPointsList.append(i);

        midPointsList.append(itemPtr->getMiddlePoint());
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Circle::Type)
    {
        Circle *itemPtr = dynamic_cast<Circle *>(item);
        itemList.append(itemPtr);
        centerPointsList.append(itemPtr->getCenter());
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Ellipse::Type)
    {
        Ellipse *itemPtr = dynamic_cast<Ellipse *>(item);
        itemList.append(itemPtr);
        centerPointsList.append(itemPtr->getCenter());
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Text::Type)
    {
        Text *itemPtr = dynamic_cast<Text *>(item);
        itemPtr->setPos(itemPtr->position);
        itemPtr->setPlainText(itemPtr->str);
        itemPtr->setTransform(QTransform::fromScale(1, -1));
        itemList.append(itemPtr);
        endPointsList.append(itemPtr->getEndPoint());
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

        foreach (QPointF i, itemPtr->getEndPoints())
            endPointsList.append(i);

        centerPointsList.append(itemPtr->getCenter());
        midPointsList.append(itemPtr->getMiddlePoint());
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == Image::Type)
    {
        Image *itemPtr = dynamic_cast<Image *>(item);
        itemList.append(itemPtr);

        foreach (QPointF i, itemPtr->getEndPoints())
            endPointsList.append(i);

        centerPointsList.append(itemPtr->getCenter());

        foreach (QPointF i, itemPtr->getMiddlePoints())
            midPointsList.append(i);

        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == DimHorizontal::Type)
    {
        DimHorizontal *itemPtr = dynamic_cast<DimHorizontal *>(item);
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == DimVertical::Type)
    {
        DimVertical *itemPtr = dynamic_cast<DimVertical *>(item);
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == DimRadial::Type)
    {
        DimRadial *itemPtr = dynamic_cast<DimRadial *>(item);
        itemList.append(itemPtr);
        mUndoStack->push(new CadCommandAdd(this, itemPtr));
    }

    else if (item->type() == DimDiametric::Type)
    {
        DimDiametric *itemPtr = dynamic_cast<DimDiametric *>(item);
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
                startP = tempPoint;
            break;

        case PointMode:
            startP = tempPoint;
            pointItem = new Point(++id, startP);
            drawEntity(pointItem);
            break;

        case LineMode:
            if (mFirstClick)
            {
                startP = tempPoint;
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                endP = tempPoint;
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
                startP = tempPoint;
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                endP = tempPoint;
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
                startP = tempPoint;
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                midP = tempPoint;
                mFirstClick = false;
                mSecondClick = false;
                mThirdClick = true;
            }

            else if (!mSecondClick && mThirdClick)
            {
                endP = tempPoint;
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
            startP = tempPoint;
            textItem = new Text(++id, startP, str);
            drawEntity(textItem);
            break;

        case ArcMode:
            if (mFirstClick)
            {
                startP = tempPoint;
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                midP = tempPoint;
                mSecondClick = false;
                mThirdClick = true;
            }

            else if (!mSecondClick && mThirdClick)
            {
                endP = tempPoint;
                mPaintFlag = true;
                mThirdClick = false;
            }

            if (mPaintFlag)
            {
                arcItem = new Arc(++id, startP, midP, endP);
                drawEntity(arcItem);
            }
            break;

        case ImageMode:
            startP = tempPoint;
            imageItem = new Image(++id, startP, imagePath);
            drawEntity(imageItem);
            break;

        case DeleteMode:
            clickedItem = itemAt(mouseEvent->scenePos().toPoint(), QTransform());

            if (clickedItem)
            {
                mUndoStack->push(new CadCommandDelete(this, clickedItem));
                clickedItem->setSelected(false);
            }
            break;

        case DimHorizontalMode:
            if (mFirstClick)
            {
                startP = tempPoint;
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                midP = tempPoint;
                mSecondClick = false;
                mThirdClick = true;
            }

            else if (!mSecondClick && mThirdClick)
            {
                endP = tempPoint;
                mPaintFlag = true;
                mThirdClick = false;
            }

            if (mPaintFlag)
            {
                dimHorizontalItem = new DimHorizontal(++id, startP, midP, endP);
                drawEntity(dimHorizontalItem);
            }
            break;

        case DimVerticalMode:
            if (mFirstClick)
            {
                startP = tempPoint;
                mFirstClick = false;
                mSecondClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                midP = tempPoint;
                mSecondClick = false;
                mThirdClick = true;
            }

            else if (!mSecondClick && mThirdClick)
            {
                endP = tempPoint;
                mPaintFlag = true;
                mThirdClick = false;
            }

            if (mPaintFlag)
            {
                dimVerticalItem = new DimVertical(++id, startP, midP, endP);
                drawEntity(dimVerticalItem);
            }
            break;

        case DimRadialMode:
            if (mFirstClick)
            {
                clickedItem = itemAt(mouseEvent->scenePos().toPoint(), QTransform());

                if (clickedItem)
                {
                    if (clickedItem->type() == Circle::Type)
                    {
                        Circle *itemPtr = dynamic_cast<Circle *>(clickedItem);
                        radValue = itemPtr->radius;
                    }

                    else if (clickedItem->type() == Arc::Type)
                    {
                        Arc *itemPtr = dynamic_cast<Arc *>(clickedItem);
                        radValue = itemPtr->rad;
                    }

                    startP = tempPoint;
                    mFirstClick = false;
                    mSecondClick = true;
                }

                else
                    mFirstClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                endP = tempPoint;
                mPaintFlag = true;
                mSecondClick = false;
            }

            if (mPaintFlag)
            {
                dimRadialItem = new DimRadial(++id, radValue, startP, endP);
                drawEntity(dimRadialItem);
            }
            break;

        case DimDiametricMode:
            if (mFirstClick)
            {
                clickedItem = itemAt(mouseEvent->scenePos().toPoint(), QTransform());

                if (clickedItem)
                {
                    if (clickedItem->type() == Circle::Type)
                    {
                        Circle *itemPtr = dynamic_cast<Circle *>(clickedItem);
                        diaValue = 2 * itemPtr->radius;
                    }

                    else if (clickedItem->type() == Arc::Type)
                    {
                        Arc *itemPtr = dynamic_cast<Arc *>(clickedItem);
                        diaValue = 2 * itemPtr->rad;
                    }

                    startP = tempPoint;
                    mFirstClick = false;
                    mSecondClick = true;
                }

                else
                    mFirstClick = true;
            }

            else if (!mFirstClick && mSecondClick)
            {
                endP = tempPoint;
                mPaintFlag = true;
                mSecondClick = false;
            }

            if (mPaintFlag)
            {
                dimDiametricItem = new DimDiametric(++id, diaValue, startP, endP);
                drawEntity(dimDiametricItem);
            }
            break;

        default:
            ;
        }
    }

    // shows cut/copy/paste actions in context menu
    else
    {
        if (mouseEvent->button() == Qt::RightButton)
        {
            contextItem = itemAt(mouseEvent->scenePos().toPoint(), QTransform());
            contextPosition = mouseEvent->scenePos();

            if (!contextItem || !contextItem->isSelected())
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

                // ids of items are passed to cut/copy functions
                if (contextItem->type() == Point::Type)
                {
                    Point *itemPtr = dynamic_cast<Point *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == Line::Type)
                {
                    Line *itemPtr = dynamic_cast<Line *>(contextItem);
                    contextItemId = itemPtr->id;
                    lineStartPoint = itemPtr->startP;
                    lineEndPoint = itemPtr->endP;
                }

                else if (contextItem->type() == Circle::Type)
                {
                    Circle *itemPtr = dynamic_cast<Circle *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == Ellipse::Type)
                {
                    Ellipse *itemPtr = dynamic_cast<Ellipse *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == Text::Type)
                {
                    Text *itemPtr = dynamic_cast<Text *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == Arc::Type)
                {
                    Arc *itemPtr = dynamic_cast<Arc *>(contextItem);
                    contextItemId = itemPtr->id;
                    arcStartPoint = itemPtr->p1;
                    arcMidPoint = itemPtr->p2;
                    arcEndPoint = itemPtr->p3;
                }

                else if (contextItem->type() == Image::Type)
                {
                    Image *itemPtr = dynamic_cast<Image *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == DimHorizontal::Type)
                {
                    DimHorizontal *itemPtr =
                            dynamic_cast<DimHorizontal *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == DimVertical::Type)
                {
                    DimVertical *itemPtr =
                            dynamic_cast<DimVertical *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == DimRadial::Type)
                {
                    DimRadial *itemPtr = dynamic_cast<DimRadial *>(contextItem);
                    contextItemId = itemPtr->id;
                }

                else if (contextItem->type() == DimDiametric::Type)
                {
                    DimDiametric *itemPtr =
                            dynamic_cast<DimDiametric *>(contextItem);
                    contextItemId = itemPtr->id;
                }
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
    /* if any item's moved and its position changes, only then create undo
     * commands
     */
    foreach (entityPos item, selectedEntities)
    {
        if (item.first->scenePos() != item.second)
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
                itemPtr->setTransformOriginPoint(0, 0);
                mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                    itemPtr->scenePos()));
                update(itemPtr->boundingRect());
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

            else if (item.first->type() == Image::Type)
            {
                Image *itemPtr = dynamic_cast<Image *>(item.first);
                mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                    itemPtr->scenePos()));
            }

            else if (item.first->type() == DimHorizontal::Type)
            {
                DimHorizontal *itemPtr = dynamic_cast<DimHorizontal *>(item.first);
                mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                    itemPtr->scenePos()));
            }

            else if (item.first->type() == DimVertical::Type)
            {
                DimVertical *itemPtr = dynamic_cast<DimVertical *>(item.first);
                mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                    itemPtr->scenePos()));
            }

            else if (item.first->type() == DimRadial::Type)
            {
                DimRadial *itemPtr = dynamic_cast<DimRadial *>(item.first);
                mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                    itemPtr->scenePos()));
            }

            else if (item.first->type() == DimDiametric::Type)
            {
                DimDiametric *itemPtr = dynamic_cast<DimDiametric *>(item.first);
                mUndoStack->push(new CadCommandMove(itemPtr, item.second,
                                                    itemPtr->scenePos()));
            }
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
                itemPtr->setTransformOriginPoint(0, 0);
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
                stream->writeAttribute("angle", QString::number(itemPtr->theta));
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

            else if (item->type() == Image::Type)
            {
                Image *itemPtr = dynamic_cast<Image *>(item);
                stream->writeStartElement("Image");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("x", QString::number(itemPtr->startP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y", QString::number(itemPtr->startP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("path", itemPtr->path);
                stream->writeEndElement();  //end of Image Item
            }

            else if (item->type() == DimHorizontal::Type)
            {
                DimHorizontal *itemPtr = dynamic_cast<DimHorizontal *>(item);
                stream->writeStartElement("DimHorizontal");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("x1", QString::number(itemPtr->startP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y1", QString::number(itemPtr->startP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("x2", QString::number(itemPtr->midP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y2", QString::number(itemPtr->midP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("x3", QString::number(itemPtr->endP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y3", QString::number(itemPtr->endP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeEndElement();  //end of Horizontal Dimension Item
            }

            else if (item->type() == DimVertical::Type)
            {
                DimVertical *itemPtr = dynamic_cast<DimVertical *>(item);
                stream->writeStartElement("DimVertical");
                stream->writeAttribute("id", QString::number(itemPtr->id));
                stream->writeAttribute("x1", QString::number(itemPtr->startP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y1", QString::number(itemPtr->startP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("x2", QString::number(itemPtr->midP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y2", QString::number(itemPtr->midP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeAttribute("x3", QString::number(itemPtr->endP.x()
                                                             + itemPtr->scenePos()
                                                             .x()));
                stream->writeAttribute("y3", QString::number(itemPtr->endP.y()
                                                             + itemPtr->scenePos()
                                                             .y()));
                stream->writeEndElement();  //end of Vertical Dimension Item
            }

            else if (item->type() == DimRadial::Type)
            {
                DimRadial *itemPtr = dynamic_cast<DimRadial *>(item);
                stream->writeStartElement("DimRadial");
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
                stream->writeAttribute("rad", QString::number(itemPtr->rad));
                stream->writeEndElement();  //end of Radial Dimension Item
            }

            else if (item->type() == DimDiametric::Type)
            {
                DimDiametric *itemPtr = dynamic_cast<DimDiametric *>(item);
                stream->writeStartElement("DimDiametric");
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
                stream->writeAttribute("dia", QString::number(itemPtr->dia));
                stream->writeEndElement();  //end of Diametric Dimension Item
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
                if (attribute.name() == "angle")
                    angle = attribute.value().toString().toDouble();
            }

            ellipseItem = new Ellipse(id, startP, rad, radM, angle);
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

        if (stream->isStartElement() && stream->name() == "Image")
        {
            foreach (QXmlStreamAttribute attribute, stream->attributes())
            {
                if (attribute.name() == "id")
                    id = attribute.value().toString().toDouble();
                if (attribute.name() == "x")
                    startP.setX(attribute.value().toString().toDouble());
                if (attribute.name() == "y")
                    startP.setY(attribute.value().toString().toDouble());
                if (attribute.name() == "path")
                    imagePath = attribute.value().toString();
            }

            imageItem = new Image(id, startP, imagePath);
            drawEntity(imageItem);
        }

        if (stream->isStartElement() && stream->name() == "DimHorizontal")
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

            dimHorizontalItem = new DimHorizontal(id, startP, midP, endP);
            drawEntity(dimHorizontalItem);
        }

        if (stream->isStartElement() && stream->name() == "DimVertical")
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

            dimVerticalItem = new DimVertical(id, startP, midP, endP);
            drawEntity(dimVerticalItem);
        }

        if (stream->isStartElement() && stream->name() == "DimRadial")
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
                if (attribute.name() == "rad")
                    radValue = attribute.value().toString().toDouble();
            }

            dimRadialItem = new DimRadial(id, radValue, startP, endP);
            drawEntity(dimRadialItem);
        }

        if (stream->isStartElement() && stream->name() == "DimDiametric")
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
                if (attribute.name() == "dia")
                    diaValue = attribute.value().toString().toDouble();
            }

            dimDiametricItem = new DimDiametric(id, diaValue, startP, endP);
            drawEntity(dimDiametricItem);
        }
    }
}

void CadGraphicsScene::cut(getEntity *obj)
{
    // id of item pasted is kept same as that of the item being cut
    removeItem(obj);
    clipboardStack::instance()->push(obj->clone(contextItemId));
}

void CadGraphicsScene::copy(getEntity *obj)
{
    // id of item pasted is one more than total number of items in the scene
    clipboardStack::instance()->push(obj->clone(++id));
}

void CadGraphicsScene::paste(const QPointF &pos)
{
    // gets the items cut/copy from clipboardStack to paste
    getEntity *pasteEntity = clipboardStack::instance()->pop();

    if (pasteEntity->type() == Point::Type)
    {
        Point *tempItem = dynamic_cast<Point *>(pasteEntity);
        tempItem->position = pos;

        Point *itemPtr = new Point(tempItem->id, tempItem->position);
        drawEntity(itemPtr);
    }

    if (pasteEntity->type() == Line::Type)
    {
        Line *tempItem = dynamic_cast<Line *>(pasteEntity);
        tempItem->startP = pos;

        /* calculates difference between startP of line being cut/copy and line
         * being pasted for proper pasting of line
         */
        differenceX = tempItem->startP.x() - lineStartPoint.x();
        differenceY = tempItem->startP.y() - lineStartPoint.y();
        tempItem->endP = QPointF(lineEndPoint.x() + differenceX,
                                lineEndPoint.y() + differenceY);

        Line *itemPtr = new Line(tempItem->startP, tempItem->endP);
        drawEntity(itemPtr);
    }

    if (pasteEntity->type() == Circle::Type)
    {
        Circle *tempItem = dynamic_cast<Circle *>(pasteEntity);
        tempItem->centerP = pos;

        Circle *itemPtr = new Circle(tempItem->id, tempItem->centerP,
                                     tempItem->radius);
        drawEntity(itemPtr);
    }

    if (pasteEntity->type() == Ellipse::Type)
    {
        Ellipse *tempItem = dynamic_cast<Ellipse *>(pasteEntity);
        tempItem->p1 = pos;

        Ellipse *itemPtr = new Ellipse(tempItem->id, tempItem->p1,
                                       tempItem->majRadius, tempItem->minRadius,
                                       tempItem->theta);
        drawEntity(itemPtr);
    }

    if (pasteEntity->type() == Text::Type)
    {
        Text *tempItem = dynamic_cast<Text *>(pasteEntity);
        tempItem->position = pos;

        Text *itemPtr = new Text(tempItem->id, tempItem->position, tempItem->str);
        drawEntity(itemPtr);
    }

    if (pasteEntity->type() == Image::Type)
    {
        Image *tempItem = dynamic_cast<Image *>(pasteEntity);
        tempItem->startP = pos;

        Image *itemPtr = new Image(tempItem->id, tempItem->startP, tempItem->path);
        drawEntity(itemPtr);
    }

    if (pasteEntity->type() == Arc::Type)
    {
        Arc *tempItem = dynamic_cast<Arc *>(pasteEntity);
        tempItem->p1 = pos;

        /* calculates difference between p1 of arc being cut/copy and arc
         * being pasted for proper pasting of arc
         */
        differenceX = tempItem->p1.x() - arcStartPoint.x();
        differenceY = tempItem->p1.y() -arcStartPoint.y();
        tempItem->p2 = QPointF(arcMidPoint.x() + differenceX,
                              arcMidPoint.y() + differenceY);
        tempItem->p3 = QPointF(arcEndPoint.x() + differenceX,
                              arcEndPoint.y() + differenceY);
        Arc *itemPtr = new Arc(tempItem->id, tempItem->p1, tempItem->p2,
                                tempItem->p3);
        drawEntity(itemPtr);
    }

    setMode(NoMode);
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

QString CadGraphicsScene::setStatusBarMessage()
{
    switch (entityMode)
    {
    case NoMode:
        message = "";
        break;

    case PointMode:
        message = "POINT: Specify a point";
        break;

    case LineMode:
        if (mFirstClick)
            message = "LINE: Specify start point";
        else if (!mFirstClick && mSecondClick)
            message = "LINE: Specify end point";
        break;

    case CircleMode:
        if (mFirstClick)
            message = "CIRCLE: Specify center";
        else if (!mFirstClick && mSecondClick)
            message = "CIRCLE: Specify point for radius";
        break;

    case EllipseMode:
        if (mFirstClick)
            message = "ELLIPSE: Specify center";
        else if (!mFirstClick && mSecondClick)
            message = "ELLIPSE: Specify next point";
        else if (!mSecondClick && mThirdClick)
            message = "ELLIPSE: Specify last point";
        break;

    case TextMode:
        message = "TEXT: Specify position";
        break;

    case ArcMode:
        if (mFirstClick)
            message = "ARC: Specify first point";
        else if (!mFirstClick && mSecondClick)
            message = "ARC: Specify next point";
        else if (!mSecondClick && mThirdClick)
            message = "ARC: Specify last point";
        break;

    case ImageMode:
        message = "IMAGE: Specify position";
        break;

    case DimHorizontalMode:
        if (mFirstClick)
            message = "Horizontal Dim: Specify first point";
        else if (!mFirstClick && mSecondClick)
            message = "Horizontal Dim: Specify next point";
        else if (!mSecondClick && mThirdClick)
            message = "Horizontal Dim: Specify last point";
        break;

    case DimVerticalMode:
        if (mFirstClick)
            message = "Vertical Dim: Specify first point";
        else if (!mFirstClick && mSecondClick)
            message = "Vertical Dim: Specify next point";
        else if (!mSecondClick && mThirdClick)
            message = "Vertical Dim: Specify last point";
        break;

    case DimRadialMode:
        if (mFirstClick)
            message = "Radial Dim: Select arc or circle entity";
        else if (!mFirstClick && mSecondClick)
            message = "Radial Dim: Specify final point to draw";
        break;

    case DimDiametricMode:
        if (mFirstClick)
            message = "Diametric Dim: Select arc or circle entity";
        else if (!mFirstClick && mSecondClick)
            message = "Diametric Dim: Specify final point to draw";
        break;

    default:
        break;
    }

    return message;
}
