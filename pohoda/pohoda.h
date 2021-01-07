#ifndef POHODA_H
#define POHODA_H

#include <QString>
#include <QtNetwork/QtNetwork>

#include "pohodastockrequest.h"
#include "pohodastoragerequest.h"
#include "pohodasupplierrequest.h"
#include "pohodaaccountingrequest.h"
#include "pohodareceipeinrequest.h"
#include "pohodareceipeoutrequest.h"

typedef enum _ERequestType
{
    ERequestType_none = 0,
    ERequestType_stock = 1,
    ERequestType_storage = 2,
    ERequestType_supplier = 3,
    ERequestType_accounting = 4,
    ERequestType_receipeIn = 5,
    ERequestType_receipeOut = 6
} ERequestType;

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
    bool receipeIn(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list, bool waitForFinish = false);
    bool receipeOut(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list, bool waitForFinish = false);

    //response parse from POHODA xml
    QList<SStorage> getStorageList() {return m_storageRequest.getResponse(); };
    QList<SSupplier> getSupplierList() { return m_supplierRequest.getResponse(); };
    QList<SStock> getStockList() { return m_stockRequest.getResponse(); };
    QList<SAccounting> getAccountingList() { return m_accountingRequest.getResponse(); };
    SReceipe getReceipeIn() { return m_receipeInRequest.getResponse(); };
    SReceipe getReceipeOut() { return m_receipeOutRequest.getResponse(); };

    uint getDuration() { return m_durationMs; };
    void setHost(const QString& host) { m_host = host; }
    void setScheme(const QString& scheme) { m_scheme = scheme; }
    void setPath(const QString& path) { m_path = path; }
    void setPort(int port) { m_port = port; }

    PohodaStockRequest* getStockRequest() { return &m_stockRequest; };
    PohodaStorageRequest* getStorageRequest() { return &m_storageRequest; };
    PohodaSupplierRequest* getSupplierRequest() { return &m_supplierRequest; }
    PohodaAccountingRequest* getAccountingRequest() { return &m_accountingRequest;}
    PohodaReceipeInRequest* getReceipeInRequest() { return &m_receipeInRequest; }
    PohodaReceipeOutRequest* getReceipeOutRequest() { return &m_receipeOutRequest; }

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
    PohodaReceipeInRequest  m_receipeInRequest;
    PohodaReceipeOutRequest m_receipeOutRequest;

    ERequestType        m_type;
    qint64              m_startTime;
    qint64              m_durationMs;

    QString             m_host;
    int                 m_port;
    QString             m_scheme;
    QString             m_path;
};

#endif // POHODA_H
