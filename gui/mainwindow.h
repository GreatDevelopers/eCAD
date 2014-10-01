#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QMdiArea>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>

#include "ui_mainwindow.h"
#include "cadgraphicsview.h"
//#include "line.h"
//#include "circle.h"
//#include "ellipse.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    bool mFirstClick;
    bool mPaintFlag;
    double mStartX;
    double mStartY;
    double mEndX;
    double mEndY;
    double scaleFactor;

    QPainter *painter;
    QPrinter *printer;
    QPixmap image;
    QImage *imageObject;
    CadGraphicsView *view;

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void drawPoint();
//    void drawLine();
//    void drawCircle();
//    void drawEllipse();
    void newFile();

    void setNoMode();

    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionInsert_Image_triggered();

    void filePrintPreview();
    void filePrint();
    void print(QPrinter *);

    CadGraphicsView *createMdiView();
};

#endif // MAINWINDOW_H
