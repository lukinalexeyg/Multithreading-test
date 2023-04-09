#ifndef ELEMENTLISTITEM_H
#define ELEMENTLISTITEM_H

#include "element.h"

#include <QThread>

class ElementListItem
{
public:
    ElementListItem(const ElementPtr &element, QThread *thread = nullptr);

    const ElementPtr &element() const   { return m_element; }

    const QThread* thread() const       { return m_thread; }
    void setThread(QThread *thread)     { m_thread = thread; }

private:
    ElementPtr m_element;
    QThread *m_thread;
};

#endif // ELEMENTLISTITEM_H
