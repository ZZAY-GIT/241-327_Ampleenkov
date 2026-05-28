#include "httpclient.h"



HttpClient * HttpClient::m_instance = nullptr;

HttpClient::HttpClient(QObject *parent)
    : QObject{parent}
{
    m_networkAccessManager = new QNetworkAccessManager(this);
}

HttpClient *HttpClient::getInstance()
{
    if (!m_instance) {
        m_instance = new HttpClient();
    }
    return m_instance;
}

void HttpClient::list()
{
    QNetworkRequest request(QUrl("http://127.0.0.1/devices"));
    QNetworkReply* reply = m_networkAccessManager->get(request);
    QObject::connect(
        reply, &QNetworkReply::finished, this,
        [this, reply]{
            if (reply->error() != QNetworkReply::NoError) {
                qWarning() << "Get error:" << reply->errorString();
            }
            emit dataReceived(reply->readAll());
            reply->deleteLater();
        });
}

void HttpClient::get(const int id)
{
    QString strin = QString("http://127.0.0.1:8000/devices/%1").arg(id);
    qDebug() << "Отправляем запрос на: " << strin;
    QUrl url(strin);
    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkAccessManager->get(request);
    QObject::connect(
        reply, &QNetworkReply::finished, this,
        [this, reply](){
            if (reply->error() != QNetworkReply::NoError) {
                qWarning() << "Get error:" << reply->errorString();
            }
            emit dataReceived(reply->readAll());
            reply->deleteLater();
        });
}

void HttpClient::create(const QByteArray& data)
{
    QNetworkRequest request(QUrl("http://127.0.0.1:8000/devices"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = m_networkAccessManager->post(request, data);
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
        if (reply->error() != QNetworkReply::NoError)
            qWarning() << "Error:" << reply->errorString();
        emit dataReceived(reply->readAll());
        reply->deleteLater();
    });
}

void HttpClient::update(const int id, const QByteArray& data)
{
    QString strin = QString("http://127.0.0.1:8000/devices/%1").arg(id);
    qDebug() << "Отправляем запрос на: " << strin;
    QUrl url(strin);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = m_networkAccessManager->put(request, data);
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
        if (reply->error() != QNetworkReply::NoError)
            qWarning() << "Update error:" << reply->errorString();
        emit dataReceived(reply->readAll());
        reply->deleteLater();
    });
}

void HttpClient::remove(const int id)
{
    QString strin = QString("http://127.0.0.1:8000/devices/%1").arg(id);
    qDebug() << "Отправляем запрос на: " << strin;
    QUrl url(strin);
    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkAccessManager->deleteResource(request);
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
        if (reply->error() != QNetworkReply::NoError)
            qWarning() << "Remove error:" << reply->errorString();
        emit dataReceived(reply->readAll());
        reply->deleteLater();
    });
}


HttpClient::~HttpClient()
{
    delete m_networkAccessManager;
}