#include "pohodastoragerequest.h"

#include <QtXml/QtXml>

#define xml_foreach(node, list)                                                   \
  int __i = 0;                                                                    \
  int __count = (list).count();                                                   \
  if (__count > 0)                                                                \
    for (QDomNode node = (list).at(0); __i < __count; __i++, node = (list).at(__i))


PohodaStorageRequest::PohodaStorageRequest()
{

}

QString PohodaStorageRequest::getRequest(const QString& ico)
{
    QDomDocument xmlDoc;
    QString xmlText("");

    QString appDir =  QCoreApplication::applicationDirPath();

    QFile f(appDir + "/xml/storage_export.xml");
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

void PohodaStorageRequest::setResponse(const QString& responseXml)
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

        QDomElement listStore = responsePackItem.firstChildElement();
        tagName = listStore.tagName();

        QDomNodeList store = listStore.childNodes();

        xml_foreach(sNode, store)
        {            
            SStorage s;

            QDomElement idElement = sNode.firstChildElement("sto:id");
            if(!idElement.isNull())
                s.id = idElement.text().toInt();

            QDomElement nameElement = sNode.firstChildElement("sto:name");
            if(!nameElement.isNull())
                s.name = nameElement.text();

            QDomElement textElement = sNode.firstChildElement("sto:text");
            if(!textElement.isNull())
                s.text = textElement.text();

            m_list.append(s);
        }
    }
}
