#ifndef CADGRAPHICSVIEW_H
#define CADGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QUndoStack>
#include <QUndoView>
#include <QFileDialog>

#include "cadgraphicsscene.h"
#include "cadscriptwidget.h"
#include "cadcommandwidget.h"

class CadGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CadGraphicsView();
    void newFile();
    void cursorMode();
    void showUndoStack();

    CadGraphicsScene *scene;
    QUndoStack *undoStack;
    QUndoView *undoView;
    CadScriptWidget *scriptWidget;
    CadCommandWidget *commandWidget;

    double scaleFactor;
    bool isPanning;

public slots:
    void drawPoint();
    void drawLine();
    void drawCircle();
    void drawEllipse();
    void drawText();
    void drawArc();
    void drawImage();
    void setNoMode();
    void selectWindow();
    void deleteSingleItem();

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // CADGRAPHICSVIEW_H
