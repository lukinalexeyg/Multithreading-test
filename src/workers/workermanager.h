#ifndef WORKERMANAGER_H
#define WORKERMANAGER_H

#include "elementlist.h"
#include "worker.h"

class WorkerManager : public QObject
{
    Q_OBJECT

public:
    explicit WorkerManager(const ElementListPtr &elementList, QObject *parent = nullptr);

    ~WorkerManager();

public:
    Q_SLOT void set(int count, int duration);

    Q_SLOT void reset();

    void start();

    void stop();

public:
    static constexpr int defaultWorkersCount = 5;
    static constexpr int maxWorkersCount = 100;

    QThread *m_thread;

private:
    ElementListPtr m_elementList;
    QList<Worker*> m_workers;
    bool m_active;
};

#endif // WORKERMANAGER_H
