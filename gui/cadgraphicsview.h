#ifndef CADGRAPHICSVIEW_H
#define CADGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QUndoStack>
#include <QUndoView>

#include "cadgraphicsscene.h"

class CadGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CadGraphicsView();
    void newFile();
    void drawPoint();
    void drawLine();
    void drawCircle();
    void drawEllipse();
    void drawText();
    void drawArc();
    void setNoMode();
    void cursorMode();
    void showUndoStack();
    void editCut();
    void editCopy();
    void editPaste();

    CadGraphicsScene *scene;
    QUndoStack *undoStack;
    QUndoView *undoView;

    double scaleFactor;
    bool isPanning;

public slots:
    void selectWindow();

protected:
    void wheelEvent(QWheelEvent *event);

private:
    QString curFileName;
    bool isUntitled;
};

#endif // CADGRAPHICSVIEW_H
