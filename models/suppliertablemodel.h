#ifndef SUPPLIERTABLEMODEL_H
#define SUPPLIERTABLEMODEL_H

#include <QAbstractTableModel>
#include "pohoda/pohoda.h"

typedef enum _ESupplierColumn
{
    ESupplierColumn_Id = 0,
    ESupplierColumn_Company,
    ESupplierColumn_Name,
    ESupplierColumn_Division,
    ESupplierColumn_Street,
    ESupplierColumn_City,
    ESupplierColumn_Zip,
    ESupplierColumn_Ico,
    ESupplierColumn_Dic,

    /*...*/
    ESupplierColumn_Size
} ESupplierColumn;


class SupplierTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    SupplierTableModel(QObject *parent = nullptr);
    SupplierTableModel(const QList<SSupplier>& stock, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<SSupplier> m_list;
};

#endif // SUPPLIERTABLEMODEL_H
