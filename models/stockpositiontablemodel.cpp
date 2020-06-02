#include "stockpositiontablemodel.h"
#include <QTableView>

struct PositionKeyMatch {
    PositionKeyMatch(const int & s1, const int & s2, const int & s3, const int & s4)
    : position1(s1), position2(s2), position3(s3), position4(s4) {}

    bool operator()(const SStockPosition & x) const
    {
        return  (x.index1 == position1) &&
                (x.index2 == position2) &&
                (x.index3 == position3) &&
                (x.index4 == position4);
    }

    int position1;
    int position2;
    int position3;
    int position4;
};


StockPositionTableModel::StockPositionTableModel( QObject *parent)
    : QAbstractTableModel(parent)
{

}


StockPositionTableModel::StockPositionTableModel(const QList<SStockPosition>& positions, const SPositionConfig& positionConfig, QObject *parent, bool emptyPosVisible)
    : QAbstractTableModel(parent),
      m_positions(positions),
      m_positionConfig(positionConfig),
      m_emptyPosVisible(emptyPosVisible)
{    

    int k_max = m_positionConfig.names1.size();
    int l_max = m_positionConfig.names2.size();
    int m_max = m_positionConfig.names3.size();
    int n_max = m_positionConfig.names4.size();

    for(int k = 0; k<k_max; k++)
    {
        for(int l = 0; l<l_max; l++)
        {
             for(int m = 0; m<m_max; m++)
             {
                 if(m_positionConfig.use4)
                 {
                     bool n_found3 = false;
                     bool n_found4 = false;

                     QList<SStockPosition>::iterator pos = std::find_if(m_positions.begin(), m_positions.end(), PositionKeyMatch(k,l,m,-1));
                     if(pos != m_positions.end())
                     {
                         m_positionsMap.insert(getPositionName(pos[0]), pos[0]);
                         n_found3 = true;
                     }

                     for(int n = 0; n<n_max; n++)
                     {
                         QList<SStockPosition>::iterator pos = std::find_if(m_positions.begin(), m_positions.end(), PositionKeyMatch(k,l,m,n));
                         if(pos != m_positions.end())
                         {
                             m_positionsMap.insert(getPositionName(pos[0]), pos[0]);
                             n_found4 = true;
                         }
                         else if(m_emptyPosVisible && m_positionConfig.mandatory4)
                         {
                             SStockPosition posNew = {};
                             posNew.stock.id = -1;
                             posNew.index1 = k;
                             posNew.index2 = l;
                             posNew.index3 = m;
                             posNew.index4 = n;

                             m_positionsMap.insert(getPositionName(posNew), posNew);
                         }
                     }

                    if(m_emptyPosVisible && !m_positionConfig.mandatory4 && !n_found3 && !n_found4)
                    {
                        SStockPosition posNew = {};
                        posNew.stock.id = -1;
                        posNew.index1 = k;
                        posNew.index2 = l;
                        posNew.index3 = m;
                        posNew.index4 = -1;

                        m_positionsMap.insert(getPositionName(posNew), posNew);
                    }
                }
            }
        }
    }
}

int StockPositionTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_positionsMap.count(); //m_positions.count();
}

int StockPositionTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return EStockPositionColumn_Size;
}

QString StockPositionTableModel::getPositionName(const SStockPosition& position) const
{
    SPositionConfig config = m_positionConfig;

    char cbuffer[128] = "";

    if(config.use4 && position.index4>=0)
    {
        QString s01 = config.names1.at(position.index1);
        QString s02 = config.names2.at(position.index2);
        QString s03 = config.names3.at(position.index3);
        QString s04 = config.names4.at(position.index4);

        ::snprintf(cbuffer, sizeof(cbuffer), config.format4.toLatin1().constData(),
                   s01.toLatin1().constData(),
                   s02.toLatin1().constData(),
                   s03.toLatin1().constData(),
                   s04.toLatin1().constData());
    }
    else if(config.use3 &&position.index3>=0)
    {
        QString s01 = config.names1.at(position.index1);
        QString s02 = config.names2.at(position.index2);
        QString s03 = config.names3.at(position.index3);
        ::snprintf(cbuffer, sizeof(cbuffer), config.format3.toLatin1().constData(),
                   s01.toLatin1().constData(),
                   s02.toLatin1().constData(),
                   s03.toLatin1().constData());
    }
    else if(config.use2 && position.index2>=0)
    {
        QString s01 = config.names1.at(position.index1);
        QString s02 = config.names2.at(position.index2);
        ::snprintf(cbuffer, sizeof(cbuffer), config.format2.toLatin1().constData(),
                   s01.toLatin1().constData(),
                   s02.toLatin1().constData());
    }
    else if(config.use1 && position.index1>=0)
    {
        QString s01 = config.names1.at(position.index1);
        ::snprintf(cbuffer, sizeof(cbuffer), config.format1.toLatin1().constData(),
                   s01.toLatin1().constData());
    }

    return QString::fromLatin1(cbuffer);
}

int StockPositionTableModel::getLoadMax(const SStockPosition& position) const
{
    SPositionConfig config = m_positionConfig;

    if(config.use4 && position.index4>=0)
    {
        return config.load4;
    }
    else if(config.use3 &&position.index3>=0)
    {
        return config.load3;
    }
    else if(config.use2 && position.index2>=0)
    {
        return config.load2;
    }
    else if(config.use1 && position.index1>=0)
    {
        return config.load1;
    }

    return 0;
}

QVariant StockPositionTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_positionsMap.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString key = m_positionsMap.keys().at(index.row());
        SStockPosition position = m_positionsMap.values().at(index.row());

        switch(index.column())
        {
        case EStockPositionColumn_Position:
            return key;
        case EStockPositionColumn_Load:
            return ((position.quantity*position.stock.mass)/getLoadMax(position))*100;
        case EStockPositionColumn_Material:
            return position.stock.material;
        case EStockPositionColumn_Variant:
            return position.stock.variant;
        case EStockPositionColumn_Count:
            return position.quantity;
        case EStockPositionColumn_Mass:
            return position.stock.mass;
        case EStockPositionColumn_Weight:
            return (position.quantity*position.stock.mass);
        case EStockPositionColumn_WeightMax:
            return getLoadMax(position);
        }
    }
    return QVariant();
}

QVariant StockPositionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case EStockPositionColumn_Position:
                return tr("Position");
            case EStockPositionColumn_Load:
                return tr("Load [%]");
            case EStockPositionColumn_Material:
                return tr("Material");
            case EStockPositionColumn_Variant:
                return tr("Variant");
            case EStockPositionColumn_Count:
                return tr("Count");
            case EStockPositionColumn_Mass:
                return tr("Mass [kg]");
            case EStockPositionColumn_Weight:
                return tr("Load [kg]");
            case EStockPositionColumn_WeightMax:
                return tr("Max [kg]");;

            default:
                return tr("unknown");
            }
        }
    }
    return QVariant();
}
