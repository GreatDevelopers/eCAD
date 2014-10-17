#ifndef text_H
#define text_H

#include <QGraphicsTextItem>
#include <QPen>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Text:public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 3 };
    Text( int, QGraphicsItem *parent=0);
    int type() const;
    int id;
signals:
    void lostFocus(Text *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // text_H
