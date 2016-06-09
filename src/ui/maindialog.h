/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-14
 * \file    maindialog.h
 * \brief   Defines the structure of the Main dialog.
 */

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "../gamelistmodel.h"

namespace Ui {
class MainDialog;
}

/*!
 * \brief The MainDialog class provides the interface for creating, joining, and
 *        listing the user's current games.
 */
class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void processUserGamesResponse(const QJsonRpcMessage& response);
    void displayGame(const QModelIndex& index);
    void on_pbCreateGame_clicked();
    void on_pbJoinGame_clicked();
    void sendListGamesRequest();

private:
    Ui::MainDialog *ui;
    GameListModel m_UsersGames;
    QJsonRpcHttpClient m_Client;
};

#endif // MAINDIALOG_H
