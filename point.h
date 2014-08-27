#ifndef POINT_H
#define POINT_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>

using namespace std;

class point: public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    point();
    QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QString coordinateX, coordinateY;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QPointF p1;
    int x1, y1;
    bool mClick;
    bool mPaintFlag;
};

#endif // POINT_H
