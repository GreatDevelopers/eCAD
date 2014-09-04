#include "mdichild.h"

MdiChild::MdiChild(QWidget *parent):
    QMdiSubWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

MdiChild::~MdiChild()
{
}

QSize MdiChild::sizeHint() const {
    return QSize(640, 480);
}

void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    fileName = tr("Document %1").arg(sequenceNumber++);
    setWindowTitle(fileName + "[*]");

    scene = new CadGraphicsScene;
    view = new CadGraphicsView;
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setScene(scene);
    setWidget(view);
}

