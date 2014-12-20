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
    QDialog *aboutDialog;
    typedef QPair<QMdiSubWindow *, CadGraphicsView *> windowViewPair;
    QList<windowViewPair> windowViewList;
    QList<CadScriptWidget *> scriptWidgetList;
    QList<CadCommandWidget *> commandWidgetList;
    QList<QAction *> undoList, redoList;

    bool eventFilter(QObject *obj, QEvent *event);
    bool isEntitySelected;
    bool autoSaveCount;
    QString curFileName, fileName;
    int fileNumber;

private slots:
    void newFile();
    void closeActiveWindow();
    void setNoMode();
    void showUndoStack();
    void toggleWidgets();
    void toggleToolBar(bool);
    void hideStatusBar(bool ok);
    bool saveFile(const QString &fileName);
    bool saveFileAs();
    bool save();
    void autoSave();
    void openFile();
    void exportFile(QAction *action);
    void showGrid(bool);
    void zoomIn();
    void zoomOut();
    void panning();
    void selectAll();
    void deselectAll();
    void cutOperation();
    void copyOperation();
    void pasteOperation();
    void toggleMenuActions();
    void selectWindow();
    void deleteItems();
    void toggleActions(bool b);
    void filePrintPreview();
    void closeEvent(QCloseEvent *event);
    void filePrint();
    void print(QPrinter *);
    void showAboutDialog();
    void updateView();
    CadGraphicsView *createMdiView();
};

#endif // MAINWINDOW_H
