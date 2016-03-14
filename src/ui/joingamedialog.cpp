#include "joingamedialog.h"
#include "ui_joingamedialog.h"
#include "../user.h"
#include <QMessageBox>
#include "lobbydialog.h"

JoinGameDialog::JoinGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinGameDialog),
    m_Client("http://localhost:8000", this)
{
    ui->setupUi(this);
    connect(ui->tvGames, SIGNAL(activated(QModelIndex)),
            this, SLOT(joinGame(QModelIndex)));
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(processListGamesResponse(QJsonRpcMessage)));
    connect(ui->pbRefresh, SIGNAL(clicked(bool)), this, SLOT(sendListGamesRequest()));
    sendListGamesRequest();
}

JoinGameDialog::~JoinGameDialog()
{
    delete ui;
}

void JoinGameDialog::on_tbToggleFilters_toggled(bool checked)
{
    ui->qwFilters->setVisible(checked);
    ui->tbToggleFilters->setText(checked ? "-" : "+");
}

void JoinGameDialog::sendListGamesRequest()
{
    // Set up JSON-RPC parameters
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject filterObj;
    filterObj.insert("myGames", false);
    filterObj.insert("notFull", ui->cbNotFull->isChecked());
    filterObj.insert("notEmpty", ui->cbNotEmpty->isChecked());
    filterObj.insert("isPublic", ui->cbNotEmpty->isChecked());
    QJsonArray listGamesParams = QJsonArray() << userObj << filterObj;

    // Attempt to log in
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("listGames", listGamesParams);
    m_Client.sendMessage(message);
}

void JoinGameDialog::processListGamesResponse(const QJsonRpcMessage& response)
{
    if (response.errorCode() != QJsonRpc::NoError) {
        QMessageBox::critical(this, "Game list error", response.errorMessage());
        return;
    }

    // Ensure result is an array of game objects
    if (response.result().isArray())
    {
        // Convert value to array, and parse game objects from it
        QJsonArray jsonArray = response.result().toArray();
        GameList usersGamesList;

        foreach(const QJsonValue& jsonValue, jsonArray)
            if (jsonValue.isObject())
                usersGamesList.append(Game(jsonValue.toObject()));

        // Create a model and set it's view
        m_Games.setList(usersGamesList);
        ui->tvGames->setModel(&m_Games);
    } else {
        QMessageBox::critical(this, "Game list error", "Response result formatted improperly");
        return;
    }
}

void JoinGameDialog::joinGame(const QModelIndex& index)
{
    QString gameName = m_Games.index(index.row(), 0, QModelIndex()).data().toString();
    LobbyDialog *lobby = new LobbyDialog(gameName);
    lobby->show();
    this->close();
}
