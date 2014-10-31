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

    QFont font() const
    {
        return myFont;
    }

    QColor textColor() const
    {
        return myTextColor;
    }

    void setTextColor(const QColor &color);
    void setFont(const QFont &font);
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
    void textInserted(QGraphicsTextItem *item);
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
    QPen paintpen, linePen;

    QList<QGraphicsItem *> itemList;
    Point *pointItem;
    Line *lineItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
    mText *textItem;
    QColor myTextColor;
    QFont myFont;

    typedef QPair<QGraphicsItem *, QPointF> itemPos;
    QList<itemPos> selectedItems;
};

#endif // CADGRAPHICSSCENE_H
