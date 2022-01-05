#include "elementlist.h"

#include <QRandomGenerator>



ElementList::ElementList(QObject *parent) :
    QObject(parent)
{
}



ElementList::~ElementList()
{
    QMutexLocker mutexLocker(&m_mutex);
    qDeleteAll(m_items);
}



int ElementList::count() const
{
    QMutexLocker mutexLocker(&m_mutex);
    return m_items.count();
}



void ElementList::append(Element *element)
{
    QMutexLocker mutexLocker(&m_mutex);

    ElementListItem *item = new ElementListItem(element);
    m_items.append(item);

    emit appended(element->visibleText());
}



bool ElementList::remove(int index)
{
    QMutexLocker mutexLocker(&m_mutex);
    return _remove(index);
}



bool ElementList::remove(Element *element)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = _indexOf(element);

    return _remove(index, QThread::currentThread());
}



bool ElementList::remove(UnusedElementType type)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = unusedElementIndex(type);

    return _remove(index);
}



bool ElementList::_remove(int index, QThread *thread)
{
    if (index < 0 || index >= m_items.count())
        return false;

    QThread *t = m_items.at(index)->thread;

    if (t == thread) {
        delete m_items.takeAt(index);
        emit removed(index);
        return true;
    }

    return false;
}



Element *ElementList::get(int index)
{
    QMutexLocker mutexLocker(&m_mutex);
    return _get(index);
}



Element *ElementList::get(UnusedElementType type)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = unusedElementIndex(type);

    return _get(index);
}



Element *ElementList::_get(int index)
{
    if (index < 0 || index >= m_items.count())
        return nullptr;

    ElementListItem *item = m_items.at(index);

    if (item->thread != nullptr)
        return nullptr;

    item->thread = QThread::currentThread();

    emit edited(index, item->element->visibleText(), true);

    return item->element;
}



bool ElementList::set(Element *element)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = _indexOf(element);
    if (index < 0)
        return false;

    ElementListItem *item = m_items.at(index);
    emit edited(index, item->element->visibleText(), item->thread != nullptr);

    return true;
}



bool ElementList::release(Element *element)
{
    return release(false, element);
}



bool ElementList::releaseAll()
{
    return release(true, nullptr);
}



bool ElementList::release(bool all, Element *element)
{
    QMutexLocker mutexLocker(&m_mutex);

    QThread *currentThread = QThread::currentThread();
    bool released = false;

    for (int i = 0; i < m_items.count(); ++i) {
        ElementListItem *item = m_items.at(i);

        if ((all || item->element == element) && item->thread == currentThread) {
            item->thread = nullptr;
            emit edited(i, item->element->visibleText(), false);

            if (all)
                released = true;
            else
                return true;
        }
    }

    return released;
}



int ElementList::indexOf(Element *element) const
{
    QMutexLocker mutexLocker(&m_mutex);
    return _indexOf(element);
}



int ElementList::_indexOf(Element *element) const
{
    for (int i = 0; i < m_items.count(); ++i)
        if (m_items.at(i)->element == element)
            return i;

    return -1;
}



int ElementList::unusedElementIndex(UnusedElementType type) const
{
    if (m_items.isEmpty())
        return -1;

    QList<int> indexes;

    for (int i = 0; i < m_items.count(); ++i)
        if (m_items.at(i)->thread == nullptr)
            indexes.append(i);

    if (indexes.isEmpty())
        return -1;

    if (type == First)
        return indexes.first();

    if (type == Last)
        return indexes.last();

    const int i = QRandomGenerator::global()->bounded(indexes.count());

    return indexes.at(i);
}
