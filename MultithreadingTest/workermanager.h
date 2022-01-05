#ifndef WORKERMANAGER_H
#define WORKERMANAGER_H

#include "elementlist.h"
#include "worker.h"

class WorkerManager : public QObject
{
    Q_OBJECT

public:
    explicit WorkerManager(ElementList *elementList, QObject *parent = nullptr);
    ~WorkerManager();

    Q_SLOT void set(int count, int duration);
    Q_SLOT void reset();

    void start();
    void stop();

public:
    static constexpr int defaultWorkersCount = 3;
    static constexpr int maxWorkersCount = 100;

public:
    QThread *m_thread;

private:
    ElementList *m_elementList;
    QList<Worker*> m_workers;
    bool m_active;
};

#endif // WORKERMANAGER_H
