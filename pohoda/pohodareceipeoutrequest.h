#ifndef POHODARECEIPEOUTQUEST_H
#define POHODARECEIPEOUTQUEST_H

#include <QString>
#include <QDateTime>
#include "pohodasupplierrequest.h"
#include "pohodastockrequest.h"
#include "pohodareceipeinrequest.h"


class PohodaReceipeOutRequest
{        
public:
    PohodaReceipeOutRequest();

    QString getRequest(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list);
    void setResponse(const QString& responseXml);
    SReceipe getResponse() { return m_receipe; }

private:
    SReceipe m_receipe;
};

#endif // POHODARECEIPEOUTQUEST_H
