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



void ElementList::append(const ElementPtr &element)
{
    QMutexLocker mutexLocker(&m_mutex);

    ElementListItem *item = new ElementListItem(element);
    m_items.append(item);

    emit appended(element->displayedText());
}



bool ElementList::remove(const int index)
{
    QMutexLocker mutexLocker(&m_mutex);
    return _remove(index);
}



bool ElementList::remove(const ElementPtr &element)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = _indexOf(element);

    return _remove(index, QThread::currentThread());
}



bool ElementList::remove(const UnusedElementType type)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = unusedElementIndex(type);

    return _remove(index);
}



bool ElementList::_remove(const int index, QThread *thread)
{
    if (index < 0 || index >= m_items.count())
        return false;

    const QThread *t = m_items.at(index)->thread();

    if (t == thread) {
        delete m_items.takeAt(index);
        emit removed(index);
        return true;
    }

    return false;
}



ElementPtr ElementList::get(const int index)
{
    QMutexLocker mutexLocker(&m_mutex);
    return _get(index);
}



ElementPtr ElementList::get(const UnusedElementType type)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = unusedElementIndex(type);

    return _get(index);
}



ElementPtr ElementList::_get(const int index)
{
    if (index < 0 || index >= m_items.count())
        return nullptr;

    ElementListItem *item = m_items.at(index);

    if (item->thread() != nullptr)
        return nullptr;

    item->setThread(QThread::currentThread());

    emit edited(index, item->element()->displayedText(), true);

    return item->element();
}



bool ElementList::set(const ElementPtr &element)
{
    QMutexLocker mutexLocker(&m_mutex);

    const int index = _indexOf(element);
    if (index < 0)
        return false;

    ElementListItem *item = m_items.at(index);
    emit edited(index, item->element()->displayedText(), item->thread() != nullptr);

    return true;
}



bool ElementList::release(const ElementPtr &element)
{
    return release(false, element);
}



bool ElementList::releaseAll()
{
    return release(true, nullptr);
}



bool ElementList::release(bool all, const ElementPtr &element)
{
    QMutexLocker mutexLocker(&m_mutex);

    QThread *currentThread = QThread::currentThread();
    bool released = false;

    for (int i = 0; i < m_items.count(); ++i) {
        ElementListItem *item = m_items.at(i);

        if ((all || item->element() == element) && item->thread() == currentThread) {
            item->setThread(nullptr);
            emit edited(i, item->element()->displayedText(), false);

            if (all)
                released = true;
            else
                return true;
        }
    }

    return released;
}



int ElementList::indexOf(const ElementPtr &element) const
{
    QMutexLocker mutexLocker(&m_mutex);
    return _indexOf(element);
}



int ElementList::_indexOf(const ElementPtr &element) const
{
    for (int i = 0; i < m_items.count(); ++i)
        if (m_items.at(i)->element() == element)
            return i;

    return -1;
}



int ElementList::unusedElementIndex(const UnusedElementType type) const
{
    if (m_items.isEmpty())
        return -1;

    QList<int> indexes;

    for (int i = 0; i < m_items.count(); ++i)
        if (m_items.at(i)->thread() == nullptr)
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
