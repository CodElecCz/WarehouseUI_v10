#ifndef RECEIPETABLEMODEL_H
#define RECEIPETABLEMODEL_H

#include <QAbstractTableModel>
#include "dbs/database.h"

typedef enum _EReceipeColumn
{
    EReceipeColumn_Material = 0,
    EReceipeColumn_Variant,
    EReceipeColumn_Quantity,
    EReceipeColumn_Unit,
    EReceipeColumn_Position1,
    EReceipeColumn_Position2,
    EReceipeColumn_Position3,
    EReceipeColumn_Position4,
    /*...*/
    EReceipeColumn_Size
} EReceipeColumn;

class ReceipeTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ReceipeTableModel(QObject *parent = nullptr);
    ReceipeTableModel(Database *dbs, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex()) override;       

    QList<SReceipeItem> getList() { return m_list; };
    void setDisplayNames1(const QStringList& list) { m_indexNames1.clear(); m_indexNames1.append(list); }
    void setDisplayNames2(const QStringList& list) { m_indexNames2.clear(); m_indexNames2.append(list); }
    void setDisplayNames3(const QStringList& list) { m_indexNames3.clear(); m_indexNames3.append(list); }
    void setDisplayNames4(const QStringList& list) { m_indexNames4.clear(); m_indexNames4.append(list); }

private:
    Database            *dbs;

    QList<SReceipeItem> m_list;
    QStringList         m_indexNames1;
    QStringList         m_indexNames2;
    QStringList         m_indexNames3;
    QStringList         m_indexNames4;

};

#endif // RECEIPETABLEMODEL_H
