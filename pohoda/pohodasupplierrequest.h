#ifndef POHODASUPPLIERREQUEST_H
#define POHODASUPPLIERREQUEST_H

#include <QString>
#include <QList>

typedef struct _SSupplier
{
    int id;

    QString company;
    QString name;
    QString division;
    QString city;
    QString street;
    QString zip;
    QString ico;
    QString dic;

} SSupplier;

class PohodaSupplierRequest
{        
public:
    PohodaSupplierRequest();

    QString getRequest(const QString& ico);
    void setResponse(const QString& responseXml);
    QList<SSupplier> getResponse()  { return m_list; }

private:
    QList<SSupplier> m_list;
};

#endif // POHODASUPPLIERREQUEST_H
