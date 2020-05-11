#include "workermanager.h"
#include "log.h"



WorkerManager::WorkerManager(ElementList *elementList, QObject *parent) :
    QObject(parent),
    m_elementList(elementList),
    m_active(false)
{
}



WorkerManager::~WorkerManager()
{
    for (Worker *worker : qAsConst(m_workers))
        Worker::stopThread(worker->m_thread, 3000);
}



void WorkerManager::set(int count, int duration)
{
    const int dif = count - m_workers.count();

    if (dif > 0)
        for (int i = 0; i < qAbs(dif); ++i) {
            Worker *worker = new Worker(m_elementList);
            DEBUG_LOG worker << "+";
            m_workers.append(worker);
            worker->m_thread = new QThread(this);
            connect(worker->m_thread, &QThread::finished, worker, &QObject::deleteLater);
            worker->moveToThread(worker->m_thread);
            worker->m_thread->start();
            if (m_active)
                QMetaObject::invokeMethod(worker, "start", Qt::ConnectionType::QueuedConnection);
        }

    else if (dif < 0)
        for (int i = m_workers.count()-1; i >= count; --i) {
            Worker *worker = m_workers.at(i);
            DEBUG_LOG worker << "-";
            worker->stop();
            Worker::stopThread(worker->m_thread, 3000);
            m_workers.removeAt(i);
        }

    for (Worker *worker : qAsConst(m_workers))
        worker->setIdleDuration(duration);

    DEBUG_LOG m_workers.count();
}



void WorkerManager::start()
{
    m_active = true;

    for (Worker *worker : qAsConst(m_workers))
        QMetaObject::invokeMethod(worker, "start", Qt::ConnectionType::QueuedConnection);
}



void WorkerManager::stop()
{
    m_active = false;

    for (Worker *worker : qAsConst(m_workers))
        worker->stop();
}
