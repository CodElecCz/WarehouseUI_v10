#include <QDockWidget>
#include <QTextBrowser>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"

void MainWindow::createStockWindow(bool showWindow)
{    
    QDockWidget *dock = new QDockWidget(tr("StockView"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);    

    stockView = new StockView(dock);

    dock->setWidget(stockView);
    //dock->resize(250,400);

    ui->menuTools->addAction(dock->toggleViewAction());
    DockSupport::tabifyDockWidget(dock, "StockView", Qt::BottomDockWidgetArea);

    if(showWindow)
        dock->show();

    connect(stockView, SIGNAL(importData()), this, SLOT(stockView_importData()));
}

void MainWindow::closeStockWindow()
{
    if(stockView)
    {
        disconnect(stockView, SIGNAL(importData()), this, SLOT(stockView_importData()));
    }
}

void MainWindow::stockView_importData()
{
    if(!m_defAccoutningUnitIco.isEmpty())
    {
        pohoda->stock(m_defAccoutningUnitIco);
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Accounting unit not found"),
            QObject::tr("Unable to find default accounting unit ICO.\n"
                        "Define 'AccountingUnitIco' in Warehouse.ini.\n"
                        "\n\n"
                        "Click OK to exit."), QMessageBox::Ok);
    }
}
