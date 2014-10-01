#ifndef CADGRAPHICSVIEW_H
#define CADGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "cadgraphicsscene.h"

class CadGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CadGraphicsView();
    void newFile();
    void drawPoint();
    void setNoMode();

protected:
     void wheelEvent(QWheelEvent* event);

private:
    CadGraphicsScene *scene;
    QString curFileName;
    bool isUntitled;
    double scaleFactor;
};

#endif // CADGRAPHICSVIEW_H
