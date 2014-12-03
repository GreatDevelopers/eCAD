#ifndef IMAGE_H
#define IMAGE_H

#include <QPainter>
#include <QGraphicsPixmapItem>

class Image : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Image(int i, QPointF, QString);
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 7 };
    int type() const;

    int id;
    QPointF startP;
    QString path;
    QImage img;
    QPixmap imagePixmap;
};

#endif // IMAGE_H
