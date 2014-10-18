#ifndef MTEXT_H
#define MTEXT_H

#include <QGraphicsTextItem>
#include <QPen>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class mText : public QGraphicsTextItem
{
    Q_OBJECT
public:
    mText( int, QGraphicsItem *parent=0 );
    enum { Type = UserType + 5 };
    int type() const;
    int id;

signals:
    void lostFocus(mText *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MTEXT_H
