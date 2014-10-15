#include "cadgraphicsview.h"

CadGraphicsView::CadGraphicsView()
{
    scaleFactor = 1.15;
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
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
    setDragMode(QGraphicsView::RubberBandDrag);
}

void CadGraphicsView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    if (event->delta() > 0)
    {
        // Zooming in
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        // Zooming out
        scale(1.0/scaleFactor, 1.0/scaleFactor);
    }
}

void CadGraphicsView::setNoMode()
{
    // sets the mode to NoMode for scene
    scene->setMode(CadGraphicsScene::NoMode);
}

void CadGraphicsView::drawPoint()
{
    // sets the mode to PointMode for scene
    scene->setMode(CadGraphicsScene::PointMode);
}

void CadGraphicsView::drawLine()
{
    // sets the mode to LineMode for scene
    scene->setMode(CadGraphicsScene::LineMode);
}

void CadGraphicsView::drawCircle()
{
    // sets the mode to CircleMode for scene
    scene->setMode(CadGraphicsScene::CircleMode);
}

void CadGraphicsView::drawEllipse()
{
    // sets the mode to EllipseMode for scene
    scene->setMode(CadGraphicsScene::EllipseMode);
}

void CadGraphicsView::showUndoStack()
{
    // shows the undoStack window
    if (undoView == 0)
    {
        undoView = new QUndoView(undoStack);
        undoView->setWindowTitle("Undo Stack");
        undoView->setAttribute(Qt::WA_QuitOnClose, false);
    }
    undoView->show();
}
