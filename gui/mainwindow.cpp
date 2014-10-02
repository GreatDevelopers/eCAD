#include "mainwindow.h"

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QXmlStreamWriter>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle(tr("eCAD"));

    setCentralWidget(mdiArea);

    qApp->installEventFilter(this);

    //    connect(pointButton, SIGNAL(clicked()), this, SLOT(drawPoint()));
    //    connect(lineButton, SIGNAL(clicked()), this, SLOT(drawLine()));
    //    connect(circleButton, SIGNAL(clicked()), this, SLOT(drawCircle()));
    //    connect(ellipseButton, SIGNAL(clicked()), this, SLOT(drawEllipse()));

    connect(actionPoints, SIGNAL(triggered()), this, SLOT(drawPoint()));
    connect(actionLine, SIGNAL(triggered()), this, SLOT(drawLine()));
    //    connect(actionCircle, SIGNAL(triggered()), this, SLOT(drawCircle()));
    //    connect(actionEllipse, SIGNAL(triggered()), this, SLOT(drawEllipse()));

    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(filePrint()));
    connect(actionPrintPreview, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
    connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(on_actionZoom_In_triggered()));
    connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(on_actionZoom_Out_triggered()));
    connect(actionInsert_Image,SIGNAL(triggered()),this,SLOT(on_actionInsert_Image_triggered()));

    new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(setNoMode()));
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
    createMdiView();
    view->newFile();
    view->show();
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
    //scene->render( &painter, page );
}

CadGraphicsView *MainWindow::createMdiView()
{
    view = new CadGraphicsView;
    mdiArea->addSubWindow(view);
    return view;
}

void MainWindow::drawPoint()
{
    view->drawPoint();
}

void MainWindow::setNoMode()
{
    view->setNoMode();
}

void MainWindow::drawLine()
{
   view->drawLine();
}

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

void MainWindow::on_actionOpen_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("file Name(*.xml)"));
    QMainWindow::statusBar()->showMessage("File opened successfully");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        else{
            QXmlStreamReader  stream( &file );
            CadGraphicsScene* newScene = new CadGraphicsScene();
            while ( !stream.atEnd() )
            {
                stream.readNext();
                if ( stream.isStartElement() )
                {
                    if ( stream.name() == "SceneData" )
                        newScene->readStream( &stream );
                    else
                        stream.raiseError( QString("Unrecognised element '%1'").arg(stream.name().toString()) );
                }
            }

            // check if error occured
            if ( stream.hasError() )
            {
                file.close();
                QMessageBox::warning(this,"Error", QString("Failed to load '%1' (%2)").arg(filename).arg(stream.errorString()));
                delete newScene;
                return;
            }

            // close file, display new scene, delete old scene, and display useful message
            file.close();

            view->setScene( newScene );
            delete view->scene;
            view->scene = newScene;
            QMessageBox::warning(this,"Done", QString("Loaded '%1'").arg(filename));
            return;
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString filename=QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("file Name(*.xml)"));
    if(!filename.isEmpty()) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        } else {
            QXmlStreamWriter xmlWriter(&file);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();
            xmlWriter.writeStartElement("SceneData");
            xmlWriter.writeAttribute("version", "v1.0");
            xmlWriter.writeStartElement("Entities");

            view->scene->writeStream(&xmlWriter);

            xmlWriter.writeEndElement();   //end of Entities
            xmlWriter.writeEndElement();   //end of SceneData
            QMessageBox::warning(this,"Saved", QString("Saved Scene Data to '%1'").arg(filename));
            file.close();
        }
    }
}

void MainWindow::on_actionZoom_In_triggered()
{
    // Zoom in
    //graphicsView->scale(scaleFactor, scaleFactor);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    // Zoom out
    //graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

void MainWindow::on_actionInsert_Image_triggered(){
    QString imagePath =QFileDialog::getOpenFileName(this,tr("open File"),"",tr("JPEG(*.jpg *.jpeg);;PNG(*.png)"));
    imageObject =new QImage();
    imageObject->load(imagePath);
    image = QPixmap::fromImage(*imageObject);
    //scene =new CadGraphicsScene(this);
    //scene->addPixmap(image);
    //scene->setSceneRect(image.rect());
    //graphicsView->setScene(scene);
}
