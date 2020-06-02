#include "storagetablemodel.h"

#include <QTableView>

StorageTableModel::StorageTableModel( QObject *parent)
    : QAbstractTableModel(parent)
{

}

StorageTableModel::StorageTableModel(const QList<SStorage> &storage, QObject *parent)
    : QAbstractTableModel(parent)
{    
    m_list.append(storage);
}

int StorageTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int StorageTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return EStorageColumn_Size;
}

QVariant StorageTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        SStorage s = m_list.at(index.row());

        switch(index.column())
        {
        case EStorageColumn_Id:
            return QString::number(s.id);
        case EStorageColumn_Name:
            return s.name;
        case EStorageColumn_Text:
            return s.text;
        }
    }
    return QVariant();
}

QVariant StorageTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case EStorageColumn_Id:
                return tr("Id");
            case EStorageColumn_Name:
                return tr("Name");
            case EStorageColumn_Text:
                return tr("Text");
            default:
                return tr("unknown");
            }
        }
    }
    return QVariant();
}
