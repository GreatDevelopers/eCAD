/**
 * text.h
 *
 * Copyright (C) 2014  GreatDevelopers
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

#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>

class Text : public QGraphicsTextItem
{
    Q_OBJECT

public:
    Text(int, QPointF, QString);
    enum { Type = UserType + 5 };
    int type() const;
    QPointF getEndPoint();

    int id;
    QPointF position;
    QString str;

signals:
    void lostFocus(Text *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // TEXT_H
