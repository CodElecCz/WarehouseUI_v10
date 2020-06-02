#ifndef POHODAPRIJEMKAREQUEST_H
#define POHODAPRIJEMKAREQUEST_H

#include <QString>
#include <QDateTime>
#include "pohodasupplierrequest.h"
#include "pohodastockrequest.h"

typedef struct _SReceipeItem
{
    int quantity;

    //POSITION
    int index1;
    int index2;
    int index3;
    int index4;

    SStock stock;
} SReceipeItem;

typedef struct _SReceipeHeader
{
    SSupplier supplier;
    QDate     date;
    QString   text;
} SReceipeHeader;

typedef struct _SImportDetail
{
    QString state;
    int     errNo;
    QString note;
    QString valueRequested;
} SImportDetail;

typedef struct _SReceipe
{
    int         id;
    QString     number;
    QString     state; //ok, error
    QString     note;
    QDateTime   dateTime;

    SReceipeHeader header;
    QList<SReceipeItem> list;
    QList<SImportDetail> importDetails;
} SReceipe;



class PohodaPrijemkaRequest
{        
public:
    PohodaPrijemkaRequest();

    QString getRequest(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list);
    void setResponse(const QString& responseXml);
    SReceipe getResponse()  { return m_receipe; }

private:
    SReceipe m_receipe;
};

#endif // POHODAPRIJEMKAREQUEST_H
