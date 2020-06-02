#include "StorageView.h"
#include "ui_StorageView.h"
#include "models/storagetablemodel.h"

StorageView::StorageView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StorageView)
{
    ui->setupUi(this);

    StorageTableModel *model = new StorageTableModel(ui->tableView);
    ui->tableView->setModel(model);

    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);

    ui->tableView->horizontalHeader()->setSectionResizeMode(EStorageColumn_Id, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EStorageColumn_Name, QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setSectionResizeMode(EStorageColumn_Text, QHeaderView::Stretch);

    ui->tableView->setColumnWidth(EStorageColumn_Id, 35);
    ui->tableView->setColumnWidth(EStorageColumn_Name, 120);
}

StorageView::~StorageView()
{
    delete ui;
}

void StorageView::on_pushButtonPohoda_clicked(bool)
{
    emit importData();
}

void StorageView::setStorageList(const QList<SStorage> &list)
{
    StorageTableModel* model = new StorageTableModel(list, ui->tableView);
    ui->tableView->setModel(model);
}

void StorageView::changeEvent(QEvent* event)
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
