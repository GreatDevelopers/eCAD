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
    void setNoMode();
    void showUndoStack();
    CadGraphicsScene *scene;
    QUndoStack* undoStack;
    QUndoView* undoView;

protected:
    void wheelEvent(QWheelEvent* event);

private:
    QString curFileName;
    bool isUntitled;
    double scaleFactor;
};

#endif // CADGRAPHICSVIEW_H
