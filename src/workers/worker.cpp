#include "worker.h"

#include "log.h"

#include <QRandomGenerator>
#include <QTimer>



Worker::Worker(const ElementListPtr &elementList, QObject *parent) :
    QObject(parent),
    m_elementList(elementList),
    m_idleDuration(defaultIdleDuration),
    m_active(false)
{
}



void Worker::start()
{
    if (m_active)
        return;
    m_active = true;
    doAction();
}



void Worker::appendElement()
{
    appendElement(m_elementList);

    sleep();
    invokeAction();
}



void Worker::appendElement(const ElementListPtr &elementList, const QString &userText)
{
    const int variant = QRandomGenerator::global()->bounded(3);

    ElementPtr element = nullptr;

    switch (variant) {
        case 0:
            element = QSharedPointer<Element1>::create(userText);
            break;
        case 1:
            element = QSharedPointer<Element2>::create(userText);
            break;
        case 2:
            element = QSharedPointer<Element3>::create(userText);
            break;
    }

    elementList->append(element);
}



void Worker::editElement()
{
    const ElementPtr element = m_elementList->get(ElementList::UnusedElementType::Random);

    if (element != nullptr) {
        element->incCounter();
        m_elementList->set(element);
    }

    sleep();

    if (element != nullptr)
        m_elementList->release(element);

    invokeAction();
}



void Worker::removeElement()
{
    m_elementList->remove(ElementList::UnusedElementType::Random);
    sleep();
    invokeAction();
}



void Worker::sleep()
{
    QThread::msleep(static_cast<quint32>(m_idleDuration));
}



void Worker::invokeAction()
{
    if (m_active)
        QTimer::singleShot(0, this, &Worker::doAction);
}



void Worker::doAction()
{
    const int action = QRandomGenerator::global()->bounded(Action::Count);

    switch (action) {
        case Action::AppendElement :
            appendElement();
            break;
        case Action::EditElement :
            editElement();
            break;
        case Action::RemoveElement :
            removeElement();
            break;
    }
}



void Worker::stopThread(QThread *thread, const ulong time)
{
    thread->quit();

    if (!thread->wait(time)) {
        thread->terminate();
        thread->wait();
    }
}
