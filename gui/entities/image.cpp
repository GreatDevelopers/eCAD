#include "image.h"

Image::Image(int i, QPointF p, QString s)
{
    // assigns id
    id = i;

    // set values of start point i.e topLeft corner
    startP = p;
    path = s;

    // creates a pixmap
    img.load(path);
    imagePixmap = QPixmap::fromImage(img);
}

QRectF Image::boundingRect() const
{
    // bounding rectangle for image
    return QRectF(startP, QSizeF(imagePixmap.size()));
}

int Image::type() const
{
    // Enable the use of qgraphicsitem_cast with image item.
    return Type;
}

void Image::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    painter->drawPixmap(startP, imagePixmap);
}
