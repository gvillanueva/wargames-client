#include "lobbydialog.h"
#include "ui_lobbydialog.h"
#include "user.h"

LobbyDialog::LobbyDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::LobbyDialog),
    m_Client("ws://wargames.walkingtarget.com:8000/", this)
{
    ui->setupUi(this);

    // Set up JSON-RPC parameters
//    QJsonArray joinRoomParams;
//    joinRoomParams.append(User::instance().name());

//    QJsonRpcMessage message = QJsonRpcMessage::createRequest("joinRoom", joinRoomParams);
    QJsonRpcMessage listen = QJsonRpcMessage::createRequest("game.listen", QJsonObject());
    m_Client.openBlocking();
    m_Client.sendMessage(listen);
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(parseLobbyMessage(QJsonRpcMessage)));
}

LobbyDialog::~LobbyDialog()
{
    m_Client.close();
    delete ui;
}

void LobbyDialog::parseLobbyMessage(const QJsonRpcMessage& message)
{
    qDebug() << message << message.method();
    if (!message.method().compare("chat"))
    {
        if (message.result().isObject())
        {
            QJsonObject obj = message.result().toObject();
            QString user = obj["user"].toString();
            QString text = obj["text"].toString();
            QString log = QString("%0: %1").arg(user).arg(text);
            ui->teHistory->insertPlainText(log);
        }
        QJsonArray params = message.params().toArray();
        QString msg = QString("%0:%1").arg(params[0].toString()).arg(params[1].toString());
        ui->teHistory->append(msg);
    }
    else if(message.method().compare("add"))
    {
        qDebug() << message.result().toString();
    }
}

void LobbyDialog::on_pbSend_clicked()
{
    QJsonArray chatParams;
    chatParams.append(User::instance().name());
    chatParams.append(ui->leMessage->text());

    QJsonRpcMessage message = QJsonRpcMessage::createRequest("game.chat", chatParams);
    m_Client.sendMessage(message);
}
