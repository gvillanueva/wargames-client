/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-09
 * \file    gamedialog.h
 * \brief   Defines the structure of the Game dialog.
 */

#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>
#include <qjsonrpc/qjsonrpchttpclient.h>
#include "../game.h"

namespace Ui {
class GameDialog;
}

/*!
 * \brief The GameDialog class provides an interface for displaying the game
 *        state.
 */
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
