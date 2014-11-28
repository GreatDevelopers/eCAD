#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsTextItem>
#include <QPen>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Text : public QGraphicsTextItem
{
    Q_OBJECT

public:
    Text(int, QPointF, QString);
    enum { Type = UserType + 5 };
    int type() const;

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
