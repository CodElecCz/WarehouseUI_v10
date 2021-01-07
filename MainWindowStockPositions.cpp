#include <QDockWidget>
#include <QTextBrowser>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"

void MainWindow::createStockPositionsWindow(bool showWindow, const QMap<QString, SPositionConfig>& positionsMap)
{    
    QDockWidget *dock = new QDockWidget(tr("Stock Positions"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);    

    stockPositions = new StockPositions(dbs, positionsMap, dock);

    dock->setWidget(stockPositions);
    //dock->resize(250,400);

    ui->menuTools->addAction(dock->toggleViewAction());
    DockSupport::tabifyDockWidget(dock, "Stock Positions", Qt::RightDockWidgetArea);

    if(showWindow)
        dock->show();

    //connect(stockPositions, SIGNAL(importData()), this, SLOT(stockPositions_importData()));
}

void MainWindow::closeStockPositionsWindow()
{
    if(stockView)
    {
        //disconnect(stockPositions, SIGNAL(importData()), this, SLOT(stockPositions_importData()));
    }
}
