#include "cadgraphicsview.h"

CadGraphicsView::CadGraphicsView()
{
    scaleFactor = 1.15;
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
}

void CadGraphicsView::newFile()
{
    static int fileNumber = 1;
    isUntitled = true;
    curFileName = tr("Document %1").arg(fileNumber++);
    setWindowTitle(curFileName);

    scene = new CadGraphicsScene;
    scene->setSceneRect(0,0,2000,2000);
    setScene(scene);
    setDragMode(QGraphicsView::RubberBandDrag);
}

void CadGraphicsView::wheelEvent(QWheelEvent* event) {
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void CadGraphicsView::drawPoint()
{
    scene->setMode(CadGraphicsScene::PointMode);
}

void CadGraphicsView::setNoMode()
{
    scene->setMode(CadGraphicsScene::NoMode);
}

