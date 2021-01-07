#include "database.h"

#include <QtWidgets>
#include <QDebug>

using namespace log4cxx;

//LoggerPtr Database::logger(Logger::getLogger("Warehouse.Database"));

Database::Database(QObject *parent):
    QObject(parent)
{

}

bool Database::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
    {
            QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                QObject::tr("Unable to establish a database connection.\n"
                            "This example needs SQLite support. Please read "
                            "the Qt SQL driver documentation for information how "
                            "to build it."
                            ), QMessageBox::Cancel);
        return false;
    }

    return true;
}

bool Database::connect(const QString &driver, const QString &dbName, const QString &host,
                       const QString &user, const QString &passwd, int port)
{
    if (driver.contains("QSQLITE"))
    {
        QSqlDatabase::database(host, false).close();
        QSqlDatabase::removeDatabase(host);
        bool initDbs = false;

        db = QSqlDatabase::addDatabase("QSQLITE", host);
        sqlQuery = new QSqlQuery(db);

        QString appDir =  QCoreApplication::applicationDirPath();
        QFile f(appDir + "/" + host);
        if(!f.exists())
        {
            LOG4QT(WARN, QString("File not found: %1/%2").arg(appDir, host));

            QMessageBox::warning(nullptr, QObject::tr("Cannot open file"),
                QObject::tr("Unable to open database file, empty file created\n\n"
                            ), QMessageBox::Ok);

            initDbs = true;
        }

        db.setDatabaseName(appDir + "/" + host);

        if (!db.open())
        {
            LOG4QT(WARN, QString("Cannot open database"));

            QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                QObject::tr("Unable to establish a database connection.\n"
                            "This example needs SQLite support. Please read "
                            "the Qt SQL driver documentation for information how "
                            "to build it."
                            ), QMessageBox::Cancel);

            return false;
        }

        LOG4QT(INFO, QString("Open"));

        //init tables if file not exist
        if(initDbs)
        {
            LOG4QT(INFO, QString("Initialize"));

            //config.ini
            QString configFilePath = QFileInfo(QCoreApplication::applicationFilePath()).filePath();
            configFilePath.replace(".exe", ".ini");
            QFile configFile(configFilePath);
            if(configFile.exists())
            {
                QSettings *config = new QSettings(configFilePath, QSettings::IniFormat);
                config->setIniCodec("UTF-8");

                QString msg("");

                QString scriptFileName = config->value(QString("Database/ScriptFile")).toString();
                QFile scriptFile(appDir + "/" +  scriptFileName);
                if(scriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QString script(scriptFile.readAll().toStdString().c_str());

                    QString scriptSeparator = config->value(QString("Database/ScriptSeparator")).toString();
                    QStringList queryList = script.split(scriptSeparator);
                    foreach(QString sQuery, queryList)
                    {
                        query(sQuery.trimmed());
                    }
                }
            }
        }
    }

    return true;
}

void Database::close()
{
    bool status = db.isOpen();
    if(status)
    {
        db.close();

        LOG4QT(INFO, QString("Close"));
    }
}

bool Database::query(const QString& query)
{
    if(db.isOpen())
    {
        bool ok;
        QString msg("");      

        LOG4QT(INFO, QString("Query: %1").arg(query));

        ok = sqlQuery->exec(query);
        if(!ok)
        {
            LOG4QT(ERROR, QString("Query: %1").arg(sqlQuery->lastError().text()));
            return false;
        }
    }

    return true;
}

QStringList Database::queryResult(int index)
{
    QStringList list;

    if(sqlQuery->first())
    {
        do
        {
            SStorage s;
            list.append(sqlQuery->value(index).toString());
        }
        while(sqlQuery->next());
    }
    return list;
}

QString Database::getQueryLastError()
{
    return sqlQuery->lastError().text();
}

bool Database::insertStock(const SStock &s)
{
    QString sQuery = QString("INSERT OR REPLACE INTO stock VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11')").arg(
                           QString::number(s.id),
                           QString::number(s.supplier_id),
                           QString::number(s.storage_id),
                           s.code,
                           s.material,
                           s.variant,
                           s.unit,
                           QString::number(s.mass),
                           QString::number(s.count)
                           ).arg(
                           QString::number(s.availability),
                           QString::number(s.handlingInformation));

    return query(sQuery);
}

bool Database::selectStock(QList<SStock>& stock, int id)
{
    QString sQuery("");

    if(id<0)
        sQuery = QString("SELECT * FROM stock");
    else
        sQuery = QString("SELECT * FROM stock WHERE id='%1'").arg(id);

    bool ok = query(sQuery);
    if(ok)
    {
        stock.clear();

        while(sqlQuery->next())
        {
            SStock s;
            s.id = sqlQuery->value(0).toInt();
            s.supplier_id = sqlQuery->value(1).toInt();
            s.storage_id = sqlQuery->value(2).toInt();
            s.code = sqlQuery->value(3).toString();
            s.material = sqlQuery->value(4).toString();
            s.variant = sqlQuery->value(5).toString();
            s.unit = sqlQuery->value(6).toString();
            s.mass = sqlQuery->value(7).toFloat();
            s.count = sqlQuery->value(8).toFloat();
            s.availability = sqlQuery->value(9).toInt();
            s.handlingInformation = sqlQuery->value(10).toInt();

            stock.append(s);
        }
    }
    else
        return false;

    return true;
}

bool Database::insertSupplier(const SSupplier &s)
{
    QString sQuery = QString("INSERT OR REPLACE INTO supplier VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')").arg(
                           QString::number(s.id),
                           s.company, s.name, s.division,
                           s.city,s.street, s.zip, s.ico, s.dic);

    return query(sQuery);
}

bool Database::selectSupplier(QList<SSupplier>& supplier, int id)
{
    QString sQuery("");

    if(id<0)
        sQuery = QString("SELECT * FROM supplier");
    else
        sQuery = QString("SELECT * FROM supplier WHERE id='%1'").arg(id);

    bool ok = query(sQuery);
    if(ok)
    {
        supplier.clear();

        while(sqlQuery->next())
        {
            SSupplier s;
            s.id = sqlQuery->value(0).toInt();
            s.company = sqlQuery->value(1).toString();
            s.name = sqlQuery->value(2).toString();
            s.division = sqlQuery->value(3).toString();
            s.city = sqlQuery->value(4).toString();
            s.street = sqlQuery->value(5).toString();
            s.zip = sqlQuery->value(6).toString();
            s.ico = sqlQuery->value(7).toString();
            s.dic = sqlQuery->value(8).toString();

            supplier.append(s);
        }
    }
    else
        return false;

    return true;
}

bool Database::selectPostions(QList<SStockPosition> &position, int storage_id)
{
    QString sQuery("");

    if(storage_id<0)
        sQuery = QString("SELECT * FROM position_view");
    else
        sQuery = QString("SELECT * FROM position_view WHERE storage_id='%1'").arg(storage_id);


    bool ok = query(sQuery);
    if(ok)
    {
        position.clear();

        while(sqlQuery->next())
        {
            SStockPosition s = {};

            s.storage_id = sqlQuery->value(0).toInt();
            s.index1 = sqlQuery->value(1).toInt();
            s.index2 = sqlQuery->value(2).toInt();
            s.index3 = sqlQuery->value(3).toInt();
            s.index4 = sqlQuery->value(4).toInt();
            s.quantity = sqlQuery->value(5).toInt();
            s.stock.id = sqlQuery->value(6).toInt();
            s.stock.material = sqlQuery->value(7).toString();
            s.stock.variant = sqlQuery->value(8).toString();
            s.stock.mass = sqlQuery->value(9).toInt();
            s.stock.count = sqlQuery->value(10).toInt();
            s.stock.unit = sqlQuery->value(11).toString();

            position.append(s);
        }
    }
    else
        return false;

    return true;
}

bool Database::insertStorage(const SStorage &s)
{
    QString sQuery = QString("INSERT OR REPLACE INTO storage VALUES('%1', '%2', '%3')").arg(
                           QString::number(s.id),
                           s.name,
                           s.text
                           );

    return query(sQuery);
}

bool Database::selectStorage(QList<SStorage>& storage)
{
    QString sQuery = QString("SELECT * FROM storage");

    bool ok = query(sQuery);
    if(ok)
    {
        storage.clear();

        while(sqlQuery->next())
        {
            SStorage s;
            s.id = sqlQuery->value(0).toInt();
            s.name = sqlQuery->value(1).toString();
            s.text = sqlQuery->value(2).toString();

            storage.append(s);
        }
    }
    else
        return false;

    return true;
}

bool Database::insertReceipe(const SReceipe &receipe, EReceipeType type)
{
    bool ok = true;

    foreach(SReceipeItem p, receipe.list)
    {
        QString sQuery = QString("INSERT INTO receipe VALUES(null, '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', null)").arg(
                            QString::number(receipe.id),
                            receipe.number,
                            receipe.state,
                            receipe.header.date.toString("dd.MM.yyyy"),
                            QString::number(p.stock.id),
                            QString::number(receipe.header.supplier.id),
                            QString::number(p.quantity),
                            QString::number(p.stock.storage_id),
                            QString::number(p.index1)).arg(
                            QString::number(p.index2),
                            QString::number(p.index3),
                            QString::number(p.index4),
                            receipe.dateTime.toString("dd.MM.yyyy hh:mm:ss"),
                            CReceipeType[type]
                    );

        ok &= query(sQuery);
    }

    return ok;
}

EReceipeType Database::getReceipeType(QString stype)
{
    EReceipeType type = EReceipeType_none;

    if(0==QString::compare(stype, CReceipeType[EReceipeType_in]))
    {
        type = EReceipeType_in;
    }
    else if(0==QString::compare(stype, CReceipeType[EReceipeType_out]))
    {
        type = EReceipeType_in;
    }

    return type;
}

bool Database::selectReceipesFromPosition(const SStockPosition& position, QList<SReceipe>& receipes)
{
    bool ok = true;

    QString sQuery = QString("SELECT * FROM receipe_view WHERE storage_id='%1' AND position_1='%2' AND position_2='%3' AND position_3='%4' AND position_4='%5' ORDER BY record DESC").arg(
                        QString::number(position.storage_id),
                        QString::number(position.index1),
                        QString::number(position.index2),
                        QString::number(position.index3),
                        QString::number(position.index4));

    ok &= query(sQuery);
    if(ok)
    {
        receipes.clear();

        while(sqlQuery->next())
        {
            SReceipe r = {};
            r.id = sqlQuery->value(5).toInt();
            r.number = sqlQuery->value(6).toString();
            r.state = sqlQuery->value(7).toString();
            r.header.date = QDate::fromString(sqlQuery->value(8).toString(), "dd.MM.yyyy");
            r.type = getReceipeType(sqlQuery->value(10).toString());

            SReceipeItem p = {};
            p.stock.storage_id = sqlQuery->value(0).toInt();
            p.index1 = sqlQuery->value(1).toInt();
            p.index2 = sqlQuery->value(2).toInt();
            p.index3 = sqlQuery->value(3).toInt();
            p.index4 = sqlQuery->value(4).toInt();
            p.quantity = sqlQuery->value(9).toInt();
            p.stock.id = sqlQuery->value(12).toInt();
            p.stock.material = sqlQuery->value(13).toString();
            p.stock.variant = sqlQuery->value(14).toString();
            p.stock.mass = sqlQuery->value(15).toFloat();
            p.stock.unit = sqlQuery->value(16).toString();

            r.list.append(p);
            receipes.append(r);
        }
    }

    return ok;
}

bool Database::validateReceipe(const SReceipeItem &receipe, EReceipeType type)
{
    bool ok = true;
    bool valid = false;

    QString sQuery = QString("SELECT stock_id, count FROM position WHERE storage_id='%1' AND position_1='%2' AND position_2='%3' AND position_3='%4' AND position_4='%5'").arg(
                        QString::number(receipe.stock.storage_id),
                        QString::number(receipe.index1),
                        QString::number(receipe.index2),
                        QString::number(receipe.index3),
                        QString::number(receipe.index4));

    ok &= query(sQuery);

    if(ok)
    {
        int stock_id = -1;
        int count = -1;

        if(sqlQuery->first())
        {
            stock_id = sqlQuery->value(0).toInt();
            count = sqlQuery->value(1).toInt();
        }

        switch(type)
        {
        case EReceipeType_in:
            valid = (stock_id == -1 ||
                     stock_id == receipe.stock.id);
            break;
        case EReceipeType_out:
            valid = (stock_id == receipe.stock.id &&
                     count >= receipe.quantity);
            break;
        default:
            break;
        }
    }

    return ok && valid;
}
