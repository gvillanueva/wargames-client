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
    QJsonArray createParams = QJsonArray() << userObj << gameObj;

    QJsonRpcMessage request = QJsonRpcMessage::createRequest("create", createParams);
    QJsonRpcServiceReply *reply = m_Client.sendMessage(request);
    connect(reply, SIGNAL(finished()), this, SLOT(processCreateRequest()));
}

void CreateGameDialog::processCreateRequest()
{
    QJsonRpcServiceReply* reply = qobject_cast<QJsonRpcServiceReply*>(QObject::sender());
    this->setEnabled(true);

    QJsonRpcMessage response = reply->response();
    if (response.errorCode() != QJsonRpc::NoError) {
        QMessageBox::critical(this, "Create game error", response.errorMessage());
        return;
    }

    LobbyDialog *lobby = new LobbyDialog(ui->leRoomName->text());
    lobby->show();
    this->close();
}
