/**
 * image.cpp
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
    qreal extra = 1.0;

    // bounding rectangle for image
    return QRectF(QPointF(startP.x(), startP.y() - img.height()),
                  QSizeF(imagePixmap.size())).
            adjusted(-extra, -extra, extra, extra);
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

    if (option->state & QStyle::State_Selected)
    {
        // sets pen for bounding rect after selection
        painter->setPen(paintpen);
        painter->drawRect(boundingRect());
    }

    painter->scale(1, -1);
    painter->translate(0, -2 * startP.y());
    painter->drawPixmap(startP, imagePixmap);
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

QVector<QPointF> Image::getEndPoints()
{
    // returns vector containing end points of image
    endPoints.append(startP);
    endPoints.append(QPointF(startP.x() + img.width(), startP.y()));
    endPoints.append(QPointF(startP.x(), startP.y() - img.height()));
    endPoints.append(QPointF(startP.x() + img.width(), startP.y() - img.height()));
    return endPoints;
}

QPointF Image::getCenter()
{
    center = QPointF(startP.x() + img.width() / 2, startP.y() - img.height() / 2);
    return center;
}

QVector<QPointF> Image::getMiddlePoints()
{
    // returns vector containing center points of image
    middlePoints.append(QPointF(startP.x() + (img.width() / 2), startP.y()));
    middlePoints.append(QPointF(startP.x(), startP.y() - img.height() / 2));
    middlePoints.append(QPointF(startP.x() + (img.width() / 2),
                                startP.y() - img.height()));
    middlePoints.append(QPointF(startP.x() + img.width(),
                                startP.y() - (img.height() / 2)));
    return middlePoints;
}
