#ifndef POHODA_H
#define POHODA_H

#include <QString>
#include <QtNetwork/QtNetwork>

#include "pohodastockrequest.h"
#include "pohodastoragerequest.h"
#include "pohodasupplierrequest.h"
#include "pohodaaccountingrequest.h"
#include "pohodaprijemkarequest.h"
#include "pohodavydejkarequest.h"

typedef enum _ERequestType
{
    ERequestType_none = 0,
    ERequestType_stock = 1,
    ERequestType_storage = 2,
    ERequestType_supplier = 3,
    ERequestType_accounting = 4,
    ERequestType_prijemka = 5,
    ERequestType_vydejka = 6
} ERequestType;

typedef enum _EReceipeType
{
    EReceipeType_none = 0,
    EReceipeType_in = 1,
    EReceipeType_out = 2,

    /* ... */
    EReceipeType_count = 3
} EReceipeType;

#define POHODA_POST_TIMEOUT_MS 3000

class Pohoda : public QObject
{
    Q_OBJECT

public:
    Pohoda(QObject *parent);

    //request
    bool stock(const QString& ico, bool waitForFinish = false);
    bool storage(const QString& ico, bool waitForFinish = false);
    bool supplier(const QString& ico, bool waitForFinish = false);
    bool accounting(bool waitForFinish = false);
    bool prijemka(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list, bool waitForFinish = false);
    bool vydejka(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list, bool waitForFinish = false);

    //response parse from POHODA xml
    QList<SStorage> getStorageList() {return m_storageRequest.getResponse(); };
    QList<SSupplier> getSupplierList() { return m_supplierRequest.getResponse(); };
    QList<SStock> getStockList() { return m_stockRequest.getResponse(); };
    QList<SAccounting> getAccountingList() { return m_accountingRequest.getResponse(); };
    SReceipe getPrijemka() { return m_prijemkaRequest.getResponse(); };
    SReceipe getVydejka() { return m_vydejkaRequest.getResponse(); };

    uint getDuration() { return m_durationMs; };
    void setHost(const QString& host) { m_host = host; }
    void setScheme(const QString& scheme) { m_scheme = scheme; }
    void setPath(const QString& path) { m_path = path; }
    void setPort(int port) { m_port = port; }

    PohodaStockRequest* getStockRequest() { return &m_stockRequest; };
    PohodaStorageRequest* getStorageRequest() { return &m_storageRequest; };
    PohodaSupplierRequest* getSupplierRequest() { return &m_supplierRequest; }
    PohodaAccountingRequest* getAccountingRequest() { return &m_accountingRequest;}
    PohodaPrijemkaRequest* getPrijemkaRequest() { return &m_prijemkaRequest; }
    PohodaVydejkaRequest* getVydejkaRequest() { return &m_vydejkaRequest; }

private:
    bool post(const QString& xml, bool waitForFinish = false);

signals:
    void postResponse(ERequestType type, const QString& xml);

private slots:
    void onFinish(QNetworkReply *reply);

private:
    PohodaStockRequest      m_stockRequest;
    PohodaStorageRequest    m_storageRequest;
    PohodaSupplierRequest   m_supplierRequest;
    PohodaAccountingRequest m_accountingRequest;
    PohodaPrijemkaRequest   m_prijemkaRequest;
    PohodaVydejkaRequest    m_vydejkaRequest;

    ERequestType        m_type;
    qint64              m_startTime;
    qint64              m_durationMs;

    QString             m_host;
    int                 m_port;
    QString             m_scheme;
    QString             m_path;
};

#endif // POHODA_H
