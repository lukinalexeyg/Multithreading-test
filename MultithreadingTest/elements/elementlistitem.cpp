#include "elementlistitem.h"



ElementListItem::ElementListItem(Element *element, QThread *thread) : element(element), thread(thread)
{
}



ElementListItem::~ElementListItem()
{
    delete element;
}
