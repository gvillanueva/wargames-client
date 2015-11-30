#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "gamelistmodel.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void processUserGamesResponse(const QJsonRpcMessage& response);
    void displayGame(const QModelIndex& index);

private:
    Ui::MainDialog *ui;
    GameListModel m_UsersGames;
    QJsonRpcHttpClient m_Client;
};

#endif // MAINDIALOG_H
