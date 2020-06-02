#ifndef POHODAACCOUNTINGREQUEST_H
#define POHODAACCOUNTINGREQUEST_H

#include <QString>
#include <QList>

typedef struct _SAccounting
{
    int year;

    QString company;
    QString surname;
    QString name;
    QString city;
    QString street;
    QString zip;
    QString ico;
    QString dic;
    QString dataFile;

} SAccounting;

#define ACCOUNTING_COLUMN_COUNT 9

class PohodaAccountingRequest
{        
public:
    PohodaAccountingRequest();

    QString getRequest();
    void setResponse(const QString& responseXml);
    QList<SAccounting> getResponse() { return m_list; }

public:
    QList<SAccounting> m_list;
};

#endif // POHODAACCOUNTINGREQUEST_H
