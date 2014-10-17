#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QXmlStreamWriter>
#include <QUndoStack>

#include "cadcommandadd.h"
#include "cadcommanddelete.h"
#include "cadcommandmove.h"

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
    void editorLostFocus(mText *item);
    void selectGroups();

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

    QList<QGraphicsItemGroup *> groupList;
    Point *pointItem;
    Line *lineItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
    mText *textItem;
    QColor myTextColor;
    QFont myFont;

    typedef QPair<QGraphicsItemGroup *, QPointF> itemPos;
    QList<itemPos> selectedGroups;
};

#endif // CADGRAPHICSSCENE_H
