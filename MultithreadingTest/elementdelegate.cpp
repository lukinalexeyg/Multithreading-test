#include "elementdelegate.h"

#include "elementmodel.h"

#include <QPainter>



void ElementDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
         return;

    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->setBrush(QBrush(QColor(0xcc, 0xe8, 0xff)));
        painter->fillRect(option.rect, painter->brush());
    }

    ElementModelItem *elementModelItem = index.data().value<ElementModelItem*>();

    if (elementModelItem->locked())
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->drawText(option.rect, option.displayAlignment, elementModelItem->text());

    painter->restore();
}
