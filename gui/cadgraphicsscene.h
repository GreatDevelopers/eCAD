#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QXmlStreamWriter>
#include <QUndoStack>

#include "cadcommandadd.h"
#include "cadcommanddelete.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CadGraphicsScene(QObject *parent, QUndoStack *);
    enum Mode { NoMode, PointMode, LineMode, CircleMode, EllipseMode };

    void writeStream(QXmlStreamWriter *stream);
    void readStream(QXmlStreamReader *stream);

public slots:
    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void setFlags();
    void areItemsSelectable(bool);

private:
    Mode entityMode;
    QUndoStack *mUndoStack;

    bool mFirstClick;
    bool mSecondClick;
    bool mThirdClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF start_p, mid_p, end_p, move_p, check_p;
    QPen paintpen, linePen;

    QList<QGraphicsItemGroup*> groupList;
    Point *pointItem;
    Line *lineItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
};

#endif // CADGRAPHICSSCENE_H
