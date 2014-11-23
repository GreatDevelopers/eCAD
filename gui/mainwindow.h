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
#include "ui_aboutdialog.h"
#include "cadgraphicsview.h"
#include "cadcommandwidget.h"
#include "cadscriptwidget.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QPainter *painter;
    QPrinter *printer;
    QPixmap image;
    QImage *imageObject;
    QGraphicsView *graphicsView;
    CadGraphicsView *view;
    CadCommandWidget *commandWidget;
    CadScriptWidget *scriptWidget;
    QDialog *aboutDialog;

    bool eventFilter(QObject *obj, QEvent *event);
    bool isEntitySelected;

private slots:
    void drawPoint();
    void drawLine();
    void drawCircle();
    void drawEllipse();
    void drawText();
    void newFile();
    void setNoMode();
    void showUndoStack();
    void toggleWidgets();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionInsert_Image_triggered();
    void selectAll();
    void deselectAll();
    void toggleMenuActions();
    void selectOneEntity();
    void deleteItems();
    void toggleActions(bool b);
    void setActions();
    void filePrintPreview();
    void closeEvent(QCloseEvent *event);
    void filePrint();
    void print(QPrinter *);
    void showAboutDialog();
    CadGraphicsView *createMdiView();
};

#endif // MAINWINDOW_H
