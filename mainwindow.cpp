#include "mainwindow.h"

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle(tr("eCAD"));
    newFile();

    qApp->installEventFilter(this);

//    connect(pointButton, SIGNAL(clicked()), this, SLOT(drawPoint()));
//    connect(lineButton, SIGNAL(clicked()), this, SLOT(drawLine()));
//    connect(circleButton, SIGNAL(clicked()), this, SLOT(drawCircle()));
//    connect(ellipseButton, SIGNAL(clicked()), this, SLOT(drawEllipse()));

//    connect(actionPoints, SIGNAL(triggered()), this, SLOT(drawPoint()));
//    connect(actionLine, SIGNAL(triggered()), this, SLOT(drawLine()));
//    connect(actionCircle, SIGNAL(triggered()), this, SLOT(drawCircle()));
//    connect(actionEllipse, SIGNAL(triggered()), this, SLOT(drawEllipse()));

    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(filePrint()));
    connect(actionPrintPreview, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
    connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(on_actionZoom_In_triggered()));
    connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(on_actionZoom_Out_triggered()));
    connect(actionInsert_Image,SIGNAL(triggered()),this,SLOT(on_actionInsert_Image_triggered()));
}


MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QMainWindow::statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
    }
    return false;
}

void MainWindow::newFile()
{
    scene =  new CadGraphicsScene;
    graphicsView->setScene(scene);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
}

void  MainWindow::filePrintPreview()
{
    // display print preview dialog
    QPrinter printer( QPrinter::HighResolution );
    QPrintPreviewDialog preview( &printer, this );
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(print(QPrinter*)) );
    preview.exec();
}

void  MainWindow::filePrint()
{
    // display print dialog and if accepted print
    QPrinter printer( QPrinter::HighResolution );
    QPrintDialog dialog( &printer, this );
    if ( dialog.exec() == QDialog::Accepted ) print( &printer );
}

void  MainWindow::print( QPrinter* printer )
{
    QPainter painter( printer );
    int w = printer->pageRect().width();
    int h = printer->pageRect().height();
    QRect page( 0, 0, w, h );

    QFont font = painter.font();
    font.setPixelSize( (w+h) / 100 );
    painter.setFont( font );

    painter.drawText( page, Qt::AlignBottom | Qt::AlignRight,
                      QDateTime::currentDateTime().toString( Qt::DefaultLocaleShortDate ) );

    page.adjust( w/20, h/20, -w/20, -h/20 );
    scene->render( &painter, page );
}

//void MainWindow::drawPoint(){
//    point_entity = new point;
//    scene->addItem(point_entity);
//    qDebug() << "Point Created";
//}

//void MainWindow::drawLine(){
//    line_entity = new line;
//    scene->addItem(line_entity);
//    qDebug() << "Line Created";
//}

//void MainWindow::drawCircle(){
//    circle_entity = new circle;
//    scene->addItem(circle_entity);
//    qDebug() << "Circle Created";
//}

//void MainWindow::drawEllipse(){
//    ellipse_entity = new ellipse;
//    scene->addItem(ellipse_entity);
//    qDebug() << "Ellipse Created";
//}

void MainWindow::wheelEvent(QWheelEvent* event) {
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        graphicsView->scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("file Name(*.dxf)"));
    QMainWindow::statusBar()->showMessage("File opened successfully");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        else{
            QTextStream in(&file);
            QString line = in.readAll();
            QMainWindow::statusBar()->showMessage(line);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString filename=QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("file Name(*.dxf)"));
    if(!filename.isEmpty()) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        } else {
            QTextStream stream(&file);
            QList<QGraphicsItem*> allItems = scene->items();
            foreach (QGraphicsItem *i, allItems) {
                    stream << "Point " << allItems.indexOf(i,0)<< "\n";
                    //stream << "x,y:"<<point_entity->coordinateX <<","<<point_entity->coordinateY <<"\n";
                }

            stream.flush();
            file.close();
        }
    }
}

void MainWindow::on_actionZoom_In_triggered(){
    QWheelEvent *event;
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        graphicsView->scale(scaleFactor, scaleFactor);
    }
}

void MainWindow::on_actionZoom_Out_triggered(){
    QWheelEvent *event;
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom out
        graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void MainWindow::on_actionInsert_Image_triggered(){
    QString imagePath =QFileDialog::getOpenFileName(this,tr("open File"),"",tr("JPEG(*.jpg *.jpeg);;PNG(*.png)"));
    imageObject =new QImage();
    imageObject->load(imagePath);
    image = QPixmap::fromImage(*imageObject);
    scene =new CadGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    graphicsView->setScene(scene);
}

void MainWindow::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (e->type() == QEvent::GraphicsSceneMouseMove)
    {
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(e);
        qDebug() << "mouse";
        QMainWindow::statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
    }
}
