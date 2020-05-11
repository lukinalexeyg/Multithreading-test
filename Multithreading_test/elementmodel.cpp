#include "elementmodel.h"
#include "log.h"



ElementModel::ElementModel(QObject *parent) :
    QAbstractListModel(parent)
{
}



ElementModel::~ElementModel()
{
    qDeleteAll(m_items);
}



void ElementModel::append(const QString &text)
{
    FUNCTION_LOG

    ElementModelItem *item = new ElementModelItem(text, false);
    m_items.append(item);

    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    endInsertRows();
}



void ElementModel::edit(int row, const QString &text, bool locked)
{
    FUNCTION_LOG

    if (!isRowValid(row))
        return;

    m_items.at(row)->set(text, locked);
    const QModelIndex modelIndex = QModelIndex().siblingAtRow(row);
    emit dataChanged(modelIndex, modelIndex);
}



void ElementModel::remove(int index)
{
    FUNCTION_LOG

    if (!isRowValid(index))
        return;

    beginRemoveRows(QModelIndex(), index, index);
    delete m_items.takeAt(index);
    endRemoveRows();
}



QVariant ElementModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();

    if (!index.isValid() || !isRowValid(row))
        return QVariant();

    if (role == Qt::DisplayRole)
        return QVariant::fromValue<ElementModelItem*>(m_items.at(row));

    return QVariant();
}



int ElementModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_items.count();
}



bool ElementModel::isRowValid(int row) const
{
    return row >= 0 && row < m_items.count();
}
