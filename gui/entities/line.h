#ifndef LINE_H
#define LINE_H

#include <QPainter>
#include <QGraphicsLineItem>

class Line : public QObject, public QGraphicsLineItem {
        Q_OBJECT
    public:
        Line(int, QPointF, QPointF);
        QRectF boundingRect() const;
        virtual void paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget);
        enum { Type = UserType + 2 };
        int type() const;
        int id;

        QPointF startP, endP;

    private:
        QVector<QPointF> stuff;
};

#endif // LINE_H
