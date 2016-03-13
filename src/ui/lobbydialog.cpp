#include "lobbydialog.h"
#include "ui_lobbydialog.h"
#include "../user.h"
#include <QStandardItemModel>

LobbyDialog::LobbyDialog(const QString& gameName, QWidget *parent) :
    QDialog(parent), ui(new Ui::LobbyDialog),
    m_Client("ws://localhost:8000/", this),
    m_GameName(gameName)
{
    ui->setupUi(this);

    // Set up JSON-RPC parameters
    QJsonArray connectParams;
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", gameName);
    connectParams.append(userObj);
    connectParams.append(gameObj);

    QJsonRpcMessage request = QJsonRpcMessage::createRequest("game.connect", connectParams);
    m_Client.openBlocking();
    m_Client.sendMessage(request);
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(parseLobbyMessage(QJsonRpcMessage)));
    connect(&m_Client, SIGNAL(notificationReceived(QJsonRpcMessage)),
            this, SLOT(parseLobbyMessage(QJsonRpcMessage)));

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setRowCount(8);
    model->setColumnCount(8);

    for (int c = 0; c < 8; c++)
    {
        QStandardItem *wPawn = new QStandardItem;
        wPawn->setText(QString("White Pawn %0").arg(QString::number(c)));
        wPawn->setIcon(QIcon(":/chess/wPawn.bmp"));
        model->setItem(1, c, wPawn);

        QStandardItem *bPawn = new QStandardItem;
        bPawn->setText(QString("Black Pawn %0").arg(QString::number(c)));
        bPawn->setIcon(QIcon(":/chess/bPawn.bmp"));
        model->setItem(6, c, bPawn);
    }
    ui->tableView->setModel(model);
}

LobbyDialog::~LobbyDialog()
{
    m_Client.close();
    delete ui;
}

void LobbyDialog::parseLobbyMessage(const QJsonRpcMessage& message)
{
    qDebug() << message << message.method();
    if (!message.method().compare("chatted"))
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
    else if(!message.method().compare("moved"))
    {
        QJsonArray params = message.params().toArray();
        QJsonObject userObj = params[0].toObject();
        QJsonObject unitObj = params[1].toObject();
        QJsonObject moveObj = params[2].toObject();


        qDebug() << message.result().toString();
    }
    else if(!message.method().compare("joined"))
    {
        qDebug() << message.result().toString();
    }
    else if(!message.method().compare("left"))
    {
        qDebug() << message.result().toString();
    }
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

    QJsonRpcMessage message = QJsonRpcMessage::createRequest("game.chat", chatParams);
    m_Client.sendMessage(message);
}
