#include "receipelisttablemodel.h"

#include <QTableView>

ReceipeListTableModel::ReceipeListTableModel( QObject *parent)
    : QAbstractTableModel(parent)
{

}

ReceipeListTableModel::ReceipeListTableModel(const QList<SReceipe> &receipe, QObject *parent)
    : QAbstractTableModel(parent)
{    
    m_list.append(receipe);
}

int ReceipeListTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int ReceipeListTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return EReceipeListColumn_Size;
}

QVariant ReceipeListTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        SReceipe receipe = m_list.at(index.row());

        switch(index.column())
        {
        case EReceipeListColumn_date:
            return receipe.header.date.toString("dd.MM.yyyy");
        case EReceipeListColumn_type:
            return CReceipeType[receipe.type];
        case EReceipeListColumn_quantity:
            return receipe.list.first().quantity;
        case EReceipeListColumn_unit:
            return receipe.list.first().stock.unit;
        case EReceipeListColumn_material:
            return receipe.list.first().stock.material;
        case EReceipeListColumn_variant:
            return receipe.list.first().stock.variant;
        }
    }
    return QVariant();
}

QVariant ReceipeListTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case EReceipeListColumn_date:
                return tr("Date");
            case EReceipeListColumn_type:
                return tr("Type");
            case EReceipeListColumn_quantity:
                return tr("Count");
            case EReceipeListColumn_unit:
                return tr("Unit");
            case EReceipeListColumn_material:
                return tr("Material");
            case EReceipeListColumn_variant:
                return tr("Variant");

            default:
                return tr("unknown");
            }
        }
    }
    return QVariant();
}
