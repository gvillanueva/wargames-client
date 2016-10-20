/*!
 * \author  Giancarlo Villanueva
 * \date    2016-04-27
 * \file    gamelistdialog.h
 * \brief   Define the structure of the Game List dialog.
 */

#ifndef GAMELISTDIALOG_H
#define GAMELISTDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "../gamelistmodel.h"

namespace Ui {
class GameListDialog;
}

/*!
 * \brief The GameListDialog class provides the interface for searching through
 *        existing games.
 */
class GameListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameListDialog(QWidget *parent = 0);
    ~GameListDialog();

private slots:
    void on_tbToggleFilters_toggled(bool checked);
    void processListGamesResponse(const QJsonRpcMessage& response);
    void sendListGamesRequest();
    void joinGame(const QModelIndex& index);

private:
    Ui::GameListDialog *ui;
    GameListModel m_Games;
    QJsonRpcHttpClient m_Client;
};

#endif // GAMELISTDIALOG_H
