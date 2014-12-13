#ifndef IMAGE_H
#define IMAGE_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "getEntity.h"

class Image : public getEntity
{
    Q_OBJECT

public:
    Image(QObject *parent = 0) : getEntity(parent) {}
    Image(int i, QPointF, QString);
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    enum { Type = UserType + 7 };
    int type() const;
    getEntity *clone(int);

    int id;
    QPointF startP;
    QString path;
    QImage img;
    QPixmap imagePixmap;
};

#endif // IMAGE_H
