#ifndef CADGRAPHICSVIEW_H
#define CADGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

#include "cadgraphicsscene.h"

class CadGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CadGraphicsView();
    ~CadGraphicsView();

    void newFile();
    bool drawPoint();

protected:
    void wheelEvent(QWheelEvent* event);

private:
    bool isUntitled;
    double scaleFactor;
    QString curFileName;
    CadGraphicsScene *scene;
};

#endif // CADGRAPHICSVIEW_H
