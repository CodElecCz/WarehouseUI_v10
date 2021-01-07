#ifndef STOCKPOSITIONS_H
#define STOCKPOSITIONS_H

#include <QWidget>
#include "dbs/database.h"


typedef enum _EStockPositionsTab
{
    EStockPositionsTab_Positions = 0,
    EStockPositionsTab_Movements = 1,
    EStockPositionsTab_Material = 2
} EStockPositionsTab;

namespace Ui {
class StockPositions;
}

class StockPositions : public QWidget
{
    Q_OBJECT

public:
    explicit StockPositions(QWidget *parent = nullptr);
    explicit StockPositions(Database *dbs, const QMap<QString, SPositionConfig>& positionsMap, QWidget *parent = nullptr);
    ~StockPositions();

    void setStorageList(const QList<SStorage>& storageList);
    void updatePositions();

private:
    void updatePositionsFromDbs(int storage_id = -1, QString storageName = "");

private slots:
    void on_comboBoxStorage_currentIndexChanged(int index);
    void on_tabWidget_currentChanged(int index);

protected:
    void changeEvent(QEvent* event);

private:
    Ui::StockPositions  *ui;
    Database            *dbs;

    QMap<QString, SPositionConfig>  m_positionsMap;
    QList<SStorage>                 m_storages;
};

#endif // STOCKPOSITIONS_H
