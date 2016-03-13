#include <QEventLoop>
#include <QTimer>
#include <QtWebSockets/QWebSocket>

#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#else
#include "json/qjsondocument.h"
#endif

#include <private/qjsonrpcsocket_p.h>
#include <private/qjsonrpcservicereply_p.h>
#include "jsonrpcwebsocketclient.h"

class JsonRpcWebSocketReplyPrivate : public QJsonRpcServiceReplyPrivate
{
public:
    JsonRpcWebSocketResponse *websockResponse;
};

JsonRpcWebSocketReply::JsonRpcWebSocketReply(const QJsonRpcMessage& request,
                                    JsonRpcWebSocketResponse* response, QObject *parent)
    : QJsonRpcServiceReply(*new JsonRpcWebSocketReplyPrivate, parent)
{
    Q_D(JsonRpcWebSocketReply);
    d->request = request;
    d->websockResponse = response;
    connect(d->websockResponse, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(responseReceived(QJsonRpcMessage)));
}

void JsonRpcWebSocketReply::responseReceived(const QJsonRpcMessage& response)
{
    Q_D(JsonRpcWebSocketReply);
    d->response = response;
    Q_EMIT messageReceived(response);
    Q_EMIT finished();
}


class JsonRpcWebSocketClientPrivate : public QJsonRpcAbstractSocketPrivate
{
public:
    void initializeWebSocket(JsonRpcWebSocketClient *client)
    {
        websocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, client);
        QObject::connect(websocket, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),
                         client, SLOT(handleAuthenticationRequired(QNetworkProxy,QAuthenticator*)));
        QObject::connect(websocket, SIGNAL(sslErrors(QList<QSslError>)),
                         websocket, SLOT(ignoreSslErrors()));
        QObject::connect(client, SIGNAL(destroyed(QObject*)),
                         websocket, SLOT(close()));
        QObject::connect(websocket, SIGNAL(textMessageReceived(QString)),
                         client, SLOT(internalTextMessageReceived(QString)));
        QObject::connect(websocket, SIGNAL(binaryMessageReceived(QByteArray)),
                         client, SLOT(internalBinaryMessageReceived(QByteArray)));

        // Debug connection
        QObject::connect(websocket, SIGNAL(connected()),
                         client, SLOT(debugConnect()));
    }

    ~JsonRpcWebSocketClientPrivate() {
        websocket->close();
        delete websocket;
    }

    JsonRpcWebSocketResponse* writeMessage(const QJsonRpcMessage& message)
    {
        qDebug(__func__);
        QByteArray data = message.toJson();
        websocket->sendBinaryMessage(data);
        JsonRpcWebSocketResponse* response = new JsonRpcWebSocketResponse;
        responses.insert(message.id(), response);
        return response;
    }

    QHash<int, JsonRpcWebSocketResponse*> responses;
    QUrl endpoint;
    QWebSocket *websocket;
    QSslConfiguration sslConfiguration;
};

JsonRpcWebSocketClient::JsonRpcWebSocketClient(QObject *parent) :
    QJsonRpcAbstractSocket(*new JsonRpcWebSocketClientPrivate, parent)
{
    Q_D(JsonRpcWebSocketClient);
    d->initializeWebSocket(this);
}

JsonRpcWebSocketClient::JsonRpcWebSocketClient(const QString &endpoint, QObject *parent) :
    QJsonRpcAbstractSocket(*new JsonRpcWebSocketClientPrivate, parent)
{
    Q_D(JsonRpcWebSocketClient);
    d->endpoint = QUrl::fromUserInput(endpoint);
    d->initializeWebSocket(this);
}

#define Properties {
QUrl JsonRpcWebSocketClient::endPoint() const
{
    Q_D(const JsonRpcWebSocketClient);
    return d->endpoint;
}

void JsonRpcWebSocketClient::setEndPoint(const QUrl &endpoint)
{
    Q_D(JsonRpcWebSocketClient);
    d->endpoint = endpoint;
}

void JsonRpcWebSocketClient::setEndPoint(const QString &endpoint)
{
    Q_D(JsonRpcWebSocketClient);
    d->endpoint = QUrl::fromUserInput(endpoint);
}

QWebSocket* JsonRpcWebSocketClient::websocket()
{
    Q_D(JsonRpcWebSocketClient);
    return d->websocket;
}

QSslConfiguration JsonRpcWebSocketClient::sslConfiguration() const
{
    Q_D(const JsonRpcWebSocketClient);
    return d->sslConfiguration;
}

void JsonRpcWebSocketClient::setSslConfiguration(const QSslConfiguration &sslConfiguration)
{
    Q_D(JsonRpcWebSocketClient);
    d->sslConfiguration = sslConfiguration;
}
#define EndProperties }

void JsonRpcWebSocketClient::open()
{
    Q_D(JsonRpcWebSocketClient);
    d->websocket->open(d->endpoint);
}

void JsonRpcWebSocketClient::openBlocking(int msecs)
{
    Q_D(JsonRpcWebSocketClient);
    d->websocket->open(d->endpoint);

    QEventLoop responseLoop;
    connect(d->websocket, SIGNAL(connected()), &responseLoop, SLOT(quit()));
    QTimer::singleShot(msecs, &responseLoop, SLOT(quit()));
    responseLoop.exec();
}

void JsonRpcWebSocketClient::close()
{
    Q_D(JsonRpcWebSocketClient);
    d->websocket->close();
}

void JsonRpcWebSocketClient::notify(const QJsonRpcMessage &message)
{
    Q_D(JsonRpcWebSocketClient);
    if (d->endpoint.isEmpty()) {
        qJsonRpcDebug() << Q_FUNC_INFO << "invalid endpoint specified";
        return;
    }

    /*QNetworkReply *reply = */d->writeMessage(message);
    //connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));

    // notify() sends notifications, i.e. a JSON-RPC request that expects no response
    // NOTE: we might want to connect this to a local slot to track errors
    //       for debugging later?
    //connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), reply, SLOT(deleteLater()));
}

QJsonRpcServiceReply* JsonRpcWebSocketClient::sendMessage(const QJsonRpcMessage &message)
{
    Q_D(JsonRpcWebSocketClient);
    if (d->endpoint.isEmpty()) {
        qJsonRpcDebug() << Q_FUNC_INFO << "invalid endpoint specified";
        return 0;
    }

    JsonRpcWebSocketResponse *response = d->writeMessage(message);
    JsonRpcWebSocketReply *serviceReply = new JsonRpcWebSocketReply(message, response);
    return serviceReply;
}

QJsonRpcMessage JsonRpcWebSocketClient::sendMessageBlocking(const QJsonRpcMessage &message, int msecs)
{
    QJsonRpcServiceReply *reply = sendMessage(message);
    QScopedPointer<QJsonRpcServiceReply> replyPtr(reply);

    QEventLoop responseLoop;
    connect(reply, SIGNAL(finished()), &responseLoop, SLOT(quit()));
    QTimer::singleShot(msecs, &responseLoop, SLOT(quit()));
    responseLoop.exec();

    if (!reply->response().isValid())
        return message.createErrorResponse(QJsonRpc::TimeoutError, "request timed out");
    return reply->response();
}

QJsonRpcMessage JsonRpcWebSocketClient::invokeRemoteMethodBlocking(const QString &method, const QVariant &param1,
                                                               const QVariant &param2, const QVariant &param3,
                                                               const QVariant &param4, const QVariant &param5,
                                                               const QVariant &param6, const QVariant &param7,
                                                               const QVariant &param8, const QVariant &param9,
                                                               const QVariant &param10)
{
    QVariantList params;
    if (param1.isValid()) params.append(param1);
    if (param2.isValid()) params.append(param2);
    if (param3.isValid()) params.append(param3);
    if (param4.isValid()) params.append(param4);
    if (param5.isValid()) params.append(param5);
    if (param6.isValid()) params.append(param6);
    if (param7.isValid()) params.append(param7);
    if (param8.isValid()) params.append(param8);
    if (param9.isValid()) params.append(param9);
    if (param10.isValid()) params.append(param10);

    QJsonRpcMessage request =
        QJsonRpcMessage::createRequest(method, QJsonArray::fromVariantList(params));
    return sendMessageBlocking(request);
}

QJsonRpcServiceReply *JsonRpcWebSocketClient::invokeRemoteMethod(const QString &method, const QVariant &param1,
                                                             const QVariant &param2, const QVariant &param3,
                                                             const QVariant &param4, const QVariant &param5,
                                                             const QVariant &param6, const QVariant &param7,
                                                             const QVariant &param8, const QVariant &param9,
                                                             const QVariant &param10)
{
    QVariantList params;
    if (param1.isValid()) params.append(param1);
    if (param2.isValid()) params.append(param2);
    if (param3.isValid()) params.append(param3);
    if (param4.isValid()) params.append(param4);
    if (param5.isValid()) params.append(param5);
    if (param6.isValid()) params.append(param6);
    if (param7.isValid()) params.append(param7);
    if (param8.isValid()) params.append(param8);
    if (param9.isValid()) params.append(param9);
    if (param10.isValid()) params.append(param10);

    QJsonRpcMessage request =
        QJsonRpcMessage::createRequest(method, QJsonArray::fromVariantList(params));
    return sendMessage(request);
}

void JsonRpcWebSocketClient::handleAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator)
{
    Q_UNUSED(proxy)
    Q_UNUSED(authenticator)
}

void JsonRpcWebSocketClient::internalTextMessageReceived(const QString& message)
{
    qDebug(__func__);
    processMessage(QJsonDocument::fromJson(message.toUtf8()));
}

void JsonRpcWebSocketClient::internalBinaryMessageReceived(const QByteArray& message)
{
    qDebug(__func__);
    processMessage(QJsonDocument::fromJson(message));
}

void JsonRpcWebSocketClient::processMessage(const QJsonDocument& doc)
{
    Q_D(JsonRpcWebSocketClient);
    if (doc.isEmpty() || doc.isNull() || !doc.isObject())
    {
        QJsonRpcMessage error =  QJsonRpcMessage().
                createErrorResponse(QJsonRpc::ParseError,
                                    "unable to process incoming JSON data",
                                    QString::fromUtf8(doc.toBinaryData()));
        Q_EMIT messageError(error);
    }
    else
    {
        qJsonRpcDebug() << "received: " << doc.toJson();
        QJsonRpcMessage response = QJsonRpcMessage::fromObject(doc.object());
        if (0 == response.id())
            Q_EMIT notificationReceived(response);
        else if (d->responses.contains(response.id())) {
            d->responses[response.id()]->messageReceived(response);
            Q_EMIT messageReceived(response);
        }
        else
        {
            QJsonRpcMessage error =  QJsonRpcMessage().
                    createErrorResponse(QJsonRpc::InternalError,
                                        "invalid response id",
                                        QString::fromUtf8(doc.toBinaryData()));
            Q_EMIT messageError(error);
        }
    }
}
