#ifndef WORKER_H
#define WORKER_H

#include "elementlist.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(const ElementListPtr &elementList, QObject *parent = nullptr);

public:
    Q_SLOT void start();

    void stop() { m_active = false; }

    void setIdleDuration(const int duration) { m_idleDuration = duration; }

    static void appendElement(const ElementListPtr &elementList, const QString &userText = QString());

    static void stopThread(QThread *thread, ulong time = ULONG_MAX);

public:
    static constexpr int defaultIdleDuration = 1000;
    static constexpr int maxIdleDuration = 60000;

    QThread *m_thread;

private:
    enum Action {
        AppendElement,
        EditElement,
        RemoveElement,
        Count
    };

    ElementListPtr m_elementList;
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
