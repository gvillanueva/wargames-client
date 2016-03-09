#include "maindialog.h"
#include "ui_maindialog.h"
#include "user.h"
#include "game.h"
#include "gamedialog.h"
#include <QMessageBox>
#include "creategamedialog.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    m_UsersGames(this),
    m_Client("http://wargames.walkingtarget.com/api/matchmaker.php", this)
{
    ui->setupUi(this);

    connect(ui->lvMyGames, SIGNAL(activated(QModelIndex)),
            this, SLOT(displayGame(QModelIndex)));

    // Set up JSON-RPC parameters
    QJsonArray getUserGamesParams;
    getUserGamesParams.insert(0, QJsonValue(User::instance().authToken()));

    // Attempt to log in
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("getUserGames", getUserGamesParams);
    m_Client.sendMessage(message);
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(processUserGamesResponse(QJsonRpcMessage)));
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::processUserGamesResponse(const QJsonRpcMessage& response)
{
    if (response.errorCode() != QJsonRpc::NoError) {
        QMessageBox::critical(this, "Get user's games error", response.errorMessage());
        return;
    }

    QJsonValue value = response.result();
    if (value.isArray())
    {
        GameList usersGamesList;

        // Convert value to array, and parse game objects from it
        QJsonArray jsonArray = value.toArray();
        foreach(const QJsonValue& jsonValue, jsonArray)
            if (jsonValue.isObject())
                usersGamesList.append(Game(jsonValue.toObject()));

        // Create a model and set it's view
        m_UsersGames.setList(usersGamesList);
        ui->lvMyGames->setModel(&m_UsersGames);
    }
}

void MainDialog::displayGame(const QModelIndex& index)
{
    GameDialog *gd = new GameDialog;
    gd->show();
}

void MainDialog::on_pbCreateGame_clicked()
{
    CreateGameDialog d;
    d.exec();
}
