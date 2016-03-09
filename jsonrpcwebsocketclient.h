#ifndef JSONRPCWEBSOCKETCLIENT_H
#define JSONRPCWEBSOCKETCLIENT_H

#include <QObject>
#include <qjsonrpcsocket.h>
#include <qjsonrpcservicereply.h>
//#include <QWebSocket>

class QWebSocket;
class JsonRpcWebSocketReplyPrivate;
class JsonRpcWebSocketReply : public QJsonRpcServiceReply
{
    Q_OBJECT
public:
    JsonRpcWebSocketReply(const QJsonRpcMessage& request,
                          QWebSocket* socket, QObject *parent = 0);
    virtual ~JsonRpcWebSocketReply() {}

Q_SIGNALS:
    void messageReceived(const QJsonRpcMessage &message);

private Q_SLOTS:
    void binaryMessageReceived(const QByteArray& data);
    void textMessageReceived(const QString& data);

private:
    Q_DECLARE_PRIVATE(JsonRpcWebSocketReply)
};


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
    void debugConnect()
    {
        qDebug() << "connected";
    }

private:
    Q_DECLARE_PRIVATE(JsonRpcWebSocketClient)
};

#endif // JSONRPCWEBSOCKETCLIENT_H
