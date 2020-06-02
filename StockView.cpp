#include "StockView.h"
#include "ui_StockView.h"
#include "models/stocktablemodel.h"

StockView::StockView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockView)
{
    ui->setupUi(this);

    StockTableModel *model = new StockTableModel(ui->tableView);
    ui->tableView->setModel(model);

    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);

    ui->tableView->horizontalHeader()->setSectionResizeMode(EStockColumn_Id, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EStockColumn_SupplierId, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EStockColumn_StorageId, QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setSectionResizeMode(EStockColumn_Code, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EStockColumn_Material, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EStockColumn_Variant, QHeaderView::Stretch);

    ui->tableView->setColumnWidth(EStockColumn_Id, 35);
    ui->tableView->setColumnWidth(EStockColumn_SupplierId, 80);
    ui->tableView->setColumnWidth(EStockColumn_StorageId, 60);
    ui->tableView->setColumnWidth(EStockColumn_Unit, 60);
    ui->tableView->setColumnWidth(EStockColumn_Mass, 60);
    ui->tableView->setColumnWidth(EStockColumn_Count, 60);
}

StockView::~StockView()
{
    delete ui;
}

void StockView::on_pushButtonPohoda_clicked(bool)
{
    emit importData();
}

void StockView::setStockList(QList<SStock> list)
{
    StockTableModel* model = new StockTableModel(list, ui->tableView);
    ui->tableView->setModel(model);
}

void StockView::changeEvent(QEvent* event)
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
