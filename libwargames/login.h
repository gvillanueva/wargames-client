#ifndef LIBWARGAMES_LOGIN_H
#define LIBWARGAMES_LOGIN_H

#include <QObject>
#include <qjsonrpc/qjsonrpchttpclient.h>

class QJsonRpcServiceReply;
namespace wargames
{

class LoginResponse : public QObject
{
    Q_OBJECT
public:
    LoginResponse(QJsonRpcServiceReply *reply, QJsonRpcHttpClient *client);
    ~LoginResponse();
    bool hasError() const;
    QString errorMessage() const;
    QString authToken() const;

signals:
    void finished();

private:
    QJsonRpcServiceReply *m_reply;
    QJsonRpcHttpClient *m_client;
};

class LoginRequest : public QObject
{
    Q_OBJECT
public:
    LoginRequest(const QString& username, const QString& password);
    LoginResponse* send();

private:
    QJsonArray params;
    QJsonRpcHttpClient *m_client;
};

}

#endif//LIBWARGAMES_LOGIN_H
