#include "elementlistitem.h"



ElementListItem::ElementListItem(const ElementPtr &element, QThread *thread) :
    m_element(element),
    m_thread(thread)
{
}
