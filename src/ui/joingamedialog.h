#ifndef JOINGAMEDIALOG_H
#define JOINGAMEDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "../gamelistmodel.h"

namespace Ui {
class JoinGameDialog;
}

class JoinGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JoinGameDialog(QWidget *parent = 0);
    ~JoinGameDialog();

private slots:
    void on_tbToggleFilters_toggled(bool checked);
    void processListGamesResponse(const QJsonRpcMessage& response);
    void sendListGamesRequest();
    void joinGame(const QModelIndex& index);

private:
    Ui::JoinGameDialog *ui;
    GameListModel m_Games;
    QJsonRpcHttpClient m_Client;
};

#endif // JOINGAMEDIALOG_H
