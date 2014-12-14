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
    QPen paintpen(Qt::black);
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);
    paintpen.setStyle(Qt::DashLine);
    paintpen.setColor(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawPixmap(startP, imagePixmap);

    if (option->state & QStyle::State_Selected)
    {
        // sets pen for bounding rect after selection
        painter->setPen(paintpen);
        painter->drawRect(boundingRect());
    }
}

getEntity *Image::clone(int i)
{
    Image *image = new Image;
    image->id = i;
    image->startP = startP;
    image->path = path;
    image->img = img;
    image->imagePixmap = imagePixmap;
    return image;
}
