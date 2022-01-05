#ifndef WORKER_H
#define WORKER_H

#include "elementlist.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(ElementList *elementList, QObject *parent = nullptr);

    Q_SLOT void start();
    void stop()                          { m_active = false; }
    void setIdleDuration(int duration)   { m_idleDuration = duration; }

    static void stopThread(QThread *thread, ulong time = ULONG_MAX);

public:
    static constexpr int defaultIdleDuration = 1000;
    static constexpr int maxIdleDuration = 60000;

public:
    QThread *m_thread;

private:
    enum Action {
        AppendElement,
        EditElement,
        RemoveElement,
        Count
    };

    ElementList *m_elementList;
    int m_idleDuration;
    bool m_active;

private:
    void appendElement();
    void editElement();
    void removeElement();
    void sleep();
    void invokeAction();
    void doAction();
};

#endif // WORKER_H
