#ifndef ELEMENTDELEGATE_H
#define ELEMENTDELEGATE_H

#include <QStyledItemDelegate>

class ElementDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // ELEMENTDELEGATE_H
