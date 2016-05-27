#include "lobbydialog.h"
#include "ui_lobbydialog.h"
#include "../user.h"
#include <QGraphicsPixmapItem>

const int SQ_SIZE = 48;
const int SQ_ROWS = 8;
const int SQ_COLS = 8;

void MyPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    emit moved(this->data(0).toInt(), pos());
}

LobbyDialog::LobbyDialog(const QString& gameName, QWidget *parent) :
    QDialog(parent), ui(new Ui::LobbyDialog),
    m_Client("ws://localhost:8000/", this),
    m_GameName(gameName),
    m_Scene(0)
{
    ui->setupUi(this);

    // Configure handlers list
    m_Handlers["joined"] = joined;
    m_Handlers["left"] = left;
    m_Handlers["chatted"] = chatted;
    m_Handlers["moved"] = moved;

    m_Scene = new QGraphicsScene(0, 0, SQ_SIZE * SQ_ROWS, SQ_SIZE * SQ_COLS);

    // Set up JSON-RPC parameters
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", gameName);
    QJsonArray connectParams = QJsonArray() << userObj << gameObj;

    QJsonRpcMessage request = QJsonRpcMessage::createRequest("Game.join", connectParams);
    m_Client.openBlocking();
    m_Client.sendMessage(request);
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(parseLobbyMessage(QJsonRpcMessage)));
    connect(&m_Client, SIGNAL(notificationReceived(QJsonRpcMessage)),
            this, SLOT(parseLobbyMessage(QJsonRpcMessage)));

    ui->graphicsView->setScene(m_Scene);
}

LobbyDialog::~LobbyDialog()
{
    m_Client.close();
    delete ui;
}

void LobbyDialog::parseLobbyMessage(const QJsonRpcMessage& message)
{
    qDebug() << message << message.method();

    // Call the method appropriate for handling the message
    (this->*m_Handlers[message.method()])(message);
}

void LobbyDialog::joined(const QJsonRpcMessage &message)
{
    qDebug() << message.result().toString();
}

void LobbyDialog::left(const QJsonRpcMessage &message)
{
    qDebug() << message.result().toString();
}

void LobbyDialog::chatted(const QJsonRpcMessage &message)
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

void LobbyDialog::moved(const QJsonRpcMessage &message)
{
    QJsonArray params = message.params().toArray();
    QJsonObject userObj = params[0].toObject();
    QJsonObject unitObj = params[1].toObject();
    QJsonObject moveObj = params[2].toObject();

    foreach(QGraphicsItem* item, m_Scene->items())
    {
        if (item->data(0).toInt() == unitObj["id"].toInt())
        {
            item->setPos(QPointF(moveObj["x"].toDouble(),
                                 moveObj["y"].toDouble()));
            break;
        }
    }

    qDebug() << message.result().toString();
}

void LobbyDialog::sendMoved(int id, const QPointF pos)
{
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", m_GameName);
    QJsonObject unitObj;
    unitObj.insert("id", id);
    QJsonObject moveObj;
    moveObj.insert("x", pos.x());
    moveObj.insert("y", pos.y());
    QJsonArray moveParams = QJsonArray() << userObj << gameObj << unitObj << moveObj;

    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Game.move", moveParams);
    m_Client.sendMessage(message);
}

void LobbyDialog::on_pbSend_clicked()
{
    QJsonObject userObj;
    userObj.insert("name", User::instance().name());
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", m_GameName);
    QJsonObject messageObj;
    messageObj.insert("text", ui->leMessage->text());
    ui->leMessage->clear();
    QJsonArray chatParams = QJsonArray() << userObj << gameObj << messageObj;

    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Game.chat", chatParams);
    m_Client.sendMessage(message);
}
