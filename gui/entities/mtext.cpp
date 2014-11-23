#include "mtext.h"

mText::mText(int i, QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    //assigns id
    id = i;
}

int mText::type() const
{
    // Enable the use of qgraphicsitem_cast with text item.
    return Type;
}

QVariant mText::itemChange(GraphicsItemChange change,
                           const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}

void mText::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void mText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
