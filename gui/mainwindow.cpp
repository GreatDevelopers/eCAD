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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle(tr("eCAD"));
    setCentralWidget(mdiArea);
    fileNumber = 0;
    messageLeft = new QLabel(this->mainStatusBar);
    messageMiddle = new QLabel(this->mainStatusBar);
    mainStatusBar->addPermanentWidget(messageLeft, 1);
    mainStatusBar->addPermanentWidget(messageMiddle, 4);
    messageLeft->setText("Welcome to eCAD");

    // shortcut keys
    new QShortcut(QKeySequence(Qt::Key_Escape),
                  this, SLOT(setNoMode()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_U),
                  this, SLOT(showUndoStack()));
    new QShortcut(QKeySequence(Qt::Key_Delete),
                  this, SLOT(deleteItems()));

    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateView()));
    connect(actionNew, SIGNAL(triggered()),
            this, SLOT(newFile()));
    connect(actionOpen, SIGNAL(triggered()),
            this, SLOT(openFile()));
    connect(actionSave, SIGNAL(triggered()),
            this, SLOT(save()));
    connect(actionSaveAs, SIGNAL(triggered()),
            this, SLOT(saveFileAs()));
    connect(actionClose, SIGNAL(triggered()),
            this, SLOT(closeActiveWindow()));
    connect(actionQuit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(actionPrint, SIGNAL(triggered()),
            this, SLOT(filePrint()));
    connect(actionPrintPreview, SIGNAL(triggered()),
            this, SLOT(filePrintPreview()));
    connect(actionZoomIn, SIGNAL(triggered()),
            this, SLOT(zoomIn()));
    connect(actionZoomOut, SIGNAL(triggered()),
            this, SLOT(zoomOut()));
    connect(actionPanning, SIGNAL(triggered()),
            this, SLOT(panning()));
    connect(actionAbout, SIGNAL(triggered()),
            this, SLOT(showAboutDialog()));
    connect(actionCut, SIGNAL(triggered()),
            this, SLOT(cutOperation()));
    connect(actionCopy, SIGNAL(triggered()),
            this, SLOT(copyOperation()));
    connect(actionPaste, SIGNAL(triggered()),
            this, SLOT(pasteOperation()));
    connect(menuExport, SIGNAL(triggered(QAction *)),
            this, SLOT(exportFile(QAction *)));

    connect(actionCommandConsole, SIGNAL(triggered()),
            this, SLOT(toggleWidgets()));
    connect(actionScripting, SIGNAL(triggered()),
            this, SLOT(toggleWidgets()));

    connect(actionSelectAll, SIGNAL(triggered()),
            this, SLOT(selectAll()));
    connect(actionDeselectAll, SIGNAL(triggered()),
            this, SLOT(deselectAll()));
    connect(actionSelectWindow, SIGNAL(triggered()),
            this, SLOT(selectWindow()));
    connect(actionGrid, SIGNAL(toggled(bool)),
            this, SLOT(showGrid(bool)));
    connect(actionToolbar, SIGNAL(toggled(bool)),
            this, SLOT(toggleToolBar(bool)));
    connect(actionStatusBar, SIGNAL(toggled(bool)),
            this, SLOT(hideStatusBar(bool)));

    actionStatusBar->setChecked(true);
    actionScripting->setChecked(true);

    // toggle actions to false
    toggleActions(0);
}

MainWindow::~MainWindow()
{
}

void MainWindow::toggleActions(bool b)
{
    actionSave->setEnabled(b);
    actionSaveAs->setEnabled(b);
    actionPrint->setEnabled(b);
    actionPrintPreview->setEnabled(b);
    actionGrid->setEnabled(b);
    actionZoomIn->setEnabled(b);
    actionZoomOut->setEnabled(b);
    actionPoints->setEnabled(b);
    actionLine->setEnabled(b);
    actionCircle->setEnabled(b);
    actionEllipse->setEnabled(b);
    actionText->setEnabled(b);
    actionInsertImage->setEnabled(b);
    menuToolbars->setEnabled(b);
    actionCommandConsole->setEnabled(b);
    actionScripting->setEnabled(b);
    actionArc->setEnabled(b);
    actionPanning->setEnabled(b);
    actionCut->setEnabled(b);
    actionCopy->setEnabled(b);
    actionPaste->setEnabled(b);
    actionToolbar->setEnabled(b);
    actionClose->setEnabled(b);
    actionImportImage->setEnabled(b);
    actionExportImage->setEnabled(b);
    actionExportPDF->setEnabled(b);
    menuImport->setEnabled(b);
    menuExport->setEnabled(b);
    actionGridSnap->setEnabled(b);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMouseMove)
    {
        QGraphicsSceneMouseEvent *mouseEvent =
                static_cast<QGraphicsSceneMouseEvent *>(event);
        QString showMessage = QString("Mouse move (%1,%2)").
                arg(mouseEvent->scenePos().x()).
                arg(mouseEvent->scenePos().y());
        messageLeft->setText(showMessage);
    }
}

void MainWindow::closeActiveWindow()
{
    // closes the active subwindow
    mdiArea->closeActiveSubWindow();
}

void MainWindow::cutOperation()
{
    foreach (QGraphicsItem *item, view->scene->items())
    {
        if (item->isSelected())
        {
            if (actionCut)
            {
                if (item->type() == Point::Type)
                {
                    Point *itemPtr = dynamic_cast<Point *>(item);
                    view->scene->contextItemId = itemPtr->id;
                }

                else if (item->type() == Circle::Type)
                {
                    Circle *itemPtr = dynamic_cast<Circle *>(item);
                    view->scene->contextItemId = itemPtr->id;
                }

                else if (item->type() == Ellipse::Type)
                {
                    Ellipse *itemPtr = dynamic_cast<Ellipse *>(item);
                    view->scene->contextItemId = itemPtr->id;
                }

                else if (item->type() == Line::Type)
                {
                    Line *itemPtr = dynamic_cast<Line *>(item);
                    view->scene->contextItemId = itemPtr->id;
                }

                else if (item->type() == Text::Type)
                {
                    Text *itemPtr = dynamic_cast<Text *>(item);
                    view->scene->contextItemId = itemPtr->id;
                }

                view->scene->cut(static_cast<getEntity *>(item));
            }
        }
    }
}

void MainWindow::copyOperation()
{
    foreach (QGraphicsItem *item, view->scene->items())
    {
        if (item->isSelected())
        {
            if (actionCopy)
            {
                view->scene->copy(static_cast<getEntity *>(item));
            }
        }
    }
}

void MainWindow::pasteOperation()
{
    view->scene->paste(view->scene->startP);
}

void MainWindow::newFile()
{
    // creates a new file
    createMdiView();
    view->newFile();
    curFileName = tr("Drawing %1").arg(++fileNumber);
    view->setWindowTitle(curFileName);
    view->scene->installEventFilter(this);
    view->show();
    isEntitySelected = false;

    // grid is set to true for every new document
    view->scene->isGridVisible = true;

    // appends script widget and command widget in their respective lists
    scriptWidgetList.append(view->scriptWidget);
    commandWidgetList.append(view->commandWidget);

    // appends undoAction and redoAction in their respective lists
    undoList.append(view->undoAction);
    redoList.append(view->redoAction);

    /**
     * associate undoAction, redoAction, script widget and command widget
     * with current view
     */
    updateView();

    // command widget must be hidden at startup
    view->commandWidget->hide();

    // connect signals
    connect(view->scene, SIGNAL(changed(QList<QRectF>)),
            this, SLOT(toggleMenuActions()));
    connect(view->scene, SIGNAL(setMessage()),
            this, SLOT(setStatusBarMessage()));
    connect(actionPoints, SIGNAL(triggered()),
            view, SLOT(drawPoint()));
    connect(actionLine, SIGNAL(triggered()),
            view, SLOT(drawLine()));
    connect(actionCircle, SIGNAL(triggered()),
            view, SLOT(drawCircle()));
    connect(actionEllipse, SIGNAL(triggered()),
            view, SLOT(drawEllipse()));
    connect(actionText, SIGNAL(triggered()),
            view, SLOT(drawText()));
    connect(actionArc, SIGNAL(triggered()),
            view, SLOT(drawArc()));
    connect(actionInsertImage, SIGNAL(triggered()),
            view, SLOT(drawImage()));
    connect(actionSelectEntity, SIGNAL(triggered()),
            view, SLOT(setNoMode()));
    connect(actionDeleteEntity, SIGNAL(triggered()),
            view, SLOT(deleteSingleItem()));
    connect(actionImportImage, SIGNAL(triggered()),
            view, SLOT(drawImage()));
    connect(actionDeleteSelected, SIGNAL(triggered()),
            view->scene, SLOT(deleteItems()));
    connect(actionInvertSelection, SIGNAL(triggered()),
            view->scene, SLOT(invertSelection()));
    connect(actionGridSnap, SIGNAL(toggled(bool)),
            this, SLOT(setSnapping()));

    // toggle actions to true
    toggleActions(1);

    view->firstSave = true;
    autoSaveCount = true;

    // sets timer for 60 seconds to auto save
    QTimer *timer = new QTimer(this);
    timer->setInterval(60000);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(autoSave()));
    timer->start();
}

void MainWindow::updateView()
{
    // updates the view according to the subwindow activated
    QMdiSubWindow *m = mdiArea->activeSubWindow();

    foreach (windowViewPair v, windowViewList)
    {
        if (m == v.first)
        {
            view = v.second;

            /**
             * hides all other undoAction, redoAction, script widget and command
             * widget not associated with current view
             */
            foreach (CadScriptWidget *sw, scriptWidgetList)
                removeDockWidget(sw);

            foreach (CadCommandWidget *cw, commandWidgetList)
                removeDockWidget(cw);

            foreach (QAction *undo, undoList)
                menuEdit->removeAction(undo);

            foreach (QAction *redo, redoList)
                menuEdit->removeAction(redo);

            // adds script and command widgets associated with current view
            addDockWidget(Qt::RightDockWidgetArea, view->scriptWidget);
            view->scriptWidget->getCurrentScene(view->scene);

            if (actionScripting->isChecked())
                view->scriptWidget->show();

            view->commandWidget->setMinimumHeight(50);
            view->commandWidget->setMaximumHeight(100);
            addDockWidget(Qt::BottomDockWidgetArea, view->commandWidget);

            if (actionCommandConsole->isChecked())
                view->commandWidget->show();

            // adds undoAction and redoAction associated with current view
            view->undoAction->setShortcut(QKeySequence::Undo);
            view->redoAction->setShortcut(QKeySequence::Redo);
            menuEdit->addAction(view->undoAction);
            menuEdit->addAction(view->redoAction);

            // checks/unchecks the actionGrid according to the grid visibility
            if (view->scene->isGridVisible)
                actionGrid->setChecked(true);
            else
                actionGrid->setChecked(false);
        }
    }
}

void MainWindow::toggleWidgets()
{
    // toggles Command Widget
    if (actionCommandConsole->isChecked())
        view->commandWidget->show();
    else
        view->commandWidget->hide();

    // toggles Script Widget
    if (actionScripting->isChecked())
        view->scriptWidget->show();
    else
        view->scriptWidget->hide();
}

void MainWindow::toggleMenuActions()
{
    /**
     * enables/disables the following menu actions
     * Select_Entity, Select_All and Deselect_All
     * Delete_Selected
    */
    if (view->scene->items().isEmpty())
    {
        actionSelectAll->setEnabled(false);
        actionDeselectAll->setEnabled(false);
        actionSelectEntity->setEnabled(false);
        actionDeleteEntity->setEnabled(false);
        actionDeleteSelected->setEnabled(false);
        actionSelectWindow->setEnabled(false);
        actionCut->setEnabled(false);
        actionCopy->setEnabled(false);

        if (clipboardStack::instance()->isEmpty())
        {
            actionPaste->setEnabled(false);
        }

        else
        {
            actionPaste->setEnabled(true);
        }
    }

    else
    {
        actionSelectAll->setEnabled(true);
        actionSelectEntity->setEnabled(true);
        actionInvertSelection->setEnabled(true);
        actionSelectWindow->setEnabled(true);
        actionDeleteEntity->setEnabled(true);
        actionCut->setEnabled(true);
        actionCopy->setEnabled(true);
        actionPaste->setEnabled(false);

        foreach (QGraphicsItem *item, view->scene->items())
        {
            if (item->isSelected())
            {
                isEntitySelected = true;
            }

            if (isEntitySelected == true)
            {
                actionDeselectAll->setEnabled(true);
                actionSelectEntity->setEnabled(false);
                actionDeleteSelected->setEnabled(true);
                actionCut->setEnabled(true);
                actionCopy->setEnabled(true);
                actionPaste->setEnabled(false);
            }

            else
            {
                actionDeselectAll->setEnabled(false);
                actionSelectEntity->setEnabled(true);
                actionDeleteSelected->setEnabled(false);
                actionCut->setEnabled(false);
                actionCopy->setEnabled(false);

                if (clipboardStack::instance()->isEmpty())
                {
                    actionPaste->setEnabled(false);
                }

                else
                {
                    actionPaste->setEnabled(true);
                }
            }
        }

        isEntitySelected = false;
    }
}

void MainWindow::exportFile(QAction *action)
{
    // export file dialog box
    QFileDialog fileDialog(this);
    if (action == actionExportImage)
    {
        fileDialog.setNameFilter("PNG (*.png);;"
                                 "JPG (*.jpg)");

        if (fileDialog.selectedNameFilter() == fileDialog.nameFilters().at(0))
            fileDialog.setDefaultSuffix("png");
        if (fileDialog.selectedNameFilter() == fileDialog.nameFilters().at(1))
            fileDialog.setDefaultSuffix("jpg");
    }

    else if (action == actionExportPDF)
    {
        fileDialog.setNameFilter(" PDF (*.pdf)");
        fileDialog.setDefaultSuffix("pdf");
    }

    fileDialog.setWindowModality(Qt::WindowModal);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.exec();

    QStringList files = fileDialog.selectedFiles();
    fileName = files.at(0);

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        else
        {
            if (action == actionExportImage)
            {
                /**
                 * selections are cleared and the area in which items are present
                 * is grabbed to be saved as an image
                 */
                view->scene->clearSelection();
                view->scene->setSceneRect(view->scene->itemsBoundingRect()
                                          .adjusted(-10, -10, 10, 10));
                QImage image(view->scene->sceneRect().size().toSize(),
                             QImage::Format_ARGB32);
                image.fill(QColor(Qt::white));

                QPainter painter(&image);
                view->scene->isGridVisible = false;
                view->scene->clearSelection();

                foreach (QGraphicsItem *item, view->scene->items())
                    item->setTransform(QTransform::fromScale(1, -1));

                view->scene->setSceneRect(view->scene->itemsBoundingRect()
                                          .adjusted(-10, -10, 10, 10));
                view->scene->render(&painter);
                image.save(fileName);
                view->modifySceneRect();

                foreach (QGraphicsItem *item, view->scene->items())
                    item->setTransform(QTransform::fromScale(1, 1));

                view->scene->isGridVisible = true;
            }

            else if (action == actionExportPDF)
            {
                QPrinter printer(QPrinter::HighResolution);
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setOutputFileName(fileName);
                print(&printer);
            }
        }
    }
}

void MainWindow::filePrintPreview()
{
    // display print preview dialog
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)),
            this, SLOT(print(QPrinter *)));
    preview.exec();
}

void MainWindow::filePrint()
{
    // display print dialog and if accepted print
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);

    if (dialog.exec() == QDialog::Accepted)
        print(&printer);
}

void MainWindow::print(QPrinter *printer)
{
    // print the page
    QPainter painter(printer);
    int w = printer->pageRect().width();
    int h = printer->pageRect().height();
    QRect page(0, 0, w, h);

    QFont font = painter.font();
    font.setPixelSize((w+h)/100);
    painter.setFont(font);

    painter.drawText(page, Qt::AlignBottom | Qt::AlignRight,
                     QDateTime::currentDateTime().
                     toString(Qt::DefaultLocaleShortDate));

    page.adjust(w/20, h/20, -w/20, -h/20);

    /**
     * hides/disables grid for print and print preview and clears selection
     * so that it is not printed and the area in which items are present is
     * grabbed to be printed
     */
    view->scene->isGridVisible = false;
    view->scene->clearSelection();

    foreach (QGraphicsItem *item, view->scene->items())
        item->setTransform(QTransform::fromScale(1, -1));

    view->scene->setSceneRect(view->scene->itemsBoundingRect()
                              .adjusted(-10, -10, 10, 10));
    view->scene->render(&painter, page);
    view->modifySceneRect();

    foreach (QGraphicsItem *item, view->scene->items())
        item->setTransform(QTransform::fromScale(1, 1));

    view->scene->isGridVisible = true;
}

CadGraphicsView *MainWindow::createMdiView()
{
    // creates a graphicsView and add it to the MDI window
    view = new CadGraphicsView;
    QMdiSubWindow *w = mdiArea->addSubWindow(view);
    mdiArea->setActiveSubWindow(w);
    windowViewList.append(qMakePair(w, view));
    return view;
}

void MainWindow::setSnapping()
{
    // sets snapping value for Snap-to-Grid
    if (!actionGridSnap->isChecked())
        view->scene->snapTo = 1;
    else
        view->scene->snapTo = 50;
}

void MainWindow::toggleToolBar(bool ok)
{
    // hides/shows toolbar on toggling Toolbar option
    mainToolBar->setVisible(ok);
}

void MainWindow::hideStatusBar(bool ok)
{
    // hides/show the status bar on toggling of button
    mainStatusBar->setVisible(ok);
}

void MainWindow::showUndoStack()
{
    // calls an undo stack function of graphicsView
    view->showUndoStack();
}

void MainWindow::setNoMode()
{
    // calls the setNoMode function of graphicsView
    view->setNoMode();
}

void MainWindow::deleteItems()
{
    // calls the deleteItems function of graphicsScene
    view->scene->deleteItems();
}

void MainWindow::openFile()
{
    // open file dialog box
    QString openFileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        QString(),
                                                        tr("file Name(*.xml)"));

    if (openFileName.isNull())
        return;

    else
    {
        newFile();
        view->setWindowTitle(openFileName);
        --fileNumber;
        QFile file(openFileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        else
        {
            QXmlStreamReader  stream(&file);

            while (!stream.atEnd())
            {
                stream.readNext();

                if (stream.isStartElement())
                {
                    if (stream.name() == "SceneData")
                        view->scene->readStream(&stream);
                    else
                        stream.raiseError(QString("Unrecognised element '%1'").
                                          arg(stream.name().toString()));
                }
            }

            // check if error occured
            if (stream.hasError())
            {
                file.close();
                QMessageBox::warning(this, "Error",
                                     QString("Failed to load '%1' (%2)").
                                     arg(openFileName).arg(stream.errorString()));
                delete view->scene;
                return;
            }

            // close file and display useful message
            file.close();
            mainStatusBar->showMessage(QString("Loaded '%1' successfully")
                                       .arg(openFileName), 3000);
            return;
        }
    }
}

void MainWindow::autoSave()
{
    if (autoSaveCount && curFileName.startsWith("Drawing"))
        return;
    else
        saveFile(view->currentFile);
}

bool MainWindow::saveFileAs()
{
    // save file dialog box
    QFileDialog fileDialog(this);
    fileDialog.setDefaultSuffix("xml");
    fileDialog.setWindowModality(Qt::WindowModal);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.exec();

    QStringList files = fileDialog.selectedFiles();

    if(files.isEmpty())
        return false;

    autoSaveCount = false;

    return saveFile(files.at(0));
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    QFileInfo info(file);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        view->firstSave = false;
        return false;
    }

    else
    {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("SceneData");
        xmlWriter.writeAttribute("version", "v1.0");
        xmlWriter.writeStartElement("Entities");

        view->scene->writeStream(&xmlWriter);

        xmlWriter.writeEndElement();   //end of Entities
        xmlWriter.writeEndElement();   //end of SceneData
        mainStatusBar->showMessage(QString("File saved %1").arg(fileName));
        file.close();
        view->currentFile = fileName;
        view->setWindowTitle(info.fileName());
        return true;
    }
}

bool MainWindow::save()
{
    if (view->firstSave)
    {
        view->firstSave = false;
        return saveFileAs();
    }
    else
    {
        if (view->currentFile.isEmpty())
            return saveFileAs();
        else
            return saveFile(view->currentFile);
    }
}

void MainWindow::showGrid(bool b)
{
    // enables/disables grid
    if (!actionGrid->isChecked())
        view->scene->isGridVisible = false;
    else
        view->scene->isGridVisible = true;

    view->scene->update(view->scene->sceneRect());
}

void MainWindow::zoomIn()
{
    // Zoom in
    view->scale(view->scaleFactor, view->scaleFactor);
}

void MainWindow::zoomOut()
{
    // Zoom out
    view->scale(1.0 / view->scaleFactor, 1.0 / view->scaleFactor);
}

void MainWindow::panning()
{
    view->setNoMode();
    view->isPanning = true;
}

void MainWindow::selectAll()
{
    // selects all items in the scene
    view->scene->setMode(CadGraphicsScene::NoMode);
    view->scene->selectDeselectAllItems(1);
}

void MainWindow::deselectAll()
{
    // deselects all items in the scene
    view->scene->selectDeselectAllItems(0);
}

void MainWindow::selectWindow()
{
    QMessageBox messageBox;
    QString message = "This selection works by default.\nYou can drag the mouse"
            "in the drawing area to make selection using window.";
    messageBox.setText(message);
    messageBox.exec();
    view->viewport()->setCursor(Qt::CrossCursor);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();

    if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation?",
                                                  "Are you sure you want to exit?",
                                                  QMessageBox::Yes|QMessageBox::No))
        event->accept();
}

void MainWindow::showAboutDialog()
{
    aboutDialog = new QDialog(this);
    Ui::About aboutUi;
    aboutUi.setupUi(aboutDialog);
    connect(aboutUi.Close, SIGNAL(pressed()), aboutDialog, SLOT(close()));
    aboutDialog->show();
}

void MainWindow::setStatusBarMessage()
{
    messageMiddle->setText(view->scene->setStatusBarMessage());
}
