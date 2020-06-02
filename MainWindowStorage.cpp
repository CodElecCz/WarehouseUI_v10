#include <QDockWidget>
#include <QTextBrowser>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"

void MainWindow::createStorageWindow(bool showWindow)
{
    QDockWidget *dock = new QDockWidget(tr("StorageView"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);    

    storageView = new StorageView(dock);

    dock->setWidget(storageView);
    //dock->resize(250,400);

    ui->menuTools->addAction(dock->toggleViewAction());
    tabifyBottomDockWidget(dock, "StorageView");

    //resize Qt buck
    dock->setFloating(true);
    dock->hide();
    dock->setFloating(false);

    if(showWindow)
        dock->show();

    connect(storageView, SIGNAL(importData()), this, SLOT(storageView_importData()));
}

void MainWindow::closeStorageWindow()
{
    if(storageView)
    {
        disconnect(supplierView, SIGNAL(importData()), this, SLOT(storageView_importData()));
    }
}

void MainWindow::storageView_importData()
{
    if(!m_defAccoutningUnitIco.isEmpty())
    {
        pohoda->storage(m_defAccoutningUnitIco);
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
