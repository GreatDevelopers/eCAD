#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>

#include "point.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CadGraphicsScene(QObject *parent = 0);
    enum Mode { PointMode };

signals:
    void createdPoint(Point *pointItem);

public slots:
    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    Mode entityMode;
};



#endif // CADGRAPHICSSCENE_H
