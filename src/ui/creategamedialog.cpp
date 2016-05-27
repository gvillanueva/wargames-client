#include "creategamedialog.h"
#include "ui_creategamedialog.h"
#include "../user.h"
#include <QMessageBox>

#include "lobbydialog.h"

/* NOTES
 * * Maybe one day, game system will determine minimum player number.
 */
CreateGameDialog::CreateGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateGameDialog),
    m_Client("http://localhost:8000")
{
    ui->setupUi(this);
    connect(ui->bbButtons, SIGNAL(accepted()), this, SLOT(accept()));

    sendListSystemsRequest();
}

CreateGameDialog::~CreateGameDialog()
{
    delete ui;
}

void CreateGameDialog::accept()
{
    // Disable UI until response is returned
    this->setEnabled(false);

    // Create parameter array for "create" request
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject gameObj;
    gameObj.insert("name", ui->leRoomName->text());
    gameObj.insert("maxUsers", ui->sbMaxPlayers->value());
    gameObj.insert("system", ui->cmbSystem->currentText());
    QJsonArray createParams = QJsonArray() << userObj << gameObj;

    QJsonRpcMessage request = QJsonRpcMessage::createRequest("Game.create", createParams);
    QJsonRpcServiceReply *reply = m_Client.sendMessage(request);
    connect(reply, SIGNAL(finished()), this, SLOT(processCreateRequest()));
}

void CreateGameDialog::sendListSystemsRequest()
{
    // Request list of installed game systems
    QJsonRpcMessage request = QJsonRpcMessage::createRequest("Game.listSystems", QJsonValue());
    QJsonRpcServiceReply *response = m_Client.sendMessage(request);
    connect(response, SIGNAL(finished()), this, SLOT(processListSystemsResponse()));
}

void CreateGameDialog::processListSystemsResponse()
{
    QJsonRpcServiceReply* reply = qobject_cast<QJsonRpcServiceReply*>(QObject::sender());
    QJsonRpcMessage response = reply->response();
    if (response.errorCode() != QJsonRpc::NoError) {
        QMessageBox::critical(this, "System list error", response.errorMessage());
        return;
    }

    // Ensure result is an array of game objects
    if (response.result().isArray())
    {
        // Convert value to array, and parse game objects from it
        QJsonArray jsonArray = response.result().toArray();
        QStringList systemList;

        foreach(const QJsonValue& jsonValue, jsonArray)
            if (jsonValue.isString())
                systemList.append(jsonValue.toString());

        // Create a model and set it's view
        ui->cmbSystem->addItems(systemList);
    } else {
        QMessageBox::critical(this, "System list error", "Response result formatted improperly");
        return;
    }
}

void CreateGameDialog::processCreateRequest()
{
    QJsonRpcServiceReply* reply = qobject_cast<QJsonRpcServiceReply*>(QObject::sender());

    QJsonRpcMessage response = reply->response();
    if (response.errorCode() != QJsonRpc::NoError) {
        QMessageBox::critical(this, "Create game error", response.errorMessage());
        this->setEnabled(true);
        return;
    }

    LobbyDialog *lobby = new LobbyDialog(ui->leRoomName->text());
    lobby->show();
    this->close();
}
