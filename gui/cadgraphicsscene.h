#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QXmlStreamWriter>

#include "point.h"
#include "line.h"
#include "circle.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CadGraphicsScene(QObject *parent = 0);
    enum Mode { NoMode, PointMode, LineMode, CircleMode };

    void writeStream(QXmlStreamWriter *stream);
    void readStream(QXmlStreamReader *stream);

signals:
    void createdPoint(Point *pointItem);

public slots:
    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void setFlags();

private:
    Mode entityMode;
    bool mFirstClick;
    bool mSecondClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF start_p, end_p, move_p, check_p;
    QPen paintpen, linePen;
};



#endif // CADGRAPHICSSCENE_H
