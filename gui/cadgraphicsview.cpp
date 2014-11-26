#include "cadgraphicsview.h"

CadGraphicsView::CadGraphicsView()
{
    scaleFactor = 1.15;
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    setMouseTracking(true);
}

void CadGraphicsView::newFile()
{
    // creates a new file/document and insert scene
    static int fileNumber = 1;
    isUntitled = true;
    curFileName = tr("Document %1").arg(fileNumber++);
    setWindowTitle(curFileName);

    undoStack = new QUndoStack(this);
    undoView = 0;

    scene = new CadGraphicsScene(this, undoStack);
    scene->setSceneRect(0,0,2000,2000);
    setScene(scene);
}

void CadGraphicsView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    if (event->delta() > 0)
    {
        scale(scaleFactor, scaleFactor);
    }

    else
    {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void CadGraphicsView::setNoMode()
{
    // sets the mode to NoMode for scene
    scene->setMode(CadGraphicsScene::NoMode);
    viewport()->setCursor(Qt::ArrowCursor);
    connect(scene, SIGNAL(setSelectionSignal()), this, SLOT(selectWindow()));
}

void CadGraphicsView::drawPoint()
{
    // sets the mode to PointMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::PointMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
}

void CadGraphicsView::drawLine()
{
    // sets the mode to LineMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::LineMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
}

void CadGraphicsView::drawCircle()
{
    // sets the mode to CircleMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::CircleMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
}

void CadGraphicsView::drawEllipse()
{
    // sets the mode to EllipseMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::EllipseMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
}

void CadGraphicsView::drawText()
{
    // sets the mode to TextMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::TextMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
}

void CadGraphicsView::drawArc()
{
    // sets the mode to ArcMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::ArcMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
}

void CadGraphicsView::showUndoStack()
{
    // shows the undoStack window
    if (undoView == 0)
    {
        undoView = new QUndoView(undoStack);
        undoView->setWindowTitle("Undo Stack");
    }

    undoView->show();
}

void CadGraphicsView::selectWindow()
{
    // entity is selected if it's completely inside the selection area
    if (!scene->isInvertedSelection)
    {
        setRubberBandSelectionMode(Qt::ContainsItemBoundingRect);
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    // entity is selected even if a part of it lies inside the selection area
    else
    {
        setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
        setDragMode(QGraphicsView::RubberBandDrag);
    }
}
