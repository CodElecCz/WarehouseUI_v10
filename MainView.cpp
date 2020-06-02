#include "MainView.h"
#include "ui_MainView.h"
#include "models/comboboxdbsdelegate.h"

#include <QDateTime>
#include <QtXml>

MainView::MainView(Database *dbs, const QMap<QString, SPositionConfig>& positionsMap, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

//PAGE1 : prijemka
    QVBoxLayout* l1 = new QVBoxLayout;
    ui->page1->setLayout(l1);

    receipeInForm = new ReceipeForm(dbs, positionsMap, EReceipeType_in);
    l1->addWidget(receipeInForm);

    connect(receipeInForm, SIGNAL(receipeCreate(SReceipe)), this, SLOT(prijemkaForm_receipeCreate(SReceipe)));

//PAGE2 : vydejka
    QVBoxLayout* l2 = new QVBoxLayout;
    ui->page2->setLayout(l2);

    receipeOutForm = new ReceipeForm(dbs, positionsMap, EReceipeType_out);
    l2->addWidget(receipeOutForm);

    connect(receipeOutForm, SIGNAL(receipeCreate(SReceipe)), this, SLOT(vydejkaForm_receipeCreate(SReceipe)));

//PAGE3 : export/import POHODA xml
    highlighter = new XMLSyntaxHighlighter(ui->textEdit->document());

}

MainView::~MainView()
{
    delete ui;
}

void MainView::toggleViewAction_page1(bool state)
{
    if(!state)
    {
        ui->page1->hide();
        int index = ui->toolBox->indexOf(ui->page1);
        if(index>=0)
            ui->toolBox->removeItem(index);
    }
    else
    {
        int index = ui->toolBox->indexOf(ui->page1);
        if(index==-1)
        {
            ui->toolBox->addItem(ui->page1, QIcon(":/icons/input.ico"), tr("Receipe In"));
            ui->page1->show();
        }
    }
}

void MainView::toggleViewAction_page2(bool state)
{
    if(!state)
    {
        ui->page2->hide();
        int index = ui->toolBox->indexOf(ui->page2);
        if(index>=0)
            ui->toolBox->removeItem(index);
    }
    else
    {
        int index = ui->toolBox->indexOf(ui->page2);
        if(index==-1)
        {
            ui->toolBox->addItem(ui->page2, QIcon(":/icons/output.ico"), tr("Receipe Out"));
            ui->page2->show();
        }
    }
}


void MainView::toggleViewAction_page3(bool state)
{
    if(!state)
    {
        ui->page3->hide();
        int index = ui->toolBox->indexOf(ui->page3);
        if(index>=0)
            ui->toolBox->removeItem(index);
    }
    else
    {
        int index = ui->toolBox->indexOf(ui->page3);
        if(index==-1)
        {
            ui->toolBox->addItem(ui->page3, QIcon(":/icons/xml-file.ico"), tr("POHODA - Export/Import log"));
            ui->page3->show();
        }
    }
}



void MainView::setSupplierList(const QList<SSupplier>& supplierList)
{    
    receipeInForm->setSupplierList(supplierList);
    receipeOutForm->setSupplierList(supplierList);
}

void MainView::setStorageList(const QList<SStorage> &storageList)
{
    receipeInForm->setStorageList(storageList);
    receipeOutForm->setStorageList(storageList);
}

void MainView::prijemkaForm_receipeCreate(const SReceipe& receipe)
{
    emit prijemkaCreate(receipe);
}

void MainView::vydejkaForm_receipeCreate(const SReceipe& receipe)
{
    emit vydejkaCreate(receipe);
}

void MainView::showXml(const QString &xml)
{
    QString xmlOut;

    QXmlStreamReader reader(xml);
    QXmlStreamWriter writer(&xmlOut);
    writer.setAutoFormatting(true);

    while (!reader.atEnd())
    {
        reader.readNext();
        if (!reader.isWhitespace())
        {
            writer.writeCurrentToken(reader);
        }
    }

    ui->textEdit->setPlainText(xmlOut);
}

void MainView::changeEvent(QEvent* event)
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
