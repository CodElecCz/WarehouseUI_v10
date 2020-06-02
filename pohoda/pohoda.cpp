#include "pohoda.h"
#include "gzip.h"

#include <QByteArray>
#include <QTextCodec>

Pohoda::Pohoda(QObject *parent) :
    QObject(parent),
    m_type(ERequestType_none),
    m_host("localhost"),
    m_port(666),
    m_scheme("http"),
    m_path("/xml")
{

}

bool Pohoda::stock(const QString &ico, bool waitForFinish)
{
    QString request = m_stockRequest.getRequest(ico);

    m_type = ERequestType_stock;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    return post(request, waitForFinish);
}

bool Pohoda::supplier(const QString &ico, bool waitForFinish)
{
    QString request = m_supplierRequest.getRequest(ico);

    m_type = ERequestType_supplier;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    return post(request, waitForFinish);
}

bool Pohoda::storage(const QString &ico, bool waitForFinish)
{
    QString request = m_storageRequest.getRequest(ico);

    m_type = ERequestType_storage;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    return post(request, waitForFinish);
}

bool Pohoda::accounting(bool waitForFinish)
{
    QString request = m_accountingRequest.getRequest();

    m_type = ERequestType_accounting;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    return post(request, waitForFinish);
}

bool Pohoda::prijemka(const QString &ico, const SReceipeHeader &header, const QList<SReceipeItem> &list, bool waitForFinish)
{
    QString request = m_prijemkaRequest.getRequest(ico, header, list);

    m_type = ERequestType_prijemka;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    return post(request, waitForFinish);
}

bool Pohoda::vydejka(const QString &ico, const SReceipeHeader &header, const QList<SReceipeItem> &list, bool waitForFinish)
{
    QString request = m_vydejkaRequest.getRequest(ico, header, list);

    m_type = ERequestType_vydejka;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    return post(request, waitForFinish);
}

bool Pohoda::post(const QString& xml, bool waitForFinish)
{
    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);

    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinish(QNetworkReply*)));
    connect(netManager, SIGNAL(finished(QNetworkReply*)), netManager, SLOT(deleteLater()));

    //QByteArray data = xml.toUtf8();

    //czech characters fix
    QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
    QByteArray data = codec->fromUnicode(xml);

    QString host(m_scheme + "://" + m_host + ":" + QString::number(m_port));
    QUrl url(host + m_path);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "STORMWARE HTTP client. http://www.stormware.cz");
    request.setRawHeader("STW-Authorization", "Basic QDo=");
    request.setRawHeader("Content-Type", " text/xml");
    request.setRawHeader("Accept-Encoding", "gzip, deflate");
    request.setRawHeader("Host", host.toUtf8());
    request.setRawHeader("Content-Length", QString::number(data.length()).toUtf8());
    request.setRawHeader("Pragma", "no-cache");
    request.setRawHeader("Connection", "Keep-Alive");

    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    loop.connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    loop.connect(netManager, SIGNAL(finished(QNetworkReply*)), SLOT(quit()));

    timer.start(POHODA_POST_TIMEOUT_MS); //your predefined timeout
    netManager->post(request, data);

    if(waitForFinish)
    {
        loop.exec();
        if (timer.isActive()) //replay received before timer, you can then get replay form network access manager and do whatever you want with it
        {
            ;
        }
        else //timer elapsed, no replay from client, ups
        {
            return false;
        }
    }

    return true;
}

void Pohoda::onFinish(QNetworkReply *reply)
{
    //qDebug("reply received");

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    if ( status != 200 )
    {
        QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        qDebug() << reason;
    }
    else
    {
        QByteArray contentLength =  reply->rawHeader("Content-Length");
        QByteArray bodyCompress = reply->readAll(); // bytes

        QByteArray body;
        gzip::decompress(bodyCompress, body);

        //QString text(body);

        //czech characters fix
        QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
        QString text = codec->toUnicode(body);

        //handle response type
        switch(m_type)
        {
        case ERequestType_stock:
            m_stockRequest.setResponse(text);
            break;

        case ERequestType_storage:
            m_storageRequest.setResponse(text);
            break;

        case ERequestType_supplier:
            m_supplierRequest.setResponse(text);
            break;

        case ERequestType_accounting:
            m_accountingRequest.setResponse(text);
            break;

        case ERequestType_prijemka:
            m_prijemkaRequest.setResponse(text);
            break;

        case ERequestType_vydejka:
            m_vydejkaRequest.setResponse(text);
            break;

        default:
            break;
        }

        ERequestType type = m_type;
        m_type = ERequestType_none;
        m_durationMs = QDateTime::currentMSecsSinceEpoch() - m_startTime;

        emit postResponse(type, text);
    }
}
