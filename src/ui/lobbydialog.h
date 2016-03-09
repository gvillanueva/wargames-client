#ifndef LOBBYDIALOG_H
#define LOBBYDIALOG_H

#include <QDialog>
#include "../jsonrpc/jsonrpcwebsocketclient.h"

namespace Ui {
class LobbyDialog;
}

class LobbyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LobbyDialog(QWidget *parent = 0);
    ~LobbyDialog();

private slots:
    void parseLobbyMessage(const QJsonRpcMessage& message);

    void on_pbSend_clicked();

private:
    Ui::LobbyDialog *ui;
    JsonRpcWebSocketClient m_Client;
};

#endif // LOBBYDIALOG_H
