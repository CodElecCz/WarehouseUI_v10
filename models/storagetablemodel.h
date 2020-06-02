#ifndef STORAGETABLEMODEL_H
#define STORAGETABLEMODEL_H

#include <QAbstractTableModel>
#include "pohoda/pohoda.h"

typedef enum _EStorageColumn
{
    EStorageColumn_Id = 0,
    EStorageColumn_Name,
    EStorageColumn_Text,
    /*...*/
    EStorageColumn_Size
} EStorageColumn;

class StorageTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StorageTableModel(QObject *parent = nullptr);
    StorageTableModel(const QList<SStorage>& stock, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<SStorage> m_list;
};

#endif // STORAGETABLEMODEL_H
