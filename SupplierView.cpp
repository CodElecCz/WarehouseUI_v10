#include "SupplierView.h"
#include "ui_SupplierView.h"
#include "models/suppliertablemodel.h"

SupplierView::SupplierView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SupplierView)
{
    ui->setupUi(this);

    SupplierTableModel *model = new SupplierTableModel(ui->tableView);
    ui->tableView->setModel(model);

    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);

    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Id, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Ico, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Dic, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Zip, QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Name, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Division, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Company, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ESupplierColumn_Street, QHeaderView::Stretch);

    ui->tableView->setColumnWidth(ESupplierColumn_Id, 35);
    ui->tableView->setColumnWidth(ESupplierColumn_Zip, 40);
    ui->tableView->setColumnWidth(ESupplierColumn_Ico, 60);
    ui->tableView->setColumnWidth(ESupplierColumn_Dic, 75);
}

SupplierView::~SupplierView()
{
    delete ui;
}

void SupplierView::on_pushButtonPohoda_clicked(bool)
{
    emit importData();
}

void SupplierView::setSupplierList(const QList<SSupplier> &list)
{
    SupplierTableModel* model = new SupplierTableModel(list, ui->tableView);
    ui->tableView->setModel(model);
}

void SupplierView::changeEvent(QEvent* event)
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

