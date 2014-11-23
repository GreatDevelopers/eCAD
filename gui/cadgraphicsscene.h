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
#include "getEntity.h"
#include "clipboardstack.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit CadGraphicsScene(QObject *parent, QUndoStack *);
    enum Mode { NoMode, PointMode, LineMode, CircleMode, EllipseMode, TextMode,
                ArcMode };

    void writeStream(QXmlStreamWriter *stream);
    void readStream(QXmlStreamReader *stream);
    void drawBackground(QPainter *painter, const QRectF &rect);

    typedef QPair<QGraphicsItem *, QPointF> entityPos;
    QList<entityPos> selectedEntities;

public slots:
    void setMode(Mode mode);
    void selectItems();
    void selectDeselectAllItems(bool b);
    void invertSelection();
    void deleteItems();
    void editorLostFocus(mText *item);
    void cut(getEntity *);
    void copy(getEntity *);
    void paste(const QPointF &pos);
    void menuAction(QAction *);

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
    bool mFirstClick;
    bool mSecondClick;
    bool mThirdClick;
    bool mPaintFlag;
    QPointF startP, midP, endP;
    qreal x, y, rad, radM;
    int id;
    QString str;
    QPen paintpen, linePen;
    QPointF contextPosition;

    QList<QGraphicsItem *> itemList;
    Point *pointItem;
    Line *lineItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
    mText *textItem;
    Arc *arcItem;
    QUndoStack *mUndoStack;
    QMenu *contextMenu;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QGraphicsItem *contextItem;
};

#endif // CADGRAPHICSSCENE_H
