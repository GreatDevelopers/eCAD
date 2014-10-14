#ifndef CADTEXTITEM_H
#define CADTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class cadtextitem:public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 3 };
    cadtextitem(QGraphicsItem *parent = 0);
    int type() const { return Type; }

signals:
    void lostFocus(cadtextitem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CADTEXTITEM_H
