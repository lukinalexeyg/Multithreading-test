#ifndef ELEMENTMODEL_H
#define ELEMENTMODEL_H

#include "elementmodelitem.h"

#include <QAbstractListModel>

class ElementModel : public QAbstractListModel
{
public:
    explicit ElementModel(QObject *parent = nullptr);

    ~ElementModel();

public:
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
