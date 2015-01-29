/**
 * cadgraphicsscene.h
 *
 * Copyright (C) 2014 GreatDevelopers
 *
 * eCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CADGRAPHICSSCENE_H
#define CADGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QXmlStreamWriter>
#include <QUndoStack>
#include <QMenu>
#include <QToolTip>

#include "qmath.h"
#include "cadcommands/cadcommandadd.h"
#include "cadcommands/cadcommanddelete.h"
#include "cadcommands/cadcommandmove.h"
#include "clipboardstack.h"

class CadGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit CadGraphicsScene(QObject *parent, QUndoStack *);
    enum Mode { NoMode, PointMode, LineMode, CircleMode, EllipseMode, TextMode,
                ArcMode, ImageMode, DeleteMode, DimHorizontalMode,
                DimVerticalMode, DimRadialMode, DimDiametricMode,
                StartPointLineMode, MiddlePointLineMode, EndPointLineMode,
                CenterPointCircleMode, TwoPointCircleMode };

    void writeStream(QXmlStreamWriter *stream);
    void readStream(QXmlStreamReader *stream);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void setFlags();
    void drawEntity(QGraphicsItem *item);
    bool eventFilter(QObject *watched, QEvent *event);
    qreal roundOff(qreal, qreal);
    QString setStatusBarMessage();

    typedef QPair<QGraphicsItem *, QPointF> entityPos;
    QList<entityPos> selectedEntities;
    QPointF startP, midP, endP, tempPoint;
    bool isInvertedSelection;
    bool isGridVisible;
    int id, contextItemId;
    QString imagePath;
    QString message;
    qreal snapTo;
    QPointF endPoint;
    QPointF centerPoint;
    QPointF middlePoint;

    double lineLength;
    double lineAngle;
    double circleRadius;

    bool endPointSnap;
    bool centerSnap;
    bool middleSnap;

public slots:
    void setMode(Mode mode);
    void selectItems();
    void selectDeselectAllItems(bool b);
    void invertSelection();
    void deleteItems();
    void editorLostFocus(Text *item);
    void cut(getEntity *);
    void copy(getEntity *);
    void paste(const QPointF &pos);
    void menuAction(QAction *);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void areItemsSelectable(bool);

signals:
    void itemSelected(QGraphicsItem *item);
    void setSelectionSignal();
    void setMessage();

private:
    Mode entityMode;
    bool mFirstClick;
    bool mSecondClick;
    bool mThirdClick;
    bool mPaintFlag;
    qreal x, y, rad, radM, angle, dx, dy, radValue, diaValue;
    QString str;
    QPen paintpen, linePen;
    QPointF contextPosition;
    QPointF lineEndPoint;
    QPointF lineStartPoint;
    QPointF arcStartPoint, arcMidPoint, arcEndPoint;
    float differenceX;
    float differenceY;
    QList<QPointF> endPointsList;
    QList<QPointF> centerPointsList;
    QList<QPointF> midPointsList;

    QList<QGraphicsItem *> itemList;
    QList<QGraphicsItem *> previewList;
    Point *pointItem;
    Line *lineItem;
    Line *horizontalAxis, *verticalAxis;
    Circle *circleItem;
    Ellipse *ellipseItem;
    Text *textItem;
    Arc *arcItem;
    Image *imageItem;
    DimHorizontal *dimHorizontalItem;
    DimVertical *dimVerticalItem;
    DimRadial *dimRadialItem;
    DimDiametric *dimDiametricItem;
    QUndoStack *mUndoStack;
    QMenu *contextMenu;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QGraphicsItem *contextItem;
    QGraphicsItem *clickedItem;
};

#endif // CADGRAPHICSSCENE_H
