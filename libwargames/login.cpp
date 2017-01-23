#include "login.h"

namespace wargames
{

LoginResponse::LoginResponse(QJsonRpcServiceReply *reply,
                             QJsonRpcHttpClient *client) :
    m_reply(reply), m_client(client)
{
    m_client->setParent(this);
    connect(reply, SIGNAL(finished()), this, SIGNAL(finished()));
}

LoginResponse::~LoginResponse()
{
    if (m_reply) {
        delete m_reply;
        m_reply = 0;
    }
}

bool LoginResponse::hasError() const
{
    return m_reply->response().errorCode() != QJsonRpc::NoError;
}

QString LoginResponse::errorMessage() const
{
    return m_reply->response().errorMessage();
}

QString LoginResponse::authToken() const
{
    return m_reply->response().result().toString();
}



LoginRequest::LoginRequest(const QString& username, const QString& password)
    : QObject(NULL),
      m_client(new QJsonRpcHttpClient("http://127.0.0.1:8000", this))
{
    // Set up JSON-RPC parameters
    QJsonObject user;
    user.insert("name", username);
    user.insert("password", password);
    params = QJsonArray() << user;
}

LoginResponse* LoginRequest::send()
{
    // Attempt to log in
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("User.login", params);

    QJsonRpcServiceReply *reply = m_client->sendMessage(message);
    LoginResponse *res = new LoginResponse(reply, m_client);

    return res;
}

}
