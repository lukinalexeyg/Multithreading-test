#ifndef ELEMENTMODEL_H
#define ELEMENTMODEL_H

#include <QAbstractListModel>



class ElementModelItem {
public:
    ElementModelItem() : m_text(""), m_locked(false) {};
    ElementModelItem(const QString &text, bool locked) : m_text(text), m_locked(locked) {};
    void set(const QString &text, bool locked) { m_text = text; m_locked = locked; };
    QString text() { return m_text; }
    bool locked() { return m_locked; }

private:
    QString m_text;
    bool m_locked;
};

Q_DECLARE_METATYPE(ElementModelItem*)



class ElementModel : public QAbstractListModel
{
public:
    explicit ElementModel(QObject *parent = nullptr);
    ~ElementModel();

    void append(const QString &text);
    void edit(int row, const QString &text, bool locked);
    void remove(int row);

private:
    QList<ElementModelItem*> m_items;

private:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool isRowValid(int row) const;
};

#endif // ELEMENTMODEL_H
