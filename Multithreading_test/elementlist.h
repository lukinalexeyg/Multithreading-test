#ifndef LIST_H
#define LIST_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QThread>
#include "element.h"

class ElementList : public QObject
{
    Q_OBJECT

public:
    enum UnusedElementType {
        First,
        Last,
        Random
    };

public:
    explicit ElementList(QObject *parent = nullptr);
    ~ElementList();

    int count();

    void append(Element *element);

    bool remove(int index);
    bool remove(Element *element);
    bool remove(UnusedElementType type);

    Element *get(int index);
    Element *get(UnusedElementType type);

    bool set(Element *element);

    inline bool release(Element *element)   { return release(false, element); }
    inline bool releaseAll()                { return release(true, nullptr); }

    int indexOf(Element *element);

signals:
    void appended(const QString &text);
    void edited(int index, const QString &text, bool locked);
    void removed(int index);

private:
    struct Item {
        Element *element;
        QThread *thread;
        Item(Element *element, QThread *thread = nullptr) : element(element), thread(thread) {};
        ~Item() { delete element; };
    };

    mutable QMutex m_mutex;
    QList<Item*> m_items;

private:
    bool _remove(int index, QThread *thread = nullptr);
    Element *_get(int index);
    bool release(bool all, Element *element);
    int _indexOf(Element *element);
    int unusedElementIndex(UnusedElementType type);
};

#endif // LIST_H
