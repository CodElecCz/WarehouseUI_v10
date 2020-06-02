#include "suppliertablemodel.h"

#include <QTableView>

SupplierTableModel::SupplierTableModel( QObject *parent)
    : QAbstractTableModel(parent)
{

}

SupplierTableModel::SupplierTableModel(const QList<SSupplier> &stock, QObject *parent)
    : QAbstractTableModel(parent)
{    
    m_list.append(stock);
}

int SupplierTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int SupplierTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ESupplierColumn_Size;
}

QVariant SupplierTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        SSupplier s = m_list.at(index.row());

        switch(index.column())
        {
        case ESupplierColumn_Id:
            return QString::number(s.id);
        case ESupplierColumn_Company:
            return s.company;
        case ESupplierColumn_Name:
            return s.name;
        case ESupplierColumn_Division:
            return s.division;
        case ESupplierColumn_Street:
            return s.street;
        case ESupplierColumn_City:
            return s.city;
        case ESupplierColumn_Zip:
            return s.zip;
        case ESupplierColumn_Ico:
            return s.ico;
        case ESupplierColumn_Dic:
            return s.dic;
        }
    }
    return QVariant();
}

QVariant SupplierTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case ESupplierColumn_Id:
                return tr("Id");
            case ESupplierColumn_Company:
                return tr("Company");
            case ESupplierColumn_Name:
                return tr("Name");
            case ESupplierColumn_Division:
                return tr("Division");
            case ESupplierColumn_Street:
                return tr("Street");
            case ESupplierColumn_City:
                return tr("City");
            case ESupplierColumn_Zip:
                return tr("Zip");
            case ESupplierColumn_Ico:
                return tr("ICO");
            case ESupplierColumn_Dic:
                return tr("DIC");
            default:
                return tr("unknown");
            }
        }
    }
    return QVariant();
}
