#ifndef GAMELISTDIALOG_H
#define GAMELISTDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "../gamelistmodel.h"

namespace Ui {
class GameListDialog;
}

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
