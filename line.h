#ifndef LINE_H
#define LINE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include "ui_mainwindow.h"

class line: public QObject, public QGraphicsItem{
    Q_OBJECT
public:
    line();
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    //void keyPressEvent(QKeyEvent *e);

private:
    int x1, y1, x2, y2, w, h;
    bool mFirstClick;
    bool mSecondClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF p1, p2, move_p, check_p;
    QPen paintpen, linePen;

signals:
    void DrawFinished();
};

#endif // LINE_H
