#include "cadgraphicsview.h"
#include <QDateTime>

CadGraphicsView::CadGraphicsView() :
    QGraphicsView()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    scaleFactor = 1.15;
}

CadGraphicsView::~CadGraphicsView()
{

}

void CadGraphicsView::newFile()
{
    scene  = new CadGraphicsScene;
    scene->setSceneRect(0,0,2000,2000);
    setScene(scene);
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

bool CadGraphicsView::drawPoint()
{
    scene->setMode(CadGraphicsScene::PointMode);
    return true;
}
