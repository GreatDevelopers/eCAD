#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QXmlStreamWriter>
#include <QUndoStack>
#include <cmath>
#include <QMenu>

#include "cadcommands/cadcommandadd.h"
#include "cadcommands/cadcommanddelete.h"
#include "cadcommands/cadcommandmove.h"
#include "gEntity.h"
#include "clipboard.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CadGraphicsScene(QObject *parent, QUndoStack *);
    enum Mode { NoMode, PointMode, LineMode, CircleMode, EllipseMode, TextMode };

    void deleteItems();
    void writeStream(QXmlStreamWriter *stream);
    void readStream(QXmlStreamReader *stream);
    void drawBackground(QPainter *painter, const QRectF &rect);

public slots:
    void setMode(Mode mode);
    void selectItems();
    void editorLostFocus(mText *item);
    void cut(gEntity *);
    void copy(gEntity *);
    void paste(const QPointF &pos);
    void contextMenuaction(QAction *);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *mouseEvent);
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
    QPointF startP, midP, endP, moveP, checkP;
    qreal x, y, rad, radM;
    int id;
    QString str;
    QPen paintpen, linePen;

    QList<QGraphicsItem *> itemList;
    Point *pointItem;
    Line *lineItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
    mText *textItem;

    QMenu *contextmenu;
    QAction *cutaction;
    QAction *copyaction;
    QAction *pasteaction;
    QGraphicsItem *contextitem;
    QPointF cpos;

    typedef QPair<QGraphicsItem *, QPointF> pointPos;
    typedef QPair<QGraphicsItem *, QLineF> linePos;
    QList<pointPos> selectedPoints;
    QList<linePos> selectedLines;
};

#endif // CADGRAPHICSSCENE_H
