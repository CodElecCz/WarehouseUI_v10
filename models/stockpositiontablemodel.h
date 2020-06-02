#ifndef STOCKPOSITIONTABLEMODEL_H
#define STOCKPOSITIONTABLEMODEL_H

#include <QAbstractTableModel>
#include "dbs/database.h"

typedef enum _EStockPositionColumn
{
    EStockPositionColumn_Position,
    EStockPositionColumn_Load,
    EStockPositionColumn_Material,
    EStockPositionColumn_Variant,
    EStockPositionColumn_Count,
    EStockPositionColumn_Mass,
    EStockPositionColumn_Weight,
    EStockPositionColumn_WeightMax,

    /*...*/
    EStockPositionColumn_Size
} EStockColumn;

class StockPositionTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StockPositionTableModel(QObject *parent = nullptr);
    StockPositionTableModel(const QList<SStockPosition>& positions, const SPositionConfig& positionConfig, QObject *parent = nullptr, bool emptyPosVisible = true);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QString getPositionName(const SStockPosition &position) const;
    int getLoadMax(const SStockPosition& position) const;

private:   
    QList<SStockPosition>           m_positions;
    QMap<QString, SStockPosition>   m_positionsMap;
    SPositionConfig                 m_positionConfig;
    bool                            m_emptyPosVisible;
};

#endif // STOCKPOSITIONTABLEMODEL_H
