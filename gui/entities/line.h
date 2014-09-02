#ifndef LINE_H
#define LINE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

class Line: public QObject, public QGraphicsItem{
    Q_OBJECT
public:
    Line();
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int start_x, start_y, end_x, end_y;
    bool mFirstClick;
    bool mSecondClick;
    bool mPaintFlag;
    QVector<QPointF> stuff;
    QPointF start_p, end_p, move_p, check_p;
    QPen paintpen, linePen;
};

#endif // LINE_H
