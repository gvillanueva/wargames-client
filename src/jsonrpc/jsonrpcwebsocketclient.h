#ifndef JSONRPCWEBSOCKETCLIENT_H
#define JSONRPCWEBSOCKETCLIENT_H

#include <QObject>
#include <qjsonrpcsocket.h>
#include <qjsonrpcservicereply.h>
//#include <QWebSocket>

class JsonRpcWebSocketResponse : public QObject
{
    Q_OBJECT
signals:
    void messageReceived(const QJsonRpcMessage& message);
};


class JsonRpcWebSocketResponse;
class JsonRpcWebSocketReplyPrivate;
class JsonRpcWebSocketReply : public QJsonRpcServiceReply
{
    Q_OBJECT
public:
    JsonRpcWebSocketReply(const QJsonRpcMessage& request,
                          JsonRpcWebSocketResponse *response, QObject *parent = 0);
    virtual ~JsonRpcWebSocketReply() {}

Q_SIGNALS:
    void messageReceived(const QJsonRpcMessage &message);

private Q_SLOTS:
    void responseReceived(const QJsonRpcMessage &response);

private:
    Q_DECLARE_PRIVATE(JsonRpcWebSocketReply)
};


class QWebSocket;
class JsonRpcWebSocketClientPrivate;
class JsonRpcWebSocketClient : public QJsonRpcAbstractSocket
{
    Q_OBJECT
public:
    JsonRpcWebSocketClient(QObject *parent = 0);
    JsonRpcWebSocketClient(const QString &endPoint, QObject *parent = 0);

    QUrl endPoint() const;
    void setEndPoint(const QUrl &endPoint);
    void setEndPoint(const QString &endPoint);

    QWebSocket *websocket();

    QSslConfiguration sslConfiguration() const;
    void setSslConfiguration(const QSslConfiguration &sslConfiguration);

signals:
    void notificationReceived(const QJsonRpcMessage& message);
    void messageError(const QJsonRpcMessage& message);

public Q_SLOTS:
    void open();
    void openBlocking(int msecs = 30000);
    void close();
    virtual void notify(const QJsonRpcMessage &message);
    virtual QJsonRpcMessage sendMessageBlocking(const QJsonRpcMessage &message, int msecs = 30000);
    virtual QJsonRpcServiceReply *sendMessage(const QJsonRpcMessage &message);
    virtual QJsonRpcMessage invokeRemoteMethodBlocking(const QString &method, const QVariant &arg1 = QVariant(),
                                               const QVariant &arg2 = QVariant(), const QVariant &arg3 = QVariant(),
                                               const QVariant &arg4 = QVariant(), const QVariant &arg5 = QVariant(),
                                               const QVariant &arg6 = QVariant(), const QVariant &arg7 = QVariant(),
                                               const QVariant &arg8 = QVariant(), const QVariant &arg9 = QVariant(),
                                               const QVariant &arg10 = QVariant());
    virtual QJsonRpcServiceReply *invokeRemoteMethod(const QString &method, const QVariant &arg1 = QVariant(),
                                             const QVariant &arg2 = QVariant(), const QVariant &arg3 = QVariant(),
                                             const QVariant &arg4 = QVariant(), const QVariant &arg5 = QVariant(),
                                             const QVariant &arg6 = QVariant(), const QVariant &arg7 = QVariant(),
                                             const QVariant &arg8 = QVariant(), const QVariant &arg9 = QVariant(),
                                             const QVariant &arg10 = QVariant());

protected Q_SLOTS:
    virtual void handleAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator);
    virtual void internalTextMessageReceived(const QString& message);
    virtual void internalBinaryMessageReceived(const QByteArray& message);
    virtual void processMessage(const QJsonDocument& doc);
    void debugConnect()
    {
        qDebug() << "connected";
    }

private:
    Q_DECLARE_PRIVATE(JsonRpcWebSocketClient)
};

#endif // JSONRPCWEBSOCKETCLIENT_H
