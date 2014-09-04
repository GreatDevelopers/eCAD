#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>

#include "ui_mainwindow.h"
#include "cadgraphicsscene.h"
#include "cadgraphicsview.h"
#include "mdichild.h"
//#include "line.h"
//#include "circle.h"
//#include "ellipse.h"
#include "point.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    CadGraphicsView *activeMdiChild();
    MdiChild *getMdiChild();
    QPrinter *printer;
    QPixmap image;
    QImage *imageObject;

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void drawPoint();
//    void drawLine();
//    void drawCircle();
//    void drawEllipse();
    void newFile();
    MdiChild *createMdiChild();

    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionInsert_Image_triggered();

    void filePrintPreview();
    void filePrint();
    void print(QPrinter *);

};

#endif // MAINWINDOW_H
