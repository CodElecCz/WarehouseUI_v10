#include "receipetablemodel.h"

#include <QTableView>
#include <QColor>

ReceipeTableModel::ReceipeTableModel( QObject *parent)
    : QAbstractTableModel(parent)
{

}

ReceipeTableModel::ReceipeTableModel(Database *database, QObject *parent)
    : QAbstractTableModel(parent),
      dbs(database)
{

}


int ReceipeTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int ReceipeTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return EReceipeColumn_Size;
}

QVariant ReceipeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        SReceipeItem p = m_list.at(index.row());

        switch(index.column())
        {
        case EReceipeColumn_Material:
            return p.stock.material;
        case EReceipeColumn_Variant:
            return p.stock.variant;
        case EReceipeColumn_Quantity:
            return p.quantity;
        case EReceipeColumn_Unit:
            return p.stock.unit;
        case EReceipeColumn_Position1:
            if(p.index1>=0 && m_indexNames1.count()>p.index1)
            {
                return m_indexNames1.at(p.index1);
            }
            else
                return "";
        case EReceipeColumn_Position2:
            if(p.index2>=0 && m_indexNames2.count()>p.index2)
            {
                return m_indexNames2.at(p.index2);
            }
            else
                return "";
        case EReceipeColumn_Position3:
            if(p.index3>=0 && m_indexNames3.count()>p.index3)
            {
                return m_indexNames3.at(p.index3);
            }
            else
                return "";
        case EReceipeColumn_Position4:
            if(p.index4>=0 && m_indexNames4.count()>p.index4)
            {
                return m_indexNames4.at(p.index4);
            }
            else
                return "";

        default:
            return QString("unknown");
        }
    }
    else if (role==Qt::EditRole)
    {
        SReceipeItem p = m_list.at(index.row());

        switch(index.column())
        {
        case EReceipeColumn_Material:
            return p.stock.material;
        case EReceipeColumn_Variant:
            return p.stock.variant;
        case EReceipeColumn_Quantity:
            return p.quantity;
        case EReceipeColumn_Unit:
            return p.stock.unit;
        case EReceipeColumn_Position1:
            return p.index1;
        case EReceipeColumn_Position2:
            return p.index2;
        case EReceipeColumn_Position3:
            return p.index3;
        case EReceipeColumn_Position4:
            return p.index4;
        default:
            return tr("unknown");
        }

    }
    else if (role==Qt::BackgroundRole)
    {
        switch(index.column())
        {
        case 3:
            QBrush background(QColor(230,230,230));
            return background;
        }
    }
    return QVariant();
}

bool ReceipeTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row = index.row();

        SReceipeItem p = m_list.value(row);

        switch(index.column())
        {
        case EReceipeColumn_Material:
            p.stock.material = value.toString();
            p.stock.variant = "";
            break;
        case EReceipeColumn_Variant:
            p.stock.variant = value.toString();
            break;
        case EReceipeColumn_Quantity:
            p.quantity = value.toInt();
            if(p.quantity<=0)
                p.quantity = 1;
            break;
        case EReceipeColumn_Position1:
            p.index1 = value.toInt();
            break;
        case EReceipeColumn_Position2:
            p.index2 = value.toInt();
            break;
        case EReceipeColumn_Position3:
            p.index3 = value.toInt();
            break;
        case EReceipeColumn_Position4:
            p.index4 = value.toInt();
            break;

        default:
            return false;
        }

        m_list.replace(row, p);
        emit(dataChanged(index, index));

        return true;
    }
    else if (index.isValid() && role == Qt::UserRole)
    {
        int row = index.row();

        SReceipeItem p = m_list.value(row);

        int id = value.toInt();

        QList<SStock> stockList;
        bool ok = dbs->selectStock(stockList, id);
        if(ok && stockList.size()>0)
        {
            p.stock = stockList.at(0);
        }

        m_list.replace(row, p);
        emit(dataChanged(index, index));

        return true;
    }

    return false;
}

QVariant ReceipeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case EReceipeColumn_Material:
                return tr("Material");
            case EReceipeColumn_Variant:
                return tr("Variant");
            case EReceipeColumn_Quantity:
                return tr("Quantity");
            case EReceipeColumn_Unit:
                return tr("Unit");
            case EReceipeColumn_Position1:
                return tr("Pos1");
            case EReceipeColumn_Position2:
                return tr("Pos2");
            case EReceipeColumn_Position3:
                return tr("Pos3");
            case EReceipeColumn_Position4:
                return tr("Pos4");

            default:
                return tr("unknown");
            }
        }
    }
    return QVariant();
}

bool ReceipeTableModel::insertRows(int position, int rows, const QModelIndex &index)
 {
     Q_UNUSED(index);
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row=0; row < rows; row++)
     {
         SReceipeItem item;
         item.quantity = 1;
         item.index1 = -1;
         item.index2 = -1;
         item.index3 = -1;
         item.index4 = -1;
         item.stock.id = -1;
         item.stock.material = "";
         item.stock.variant = "";
         item.stock.unit = "";
         item.stock.code = "";

         m_list.insert(position, item);
     }

     endInsertRows();
     return true;
 }

bool ReceipeTableModel::removeRows(int position, int rows, const QModelIndex &index)
 {
     Q_UNUSED(index);
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row=0; row < rows; ++row)
     {
         m_list.removeAt(position);
     }

     endRemoveRows();
     return true;
 }

Qt::ItemFlags ReceipeTableModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     Qt::ItemFlags flags = QAbstractTableModel::flags(index);

     if (index.isValid())
     {
         int row = index.row();

         SReceipeItem p = m_list.value(row);

         switch(index.column())
         {
         case EReceipeColumn_Material:
             flags |= Qt::ItemIsEditable;
             break;
         case EReceipeColumn_Variant:
             flags |= Qt::ItemIsEditable;
             break;
         case EReceipeColumn_Quantity:
             flags |= Qt::ItemIsEditable;
             break;
         case EReceipeColumn_Unit:
             //flags |= Qt::ItemIsEditable;
             break;
         case EReceipeColumn_Position1:
         case EReceipeColumn_Position2:
         case EReceipeColumn_Position3:
         case EReceipeColumn_Position4:
             flags |= Qt::ItemIsEditable;
             break;

         default:
             return Qt::NoItemFlags;
         }
     }

     return flags;
 }
