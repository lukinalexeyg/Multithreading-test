#ifndef WORKERMANAGER_H
#define WORKERMANAGER_H

#include "elementlist.h"
#include "worker.h"

static constexpr int WORKERS_COUNT = 3;
static constexpr int MAX_WORKERS_COUNT = 100;

class WorkerManager : public QObject
{
    Q_OBJECT

public:
    explicit WorkerManager(ElementList *elementList, QObject *parent = nullptr);
    ~WorkerManager();

    Q_SLOT void set(int count, int duration);
    Q_SLOT void reset() { set(WORKERS_COUNT, WORKER_IDLE_DURATION); }

    void start();
    void stop();

public:
    QThread *m_thread;

private:
    ElementList *m_elementList;
    QList<Worker*> m_workers;
    bool m_active;
};

#endif // WORKERMANAGER_H
