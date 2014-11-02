#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QXmlStreamWriter>
#include <QUndoStack>

#include "cadcommands/cadcommandadd.h"
#include "cadcommands/cadcommanddelete.h"
#include "cadcommands/cadcommandmove.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CadGraphicsScene(QObject *parent, QUndoStack *);
    enum Mode { NoMode, PointMode, LineMode, CircleMode, EllipseMode, TextMode };

    void deleteItems();
    void writeStream(QXmlStreamWriter *stream);
    void readStream(QXmlStreamReader *stream);

public slots:
    void setMode(Mode mode);
    void selectItems();
    void editorLostFocus(mText *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void setFlags();
    void areItemsSelectable(bool);

signals:
    void itemSelected(QGraphicsItem *item);

private:
    Mode entityMode;
    QUndoStack *mUndoStack;

    bool mFirstClick;
    bool mSecondClick;
    bool mThirdClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF start_p, mid_p, end_p, move_p, check_p;
    qreal _x, _y, _rad, _radM;
    int id;
    QString _str;
    QPen paintpen, linePen;

    QList<QGraphicsItem *> itemList;
    Point *pointItem;
    Line *lineItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
    mText *textItem;

    typedef QPair<QGraphicsItem *, QPointF> itemPos;
    QList<itemPos> selectedItems;
};

#endif // CADGRAPHICSSCENE_H
