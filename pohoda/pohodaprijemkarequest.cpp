#include "pohodaprijemkarequest.h"

#include <QtXml/QtXml>

#define xml_foreach(node, list)                                                   \
  int __i = 0;                                                                    \
  int __count = (list).count();                                                   \
  if (__count > 0)                                                                \
    for (QDomNode node = (list).at(0); __i < __count; __i++, node = (list).at(__i))

PohodaPrijemkaRequest::PohodaPrijemkaRequest()
{

}

QString PohodaPrijemkaRequest::getRequest(const QString &ico, const SReceipeHeader &header, const QList<SReceipeItem> &list)
{
    QDomDocument xmlDoc;
    QString xmlText("");
    QString tagName("");

    m_receipe.header = header;
    m_receipe.list.clear();
    m_receipe.list.append(list);

    QString appDir =  QCoreApplication::applicationDirPath();

    QFile f(appDir + "/xml/prijemka_import.xml");
    if (f.open(QIODevice::ReadOnly ))
    {
        xmlDoc.setContent(&f);
        f.close();

        QDomElement dataPack = xmlDoc.documentElement();
        if(!dataPack.isNull())
            dataPack.setAttribute("ico", ico);

        QDomElement dataPackItem = dataPack.firstChildElement();
        tagName = dataPackItem.tagName();

        QDomElement prijemka = dataPackItem.firstChildElement();
        tagName = prijemka.tagName();

        QDomElement prijemkaHeader = prijemka.firstChildElement("pri:prijemkaHeader");
        if(!prijemkaHeader.isNull())
        {
            QDomElement dateHeader = prijemkaHeader.firstChildElement("pri:date");
            if(!dateHeader.isNull())            
            {
                QString sdate =  header.date.toString("yyyy-MM-dd");

                dateHeader.appendChild(xmlDoc.createTextNode(sdate));
            }

            QDomElement textHeader = prijemkaHeader.firstChildElement("pri:text");
            if(!textHeader.isNull())
                textHeader.appendChild(xmlDoc.createTextNode(header.text));

            QDomElement partnerHeader = prijemkaHeader.firstChildElement("pri:partnerIdentity");
            if(!partnerHeader.isNull())
            {
                QDomElement addressHeader = partnerHeader.firstChildElement("typ:address");
                if(!addressHeader.isNull())
                {
                    QDomElement companyAddress = addressHeader.firstChildElement("typ:company");
                    if(!companyAddress.isNull())
                        companyAddress.appendChild(xmlDoc.createTextNode(header.supplier.company));

                    QDomElement nameAddress = addressHeader.firstChildElement("typ:name");
                    if(!nameAddress.isNull())
                        nameAddress.appendChild(xmlDoc.createTextNode(header.supplier.name));

                    QDomElement divisionAddress = addressHeader.firstChildElement("typ:division");
                    if(!divisionAddress.isNull())
                        divisionAddress.appendChild(xmlDoc.createTextNode(header.supplier.division));

                    QDomElement cityAddress = addressHeader.firstChildElement("typ:city");
                    if(!cityAddress.isNull())
                        cityAddress.appendChild(xmlDoc.createTextNode(header.supplier.city));

                    QDomElement streetAddress = addressHeader.firstChildElement("typ:street");
                    if(!streetAddress.isNull())
                        streetAddress.appendChild(xmlDoc.createTextNode(header.supplier.street));

                    QDomElement zipAddress = addressHeader.firstChildElement("typ:zip");
                    if(!zipAddress.isNull())
                        zipAddress.appendChild(xmlDoc.createTextNode(header.supplier.zip));

                    QDomElement icoAddress = addressHeader.firstChildElement("typ:ico");
                    if(!icoAddress.isNull())
                        icoAddress.appendChild(xmlDoc.createTextNode(header.supplier.ico));

                    QDomElement dicAddress = addressHeader.firstChildElement("typ:dic");
                    if(!dicAddress.isNull())
                        dicAddress.appendChild(xmlDoc.createTextNode(header.supplier.dic));

                }
            }
        }

        QDomElement prijemkaDetail = prijemka.firstChildElement("pri:prijemkaDetail");
        if(!prijemkaDetail.isNull())
        {            
            QDomElement prijemkaItem = prijemkaDetail.firstChildElement("pri:prijemkaItem");
            if(!prijemkaItem.isNull())
            {
                foreach(SReceipeItem p, list)
                {                    
                    QDomNode prijemkaItemClone = prijemkaItem.cloneNode();

                    QDomElement quantityItem = prijemkaItemClone.firstChildElement("pri:quantity");
                    if(!quantityItem.isNull())
                        quantityItem.appendChild(xmlDoc.createTextNode(QString::number(p.quantity)));


                    QDomElement unitItem = prijemkaItemClone.firstChildElement("pri:unit");
                    if(!unitItem.isNull())
                        unitItem.appendChild(xmlDoc.createTextNode(p.stock.unit));

                    QDomElement stockItem = prijemkaItemClone.firstChildElement("pri:stockItem");
                    if(!stockItem.isNull())
                    {
                        QDomElement stockItemTyp = stockItem.firstChildElement("typ:stockItem");
                        if(!stockItemTyp.isNull())
                        {
                            QDomElement stockItemId = stockItemTyp.firstChildElement("typ:ids");
                            if(!stockItemId.isNull())
                                stockItemId.appendChild(xmlDoc.createTextNode(p.stock.code));
                        }
                    }

                    prijemkaDetail.appendChild(prijemkaItemClone);
                }

                prijemkaDetail.removeChild(prijemkaItem);
            }
        }

        xmlText = xmlDoc.toString();
    }

    return xmlText;
}

void PohodaPrijemkaRequest::setResponse(const QString &responseXml)
{
    QDomDocument xmlDoc;
    xmlDoc.setContent(responseXml);

    QString tagName("");

    QDomElement responsePack = xmlDoc.documentElement();
    tagName = responsePack.tagName();

    if(!responsePack.isNull())
    {
        QDomElement responsePackItem = responsePack.firstChildElement();
        tagName = responsePackItem.tagName();

        if(!responsePackItem.isNull())
        {
            m_receipe.state = responsePackItem.attribute("state", "");
            m_receipe.note = responsePackItem.attribute("note", "");

            m_receipe.dateTime.setDate(QDateTime::currentDateTime().date());
            m_receipe.dateTime.setTime(QDateTime::currentDateTime().time());

            m_receipe.importDetails.clear();
            m_receipe.id = -1;
            m_receipe.number = "";

            QDomElement prijemkaResponse = responsePackItem.firstChildElement();
            if(!prijemkaResponse.isNull())
            {
                QDomElement producedDetails = prijemkaResponse.firstChildElement("rdc:producedDetails");
                if(!producedDetails.isNull())
                {
                    QDomElement idDetails = producedDetails.firstChildElement("rdc:id");
                    if(!idDetails.isNull())
                        m_receipe.id = idDetails.text().toInt();

                    QDomElement numberDetails = producedDetails.firstChildElement("rdc:number");
                    if(!numberDetails.isNull())
                        m_receipe.number = numberDetails.text();
                }
                QDomElement importDetails = prijemkaResponse.firstChildElement("rdc:importDetails");
                if(!importDetails.isNull())
                {
                    QDomNodeList detail = importDetails.childNodes();

                    xml_foreach(sNode, detail)
                    {
                        SImportDetail i;

                        QDomElement stateElement = sNode.firstChildElement("rdc:state");
                        if(!stateElement.isNull())
                            i.state = stateElement.text();

                        QDomElement errnoElement = sNode.firstChildElement("rdc:errno");
                        if(!errnoElement.isNull())
                            i.errNo = errnoElement.text().toInt();

                        QDomElement noteElement = sNode.firstChildElement("rdc:note");
                        if(!noteElement.isNull())
                            i.note = noteElement.text();

                        QDomElement valueElement = sNode.firstChildElement("rdc:valueRequested");
                        if(!valueElement.isNull())
                            i.valueRequested = valueElement.text();

                        m_receipe.importDetails.append(i);
                    }
                }
            }
        }
    }
}


