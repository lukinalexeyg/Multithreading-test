#ifndef ELEMENTLISTITEM_H
#define ELEMENTLISTITEM_H

#include "element.h"

#include <QThread>

class ElementListItem
{
public:
    ElementListItem(Element *element, QThread *thread = nullptr);
    ~ElementListItem();

    Element *element;
    QThread *thread;
};

#endif // ELEMENTLISTITEM_H
