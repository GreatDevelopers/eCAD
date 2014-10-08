#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QXmlStreamWriter>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CadGraphicsScene(QObject *parent = 0);
    enum Mode { NoMode, PointMode, LineMode, CircleMode, EllipseMode };

    void writeStream(QXmlStreamWriter *stream);
    void readStream(QXmlStreamReader *stream);

signals:
    void createdPoint(Point *pointItem);

public slots:
    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void setFlags();
    void areItemsSelectable(bool);

private:
    Mode entityMode;
    bool mFirstClick;
    bool mSecondClick;
    bool mThirdClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF start_p, mid_p, end_p, move_p, check_p;
    QPen paintpen, linePen;
};

#endif // CADGRAPHICSSCENE_H
