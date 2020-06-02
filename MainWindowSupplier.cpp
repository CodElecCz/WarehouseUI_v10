#include <QDockWidget>
#include <QTextBrowser>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"

void MainWindow::createSupplierWindow(bool showWindow)
{
    QDockWidget *dock = new QDockWidget(tr("SupplierView"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);    

    supplierView = new SupplierView(dock);

    dock->setWidget(supplierView);
    //dock->resize(250,400);

    ui->menuTools->addAction(dock->toggleViewAction());
    tabifyBottomDockWidget(dock, "SupplierView");

    //resize Qt buck
    dock->setFloating(true);
    dock->hide();
    dock->setFloating(false);

    if(showWindow)
        dock->show();

    connect(supplierView, SIGNAL(importData()), this, SLOT(supplierView_importData()));
}

void MainWindow::closeSupplierWindow()
{
    if(supplierView)
    {
        disconnect(supplierView, SIGNAL(importData()), this, SLOT(supplierView_importData()));
    }
}

void MainWindow::supplierView_importData()
{
    if(!m_defAccoutningUnitIco.isEmpty())
    {
        pohoda->supplier(m_defAccoutningUnitIco);
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
