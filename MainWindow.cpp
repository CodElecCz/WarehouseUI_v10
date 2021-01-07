#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QProcess>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LightWidget.h"
#include "ui_AboutDialog.h"
#include "style/CustomStyle.h"

using namespace log4cxx;

//LoggerPtr MainWindow::logger(Logger::getLogger("Warehouse.MainWindow"));

MainWindow::MainWindow(QWidget *parent) :
    DockSupport(parent),
    ui(new Ui::MainWindow),

    stockView(nullptr),
    supplierView(nullptr),
    storageView(nullptr),
    stockPositions(nullptr),

    pohoda(new Pohoda(this)),
    dbs(new Database(this)),

    m_defAccoutningUnitIco(""),
    m_allowShutDown(false),
    m_pohodaDisable(false)
{
    ui->setupUi(this);   

    readAppSettings();

    initializeApp();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readAppSettings()
{
    QSettings settings("QT", "WarehouseUI");

    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();    
}

void MainWindow::writeAppSettings()
{
    QSettings settings("QT", "WarehouseUI");

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();
}

void MainWindow::initializeApp()
{   
    //config.ini
    QString configFilePath = QFileInfo(QCoreApplication::applicationFilePath()).filePath();
    configFilePath.replace(".exe", ".ini");
    QFile configFile(configFilePath);
    if(!configFile.exists())
    {
        QString msg = "Config file not found: " + configFilePath;
        //QMessageBox::question(this, "Warning", msg, QMessageBox::Ok);
        qDebug()<<msg;
    }
    QSettings *config = new QSettings(configFilePath, QSettings::IniFormat);
    config->setIniCodec("UTF-8");

    //Languages
    createLanguageMenu();
    QString languageDef = config->value(QString("Aplication/Language")).toString();
    loadLanguage(languageDef);

    SplashScreen *info = new SplashScreen;
    info->setTitle(tr("Application Loading..."));

    QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    info->setSubTitle(QString("%1 (%2.%3.%4)").arg(APP_VER).arg(date.day()).arg(date.month()).arg(date.year()));
    info->show();

    LOG4QT(INFO, info->subTitle());

    int startTime =  QDateTime::currentMSecsSinceEpoch();

    //Layout
    info->setText(tr("Loading: Settings"));

    //Menubar
    bool menuBarHide = config->value(QString("Aplication/MenuBarHide")).toBool();
    if(menuBarHide)
    {
        menuBar()->hide();
    }
    else
    {
        //About dialog
        connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutDialog()));
    }

    //Toolbar
    bool toolBarHide = config->value(QString("Aplication/ToolBarHide")).toBool();
    if(toolBarHide)
    {
        ui->mainToolBar->hide();
    }
    else
    {
        initializeToolbar();
    }

    //Statusbar
    bool statusBarHide = config->value(QString("Aplication/StatusBarHide")).toBool();
    if(statusBarHide)
    {
        ui->statusBar->hide();
    }
    LightWidget *statusDbs = new LightWidget(Qt::white, "DBS: ");
    LightWidget *statusPohoda = new LightWidget(Qt::white, "POHODA: ");
    ui->statusBar->addPermanentWidget(statusDbs);
    ui->statusBar->addPermanentWidget(statusPohoda);
    //ui->statusBar->showMessage("test");

    //CENTRAL
    QMap<QString, SPositionConfig> positionsMap;
    QStringList keyList = config->childGroups();
    foreach(QString key, keyList)
    {
        if(key.contains("Position"))
        {
            SPositionConfig positionConfig = {};

            positionConfig.use1 = config->value(key + QString("/Use01")).toBool();
            positionConfig.use2 = config->value(key + QString("/Use02")).toBool();
            positionConfig.use3 = config->value(key + QString("/Use03")).toBool();
            positionConfig.use4 = config->value(key + QString("/Use04")).toBool();

            positionConfig.mandatory1 = config->value(key + QString("/Mandatory01")).toBool();
            positionConfig.mandatory2 = config->value(key + QString("/Mandatory02")).toBool();
            positionConfig.mandatory3 = config->value(key + QString("/Mandatory03")).toBool();
            positionConfig.mandatory4 = config->value(key + QString("/Mandatory04")).toBool();

            positionConfig.load1 = config->value(key + QString("/Load01")).toInt();
            positionConfig.load2 = config->value(key + QString("/Load02")).toInt();
            positionConfig.load3 = config->value(key + QString("/Load03")).toInt();
            positionConfig.load4 = config->value(key + QString("/Load04")).toInt();

            positionConfig.names1 = config->value(key + QString("/Names01")).toString().split(";");
            positionConfig.names2 = config->value(key + QString("/Names02")).toString().split(";");
            positionConfig.names3 = config->value(key + QString("/Names03")).toString().split(";");
            positionConfig.names4 = config->value(key + QString("/Names04")).toString().split(";");

            positionConfig.format1 = config->value(key + QString("/Format01")).toString();
            positionConfig.format2 = config->value(key + QString("/Format02")).toString();
            positionConfig.format3 = config->value(key + QString("/Format03")).toString();
            positionConfig.format4 = config->value(key + QString("/Format04")).toString();

            QString storageName = config->value(key + QString("/Name")).toString();                        

            positionsMap.insert(storageName, positionConfig);
        }
    }

    view = new MainView(dbs, positionsMap, this);

    bool receipeInHide = config->value(QString("Aplication/ReceipeInHide")).toBool();
    QAction* page1 = ui->menuTools->addAction(tr("Receipe In"));
    page1->setCheckable(true);
    connect(page1, SIGNAL(toggled(bool)), view, SLOT(toggleViewAction_page1(bool)));
    page1->setChecked(!receipeInHide);
    view->toggleViewAction_page1(!receipeInHide);

    bool receipeOutHide = config->value(QString("Aplication/ReceipeOutHide")).toBool();
    QAction* page2 = ui->menuTools->addAction(tr("Receipe Out"));
    page2->setCheckable(true);
    connect(page2, SIGNAL(toggled(bool)), view, SLOT(toggleViewAction_page2(bool)));
    page2->setChecked(!receipeOutHide);
    view->toggleViewAction_page2(!receipeOutHide);

    bool pohodaLogHide = config->value(QString("Aplication/PohodaLogHide")).toBool();
    QAction* page3 = ui->menuTools->addAction(tr("POHODA - Export/Import log"));
    page3->setCheckable(true);
    connect(page3, SIGNAL(toggled(bool)), view, SLOT(toggleViewAction_page3(bool)));
    page3->setChecked(!pohodaLogHide);
    view->toggleViewAction_page3(!pohodaLogHide);

    QWidget *centralWidget = new QWidget;
    QVBoxLayout *controlsLayout = new QVBoxLayout;
    controlsLayout->addWidget(view, 1);
    controlsLayout->setSpacing(0);
    controlsLayout->setMargin(0);

    centralWidget->setLayout(controlsLayout);
    setCentralWidget(centralWidget);  

    //DOCK1 - SupplierWindow
    bool supplierViewHide = config->value(QString("Aplication/SupplierWindowHide")).toBool();
    createSupplierWindow(!supplierViewHide);

    //DOCK2 - StorageWindow
    bool storageViewHide = config->value(QString("Aplication/StorageWindowHide")).toBool();
    createStorageWindow(!storageViewHide);

    //DOCK3 - StockWindow
    bool stockViewHide = config->value(QString("Aplication/StockWindowHide")).toBool();
    createStockWindow(!stockViewHide);

    //DOCK4 - StockPositionsWindow
    bool stockPositionsWindowHide = config->value(QString("Aplication/StockPositionsWindowHide")).toBool();
    createStockPositionsWindow(!stockPositionsWindowHide, positionsMap);

    //DBS
    info->setText(tr("Loading: Database"));

    bool dbsUse = config->value(QString("Database/Use")).toBool();
    if(!dbsUse)
    {
        statusDbs->setMsg(tr("disabled"));
    }
    else
    {
        QString dbsDriver = config->value(QString("Database/Driver")).toString();
        QString dbsHost = config->value(QString("Database/Host")).toString();
        QString dbsDatabase = config->value(QString("Database/Database")).toString();
        QString dbsUser = config->value(QString("Database/User")).toString();
        QString dbsPassword = config->value(QString("Database/Password")).toString();
        int dbsPort = config->value(QString("Database/Port")).toInt();

        bool status = dbs->connect(dbsDriver, dbsDatabase, dbsHost, dbsUser, dbsPassword, dbsPort);
        if(status)
        {
            statusDbs->setMsg(tr("connected"));
            statusDbs->setColor(QColor("light green"));

            //stock init from database
            QList<SStock> stockList;
            dbs->selectStock(stockList);
            stockView->setStockList(stockList);

            //supplier init from database
            QList<SSupplier> supplierList;
            dbs->selectSupplier(supplierList);
            supplierView->setSupplierList(supplierList);
            view->setSupplierList(supplierList);

            //storage init from database
            QList<SStorage> storageList;
            dbs->selectStorage(storageList);
            storageView->setStorageList(storageList);
            view->setStorageList(storageList);

            //positions
            stockPositions->setStorageList(storageList);
        }
        else
        {
            statusDbs->setMsg(tr("error"));
            statusDbs->setColor(QColor("red"));
        }
    }

    //POHODA
    info->setText(tr("Loading: POHODA"));

    bool pohodaUse = config->value(QString("Pohoda/Use")).toBool();
    if(!pohodaUse)
    {
        statusPohoda->setMsg(tr("disabled"));

        m_pohodaDisable = !pohodaUse;

        connect(view, SIGNAL(prijemkaCreate(SReceipe)), this, SLOT(view_prijemkaCreate(SReceipe)));
        connect(view, SIGNAL(vydejkaCreate(SReceipe)), this, SLOT(view_vydejkaCreate(SReceipe)));
    }
    else
    {
        QString pohodaHost = config->value(QString("Pohoda/Host")).toString();
        int pohodaPort = config->value(QString("Pohoda/Port")).toInt();
        QString pohodaScheme = config->value(QString("Pohoda/Scheme")).toString();
        QString pohodaPath = config->value(QString("Pohoda/Path")).toString();

        pohoda->setHost(pohodaHost);
        pohoda->setPort(pohodaPort);
        pohoda->setScheme(pohodaScheme);
        pohoda->setPath(pohodaPath);

        connect(pohoda, SIGNAL(postResponse(ERequestType, QString)), this, SLOT(pohoda_postResponse(ERequestType, QString)));
        connect(view, SIGNAL(prijemkaCreate(SReceipe)), this, SLOT(view_prijemkaCreate(SReceipe)));
        connect(view, SIGNAL(vydejkaCreate(SReceipe)), this, SLOT(view_vydejkaCreate(SReceipe)));

        bool status = pohoda->accounting(true);
        if(status)
        {
            statusPohoda->setMsg(tr("connected"));
            statusPohoda->setColor(QColor("light green"));

            QList<SAccounting> accoutingList = pohoda->getAccountingList();

            QString defAccoutningUnit = config->value(QString("Pohoda/AccountingUnitIco")).toString();
            bool found = false;
            QString year = 0;
            QString dataFile("");

            foreach(SAccounting a, accoutingList)
            {
                if(0==a.ico.compare(defAccoutningUnit))
                {
                    found=true;
                    dataFile =a.dataFile;
                    year = QString::number(a.year);

                    break;
                }
            }

            if(!found)
            {
                QMessageBox::critical(this, QObject::tr("Accounting unit not found"),
                    QObject::tr("Unable to find default accounting unit ICO:%1.\n\n"
                                "Define 'AccountingUnitIco' in Warehouse.ini."
                                ).arg(defAccoutningUnit), QMessageBox::Ok);

            }
            else
            {
                m_defAccoutningUnitIco = defAccoutningUnit;

                QString winTitle = this->windowTitle();

                this->setWindowTitle(QString(tr("%1 [ID:%2; year:%3; file:%4]")).arg(winTitle, defAccoutningUnit, year, dataFile));

            }
        }
        else
        {
            statusPohoda->setMsg(tr("error"));
            statusPohoda->setColor(QColor("red"));

            QMessageBox::critical(this, QObject::tr("POHODA mServer"),
                QObject::tr("Unable to connect to POHODA mServer:\n\n"
                            "%1://%2:%3%4"
                            ).arg(pohodaScheme, pohodaHost, QString::number(pohodaPort), pohodaPath), QMessageBox::Ok);
        }
    }       

    //min time for SplashScreen
    int startDelta = QDateTime::currentMSecsSinceEpoch() - startTime;
    if(startDelta<2000)
        QThread::msleep(2000-startDelta);

    info->close();
    delete info;

    bool fullScreen = config->value(QString("Aplication/FullScreen")).toBool();
    if(fullScreen)
        showFullScreen();    
}

typedef enum _EButtonClose
{
    EButtonClose_CLOSE = 0,
    EButtonClose_SHUTDOWN,
    EButtonClose_CANCEL
} EButtonClose;

void MainWindow::closeEvent(QCloseEvent *event)
{
    int reply = closeDialog();
    if(reply==EButtonClose_CANCEL)
        event->ignore();
}

int MainWindow::closeDialog()
{
    int reply = EButtonClose_CANCEL;

    if(m_allowShutDown)
    {
        reply = QMessageBox::question(this, tr("Quit"), tr("Close application?"), tr("Close"), tr("Shutdown PC"), tr("Cancel"), 2);
    }
    else
    {
        QMessageBox::StandardButton sreply;
        sreply = QMessageBox::question(this, tr("Quit"), tr("Close application?"), QMessageBox::Yes|QMessageBox::No);
        if (sreply == QMessageBox::Yes)
        {
            reply = EButtonClose_CLOSE;
        }
        else
            reply = EButtonClose_CANCEL;
    }

    if(reply != EButtonClose_CANCEL)
    {        
        writeAppSettings();        

        this->close();

        //shutdown window
        SplashScreen *info = new SplashScreen;
        info->setTitle(tr("Application close"));
        info->show();

        //close database
        info->setText(tr("Database: Close"));
        dbs->close();

        //shutdown pc
        if(reply == EButtonClose_SHUTDOWN)
        {
            info->setText(tr("PC: Close"));

            qDebug() << "shutdown -s -f -t 10";
            QProcess::startDetached("shutdown -s -f -t 10");
        }

        info->setText(tr("Application: Quit"));
        QThread::msleep(1000);
        QApplication::quit();
    }

    return reply;
}

void MainWindow::initializeToolbar()
{

}

void MainWindow::pohoda_postResponse(ERequestType type, QString xml)
{
    //handle response type
    switch(type)
    {
    case ERequestType_stock:
        {
            QList<SStock> stockList = pohoda->getStockList();

            bool ok = true;
            foreach(SStock s, stockList)
            {
                ok &= dbs->insertStock(s);
            }

            if(!ok)
            {
                QMessageBox::critical(this, QObject::tr("Database query"),
                    QObject::tr("Error database query.\n\n"
                                "%1"
                                ).arg(dbs->getQueryLastError()), QMessageBox::Ok);
            }

            stockView->setStockList(stockList);
        }
        break;

    case ERequestType_storage:
        {
            QList<SStorage> storageList = pohoda->getStorageList();

            bool ok = true;
            foreach(SStorage s, storageList)
            {
                ok &= dbs->insertStorage(s);
            }

            if(!ok)
            {
                QMessageBox::critical(this, QObject::tr("Database query"),
                    QObject::tr("Error database query.\n\n"
                                "%1"
                                ).arg(dbs->getQueryLastError()), QMessageBox::Ok);
            }

            storageView->setStorageList(storageList);
            view->setStorageList(storageList);

            stockPositions->setStorageList(storageList);
        }
        break;

    case ERequestType_supplier:
        {
            QList<SSupplier> supplierList = pohoda->getSupplierList();

            bool ok = true;
            foreach(SSupplier s, supplierList)
            {
                ok &= dbs->insertSupplier(s);
            }

            if(!ok)
            {
                QMessageBox::critical(this, QObject::tr("Database query"),
                    QObject::tr("Error database query.\n\n"
                                "%1"
                                ).arg(dbs->getQueryLastError()), QMessageBox::Ok);
            }

            supplierView->setSupplierList(supplierList);
            view->setSupplierList(supplierList);
        }
        break;

    case ERequestType_accounting:

        break;

    case ERequestType_receipeIn:
        {
            SReceipe receipe = pohoda->getReceipeIn();

            QString msg = QString(tr("State: '%1'\nNumber: '%2'\n\n")).arg(receipe.state, receipe.number);
            if(receipe.note.length()>0)
            {
                msg.append(receipe.note);
                msg.append("\n");
            }
            foreach(SImportDetail i, receipe.importDetails)
            {
                QString msgDetail = QString("%1 [%2]: %3\n").arg(i.state, QString::number(i.errNo), i.note);
                msg.append(msgDetail);
            }
            QMessageBox::information(this, QObject::tr("In Receipe Import"), msg, QMessageBox::Ok);

            bool ok = dbs->insertReceipe(receipe, EReceipeType_in);
            if(ok)
            {
                //stock update from database
                QList<SStock> stockList;
                dbs->selectStock(stockList);
                stockView->setStockList(stockList);

                stockPositions->updatePositions();
            }
            else
            {
                QMessageBox::critical(this, QObject::tr("Database query"),
                    QObject::tr("Error database query.\n\n"
                                "%1"
                                ).arg(dbs->getQueryLastError()), QMessageBox::Ok);
            }
        }
        break;

    case ERequestType_receipeOut:
        {
            SReceipe receipe = pohoda->getReceipeOut();

            QString msg = QString(tr("State: '%1'\nNumber: '%2'\n\n")).arg(receipe.state, receipe.number);
            if(receipe.note.length()>0)
            {
                msg.append(receipe.note);
                msg.append("\n");
            }
            foreach(SImportDetail i, receipe.importDetails)
            {
                QString msgDetail = QString("%1 [%2]: %3\n").arg(i.state, QString::number(i.errNo), i.note);
                msg.append(msgDetail);
            }
            QMessageBox::information(this, QObject::tr("Out Receipe Import"), msg, QMessageBox::Ok);

            bool ok = dbs->insertReceipe(receipe, EReceipeType_out);
            if(ok)
            {
                //stock update from database
                QList<SStock> stockList;
                dbs->selectStock(stockList);
                stockView->setStockList(stockList);

                stockPositions->updatePositions();
            }
            else
            {
                QMessageBox::critical(this, QObject::tr("Database query"),
                    QObject::tr("Error database query.\n\n"
                                "%1"
                                ).arg(dbs->getQueryLastError()), QMessageBox::Ok);
            }
        }
        break;

    default:
        break;
    }

    view->showXml(xml);
}

void MainWindow::view_prijemkaCreate(SReceipe receipe)
{
    if(!m_pohodaDisable)
    {
        pohoda->receipeIn(m_defAccoutningUnitIco, receipe.header, receipe.list);
    }
    else
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        receipe.dateTime.setDate(QDateTime::currentDateTime().date());
        receipe.dateTime.setTime(QDateTime::currentDateTime().time());

        receipe.id = -1;
        receipe.number = "";
        receipe.state = "pending";

        bool ok = dbs->insertReceipe(receipe, EReceipeType_in);
        if(ok)
        {
            //stock update from database
            QList<SStock> stockList;
            dbs->selectStock(stockList);
            stockView->setStockList(stockList);

            stockPositions->updatePositions();

            QApplication::restoreOverrideCursor();
        }
        else
        {
            QApplication::restoreOverrideCursor();

            QMessageBox::critical(this, QObject::tr("Database query"),
                QObject::tr("Error database query.\n\n"
                            "%1"
                            ).arg(dbs->getQueryLastError()), QMessageBox::Ok);
        }
    }
}

void MainWindow::view_vydejkaCreate(SReceipe receipe)
{
    if(!m_pohodaDisable)
    {
        pohoda->receipeOut(m_defAccoutningUnitIco, receipe.header, receipe.list);
    }
    else
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        receipe.dateTime.setDate(QDateTime::currentDateTime().date());
        receipe.dateTime.setTime(QDateTime::currentDateTime().time());

        receipe.id = -1;
        receipe.number = "";
        receipe.state = "pending";

        bool ok = dbs->insertReceipe(receipe, EReceipeType_out);
        if(ok)
        {
            //stock update from database
            QList<SStock> stockList;
            dbs->selectStock(stockList);
            stockView->setStockList(stockList);

            stockPositions->updatePositions();

            QApplication::restoreOverrideCursor();
        }
        else
        {
            QApplication::restoreOverrideCursor();

            QMessageBox::critical(this, QObject::tr("Database query"),
                QObject::tr("Error database query.\n\n"
                            "%1"
                            ).arg(dbs->getQueryLastError()), QMessageBox::Ok);
        }
    }
}

void MainWindow::aboutDialog()
{
    QDialog *info = new QDialog(this, nullptr);
    Ui_AboutDialog aboutUi;
    aboutUi.setupUi(info);

    QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    aboutUi.labelVer->setText(QString("%1 (%2.%3.%4)").arg(APP_VER).arg(date.day()).arg(date.month()).arg(date.year()));

    info->show();  
}




