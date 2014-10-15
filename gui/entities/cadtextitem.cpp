#include "cadtextitem.h"

cadtextitem::cadtextitem(int i, QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    //assigns id
    id = i;

}

int cadtextitem::type() const
{
    // Enable the use of qgraphicsitem_cast with text item.
    return Type;
}


QVariant cadtextitem::itemChange(GraphicsItemChange change,
                                 const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}

void cadtextitem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void cadtextitem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
