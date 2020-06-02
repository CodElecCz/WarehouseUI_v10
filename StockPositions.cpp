#include <QMessageBox>
#include "StockPositions.h"
#include "ui_StockPositions.h"
#include "models/stockpositiontablemodel.h"
#include "models/progressbaritemdelegate.h"

StockPositions::StockPositions(Database *database, const QMap<QString, SPositionConfig>& positionsMap, QWidget *parent):
    QWidget(parent),
    ui(new Ui::StockPositions),

    dbs(database),
    m_positionsMap(positionsMap)
{
    ui->setupUi(this);

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
