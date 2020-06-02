#include "pohodaaccountingrequest.h"

#include <QtXml/QtXml>

#define xml_foreach(node, list)                                                   \
  int __i = 0;                                                                    \
  int __count = (list).count();                                                   \
  if (__count > 0)                                                                \
    for (QDomNode node = (list).at(0); __i < __count; __i++, node = (list).at(__i))


PohodaAccountingRequest::PohodaAccountingRequest()
{

}

QString PohodaAccountingRequest::getRequest()
{
    QDomDocument xmlDoc;
    QString xmlText("");

    QString appDir =  QCoreApplication::applicationDirPath();

    QFile f(appDir + "/xml/accounting_export.xml");
    if (f.open(QIODevice::ReadOnly ))
    {
        xmlDoc.setContent(&f);
        f.close();

        xmlText = xmlDoc.toString();
    }

    return xmlText;
}

void PohodaAccountingRequest::setResponse(const QString &responseXml)
{
    QDomDocument xmlDoc;
    xmlDoc.setContent(responseXml);

    m_list.clear();
    QString tagName;

    QDomElement responsePack = xmlDoc.documentElement();
    tagName = responsePack.tagName();

    if(!responsePack.isNull())
    {
        QDomElement responsePackItem = responsePack.firstChildElement();
        tagName = responsePackItem.tagName();

        QDomElement listAccounting = responsePackItem.firstChildElement();
        tagName = listAccounting.tagName();

        QDomNodeList accounting = listAccounting.childNodes();

        xml_foreach(sNode, accounting)
        {            
            SAccounting s;

            QDomElement idElement = sNode.firstChildElement("acu:year");
            if(!idElement.isNull())
                s.year = idElement.text().toInt();

            QDomElement dataFileElement = sNode.firstChildElement("acu:dataFile");
            if(!dataFileElement.isNull())
                s.dataFile = dataFileElement.text();

            QDomElement identityElement = sNode.firstChildElement("acu:accountingUnitIdentity");
            if(!identityElement.isNull())
            {
                QDomElement addressElement = identityElement.firstChildElement("typ:address");
                if(!addressElement.isNull())
                {
                    QDomElement companyElement = addressElement.firstChildElement("typ:company");
                    if(!companyElement.isNull())
                        s.company = companyElement.text();

                    QDomElement nameElement = addressElement.firstChildElement("typ:name");
                    if(!nameElement.isNull())
                        s.name = nameElement.text();

                    QDomElement surnameElement = addressElement.firstChildElement("typ:surname");
                    if(!surnameElement.isNull())
                        s.surname = surnameElement.text();

                    QDomElement zipElement = addressElement.firstChildElement("typ:zip");
                    if(!zipElement.isNull())
                        s.zip = zipElement.text();

                    QDomElement streetElement = addressElement.firstChildElement("typ:street");
                    if(!streetElement.isNull())
                        s.street = streetElement.text();

                    QDomElement icoElement = addressElement.firstChildElement("typ:ico");
                    if(!icoElement.isNull())
                        s.ico = icoElement.text();

                    QDomElement dicElement = addressElement.firstChildElement("typ:dic");
                    if(!dicElement.isNull())
                        s.dic = dicElement.text();

                    QDomElement cityElement = addressElement.firstChildElement("typ:city");
                    if(!cityElement.isNull())
                        s.city = cityElement.text();
                }
            }

            m_list.append(s);
        }
    }
}
