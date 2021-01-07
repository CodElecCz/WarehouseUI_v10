#include <QMessageBox>
#include "StockPositions.h"
#include "ui_StockPositions.h"
#include "models/stockpositiontablemodel.h"
#include "models/progressbaritemdelegate.h"
#include "models/receipelisttablemodel.h"

StockPositions::StockPositions(Database *database, const QMap<QString, SPositionConfig>& positionsMap, QWidget *parent):
    QWidget(parent),
    ui(new Ui::StockPositions),

    dbs(database),
    m_positionsMap(positionsMap)
{
    ui->setupUi(this);

    //StockPositionTable
    StockPositionTableModel *model = new StockPositionTableModel(ui->tableViewPosition);
    ui->tableViewPosition->setModel(model);

    QHeaderView *verticalHeader = ui->tableViewPosition->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);

    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_Material, QHeaderView::Stretch);
    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_Variant, QHeaderView::Stretch);

    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_Position, QHeaderView::Interactive);
    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_Load, QHeaderView::Interactive);
    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_Count, QHeaderView::Interactive);
    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_Mass, QHeaderView::Interactive);
    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_Weight, QHeaderView::Interactive);
    ui->tableViewPosition->horizontalHeader()->setSectionResizeMode(EStockPositionColumn_WeightMax, QHeaderView::Interactive);

    ui->tableViewPosition->setColumnWidth(EStockPositionColumn_Position, 60);
    ui->tableViewPosition->setColumnWidth(EStockPositionColumn_Load, 80);

    ui->tableViewPosition->setColumnWidth(EStockPositionColumn_Count, 60);
    ui->tableViewPosition->setColumnWidth(EStockPositionColumn_Mass, 60);
    ui->tableViewPosition->setColumnWidth(EStockPositionColumn_Weight, 60);
    ui->tableViewPosition->setColumnWidth(EStockPositionColumn_WeightMax, 60);

    ProgressBarItemDelegate* pbid = new ProgressBarItemDelegate(ui->tableViewPosition);
    ui->tableViewPosition->setItemDelegateForColumn(EStockPositionColumn_Load, pbid);

    //ReceipeListTable
    ReceipeListTableModel *model2 = new ReceipeListTableModel(ui->tableViewMovements);
    ui->tableViewMovements->setModel(model2);

    QHeaderView *verticalHeader2 = ui->tableViewMovements->verticalHeader();
    verticalHeader2->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader2->setDefaultSectionSize(20);

    ui->tableViewMovements->horizontalHeader()->setSectionResizeMode(EReceipeListColumn_material, QHeaderView::Stretch);
    ui->tableViewMovements->horizontalHeader()->setSectionResizeMode(EReceipeListColumn_variant, QHeaderView::Stretch);

    ui->tableViewMovements->horizontalHeader()->setSectionResizeMode(EReceipeListColumn_date, QHeaderView::Interactive);
    ui->tableViewMovements->horizontalHeader()->setSectionResizeMode(EReceipeListColumn_type, QHeaderView::Interactive);
    ui->tableViewMovements->horizontalHeader()->setSectionResizeMode(EReceipeListColumn_quantity, QHeaderView::Interactive);
    ui->tableViewMovements->horizontalHeader()->setSectionResizeMode(EReceipeListColumn_unit, QHeaderView::Interactive);

    ui->tableViewMovements->setColumnWidth(EReceipeListColumn_date, 80);
    ui->tableViewMovements->setColumnWidth(EReceipeListColumn_type, 40);

    ui->tableViewMovements->setColumnWidth(EReceipeListColumn_quantity, 40);
    ui->tableViewMovements->setColumnWidth(EReceipeListColumn_unit, 60);
}

StockPositions::~StockPositions()
{
    delete ui;
}

void StockPositions::updatePositions()
{
    int storage_id = ui->comboBoxStorage->currentData().toInt();
    QString storageName = ui->comboBoxStorage->currentText();
    updatePositionsFromDbs(storage_id, storageName);
}

void StockPositions::updatePositionsFromDbs(int storage_id, QString storageName)
{
    QList<SStockPosition> positions;
    dbs->selectPostions(positions, storage_id);

    StockPositionTableModel *model = new StockPositionTableModel(positions, m_positionsMap[storageName], ui->tableViewPosition);
    ui->tableViewPosition->setModel(model);    
}

void StockPositions::setStorageList(const QList<SStorage>& storageList)
{
    ui->comboBoxStorage->clear();
    foreach(SStorage s, storageList)
    {
        ui->comboBoxStorage->addItem(s.name, QVariant(s.id));
    }

    m_storages.clear();
    m_storages.append(storageList);

    if(ui->comboBoxStorage->count()>0)
    {
        on_comboBoxStorage_currentIndexChanged(0);
    }
}

void StockPositions::on_tabWidget_currentChanged(int index)
{
    switch(index)
    {
    case EStockPositionsTab_Movements:
        {
            QModelIndexList selection = ui->tableViewPosition->selectionModel()->selectedRows();

            ReceipeListTableModel *model = qobject_cast<ReceipeListTableModel *>(ui->tableViewMovements->model());
            Q_ASSERT(model);

            if(selection.count()>0)
            {
                SStockPosition p = {};
                int row = selection.at(0).row();
                p.index1 = ui->tableViewPosition->model()->data(QModelIndex(selection.at(0)), EStockPositionUserData_index1).toInt();
                p.index2 = ui->tableViewPosition->model()->data(QModelIndex(selection.at(0)), EStockPositionUserData_index2).toInt();
                p.index3 = ui->tableViewPosition->model()->data(QModelIndex(selection.at(0)), EStockPositionUserData_index3).toInt();
                p.index4 = ui->tableViewPosition->model()->data(QModelIndex(selection.at(0)), EStockPositionUserData_index4).toInt();
                p.storage_id = ui->comboBoxStorage->currentData().toInt();

                QList<SReceipe> r;
                dbs->selectReceipesFromPosition(p, r);

                QModelIndex index = selection.at(0);
                const QModelIndex positionIndex = index.sibling(index.row(), EStockPositionColumn_Position);

                QString positionName = ui->tableViewPosition->model()->data(positionIndex, Qt::DisplayRole).toString();
                ui->lineEditPosition->setText(positionName);
                ReceipeListTableModel *model = new ReceipeListTableModel(r, ui->tableViewMovements);
                ui->tableViewMovements->setModel(model);
            }
            else
            {
                ui->lineEditPosition->setText("");
                ReceipeListTableModel *model = new ReceipeListTableModel(ui->tableViewMovements);
                ui->tableViewMovements->setModel(model);
            }
        }
        break;
    }
}

void StockPositions::on_comboBoxStorage_currentIndexChanged(int index)
{
    if(index>=0 && index<m_storages.count())
    {
        SStorage s = m_storages.at(index);
        if(m_positionsMap.contains(s.name))
        {
            SPositionConfig p = m_positionsMap.value(s.name);

            int storage_id = ui->comboBoxStorage->currentData().toInt();
            updatePositionsFromDbs(storage_id, s.name);
        }
        else
        {
            QMessageBox::information(this, tr("Storage Settings"), tr("No setting found for position in warehouse.ini:\n\n[PositionXY]\nName='%1'").arg(s.name));
        }
    }
}

void StockPositions::changeEvent(QEvent* event)
{
    if(0 != event)
    {
        switch(event->type())
        {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    }
    QWidget::changeEvent(event);
}
