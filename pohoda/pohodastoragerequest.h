#ifndef POHODASTORAGEREQUEST_H
#define POHODASTORAGEREQUEST_H

#include <QString>
#include <QList>

typedef struct _SStorage
{
    int id;

    QString name;
    QString text;

} SStorage;

class PohodaStorageRequest
{        
public:
    PohodaStorageRequest();

    QString getRequest(const QString& ico);
    void setResponse(const QString& responseXml);
    QList<SStorage> getResponse()  { return m_list; }

private:

    QList<SStorage> m_list;
};

#endif // POHODASTORAGEREQUEST_H
