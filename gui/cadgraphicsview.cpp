#include "cadgraphicsview.h"

CadGraphicsView::CadGraphicsView()
{
    scaleFactor = 1.15;
    setAttribute(Qt::WA_DeleteOnClose);
    isPanning = false;
    setMouseTracking(true);
    translate((width() / 2) - 250, (height() / 2) + 300);
    scale(1, -1);
}

void CadGraphicsView::newFile()
{
    // creates a new file/document and insert scene
    undoStack = new QUndoStack(this);
    undoView = 0;

    scene = new CadGraphicsScene(this, undoStack);
    modifySceneRect();
    setScene(scene);

    undoAction = undoStack->createUndoAction(this);
    redoAction = undoStack->createRedoAction(this);

    // creates a new script widget
    scriptWidget = new CadScriptWidget;

    // creates a new command widget
    commandWidget = new CadCommandWidget;
}

void CadGraphicsView::modifySceneRect()
{
    // sets scene rect
    scene->setSceneRect(-20000, -20000, 40000, 40000);
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
    isPanning = false;
}

void CadGraphicsView::drawPoint()
{
    // sets the mode to PointMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::PointMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
    cursorMode();
}

void CadGraphicsView::drawLine()
{
    // sets the mode to LineMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::LineMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
    cursorMode();
}

void CadGraphicsView::drawCircle()
{
    // sets the mode to CircleMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::CircleMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
    cursorMode();
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
    cursorMode();
}

void CadGraphicsView::drawArc()
{
    // sets the mode to ArcMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::ArcMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
    cursorMode();
}

void CadGraphicsView::drawImage()
{
    // sets the mode to Imagemode for scene and sets/resets the flags
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("open File"), "",
                                                    tr("JPEG(*.jpg *.jpeg);;"
                                                       "PNG(*.png)"));
    if (!fileName.isEmpty())
    {
        scene->imagePath = fileName;
        scene->setMode(CadGraphicsScene::ImageMode);
        scene->setFlags();
        viewport()->setCursor(Qt::CrossCursor);
        cursorMode();
    }
}

void CadGraphicsView::drawDimHorizontal()
{
    // sets the mode to DimHorizontalMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::DimHorizontalMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
    cursorMode();
}

void CadGraphicsView::drawDimVertical()
{
    // sets the mode to DimVerticalMode for scene and sets/resets the flags
    scene->setMode(CadGraphicsScene::DimVerticalMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
    cursorMode();
}

void CadGraphicsView::deleteSingleItem()
{
    // sets delete mode to delete single item in the scene
    scene->setMode(CadGraphicsScene::DeleteMode);
    scene->setFlags();
    viewport()->setCursor(Qt::CrossCursor);
    cursorMode();
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
    /**
     * while dragging from left to right entities will be selected if they lie
     * completely inside the selection area whereas while dragging from right
     * to left all those entities will be selected whose bounding rect
     * intersects with the selection area.
     */
    if (!scene->isInvertedSelection)
        setRubberBandSelectionMode(Qt::ContainsItemShape);
    else
        setRubberBandSelectionMode(Qt::IntersectsItemShape);

    cursorMode();
}

void CadGraphicsView::cursorMode()
{
    if (!isPanning)
        setDragMode(QGraphicsView::RubberBandDrag);
    else
        setDragMode(QGraphicsView::ScrollHandDrag);
}
