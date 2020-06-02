#include "stocktablemodel.h"

#include <QTableView>

StockTableModel::StockTableModel( QObject *parent)
    : QAbstractTableModel(parent)
{

}

StockTableModel::StockTableModel(const QList<SStock> &stock, QObject *parent)
    : QAbstractTableModel(parent)
{    
    m_list.append(stock);
}

int StockTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int StockTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return EStockColumn_Size;
}

QVariant StockTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        SStock stock = m_list.at(index.row());

        switch(index.column())
        {
        case EStockColumn_Id:
            return QString::number(stock.id);
        case EStockColumn_Code:
            return stock.code;
        case EStockColumn_Material:
            return stock.material;
        case EStockColumn_Variant:
            return stock.variant;
        case EStockColumn_Count:
            return QString::number(stock.count);
        case EStockColumn_Unit:
            return stock.unit;
        case EStockColumn_Mass:
            return QString::number(stock.mass);
        case EStockColumn_Avail:
            return QString::number(stock.availability);
        case EStockColumn_Handling:
            return QString::number(stock.handlingInformation);
        case EStockColumn_SupplierId:
            return QString::number(stock.supplier_id);
        case EStockColumn_StorageId:
            return QString::number(stock.storage_id);
        }
    }
    return QVariant();
}

QVariant StockTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case EStockColumn_Id:
                return tr("Id");
            case EStockColumn_Code:
                return tr("Code");
            case EStockColumn_Material:
                return tr("Material");
            case EStockColumn_Variant:
                return tr("Variant");
            case EStockColumn_Count:
                return tr("Count");
            case EStockColumn_Unit:
                return tr("Unit");
            case EStockColumn_Mass:
                return tr("Mass [kg]");
            case EStockColumn_Avail:
                return tr("Availability [days]");
            case EStockColumn_Handling:
                return tr("Shipping [days]");
            case EStockColumn_SupplierId:
                return tr("Supplier Id");
            case EStockColumn_StorageId:
                return tr("Storage Id");
            default:
                return tr("unknown");
            }
        }
    }
    return QVariant();
}
