#include <QMessageBox>
#include <QHeaderView>
#include <QAbstractItemView>
#include "ReceipeForm.h"
#include "ui_ReceipeForm.h"
#include "models/comboboxdbsdelegate.h"

ReceipeForm::ReceipeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceipeForm)
{
    ui->setupUi(this);
}

ReceipeForm::ReceipeForm(Database *database, const QMap<QString, SPositionConfig>& positionsMap, EReceipeType type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceipeForm),
    dbs(database),
    m_positionsMap(positionsMap),
    m_type(type)
{
    ui->setupUi(this);

    receipeTableModel = new ReceipeTableModel(dbs, ui->tableView);
    ui->tableView->setModel(receipeTableModel);

    ui->tableView->horizontalHeader()->setSectionResizeMode(EReceipeColumn_Material, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EReceipeColumn_Variant, QHeaderView::Stretch);

    ui->tableView->horizontalHeader()->setSectionResizeMode(EReceipeColumn_Position1, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EReceipeColumn_Position2, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EReceipeColumn_Position3, QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(EReceipeColumn_Position4, QHeaderView::Interactive);

    ui->tableView->setColumnWidth(EReceipeColumn_Quantity, 100);
    ui->tableView->setColumnWidth(EReceipeColumn_Unit, 60);

    ui->tableView->setColumnWidth(EReceipeColumn_Position1, 45);
    ui->tableView->setColumnWidth(EReceipeColumn_Position2, 45);
    ui->tableView->setColumnWidth(EReceipeColumn_Position3, 45);
    ui->tableView->setColumnWidth(EReceipeColumn_Position4, 45);

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked
                                   | QAbstractItemView::SelectedClicked);
                                   //| QAbstractItemView::CurrentChanged);       

    //material column
    SComboBoxDbsSetup setup;
    setup.dbs = dbs;
    setup.table = "stock";
    setup.column = "material";
    setup.filter = false;
    ComboBoxDbsDelegate* cbid1 = new ComboBoxDbsDelegate(setup, ui->tableView);
    ui->tableView->setItemDelegateForColumn(EReceipeColumn_Material, cbid1);

    //variant column
    setup.dbs = dbs;
    setup.table = "stock";
    setup.column = "variant";
    setup.filter = true;
    setup.filterColumnName = "material";
    setup.filterColumnIndex = 0;
    ComboBoxDbsDelegate* cbid2 = new ComboBoxDbsDelegate(setup, ui->tableView);
    ui->tableView->setItemDelegateForColumn(EReceipeColumn_Variant, cbid2);

    //position column
    index1 = new ComboBoxItemDelegate(ui->tableView);
    index2 = new ComboBoxItemDelegate(ui->tableView);
    index3 = new ComboBoxItemDelegate(ui->tableView);
    index4 = new ComboBoxItemDelegate(ui->tableView);

    ui->tableView->setItemDelegateForColumn(EReceipeColumn_Position1, index1);
    ui->tableView->setItemDelegateForColumn(EReceipeColumn_Position2, index2);
    ui->tableView->setItemDelegateForColumn(EReceipeColumn_Position3, index3);
    ui->tableView->setItemDelegateForColumn(EReceipeColumn_Position4, index4);

    ui->dateEdit->setDate(QDateTime::currentDateTime().date());
}

ReceipeForm::~ReceipeForm()
{
    delete ui;
}

void ReceipeForm::setSupplierList(const QList<SSupplier>& supplierList)
{
    ui->comboBoxSupplier->clear();
    foreach(SSupplier s, supplierList)
    {
        ui->comboBoxSupplier->addItem(s.company, QVariant(s.id));
    }

    m_suppliers.clear();
    m_suppliers.append(supplierList);

    if(ui->comboBoxSupplier->count()>0)
    {        
        on_comboBoxSupplier_currentIndexChanged(0);
    }
}

void ReceipeForm::setStorageList(const QList<SStorage>& storageList)
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

void ReceipeForm::on_comboBoxSupplier_currentIndexChanged(int index)
{
    if(index>=0 && index<m_suppliers.count())
    {
        SSupplier s = m_suppliers.at(index);
        QString info = QString(tr("%1 %2\n%3\n%4 %5\nIC:%6\nDIC:%7")).arg(s.division, s.name, s.street, s.city, s.zip, s.ico, s.dic);
        ui->plainTextEditSupplier->setPlainText(info);
    }
}

void ReceipeForm::on_comboBoxStorage_currentIndexChanged(int index)
{
    if(index>=0 && index<m_storages.count())
    {
        SStorage s = m_storages.at(index);
        if(m_positionsMap.contains(s.name))
        {
            SPositionConfig p = m_positionsMap.value(s.name);

            if(p.use1)
                ui->tableView->showColumn(EReceipeColumn_Position1);
            else
                ui->tableView->hideColumn(EReceipeColumn_Position1);
            if(p.use2)
                ui->tableView->showColumn(EReceipeColumn_Position2);
            else
                ui->tableView->hideColumn(EReceipeColumn_Position2);
            if(p.use3)
                ui->tableView->showColumn(EReceipeColumn_Position3);
            else
                ui->tableView->hideColumn(EReceipeColumn_Position3);
            if(p.use4)
                ui->tableView->showColumn(EReceipeColumn_Position4);
            else
                ui->tableView->hideColumn(EReceipeColumn_Position4);

            index1->setDisplayNames(p.names1);
            index2->setDisplayNames(p.names2);
            index3->setDisplayNames(p.names3);
            index4->setDisplayNames(p.names4);
            receipeTableModel->setDisplayNames1(p.names1);
            receipeTableModel->setDisplayNames2(p.names2);
            receipeTableModel->setDisplayNames3(p.names3);
            receipeTableModel->setDisplayNames4(p.names4);

            //clear table
            if(receipeTableModel->rowCount()>0)
                receipeTableModel->removeRows(0, receipeTableModel->rowCount());
        }
        else
        {
            QMessageBox::information(this, tr("Storage Settings"), tr("No setting found for position in warehouse.ini:\n\n[PositionXY]\nName='%1'").arg(s.name));
        }
    }
}

void ReceipeForm::on_pushButtonPlus_clicked(bool)
{
    QList<SReceipeItem>list = receipeTableModel->getList();

    receipeTableModel->insertRows(list.count(), 1, QModelIndex());

    ui->tableView->openPersistentEditor(receipeTableModel->index(list.count(), EReceipeColumn_Material));
    //TODO: ui->tableView->openPersistentEditor(receipeTableModel->index(list.count(), EReceipeColumn_Variant));

    ui->tableView->openPersistentEditor(receipeTableModel->index(list.count(), EReceipeColumn_Position1));
    ui->tableView->openPersistentEditor(receipeTableModel->index(list.count(), EReceipeColumn_Position2));
    ui->tableView->openPersistentEditor(receipeTableModel->index(list.count(), EReceipeColumn_Position3));
    ui->tableView->openPersistentEditor(receipeTableModel->index(list.count(), EReceipeColumn_Position4));

    setLastEditableRow(true);
}

void ReceipeForm::setLastEditableRow(bool enable)
{
    int rowCount = receipeTableModel->rowCount();

    if(rowCount>0)
    {
        if(enable)
        {
            ui->tableView->openPersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Material));
            //TODO: ui->tableView->openPersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Variant));

            ui->tableView->openPersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position1));
            ui->tableView->openPersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position2));
            ui->tableView->openPersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position3));
            ui->tableView->openPersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position4));
        }
        else
        {
            ui->tableView->closePersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Material));
            //TODO: ui->tableView->closePersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Variant));

            ui->tableView->closePersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position1));
            ui->tableView->closePersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position2));
            ui->tableView->closePersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position3));
            ui->tableView->closePersistentEditor(receipeTableModel->index(rowCount-1, EReceipeColumn_Position4));
        }
    }
}

void ReceipeForm::on_pushButtonMinus_clicked(bool)
{
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();
    QModelIndex index;

    if(indexes.size()>0)
    {
        std::reverse(indexes.begin(), indexes.end());
        foreach (index, indexes)
        {
            receipeTableModel->removeRows(index.row(), 1, QModelIndex());
        }
    }
    else
    {
        int row = ui->tableView->currentIndex().row();
        if(row>=0)
        {
            receipeTableModel->removeRows(row, 1, QModelIndex());
        }
        else
            QMessageBox::information(this, tr("Delete row"), tr("No row selected or mark whole line"));
    }

}

void ReceipeForm::on_pushButtonInsert_clicked(bool)
{
    SReceipe receipe = {};
    receipe.list = receipeTableModel->getList();

    //prijemka.header.supplier;
    int supplier_id = ui->comboBoxSupplier->currentData().toInt();
    QList<SSupplier> supplierList;
    bool ok = dbs->selectSupplier(supplierList, supplier_id);
    if(ok && supplierList.size()>0)
        receipe.header.supplier = supplierList.at(0);

    receipe.header.text = ui->plainTextEditInfo->toPlainText();
    receipe.header.date.setDate(ui->dateEdit->date().year(), ui->dateEdit->date().month(), ui->dateEdit->date().day());

    SPositionConfig config = m_positionsMap.value(ui->comboBoxStorage->currentText());

    if(receipe.list.size()>0 && supplierList.size()>0)
    {
        bool validAll = true;
        QString msg("");
        int index = 1;

        //validate
        foreach(SReceipeItem item, receipe.list)
        {
            bool valid = item.stock.id>=0;
            validAll &= valid;
            if(!valid)
            {
                msg.append(tr("Row %1 not valid, select stock material and variant.\n").arg(QString::number(index)));
            }

            //position
            bool hidden1 = ui->tableView->isColumnHidden(EReceipeColumn_Position1);
            bool hidden2 = ui->tableView->isColumnHidden(EReceipeColumn_Position2);
            bool hidden3 = ui->tableView->isColumnHidden(EReceipeColumn_Position3);
            bool hidden4 = ui->tableView->isColumnHidden(EReceipeColumn_Position4);
            if((!hidden1 && item.index1<0 && config.mandatory1) ||
               (!hidden2 && item.index2<0 && config.mandatory2) ||
               (!hidden3 && item.index3<0 && config.mandatory3) ||
               (!hidden4 && item.index4<0 && config.mandatory4))
            {
                msg.append(tr("Row %1 not valid, select storage position.\n").arg(QString::number(index)));
                validAll = false;
            }
            else
            {
                //validate material and type for selected position
                bool validStock = dbs->validateReceipe(item, m_type);

                if(!validStock && m_type==EReceipeType_in)
                {
                    msg.append(tr("Row %1 not valid, storage position is not empty or ocuppied with other material.\n").arg(QString::number(index)));
                    validAll = false;
                }
                else if(!validStock && m_type==EReceipeType_out)
                {
                    msg.append(tr("Row %1 not valid, storage position is empty or ocuppied with other material or not enough qauntity.\n").arg(QString::number(index)));
                    validAll = false;
                }
            }

            index++;
        }

        if(validAll)
        {
            emit receipeCreate(receipe);

            setLastEditableRow(false);
        }
        else
        {
            QMessageBox::information(this, tr("Insert Receipe"), msg);
        }
    }
    else
    {
        QMessageBox::information(this, tr("Insert Receipe"), tr("No stock selected or no supplier item available"));
    }
}

void ReceipeForm::changeEvent(QEvent* event)
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
