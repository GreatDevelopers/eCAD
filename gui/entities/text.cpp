#include "text.h"

Text::Text(int i, QPointF p, QString s)
{
    //assigns id
    id = i;

    // sets the position and string value
    position = p;
    str = s;
}

int Text::type() const
{
    // Enable the use of qgraphicsitem_cast with text item.
    return Type;
}

QVariant Text::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);

    return value;
}

void Text::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void Text::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);

    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
