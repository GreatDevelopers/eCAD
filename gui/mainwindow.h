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
    CadGraphicsView *view;
    CadCommandWidget *commandWidget;
    CadScriptWidget *scriptWidget;
    QDialog *aboutDialog;
    typedef QPair<QMdiSubWindow *, CadGraphicsView *> windowViewPair;
    QList<windowViewPair> windowViewList;

    bool eventFilter(QObject *obj, QEvent *event);
    bool isEntitySelected;
    QString curFileName, fileName;
    int fileNumber;

private slots:
    void drawPoint();
    void drawLine();
    void drawCircle();
    void drawEllipse();
    void drawText();
    void drawArc();
    void newFile();
    void closeActiveWindow();
    void setNoMode();
    void showUndoStack();
    void toggleWidgets();
    void toggleToolBar(bool);
    void hideStatusBar(bool ok);
    void saveFile();
    void saveFileAs();
    void openFile();
    void showGrid(bool);
    void zoomIn();
    void zoomOut();
    void panning();
    void insertImage();
    void selectAll();
    void deselectAll();
    void cutOperation();
    void copyOperation();
    void pasteOperation();
    void toggleMenuActions();
    void selectOneEntity();
    void selectWindow();
    void invertSelection();
    void deleteItems();
    void toggleActions(bool b);
    void setActions();
    void filePrintPreview();
    void closeEvent(QCloseEvent *event);
    void filePrint();
    void print(QPrinter *);
    void showAboutDialog();
    void updateView();
    CadGraphicsView *createMdiView();
};

#endif // MAINWINDOW_H
