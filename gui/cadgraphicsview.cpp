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
    setDragMode(QGraphicsView::RubberBandDrag);
}

void CadGraphicsView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    static double currentScale = 1.0;  // stores the current scale value.
    static const double scaleMin = .5; // defines the min scale limit.
    static const double scaleMax = 5;

    // Scale the view / do the zoom
    if (event->delta() > 0)
    {
        // Zooming in
        if (currentScale < scaleMax) {
            scale(scaleFactor, scaleFactor);
            currentScale *= scaleFactor;
        }
    } else {
        if (currentScale > scaleMin) {
            scale(1 / scaleFactor, 1 / scaleFactor);
            currentScale /= scaleFactor;
        }
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

void CadGraphicsView::drawText()
{
    // sets the mode to TextMode for scene
    scene->setMode(CadGraphicsScene::TextMode);
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
