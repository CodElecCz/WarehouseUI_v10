#ifndef POHODARECEIPEINREQUEST_H
#define POHODARECEIPEINREQUEST_H

#include <QString>
#include <QDateTime>
#include "pohodasupplierrequest.h"
#include "pohodastockrequest.h"

typedef enum _EReceipeType
{
    EReceipeType_none = 0,
    EReceipeType_in = 1,
    EReceipeType_out = 2,

    /* ... */
    EReceipeType_count = 3
} EReceipeType;

static const char * const CReceipeType[EReceipeType_count] = {"none", "in", "out"};

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
    int             id;
    QString         number;
    QString         state; //ok, error
    QString         note;
    QDateTime       dateTime;
    EReceipeType    type;

    SReceipeHeader header;
    QList<SReceipeItem> list;
    QList<SImportDetail> importDetails;
} SReceipe;

class PohodaReceipeInRequest
{        
public:
    PohodaReceipeInRequest();

    QString getRequest(const QString& ico, const SReceipeHeader& header, const QList<SReceipeItem>& list);
    void setResponse(const QString& responseXml);
    SReceipe getResponse()  { return m_receipe; }

private:
    SReceipe m_receipe;
};

#endif // POHODARECEIPEINREQUEST_H
