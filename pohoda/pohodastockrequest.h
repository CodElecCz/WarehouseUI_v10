#ifndef POHODASTOCKREQUEST_H
#define POHODASTOCKREQUEST_H

#include <QString>
#include <QList>

typedef struct _SStock
{
    int id;
    int supplier_id;
    int storage_id;

    QString material;
    QString variant;
    QString unit;
    QString code;

    double mass;
    double count;
    int availability;
    int handlingInformation;

} SStock;

class PohodaStockRequest
{        
public:
    PohodaStockRequest();

    QString getRequest(const QString& ico);
    void setResponse(const QString& responseXml);
    QList<SStock> getResponse()  { return m_list; }

private:
    QList<SStock> m_list;
};

#endif // POHODASTOCKREQUEST_H
