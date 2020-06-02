#ifndef STOCKTABLEMODEL_H
#define STOCKTABLEMODEL_H

#include <QAbstractTableModel>
#include "pohoda/pohoda.h"

typedef enum _EStockColumn
{
    EStockColumn_Id = 0,
    EStockColumn_Code,
    EStockColumn_Material,
    EStockColumn_Variant,
    EStockColumn_Count,
    EStockColumn_Unit,
    EStockColumn_Mass,
    EStockColumn_Avail,
    EStockColumn_Handling,
    EStockColumn_SupplierId,
    EStockColumn_StorageId,
    /*...*/
    EStockColumn_Size
} EStockColumn;


class StockTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StockTableModel(QObject *parent = nullptr);
    StockTableModel(const QList<SStock>& stock, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<SStock> m_list;
};

#endif // STOCKTABLEMODEL_H
