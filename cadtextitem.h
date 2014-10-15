#ifndef cadtextitem_H
#define cadtextitem_H

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
    cadtextitem( int, QGraphicsItem *parent=0);
    int type() const;
    int id;
signals:
    void lostFocus(cadtextitem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // cadtextitem_H
