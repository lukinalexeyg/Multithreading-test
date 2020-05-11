#include <QTimer>
#include <QRandomGenerator>
#include "worker.h"
#include "log.h"



Worker::Worker(ElementList *elementList, QObject *parent) :
    QObject(parent),
    m_elementList(elementList),
    m_idleDuration(WORKER_IDLE_DURATION),
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
    const int variant = QRandomGenerator::global()->bounded(3);

    Element *element;

    switch (variant) {
    case 0:
        element = new Element1;
        break;
    case 1:
        element = new Element2;
        break;
    case 2:
        element = new Element3;
        break;
    }

    m_elementList->append(element);

    sleep();
    invokeAction();
}



void Worker::editElement()
{
    Element *element = m_elementList->get(ElementList::Random);

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
    m_elementList->remove(ElementList::Random);
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



void Worker::stopThread(QThread *thread, ulong time)
{
    thread->quit();

    if (!thread->wait(time)) {
        thread->terminate();
        thread->wait();
    }
}
