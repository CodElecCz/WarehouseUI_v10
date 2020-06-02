#include "pohodastockrequest.h"

#include <QtXml/QtXml>

#define xml_foreach(node, list)                                                   \
  int __i = 0;                                                                    \
  int __count = (list).count();                                                   \
  if (__count > 0)                                                                \
    for (QDomNode node = (list).at(0); __i < __count; __i++, node = (list).at(__i))


PohodaStockRequest::PohodaStockRequest()
{

}

QString PohodaStockRequest::getRequest(const QString &ico)
{
    QDomDocument xmlDoc;
    QString xmlText("");

    QString appDir =  QCoreApplication::applicationDirPath();

    QFile f(appDir + "/xml/stock_export.xml");
    if (f.open(QIODevice::ReadOnly ))
    {
        xmlDoc.setContent(&f);
        f.close();

        QDomElement root = xmlDoc.documentElement();
        root.setAttribute("ico", ico);

        xmlText = xmlDoc.toString();
    }

    return xmlText;
}

void PohodaStockRequest::setResponse(const QString &responseXml)
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

        QDomElement listStock = responsePackItem.firstChildElement();
        tagName = listStock.tagName();

        QDomNodeList stock = listStock.childNodes();

        xml_foreach(sNode, stock)
        {
            QDomElement stockHeader = sNode.firstChildElement();
            tagName = stockHeader.tagName();

            SStock s;

            QDomElement idElement = stockHeader.firstChildElement("stk:id");
            if(!idElement.isNull())
                s.id = idElement.text().toInt();

            QDomElement codeElement = stockHeader.firstChildElement("stk:code");
            if(!codeElement.isNull())
                s.code = codeElement.text();

            QDomElement nameElement = stockHeader.firstChildElement("stk:name");
            if(!nameElement.isNull())
            {
                QStringList sNames = nameElement.text().split("; ");

                if(sNames.size()>=1)
                    s.material = sNames[0];

                if(sNames.size()>=2)
                    s.variant = sNames[1];
            }

            QDomElement availabilityElement = stockHeader.firstChildElement("stk:availability");
            if(!availabilityElement.isNull())
                s.availability = availabilityElement.text().toInt();

            QDomElement handlingInformationElement = stockHeader.firstChildElement("stk:handlingInformation");
            if(!handlingInformationElement.isNull())
                s.handlingInformation = handlingInformationElement.text().toInt();

            QDomElement massElement = stockHeader.firstChildElement("stk:mass");
            if(!massElement.isNull())
                s.mass = massElement.text().toFloat();

            QDomElement unitElement = stockHeader.firstChildElement("stk:unit");
            if(!unitElement.isNull())
                s.unit = unitElement.text();

            QDomElement countElement = stockHeader.firstChildElement("stk:count");
            if(!countElement.isNull())
                s.count = countElement.text().toFloat();

            QDomElement storageElement = stockHeader.firstChildElement("stk:storage");
            if(!storageElement.isNull())
            {
                QDomElement storageIdElement = storageElement.firstChildElement("typ:id");
                if(!storageIdElement.isNull())
                    s.storage_id = storageIdElement.text().toInt();
            }

            QDomElement supplierElement = stockHeader.firstChildElement("stk:supplier");
            if(!supplierElement.isNull())
            {
                QDomElement supplierIdElement = supplierElement.firstChildElement("typ:id");
                if(!supplierIdElement.isNull())
                    s.supplier_id = supplierIdElement.text().toInt();
            }

            m_list.append(s);
        }
    }
}
