#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QList>
#include "pohoda/pohoda.h"

typedef struct _SStockPosition
{
    //warehouse
    int storage_id;

    //POSITION
    int index1;
    int index2;
    int index3;
    int index4;

    //material
    SStock stock;
    int quantity;

} SStockPosition;

typedef struct _SPositionConfig
{
    bool use1;
    bool use2;
    bool use3;
    bool use4;

    bool mandatory1;
    bool mandatory2;
    bool mandatory3;
    bool mandatory4;

    QStringList names1;
    QStringList names2;
    QStringList names3;
    QStringList names4;

    int load1;
    int load2;
    int load3;
    int load4;

    QString format1;
    QString format2;
    QString format3;
    QString format4;

} SPositionConfig;

class Database : public QObject
{
    Q_OBJECT

public:
    Database(QObject *parent);

    bool connect();
    bool connect(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port);
    void close();
    bool query(const QString &query);
    QStringList queryResult(int index);
    void disconnect();

    //stock table
    bool insertStock(const SStock& stock);
    bool selectStock(QList<SStock>& stock, int id = -1);

    //supplier table
    bool insertSupplier(const SSupplier& supplier);
    bool selectSupplier(QList<SSupplier>& supplier, int id = -1);

    //storage table
    bool insertStorage(const SStorage& storage);
    bool selectStorage(QList<SStorage>& storage);

    //stock movement
    bool insertReceipe(const SReceipe& receipe, EReceipeType type);
    bool validateReceipe(const SReceipeItem &receipe, EReceipeType type);

    //positions
    bool selectPostions(QList<SStockPosition>& position, int storage_id = -1);

    QString getQueryLastError();

private:
    const char* CReceipeType[EReceipeType_count] = {"none", "in", "out"};

    QSqlDatabase db;
    QSqlQuery    *sqlQuery;
};

#endif // DATABASE_H
