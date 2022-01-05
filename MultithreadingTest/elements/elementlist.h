#ifndef LIST_H
#define LIST_H

#include "elementlistitem.h"

#include <QMap>
#include <QMutex>
#include <QObject>
#include <QThread>

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

    int count() const;

    void append(const ElementPtr &element);

    bool remove(int index);
    bool remove(const ElementPtr &element);
    bool remove(UnusedElementType type);

    ElementPtr get(int index);
    ElementPtr get(UnusedElementType type);

    bool set(const ElementPtr &element);

    bool release(const ElementPtr &element);
    bool releaseAll();

    int indexOf(const ElementPtr &element) const;

signals:
    void appended(const QString &text);
    void edited(int index, const QString &text, bool locked);
    void removed(int index);

private:
    mutable QMutex m_mutex;
    QList<ElementListItem*> m_items;

private:
    bool _remove(int index, QThread *thread = nullptr);
    ElementPtr _get(int index);
    bool release(bool all, const ElementPtr &element);
    int _indexOf(const ElementPtr &element) const;
    int unusedElementIndex(UnusedElementType type) const;
};

#endif // LIST_H
