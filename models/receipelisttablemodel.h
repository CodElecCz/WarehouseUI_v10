#ifndef RECEIPELISTTABLEMODEL_H
#define RECEIPELISTTABLEMODEL_H

#include <QAbstractTableModel>
#include "pohoda/pohoda.h"

typedef enum _EReceipeListColumn
{
    EReceipeListColumn_date = 0,
    EReceipeListColumn_type,
    EReceipeListColumn_quantity,
    EReceipeListColumn_unit,
    EReceipeListColumn_material,
    EReceipeListColumn_variant,

    /*...*/
    EReceipeListColumn_Size
} EReceipeListColumn;


class ReceipeListTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ReceipeListTableModel(QObject *parent = nullptr);
    ReceipeListTableModel(const QList<SReceipe>& receipe, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QList<SReceipe> getList() { return m_list; };
    void clearList() { m_list.clear(); };
    void setList(const QList<SReceipe> receipes) { m_list.clear(); m_list.append(receipes); };

private:
    QList<SReceipe> m_list;
};

#endif // RECEIPELISTTABLEMODEL_H
