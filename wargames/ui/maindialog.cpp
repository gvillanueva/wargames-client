/*!
 * \author  Giancarlo Villanueva
 * \date    2016-05-27
 * \file    maindialog.cpp
 * \brief   Defines the behavior of the Main dialog.
 */

#include "maindialog.h"
#include "ui_maindialog.h"
#include "../user.h"
#include "../game.h"
#include "gamedialog.h"
#include <QMessageBox>
#include "creategamedialog.h"
#include "gamelistdialog.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    m_UsersGames(this),
    m_Client("http://localhost:8000", this)
{
    ui->setupUi(this);

    connect(ui->lvMyGames, SIGNAL(activated(QModelIndex)),
            this, SLOT(displayGame(QModelIndex)));
    connect(&m_Client, SIGNAL(messageReceived(QJsonRpcMessage)),
            this, SLOT(processUserGamesResponse(QJsonRpcMessage)));
    connect(ui->pbRefresh, SIGNAL(clicked(bool)), this, SLOT(sendListGamesRequest()));

    sendListGamesRequest();
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::sendListGamesRequest()
{
    // Set up JSON-RPC parameters
    QJsonObject userObj;
    userObj.insert("authToken", User::instance().authToken());
    QJsonObject filterObj;
    filterObj.insert("myGames", true);
    QJsonArray listGamesParams = QJsonArray() << userObj << filterObj;

    // Attempt to log in
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Game.listGames", listGamesParams);
    m_Client.sendMessage(message);
}

void MainDialog::processUserGamesResponse(const QJsonRpcMessage& response)
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
        m_UsersGames.setList(usersGamesList);
        ui->lvMyGames->setModel(&m_UsersGames);
    } else {
        QMessageBox::critical(this, "Game list error", "Response result formatted improperly");
        return;
    }
}

void MainDialog::displayGame(const QModelIndex& index)
{
    GameDialog *gd = new GameDialog;
    gd->show();
}

void MainDialog::on_pbCreateGame_clicked()
{
    CreateGameDialog *d = new CreateGameDialog;
    d->show();
}

void MainDialog::on_pbJoinGame_clicked()
{
    GameListDialog *d = new GameListDialog;
    d->exec();
}
