#ifndef POHODAVYDEJKAREQUEST_H
#define POHODAVYDEJKAREQUEST_H

#include <QString>
#include <QDateTime>
#include "pohodasupplierrequest.h"
#include "pohodastockrequest.h"
#include "pohodaprijemkarequest.h"


class PohodaVydejkaRequest
{        
public:
    PohodaVydejkaRequest();

    QString getRequest(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list);
    void setResponse(const QString& responseXml);
    SReceipe getResponse() { return m_receipe; }

private:
    SReceipe m_receipe;
};

#endif // POHODAVYDEJKAREQUEST_H
