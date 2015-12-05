#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "game.h"

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameDialog(QWidget *parent = 0);
    ~GameDialog();
    void setGame(const Game& value);

private:
    Ui::GameDialog *ui;
    QJsonRpcHttpClient m_Client;
};

#endif // GAMEDIALOG_H
